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

#include "dpl/units.hpp"

#include "rrm/fd/FDWidget.h"



#undef pascal

namespace rrm::fd
{ 
  struct FinaliserEmpty {
    static void Finalise(FDWidget*) { }
  };
  
  struct FinaliserRender {
    static void Finalise(FDWidget* widget) { widget->Render(); }
  };
  

  
  template <typename Scalar>
  struct DefaultFormat
  {
    static std::string Format(const Scalar& x) {
      return core::property_editor::ScalarConvert<Scalar>::ToQString(x).toStdString();
    }
  };
  
  template <typename Scalar, int N>
  struct DefaultFormat<dpl::vector_n<Scalar, N>>
  {
    template <int i>
    static std::string Format(const Scalar& x) {
      static_assert(0 <= i && i < N, "Index is outside of the bounds");
      return DefaultFormat<Scalar>::Format(x);  
    }
  };
  
  
  
  struct SetEmpty
  {
    template<typename T>
    void operator()(FDWidget*, const T&) {}
  };
  

  template<typename Derived, typename ValueType>
  struct BaseItem : DefaultFormat<ValueType>
  {
    using Type = ValueType;

    constexpr bool IsVisible() { return true; }
    constexpr bool Calculated() { return false; }
    constexpr bool IsReadOnly() { return false; }
    constexpr bool IsActive() { return false; }
    void SetActive() {}
    
    void Reset() {}

    template <typename T>
    void Set_(T&&) {}

    template <typename T>
    void Set(T&& v) {
      static_cast<Derived*>(this)->Set_(std::forward<T>(v));
    }

    Type Get() { return static_cast<Derived*>(this)->Get_(); }
  };
  
  
  template<typename Derived, typename ValueType, typename Finaliser = FinaliserEmpty>
  struct BaseWidgetItem : BaseItem<Derived, ValueType>
  {
    FDWidget* widget;
    BaseWidgetItem(FDWidget* widget = nullptr) : widget(widget) {}
   
    template <typename T>
    void Set(T&& v) {
      static_cast<Derived*>(this)->Set_(std::forward<T>(v));
      Finaliser::Finalise(widget);
    }

    constexpr auto Tooltip() { return ""; }
  };


  template <typename Derived, typename Finaliser = FinaliserRender>
  struct BaseBool : BaseWidgetItem<Derived, bool, Finaliser> { };

  

  struct LogScaleItem : BaseBool<LogScaleItem>
  {
    constexpr auto Name() { return "Log scale"; }
    auto IsVisible() { return !widget->ActiveCellInfo()->PseudoDiscrete(); }
    
    Type Get_() { return widget->ActiveCellInfo()->Lut()->GetScale(); }
    void Set_(const Type& v) {
      auto* active = widget->ActiveCellInfo();
      
      active->Lut()->SetScale(v);
      if (active == &widget->CellInfo(PRESSURE))
        widget->RefreshPerforationColours();
    }
  };



  struct ViscosityItem : BaseWidgetItem<ViscosityItem, double>
  {
    constexpr auto Name() { return "Viscosity, Pa s"; }

    Type Get_() { return widget->Model().viscosity/*/units::CentiPoise()*/; }
    void Set_(const Type& v) {
      widget->Model().viscosity = v;//units::CentiPoise(v);
      widget->UpdateFull(false);
    }
  };
  
  struct ResolutionItem : BaseWidgetItem<ResolutionItem, dpl::vector3i>
  {
    constexpr auto Name() { return "Resolution"; }

    Type Get_() { return widget->Model().Dims(); }
    void Set_(const Type& v) {
      auto& model = widget->Model();
      
      model.SetDimensions(v);
      // if constexpr (FDWidget::direction_pressure_field) {
      //   widget->PressureDirectionalProperty<0>().data.assign(model.CellCount(), 0);
      //   widget->PressureDirectionalProperty<1>().data.assign(model.CellCount(), 0);
      //   widget->PressureDirectionalProperty<2>().data.assign(model.CellCount(), 0);
      // }
      
      widget->ImageData()->SetDimensions(model.Dims() + 1);
      widget->ImageData()->SetSpacing(model.CellSize());
      
      widget->RequestRegions();
      widget->UpdateModelPoroPerm();


      widget->CellInfo().apply([](DisplayCellInfo& cd) {
        cd.UpdateSize();
      });
      
      widget->UpdateFull(true);
    }
  };

  struct GridLocationItem : BaseWidgetItem<GridLocationItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Location, m"; }

    Type Get() { return widget->Model().Location(); }
    void Set(const Type& v) {
      widget->Model().SetLocation(v);
      widget->ImageData()->SetOrigin(v.x(), v.y(), v.z());

      widget->RequestRegions();
      widget->UpdateModelPoroPerm();

      // for (auto* p : widget->DisplayProperties())
      //   p->UpdateSize();          

      widget->UpdateFull(true);
    }
  };

  struct GridSizeItem : BaseWidgetItem<GridSizeItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Size, m"; }

    Type Get_() { return widget->Model().PhysicalSize(); }
    void Set_(const Type& v) {
      auto& model = widget->Model();

      model.SetPhysicalSize(v);
      widget->ImageData()->SetSpacing(model.CellSize());

      widget->RequestRegions();
      widget->UpdateModelPoroPerm();

      // for (auto* p : widget->DisplayProperties())
      //   p->UpdateSize();          

      widget->UpdateFull(true); 
    }
  };

  struct ViewportItem : BaseWidgetItem<ViewportItem, dpl::vector2i>
  {
    constexpr auto Name() { return "Viewport, px"; }

    Type Get_() { return {widget->ViewportWidth(), widget->ViewportHeight()}; }
    void Set_(const Type& v) {
      if (widget->isMaximized())
        widget->SetOpenGLWidgetWidth(v.x());
      else {
        QSize desired = {
          widget->width() + v.x() - widget->ViewportWidth(),
          widget->height() + v.y() - widget->ViewportHeight()
        };

        widget->setGeometry(
          QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignLeft | Qt::AlignTop, desired,
            widget->geometry()));
      }
      
    }
  };

  struct ExaggerationItem : BaseWidgetItem<ExaggerationItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Exaggeration"; }

    Type Get_() { return widget->Scale(); }
    void Set_(Type v) {
      widget->SetScale(v);

      widget->ScaleChanged();
      
      widget->RefreshCubeAxesBounds();
      widget->Render();
    }
  };


  
  struct BoundingBoxItem : BaseWidgetItem<BoundingBoxItem, dpl::vector3d>
  {
    bool is_max = true;
    
    constexpr auto Name() const { return is_max ? "Max, m" : "Min, m"; }
    
    bool Calculated() const { return is_max ? widget->Axes().max_use_default : widget->Axes().min_use_default; }

    Type Get() const { return is_max ? widget->Axes().max : widget->Axes().min; }
    
    void Set(const Type& v) const {
      if (is_max) {
        widget->Axes().max_use_default = false;
        widget->Axes().max = v;
      }
      else {
        widget->Axes().min_use_default = false;
        widget->Axes().min = v;
      }
      
      widget->RefreshCubeAxesBounds();
      widget->Render();
    }

    void Reset() const {
      if (is_max)
        widget->Axes().max_use_default = true;
      else
        widget->Axes().min_use_default = true;

      widget->RefreshCubeAxesBounds();
      widget->Render();
    }
  };
  
  struct ScalarRangeItem : BaseWidgetItem<ScalarRangeItem, dpl::vector2d>
  {
  private:
    void Finalise(ContiniousCellInfo* p) {
      if (p == &widget->CellInfo(PRESSURE))
        widget->RefreshPerforationColours();

      widget->Render();
    }

    template<typename T = ContiniousCellInfo>
    auto* Active() { return static_cast<T*>(widget->ActiveCellInfo()); }
    
  public:
    
    constexpr auto Name() { return "Colour range"; }
    
    bool IsVisible() { return !Active<DisplayCellInfo>()->PseudoDiscrete(); }
    bool Calculated() { return Active()->use_default_range; }

    Type Get_() { 
      auto* active = Active();
      auto* ptr = active->Lut()->GetRange();
      
      return {active->FromSI(ptr[0]), active->FromSI(ptr[1])};
    }
    
    void Reset() {
      auto* active = Active();
      active->use_default_range = true;
      active->Lut()->SetTableRange(active->GetDefaultRange());

      Finalise(active);
    }
    
    void Set_(const Type& v) {
      auto* active = Active();
      active->use_default_range = false;     
      active->Lut()->SetTableRange(active->ToSI(v.x()), active->ToSI(v.y()));

      Finalise(active);
    }    
  };




  // struct DomainRangeItem : BaseWidgetItem<DomainRangeItem, dpl::vector_n<int, 2>>
  // {
  //   constexpr auto Name() { return "Domain range"; }
  //   // bool IsVisible() { return widget->ActiveDisplayProperty() == &widget->DomainProperty(); }
  //
  //   Type Get() { return {widget->FilterArray()->Getter().min_domain, widget->FilterArray()->Getter().max_domain}; }
  //   void Set(const Type& v) {
  //     widget->SetDomainRange(v.x(), v.y());
  //     widget->UpdateVisual();
  //   }
  // };

  

  // struct PiePoreVolumeItem : BaseWidgetItem<PiePoreVolumeItem, bool>
  // {
  //   auto Name() { return "Pore volume"; }
  //
  //   Type Get() { return widget->pie_charts_.pore_volume_; }
  //
  //   void Set(bool v) {
  //     widget->pie_charts_.pore_volume_ = v;
  //     widget->ResetTracerPieChart();
  //   }
  // };
  
  struct DomainInThresholdItem : BaseWidgetItem<DomainInThresholdItem, bool>
  {
    int domain_idx;

    auto Name() {      
      return widget->Model().Petrophysics().domains[domain_idx].name;
    }

    Type Get() { return widget->CellFilter().domains.count(domain_idx); }

    void Set(const Type& v) {
      if (v)
        widget->CellFilter().domains.insert(domain_idx);
      else
        widget->CellFilter().domains.erase(domain_idx);

      widget->UpdateVisual();
    }
  };



  struct WellNameItem : BaseWidgetItem<WellNameItem, std::string>
  {
    constexpr auto Name() { return "Name"; }

    Type Get() { return widget->ActiveVisualWell()->input->name; }
    void Set(const Type& v) {
      auto* active_well = widget->ActiveVisualWell();
      active_well->SetName(v);
      active_well->input->name = v;
      widget->WellNameChanged();
      widget->Render();
    }
  };

  struct PressureItem : BaseWidgetItem<PressureItem, double>
  {
    constexpr auto Name() { return "Pressure, MPa"; }

    Type Get() { return dpl::units::megapascal{widget->ActiveVisualWell()->input->pressure}; }
    void Set(const Type& v) {
      widget->ActiveVisualWell()->input->pressure = dpl::units::megapascal{v};
      widget->PostPressureAssignUpdate();
    }

    static auto Format(double x) { return (boost::format("%.3f") % x).str(); }
  };

  class TofThresholdItem : public BaseWidgetItem<TofThresholdItem, double>
  {
    std::string name_;
    dpl::units::second* value_ptr_;

  public:
    TofThresholdItem(FDWidget* const widget, std::string_view name, dpl::units::second& value_ptr)
      : BaseWidgetItem<TofThresholdItem, double>(widget),
        name_(name),
        value_ptr_(&value_ptr) {}
  
    auto Name() { return name_; }

    Type Get() { return value_ptr_->value == std::numeric_limits<double>::max() ? value_ptr_->value : dpl::units::year{*value_ptr_}; }

    void Set(double v) {
      *value_ptr_ = dpl::units::year{v};
      widget->HardRefreshPropertyTree();
      widget->UpdateVisual();
    }

    void Reset() {
      *value_ptr_ = std::numeric_limits<double>::max();
      widget->HardRefreshPropertyTree();
      widget->UpdateVisual();
    }
  };

  struct UpscaledPermItem : BaseWidgetItem<UpscaledPermItem, dpl::vector3d>
  {
    static constexpr auto Name() { return "Permeability, mD"; }

    Type Get() const { return widget->GetFlowSummary().upscaled_perm/units::MilliDarcy(); }

    static constexpr bool IsReadOnly() { return true; }
    
    template <int i>
    static std::string Format(const double& x) {
      return (boost::format(x < 1 ? "%.2f" : x < 10 ? "%.1f" : "%.0f") % x).str();

      
      // if (x < 1)
      //   return (boost::format("%.2f") % x).str();
      // if (x < 10)
      //   return (boost::format("%.1f") % x).str();
      // return (boost::format("%.0f") % x).str();
    }
  };

  struct WellLocationItem : BaseWidgetItem<WellLocationItem, dpl::vector3d>
  {
    static constexpr auto Name() { return "Location, m"; }

    Type Get() const { return widget->ActiveVisualWell()->input->head_location; }
    
    void Set(dpl::vector3d v) const {
      auto* active_well = widget->ActiveVisualWell();
      active_well->SetPosition(v);

      auto& well = *active_well->input;
      auto z_diff = well.head_location.z() - v.z();

      well.head_location = v;
      well.perf_top -= z_diff;

      active_well->SetPerforation(well.perf_top, well.perf_thickness);   
      
      widget->UpdateFull(false);
    }

    template <int i>
    static std::string Format(const double& x) {
      const char* frmt;

      if constexpr (i < 2)
        frmt = "%.1f";
      else
        frmt = "%.2f";
      
      return (boost::format(frmt) % x).str();
    }
  };


  
  struct PerforationItem : BaseWidgetItem<PerforationItem, dpl::vector2d>
  {
    constexpr auto Name() { return "Perforation, m"; }

    constexpr auto Tooltip() { return "[top, base]"; }

    Type Get() {  
      auto& well = *widget->ActiveVisualWell()->input;
      return {well.head_location.z() - well.perf_top, well.head_location.z() - well.perf_top - well.perf_thickness};
    }
    
    void Set(const Type& v) {
      double v0, v1;

      if (v.x() == v.y())
        return;

      if (v.x() > v.y()) {
        v0 = v.x();
        v1 = v.y();
      }
      else {
        v0 = v.y();
        v1 = v.x();
      }
      
      auto* visual_well = widget->ActiveVisualWell();
      auto& well = *visual_well->input;

      well.perf_top = well.head_location.z() - v0;
      well.perf_thickness = v0 - v1;                   

      visual_well->SetPerforation(well.perf_top, well.perf_thickness);   

      widget->UpdateFull(false);    
    }

    template <int i>
    static std::string Format(const double& x) {
      return (boost::format("%.2f") % x).str();
    }
  };
  

  struct CameraPosItem : BaseWidgetItem<CameraPosItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Camera pos"; }

    Type Get() { 
      auto* ptr = widget->Renderer()->GetActiveCamera()->GetPosition();
      return {ptr[0], ptr[1], ptr[2]};
    }
    
    void Set(const Type& v) {      
      widget->Renderer()->GetActiveCamera()->SetPosition(v);
      widget->Render();
    }
  };

  struct ViewUpItem : BaseWidgetItem<ViewUpItem, dpl::vector3d>
  {
    constexpr auto Name() { return "View up"; }

    Type Get() { 
      auto* ptr = widget->Renderer()->GetActiveCamera()->GetViewUp();
      return {ptr[0], ptr[1], ptr[2]};
    }
    
    void Set(const Type& v) {
      widget->Renderer()->GetActiveCamera()->SetViewUp(v);
      widget->Render();
    }
  };

  struct FocalPosItem : BaseWidgetItem<FocalPosItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Focal pos"; }

    Type Get() { 
      auto* ptr = widget->Renderer()->GetActiveCamera()->GetFocalPoint();
      return {ptr[0], ptr[1], ptr[2]};
    }
    
    void Set(const Type& v) {
      widget->Renderer()->GetActiveCamera()->SetFocalPoint(v);
      widget->Render();
    }
  };




  

  



  struct WellboreRadiusItem : BaseWidgetItem<WellboreRadiusItem, double>
  {
    constexpr auto Name() { return "Wellbore radius, cm"; }

    Type Get() {
      return dpl::units::centimetre{widget->ActiveVisualWell()->input->wellbore_radius};
    }
    
    void Set(Type v) {
      widget->ActiveVisualWell()->input->wellbore_radius = dpl::units::centimetre{v};
      widget->UpdateFull(false); 
    }

    static std::string Format(const double& x) {
      return (boost::format("%.2f") % x).str();
    }
  };



  


   // tree_model.AddItem(petro_fluid_cat,
   //      PureFunctionPropertyItem(this, "Fluid-fluid depth, m",
   //        [](wptr w) {
   //          return (boost::format("%.2f") % w->fluid_fluid_plane_source_->GetOrigin()[2]).str();
   //        },
   //        [](const wptr w, const double depth) {
   //          auto& plane = w->fluid_fluid_plane_source_;
   //          using v3d = dpl::vector3d;
   //
   //          v3d origin{plane->GetOrigin()};
   //          v3d p1{plane->GetPoint1()};
   //          v3d p2{plane->GetPoint2()};
   //
   //          origin.z() = depth;
   //          p1.z() = depth;
   //          p2.z() = depth;
   //          
   //          plane->SetOrigin(origin);
   //          plane->GetPoint1(p1);
   //          plane->GetPoint2(p2);
   //
   //          plane->Update();
   //
   //          w->UpdateCubeAxesBounds();
   //          w->Render();
   //        }));




  struct PermeabilityItem : BaseWidgetItem<PermeabilityItem, dpl::vector3d>
  {
    DomainSettings* domain;
    
    auto Name() { return domain->name; }

    bool IsActive() {
      return widget->ActiveDomain() == domain;
    }

    void SetActive() {
      widget->SetActiveDomain(domain);
      widget->HardRefreshPropertyTree();
    }
    
    Type Get() {  
      return domain->perm/units::MilliDarcy();
    }

    void Set(const Type& v) {
      domain->perm.x() = units::MilliDarcy(v.x());
      domain->perm.y() = units::MilliDarcy(v.y());
      domain->perm.z() = units::MilliDarcy(v.z());

      widget->UpdateModelPoroPerm();

      widget->CellInfo(PERM_X).DataArray()->Modified();
      widget->CellInfo(PERM_X).TryUpdateDefaultLutRange();

      widget->CellInfo(PERM_Y).DataArray()->Modified();
      widget->CellInfo(PERM_Y).TryUpdateDefaultLutRange();

      widget->CellInfo(PERM_Z).DataArray()->Modified();
      widget->CellInfo(PERM_Z).TryUpdateDefaultLutRange();
      
      // dpl::sfor<3>([this](auto i) {
      //   widget->CellData(PERMEABILITY)[i].DataArray()->Modified();
      //   widget->CellData(PERMEABILITY)[i].TryUpdateDefaultLutRange();
      // });

      widget->UpdateFull(true);
    }
  };


  struct PorosityItem : BaseWidgetItem<PorosityItem, double>
  {
    DomainSettings* domain;

    auto Name() { return domain->name; }

    bool IsActive() { return widget->ActiveDomain() == domain; }

    void SetActive() {
      widget->SetActiveDomain(domain);
      widget->HardRefreshPropertyTree();
    }

    Type Get() { return domain->poro*100.; }

    void Set(const Type& v) {
      domain->poro = v/100.;

      widget->PetrophysicsChanged({true, false});
      
      // widget->UpdateModelPoroPerm();         
      //
      // auto& poro_prop = widget->PorosityProperty();
      // poro_prop.DataArray()->Modified();
      // if (poro_prop.use_default_range)
      //   poro_prop.Lut()->SetTableRange(poro_prop.GetDefaultRange());
      //
      // widget->RefreshFFContactVolumes();
      //
      // widget->UpdateFull(false);
    }
  };

  

  struct DisplayPropertyItem// : BaseWidgetItem<DisplayPropertyItem, std::string>
  {
    FDWidget* widget;
    
    using ComboItem = DisplayCellInfo;
    
    constexpr auto Name() { return "Display"; }

    ComboItem* Get() { return widget->ActiveCellInfo(); }

    void Set(ComboItem* prop) {
      widget->ScalarBar()->DrawColorBarOn();
      widget->ScalarBar()->DrawAnnotationsOn();
      widget->ScalarBar()->Modified();

      prop->SetActive();
      
      widget->HardRefreshPropertyTree();
      widget->Render();
    }

    auto Items() {
      std::vector<DisplayCellInfo*> items;
      
      widget->CellInfo().apply([&items](DisplayCellInfo& cd) {
        items.push_back(&cd);
      });
      
      return items;
    }

    static std::string Caption(ComboItem* item) {
      return item->caption;
    }
  };

  


  struct SliceTypeItem
  {
    FDWidget* widget;
    
    using ComboItem = int;

    static auto Name() { return "Slice type"; }

    int Get() { return widget->CellFilter().slice_dim; }

    void Set(int v) {      
      widget->CellFilter().slice_dim = v;      
      widget->UpdateVisual();    
    }

    constexpr int Count() {
      return 3;
    }
    
    static std::string Caption(int item) {
      return item == 0 ? "X" : item == 1 ? "Y" : "Z";
    }
  };

  struct SliceValueItem : BaseWidgetItem<SliceValueItem, double>
  {
    auto Name() { return "Slice, m"; }

    Type Get_() { return widget->CellFilter().slice_value; }

    void Set_(Type v) {      
      widget->CellFilter().slice_value = v;
      widget->UpdateVisual();
    }

    void Reset() {
      widget->CellFilter().slice_value = std::numeric_limits<double>::max();
      widget->UpdateVisual();
    }
  };

   struct CutoffTypeItem : BaseWidgetItem<CutoffTypeItem, dpl::vector3i>
  {
    constexpr auto Name() { return "Cutoff type"; }

    constexpr auto Tooltip() { return "-1 - lower bound\n0 - disabled\n1 - upper bound"; }
     
    Type Get_() { return widget->CellFilter().cutoff_direction; }
    void Set_(Type v) {
      for (auto i = 0; i < 3; ++i)
        if (v[i])
          v[i] = v[i]/std::abs(v[i]);
      
      widget->CellFilter().cutoff_direction = v;
      widget->UpdateVisual();
    }
  };
  
  struct CutoffItem : BaseWidgetItem<CutoffItem, dpl::vector3d>
  {
    constexpr auto Name() { return "Cutoff, m"; }

    Type Get_() { return widget->CellFilter().cutoff_planes; }
    void Set_(const Type& v) {
      widget->CellFilter().cutoff_planes = v;
      widget->UpdateVisual();
    }
  };





  struct BoundaryConditionFaceItem : BaseWidgetItem<BoundaryConditionFaceItem, double>
  {
    std::string name;
    std::optional<dpl::units::pascal>* opt_value;
    double coef;
    
    auto Name() { return name; }

    void Set_(Type v) {
      *opt_value = dpl::units::pascal{coef*v};
      widget->PostPressureAssignUpdate();
    }

    Type Get_() {
      return *opt_value ? **opt_value/coef : std::numeric_limits<double>::max();
    }
    
    void Reset() {
      opt_value->reset();
      widget->PostPressureAssignUpdate();
    }

    static auto Format(double x) {
      return (boost::format("%.2f") % x).str();
      // return dpl::ft_format(x);
    }
  };

  
  struct BoolValueItem : BaseBool<BoolValueItem>
  {
    std::string name;
    bool* ptr;
    
    auto Name() { return name; }
    
    Type Get_() { return *ptr; }
    void Set_(const Type& v) {
      *ptr = v;
      widget->UpdateFull(false);
    }
  };

  template <
    typename Getter,
    typename Type = std::invoke_result_t<Getter, FDWidget*>,
    typename Setter = SetEmpty,
    typename Finaliser = FinaliserEmpty>
  class FunctorPropertyItem : public BaseWidgetItem<FunctorPropertyItem<Getter, Type, Setter, Finaliser>, Type, Finaliser>
  {
    std::string name_;
    Getter get_;
    Setter set_;

  public:
    FunctorPropertyItem(FDWidget* widget, std::string_view name, const Getter& get, const Setter& set = Setter())
      : BaseWidgetItem<FunctorPropertyItem<Getter, Type, Setter, Finaliser>, Type, Finaliser>{widget},
        name_{name}, get_{get}, set_{set} {}

    auto Name() { return name_; }
    Type Get_() { return get_(this->widget); }
    void Set_(const Type& v) { set_(this->widget, v); }

    constexpr bool IsReadOnly() { return std::is_same_v<Setter, SetEmpty>; }
  };

  // template <
  //   typename Getter,
  //   typename Type = std::invoke_result_t<Getter>,
  //   typename Setter = SetEmpty>
  // class GenericFunctionPropertyItem : public BaseItem<GenericFunctionPropertyItem<Getter, Type, Setter>, Type>
  // {
  //   std::string name_;
  //   Getter get_;
  //   Setter set_;
  //
  // public:
  //   GenericFunctionPropertyItem(std::string_view name, const Getter& get, const Setter& set = Setter())
  //     : name_{name}, get_{get}, set_{set} {}
  //
  //   auto Name() { return name_; }
  //   Type Get_() { return get_(); }
  //   void Set_(const Type& v) { set_(v); }
  // };  
}
