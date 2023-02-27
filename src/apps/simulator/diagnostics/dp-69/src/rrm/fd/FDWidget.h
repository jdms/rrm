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

#include "dpl/events.hpp"

#include "rrm/fd/DisplayCellInfo.hpp"
#include "rrm/fd/FDPropertyTree.h"
#include "rrm/fd/TaskScheduling.hpp"
#include "rrm/fd/core/QExtendedPieChartView.hpp"
#include "rrm/fd/core/TidyAxes.hpp"
#include "rrm/fd/core/property_editor/QPropertyTreeView.hpp"
#include "rrm/fd/visual/CellDataFilter.hpp"
#include "rrm/fd/visual/VisualVerticalWell.hpp"

#include <stratmod/smodeller.hpp>

#include <future>
#include <unordered_set>

#include <boost/container/list.hpp>

#include <QMainWindow>
#include <QMouseEvent>
#include <QSplitter>
#include <QOpenGLWidget>

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPropAssembly.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
// #include <vtkOutlineFilter.h>
#include <vtkExtractEdges.h>
#include <vtkThreshold.h>
#include <vtkClipVolume.h>
#include <vtkFeatureEdges.h>
#include <vtkImageDataGeometryFilter.h>


#include <vtkGeometryFilter.h>


// #include <QVTKOpenGLWidget.h>

// #include <QVTKOpenGLStereoWidget.h>

// #include "rrm/fd/core/local_QVTK_8_2_0/QVTKOpenGLWidget.h"

namespace rrm::fd
{
  // using FDQVTKWidget = QVTKOpenGLWidget;
  // using FDQVTKWidget = QVTKOpenGLNativeWidget;
  // using FDQVTKWidget = QVTKOpenGLStereoWidget;

  

  
  class FDWidget : public QMainWindow
  {
  Q_OBJECT
  protected:

    friend class WellPressureTask;
    friend class FDPropertyTree;

    FDPropertyTree fd_property_tree_;

    QSplitter* hsplit_;
    QSplitter* vsplit_;
    



    /*OpenGL*/QWidget* qvtk_widget_;
    
    vtkNew<vtkGenericOpenGLRenderWindow> render_window_;
    vtkRenderWindowInteractor* interactor_;
    vtkNew<vtkRenderer> renderer_;
    
    vtkNew<core::vtkCustomScalarBarActor> scalar_bar_;
    vtkNew<vtkOrientationMarkerWidget> orientation_market_widget_;
    
    vtkNew<vtkImageData> image_data_;
    vtkNew<vtkThreshold> threshold_;
    vtkNew<vtkDataSetMapper> image_data_set_mapper_;
    vtkNew<vtkActor> image_data_actor_;

    // vtkNew<vtkDataSetMapper> image_data_wire_mapper_;
    // vtkNew<vtkActor> image_data_wire_actor_;

    // vtkNew<vtkFeatureEdges> extract_edges_;
    // vtkNew<vtkThreshold> threshold_filter_;
    // vtkNew<vtkPolyDataMapper> outline_data_set_mapper_;
    // vtkNew<vtkActor> outline_actor_;
    // vtkNew<vtkImageDataGeometryFilter> geometry_filter_;
    
    
    
    vtkSmartPointer<vtkPropAssembly> surfaces_actor_;
    vtkNew<core::vtkReadOnlyArray<visual::CellDataFilter>> filter_ro_array_;

    vtkSmartPointer<vtkAssembly> surfaces_;

    struct
    {
      bool min_use_default = true;
      dpl::vector3d min;

      bool max_use_default = true;
      dpl::vector3d max;

      core::TidyAxes tidy;
    } axes_;

    StartupSettings rendering_settings_;
    PetrophysicalSettings petrophysics_;
    
    dpl::static_map<
      REGION_TYPE, RegionCellInfo,
      DOMAIN_TYPE, DomainCellInfo,
      POROSITY_TYPE, PorosityCellInfo,
      // PERMEABILITY_TYPE, std::array<PermeabilityCellData, 3>,
      PERM_X_TYPE, PermeabilityCellInfo,
      PERM_Y_TYPE, PermeabilityCellInfo,
      PERM_Z_TYPE, PermeabilityCellInfo,
      PRESSURE_TYPE, PressureCellInfo,
      TOF_FORWARD_TYPE, TofForwardCellInfo,
      TOF_BACKWARD_TYPE, TofBackwardCellInfo,
      TOF_TOTAL_TYPE, TofTotalCellInfo,
      TRACER_FORWARD_TYPE, TracerForwardCellInfo,
      TRACER_BACKWARD_TYPE, TracerBackwardCellInfo,
      DARCY_VEL_H_TYPE, CustomCellInfo,
      // DARCY_VEL_V_TYPE, CustomCellInfo,
      GRAVITY_NO_TYPE, CustomCellInfo
    > cell_info_; 

    SModeller* stratigraphy_model_ = nullptr;
    
    RegularGrid model_;

    BoundaryConditions boundary_cond_;
    std::list<VerticalWell> wells_;
    
    boost::container::list<VisualVerticalWell> visual_wells_;
    VisualVerticalWell* active_well_ = nullptr;
    
    DomainSettings* active_domain_;

    dpl::vector3d scale_{1};
    
    

    FlowSummary flow_summary_;
        
    TaskScheduler task_queue_{this, 3};
    
    // TaskDispatcher back_exec_{task_queue_};

    // core::property_editor::PropertyItem* title_prop_item_ = nullptr;
    // vtkNew<vtkTextMapper> title_mapper_;

    void EvaluateFlowDiagnostics(bool update_directional);
    void EvaluateWellAndFacePressure(bool update_directional);

    void ViewportResized();
    
    
    void InitScalarBar();
    void InitDisplayProperties();
    void InitImageData();

    void AddVisualWells();
    
    
    
    


    
    

    // CustomContiniousProperty pressure_directional_prop_[3];


    
    
    
  public:
    void RemoveActiveWell();
    size_t bound_cond_timestamp_ = 0; // TODO: Ensure valid, SCOPE

    
    void SetActiveVisualWell(VisualVerticalWell& vw);
    
    auto ViewportWidth() const { return qvtk_widget_->width(); }
    auto ViewportHeight() const { return qvtk_widget_->height(); }
    
    // void RefreshFFContactVolumes();
    
    std::filesystem::path rrm_project_path;

    // static constexpr auto direction_pressure_field = false;

    auto& RenderingSettings() { return rendering_settings_; }

    void SetRenderingSettings(const StartupSettings& rendering_settings);

    void SetPetrophysics(const PetrophysicalSettings& petrophysics) {
      petrophysics_ = petrophysics;
      if (!petrophysics_.domains.empty())
        active_domain_ = &petrophysics_.domains.begin()->second;
    }

    void SetDomainRange(int from, int to);

    void SetStratigraphyModel(SModeller& x) { stratigraphy_model_ = &x; }

    DisplayCellInfo* ActiveCellInfo();
  
    void RefreshCubeAxesBounds();
    void HardRefreshPropertyTree();

    void Render();
    
    RegularGrid& Model() { return model_; }

    auto& Petrophysics() { return petrophysics_; }

    // template <int dim>
    // auto& PressureDirectionalProperty() { return pressure_directional_prop_[dim]; }

    // auto& UpscaledPermeability() { return /*flow_summary_.*/upscaled_perm_; }

    template<typename Key>
    auto& CellInfo(Key k) { return cell_info_[k]; }
    auto& CellInfo() { return cell_info_; }
    
    
    auto& CellFilter() const { return (*filter_ro_array_)(); }

    auto* Interactor() { return interactor_; }
    auto* ImageData() { return image_data_.Get(); }
    auto* Threshold() { return threshold_.Get(); }
    auto* ImageDataActor() { return image_data_actor_.Get(); }
    auto* ScalarBar() { return scalar_bar_.Get(); }
    auto* CompassWidget() { return orientation_market_widget_.Get(); }
    auto& Wells() { return wells_; }
    auto& VisualWells() { return visual_wells_; }
    auto* Renderer() { return renderer_.Get(); }


    auto* SurfacesActor() { return surfaces_actor_.Get(); }
    void SetSurfacesActor(vtkSmartPointer<vtkPropAssembly> actor) { surfaces_actor_ = actor; }

    auto& GetBoundaryConditions() { return boundary_cond_; }
    auto& GetFlowSummary() { return flow_summary_; }

    auto* ActiveVisualWell() { return active_well_; }
    // auto& WellFlowResults() { return well_results_; }
    // auto& FluidFluidContact() { return ff_contact_; }
    auto* ActiveDomain() { return active_domain_; }
    void SetActiveDomain(DomainSettings* domain) {
      active_domain_ = domain;
    }

   
    
    void PostPressureAssignUpdate();

    void RefreshPerforationColours();
    void RequestRegions();
    void UpdateVisual();
    void UpdateFull(bool update_directional);

    void UpdateModelPoroPerm() {
      model_.SetPoroPermByDomains(petrophysics_, scale_);
    }

    auto& GetPropertyTree() { return fd_property_tree_; }

    // void RefreshTitleText();

    dpl::vector3d Scale() const { return scale_; }
    void SetScale(dpl::vector3d scale) { scale_ = scale; }

    auto& Axes() { return axes_; }

    void SetOpenGLWidgetWidth(int width) {
      auto delta = width - ViewportWidth();
      
      auto sizes = hsplit_->sizes();
      sizes[0] -= delta;
      sizes[1] += delta;
      
      hsplit_->setSizes(sizes);
    }








    struct model_event_args
    {
      bool porosity = false;
      /*dpl::vector_n<bool, 3>*/bool permeability = false;
    };


    
    // ReSharper disable CppInconsistentNaming
    dpl::event<> ScaleChanged;
    dpl::event<> VisibleVolumeChanged;
    dpl::event<> FlowDiagnosticsChanged;
    dpl::event<> WellNameChanged;
    dpl::event<model_event_args> PetrophysicsChanged;
    // ReSharper restore CppInconsistentNaming
  };
}
