/*
 * This file is part of Rapid Reservoir Modelling Software.
 *   | https://rapidreservoir.org/
 *   | https://bitbucket.org/rapidreservoirmodelling/rrm/
 *
 * Copyright (c) 2022
 *   | Dmytro Petrovskyy, PhD
 *   | dmytro.petrovsky@gmail.com
 *   | https://www.linkedin.com/in/dmytro-petrovskyy/
 *
 * RRM is free software: you can redistribute it and/or modify              
 * it under the terms of the GNU General Public License as published by     
 * the Free Software Foundation, either version 3 of the License, or        
 * (at your option) any later version.                                      
 *                                                                         
 * RRM is distributed in the hope that it will be useful,                   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            
 * GNU General Public License for more details.                             
 *                                                                         
 * You should have received a copy of the GNU General Public License        
 * along with RRM. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include "rrm/fd/RegularGrid.hpp"
#include "rrm/fd/core/vtkCustomScalarBarActor.hpp"
#include "rrm/fd/core/vtkReadOnlyArray.hpp"

#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkImageData.h>
#include <vtkIntArray.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkScalarBarActorInternal.h>
#include <vtkThreshold.h>
#include <vtkUnstructuredGrid.h>

#include <boost/format.hpp> 


namespace rrm::fd
{
  class DisplayCellInfo
  {
  public:
    using Context = std::tuple<
      RegularGrid*,
      std::list<VerticalWell>*,
      BoundaryConditions*,
      vtkImageData*,
      vtkThreshold*,
      vtkDataSetMapper*,
      core::vtkCustomScalarBarActor*>;
    
  protected:
    Context context_;
    
    vtkNew<vtkLookupTable> lut_;

    virtual void* DataPtr() { return nullptr; }

    auto* Model() const { return std::get<RegularGrid*>(context_); }
    auto* DataSetMapper() const { return std::get<vtkDataSetMapper*>(context_); }
    auto* ScalarBar() const { return std::get<core::vtkCustomScalarBarActor*>(context_); }
    auto* ImageData() const { return std::get<vtkImageData*>(context_); }
    auto* Threshold() const { return std::get<vtkThreshold*>(context_); }
    
  public:
    std::string vtk_field_name;
    std::string caption;
    std::string scalar_bar_caption;
    std::string units;


    // DisplayProperty() = default;

    
    virtual ~DisplayCellInfo() = default;

    virtual void UpdateSize() {
      DataArray()->SetVoidArray(DataPtr(), Model()->CellCount(), 1);
      // DataArray()->SetNumberOfTuples(Model()->CellCount());
    }
    
    virtual void SetActive() {
      DataSetMapper()->SetScalarModeToUseCellData();
            
      ImageData()->GetCellData()->SetActiveScalars(vtk_field_name.c_str());
      Threshold()->GetOutput()->GetCellData()->SetActiveScalars(vtk_field_name.c_str());
        
      DataSetMapper()->SetLookupTable(lut_);

      const auto& cpt = scalar_bar_caption.empty() ? caption : scalar_bar_caption;
      
      ScalarBar()->SetTitle((units.empty() ? cpt : cpt + ", " + units).c_str());                  
      ScalarBar()->SetLookupTable(lut_);    
    }
    
    virtual vtkDataArray* DataArray() const { return nullptr; }
    
    vtkLookupTable* Lut() const { return lut_; } 

    virtual bool PseudoDiscrete() = 0;
    
    bool IsActive() const {
      return ImageData()->GetCellData()->GetScalars() == DataArray();
    }
    
    virtual void SetContext(Context x) { context_ = x; }
    
    virtual void EndInit()  {      
      UpdateSize();
      auto* arr = DataArray();
      arr->SetName(vtk_field_name.c_str());
      ImageData()->GetCellData()->AddArray(arr);
    }
 
  };

  
  class IntegralDiscreteCellInfo : public DisplayCellInfo
  {
  protected:
    vtkNew<vtkIntArray> array_;

  public:
    vtkDataArray* DataArray() const override { return array_; }

    bool PseudoDiscrete() override { return true; }
  }; 
  


  class RegionCellInfo : public IntegralDiscreteCellInfo
  {
  protected:
    vtkSmartPointer<vtkDataArray> arr_;

  public:
    RegionCellInfo() {
      vtk_field_name = "region";
      caption = "Region";
    }

    void SetContext(Context x) override {
      IntegralDiscreteCellInfo::SetContext(x);
      auto* model = Model();
      arr_ = core::vtkReadOnlyArrayCreate([model](auto i) { return (*model)[REGION][i]; });
    }

    void UpdateSize() override {
      arr_->SetNumberOfTuples(Model()->CellCount());
    }

    vtkDataArray* DataArray() const override { return arr_; }
  };
  


  class DomainCellInfo : public IntegralDiscreteCellInfo
  {
  protected:
    vtkSmartPointer<vtkDataArray> arr_;
    
  public:
    DomainCellInfo() { vtk_field_name = "domain"; caption = "Domain"; }

    void SetContext(Context x) override {
      IntegralDiscreteCellInfo::SetContext(x);
      auto* model = Model();
      arr_ = core::vtkReadOnlyArrayCreate([model](auto i) { return model->Domain(i); });
    }

    void UpdateSize() override {
      arr_->SetNumberOfTuples(Model()->CellCount());
    }

    vtkDataArray* DataArray() const override { return arr_; }
  };

    




  
  class ContiniousCellInfo : public DisplayCellInfo
  {
  protected:
    vtkNew<vtkDoubleArray> array_;
  
  public:
    int component = 0;
    
    vtkDataArray* DataArray() const override { return array_; }

    bool PseudoDiscrete() override { return false; }
    
    bool use_default_range = true;

    const char* display_format = "%.0f";
    double display_unit_coef = 1.0;

    /**
     * \param value in SI units
     */
    virtual double FromSI(double value) { return value/display_unit_coef; }
    virtual double ToSI(double display_value) { return display_value*display_unit_coef; }

    /**
     * \param value in SI units
     */
    std::string FormatFromSI(double value, const char* format) {
      return (boost::format(format) % FromSI(value)).str();
    }
    
    /**
     * \param value in SI units
     */
    virtual std::string FormatFromSI(double value) {
      return FormatFromSI(value, display_format);
    }

    

    void EndInit() override {
      DisplayCellInfo::EndInit();
      PopulateLutRedWhiteBlue(lut_);
    }

    void SetActive() override {
      DisplayCellInfo::SetActive();
      ScalarBar()->SetLabelFormatMap([this](const double x) { return FormatFromSI(x); });      
    }

    void TryUpdateDefaultLutRange() {
      if (use_default_range)
        lut_->SetTableRange(GetDefaultRange());
    }
    
    virtual dpl::vector2d GetDefaultRange() {
      auto* vtk_arr = DataArray();       

      auto min = std::numeric_limits<double>::max();
      auto max = -std::numeric_limits<double>::max();

      auto cell_count = Model()->CellCount();

      for (auto i = 0; i < cell_count; ++i) {
        auto value = vtk_arr->GetComponent(i, component);

        if (max < value)
          max = value;

        if (min > value)
          min = value;
      }

      return {min, max};
    }
  };


  class CustomCellInfo final : public ContiniousCellInfo
  {
  protected:
    vtkSmartPointer<vtkDataArray> arr_;

  public:
    template<typename Func>
    void SetFunctor(Func f) {
      arr_ = core::vtkReadOnlyArrayCreate(f);
    }

    void UpdateSize() override {
      arr_->SetNumberOfTuples(Model()->CellCount());
    }

    vtkDataArray* DataArray() const override { return arr_; }
  };
  
  class PorosityCellInfo : public ContiniousCellInfo
  { 
  protected:
    void* DataPtr() override { return &(*Model())[POROSITY][0]; }

  public:
    PorosityCellInfo() {
      vtk_field_name = "poro";
      caption = "Porosity";
      units = "%";
      display_format = "%.1f";
      display_unit_coef = 0.01;
    }
  };
  
  class PressureCellInfo : public ContiniousCellInfo
  {
  protected:
    void* DataPtr() override { return &(*Model())[PRESSURE][0]; }
    
  public:
    PressureCellInfo() {
      vtk_field_name = "pressure";
      caption = "Pressure";
      units = "MPa";
      display_unit_coef = dpl::units::coef<dpl::units::megapascal> /*units::Bar()*/;
      display_format = "%.3f";
    }                  

    dpl::vector2d GetDefaultRange() override {
      auto& wells = *std::get<std::list<VerticalWell>*>(context_); 
      
      auto [minW, maxW] = std::minmax_element(
        wells.begin(), wells.end(),
        [](const VerticalWell& l, const VerticalWell& r) { return l.pressure < r.pressure; });      

      double min = minW->pressure;
      double max = maxW->pressure;
      
      for (auto& v : std::get<BoundaryConditions*>(context_)->values)
        if (v) {
          min = std::min(min, v->value);
          max = std::max(max, v->value);
        }

      return {min, max};
    }
  };
    
  
  
  class PermeabilityCellInfo : public ContiniousCellInfo
  {
  protected:
    void* DataPtr() override { return &(*Model())[PERMEABILITY][0].x(); }
    
  public:
    PermeabilityCellInfo() { units = "mD"; display_format = "%.1e"; display_unit_coef = units::MilliDarcy(); }
        
    void EndInit() override {
      ContiniousCellInfo::EndInit();

      array_->SetNumberOfComponents(3);
      // array_->SetNumberOfTuples(Model()->CellCount());
      lut_->SetVectorComponent(component);      
      lut_->SetScaleToLog10();
    }

    // void UpdateSize() override {
    //   DataArray()->SetVoidArray(DataPtr(), Model()->CellCount(), 1);
    //   // array_->SetNumberOfTuples(Model()->CellCount());
    // }

    void SetActive() override {
      ContiniousCellInfo::SetActive();

      DataSetMapper()->ColorByArrayComponent(vtk_field_name.c_str(), component);
      DataSetMapper()->SetScalarModeToUseCellFieldData();                                                          
    }
  };
        
  class TofCellInfo : public ContiniousCellInfo
  {
  public:
    
    TofCellInfo() { units = "year"; display_unit_coef = units::Year(); display_format = "%.0f"; }
    
    std::string FormatFromSI(double si_value) override {
      auto display_value = FromSI(si_value);
      return (boost::format(
        display_value < 1 ? "%.2f" :
        display_value < 10 ? "%.1f" :
          display_format) % display_value).str();
    }


    dpl::vector2d GetDefaultRange() override {
      auto* vtk_arr = DataArray();

      auto [x, y, z] = Model()->Dims();
      
      auto min_tof = std::numeric_limits<double>::max();
      auto max_tof = -std::numeric_limits<double>::max();

      for (int k = 0, idx = 0; k < z; ++k) 
        for (int j = 0; j < y; ++j)        
          for (int i = 0; i < x; ++i, ++idx) {
            if (Model()->Domain(idx) == Model()->OuterDomain())
              continue;

            if (i == 0 || i == x - 1 ||
                j == 0 || j == y - 1 ||
                k == 0 || k == z - 1)
              continue;

            auto tof = *vtk_arr->GetTuple(idx);

            // if (std::isinf(tof)) {
            //   std::cout << "INF\n";
            //   continue;
            // }
            //
            // if (tof <= 0) {
            //   std::cout << "ZERO\n";
            //   continue;
            // }

            min_tof = std::min(min_tof, tof);
            max_tof = std::max(max_tof, tof);
          }
      
      return {min_tof, max_tof};
    }
  };

  

 
  
  class TofTotalCellInfo : public TofCellInfo
  {
    vtkSmartPointer<vtkDataArray> arr_;
    
  public:
    void SetContext(Context x) override {
      TofCellInfo::SetContext(x);
      auto* model = Model();
      arr_ = core::vtkReadOnlyArrayCreate([model](auto i) { return eval::TOF_TOTAL(*model, i); });
    } 
    
    void UpdateSize() override {
      arr_->SetNumberOfTuples(Model()->CellCount());
    }
    
    vtkDataArray* DataArray() const override {
      return arr_;
    }
  };

  class TracerCellInfo : public ContiniousCellInfo
  {        
  public:
    TracerCellInfo() { display_format = "%.1f"; }

    bool PseudoDiscrete() override { return true; }
  };

  
  class TofForwardCellInfo : public TofCellInfo {
  protected:
    void* DataPtr() override { return &(*Model())[TOF_FORWARD][0]; }
  };

  class TofBackwardCellInfo : public TofCellInfo {
  protected:
    void* DataPtr() override { return &(*Model())[TOF_BACKWARD][0]; }
  };
  
  class TracerForwardCellInfo : public TracerCellInfo {
  protected:
    void* DataPtr() override { return &(*Model())[TRACER_FORWARD][0]; }
  };

  class TracerBackwardCellInfo : public TracerCellInfo {
  protected:
    void* DataPtr() override { return &(*Model())[TRACER_BACKWARD][0]; }
  };
}
