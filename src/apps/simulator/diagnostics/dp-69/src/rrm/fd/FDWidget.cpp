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

#include "rrm/fd/FDWidget.h"
#include "rrm/fd/FDPropertyTree.h"
#include "rrm/fd/FlowCalculations.hpp"
#include "rrm/fd/ImportExport.hpp"
#include "rrm/fd/component/RecoveryMapTask.h"
#include "rrm/fd/core/SceneAlgebra.hpp"

#include "dpl/general.hpp"
#include "dpl/units.hpp"

#include <vtkCallbackCommand.h>
#include <vtkCellPicker.h>
#include <vtkLabeledDataMapper.h>
#include <vtkPointData.h>

#include <QDesktopWidget>



namespace rrm::fd
{
  // void FDWidget::UpdateImageDataAndThreshold(/*bool update_threshold*/) {
  //   image_data_->Modified();
  //
  //   // if (update_threshold) {
  //   ResetTracerCharts();
  //   
  //   threshold_->Modified();
  //   threshold_->Update();
  //   // }
  // }

  void FDWidget::RequestRegions() {
    if (!stratigraphy_model_) {
      const int cells_count = model_.Dims().prod();

      for (int i = 0; i < cells_count; ++i) {
        model_[REGION][i] = 0;
        // model_[POROSITY][i] = 1.0;
        // model_[PERMEABILITY][i] = {units::MilliDarcy(), units::MilliDarcy(), units::MilliDarcy()};
      }

      return;
    }

    auto scale = rendering_settings_.stratmod.scale;
    
    auto& dims = model_.Dims();
    const auto& location = model_.Location();

    stratigraphy_model_->useDefaultCoordinateSystem();

    dpl::vector3d julio_size;
    stratigraphy_model_->getSize(julio_size.x(), julio_size.y(), julio_size.z());

    // std::cout << "STRATMOD - RequestRegions: " << julio_size << "\n\n"; 
    
    julio_size = julio_size*scale;
    
    const auto& cell_size = model_.CellSize();
    auto cells_count = dims.prod();

    const auto [x, y, z] = dims;

    std::vector<double> pos(3*cells_count);
    std::vector<int> reg(cells_count);

    dpl::vector3i idx;

    auto* coordPtr = pos.data();

    for (idx.z() = 0; idx.z() < z; ++idx.z())
      for (idx.y() = 0; idx.y() < y; ++idx.y())
        for (idx.x() = 0; idx.x() < x; ++idx.x()) {
          auto centre = location + (idx + 0.5)*cell_size;

          *coordPtr++ = centre.x()/scale.x();
          *coordPtr++ = (rendering_settings_.stratmod.y_invert ? julio_size.y() - centre.y() : centre.y())/scale.y();
          *coordPtr++ = centre.z()/scale.z();
        }


    TimestepRecords time_steps{"stratmod model"};
    stratigraphy_model_->getVolumeAttributesFromPointList(pos, reg);
    time_steps << "region sampling" >> std::cout << endl;

    auto outer_region_idx = petrophysics_.region_to_domain.size() - 1;

    for (int i = 0; i < cells_count; ++i)
      model_[REGION][i] = reg[i] < 0 ? outer_region_idx : reg[i];

    stratigraphy_model_->useOpenGLCoordinateSystem();
  }
  

  void FDWidget::SetRenderingSettings(const StartupSettings& rendering_settings) {
    rendering_settings_ = rendering_settings;
    auto& visual = rendering_settings.visual;
      
    auto& filter = CellFilter();

    auto& cutoff = visual.cutoff;
    if (cutoff.tof_forward)
      filter.max_tof_forward = *cutoff.tof_forward;

    if (cutoff.tof_backward)
      filter.max_tof_backward = *cutoff.tof_backward;
      
    filter.cutoff_direction = cutoff.type; 
    filter.cutoff_planes = cutoff.value;
      
    if (visual.axes.min) {
      axes_.min_use_default = false;
      axes_.min = *visual.axes.min;
    }

    if (visual.axes.max) {
      axes_.max_use_default = false;
      axes_.max = *visual.axes.max;
    }

    scale_ =
      visual.exaggeration
        ? *visual.exaggeration
        : dpl::vector3d{rendering_settings.stratmod.scale.max()}/rendering_settings.stratmod.scale;
      
    // (dpl::vector3d{1.0}/rendering_settings.stratmod.scale)/(1.0/rendering_settings.stratmod.scale.max());
  }

  void FDWidget::SetDomainRange(int from, int to) {
    auto& domains = CellFilter().domains;
    for (int i = from; i <= to; ++i)
      domains.insert(i);
    domains.erase(to + 1);
  }

  

    
  void FDWidget::Render() {
    // renderWindow->Render();
    render_window_->Render();
  }
  

  void FDWidget::EvaluateWellAndFacePressure(bool update_directional) {
    auto cell_count = model_.CellCount();

    TimestepRecords timesteps{"Pressure"};

    auto matrix = FlowSolver::PressureCellMatrix(model_);

    auto domain_diag_saved = matrix.diagonal;

    {
      std::vector<double> constants(cell_count, 0);
      FlowSolver::WellPressureSourceTerms(model_, wells_, matrix.diagonal, constants);

      if (!boundary_cond_.NoFlow())
        dpl::sfor<6>([&, this](auto i) {
          if (boundary_cond_.values[i])
            FlowSolver::FacePressureSourceTerms<i/2, i%2>(
              model_, matrix.diagonal, constants, *boundary_cond_.values[i]);
        });
        // if (
        //   auto add_face_source_terms = [&, this](auto pred) {
        //     dpl::sfor<6>([&, this](auto i) {
        //       if (boundary_cond_.values[i])
        //         FlowSolver::FacePressureSourceTerms<i/2, i%2>(
        //           model_, matrix.diagonal, constants, *boundary_cond_.values[i], pred);
        //     });
        //   };
        //   boundary_cond_.include_outer)
        //   add_face_source_terms([](auto) { return true; });
        // else
        //   add_face_source_terms([this](auto i) {
        //     return model_.Domain(i) != model_.OuterDomain();
        //   });


      timesteps << "matrix construction";

      // task_queue_.Push(
      //   std::make_unique<WellPressureTask>(this, linear_solver::HypreInput(matrix, std::move(constants))));

      WellPressureTask task{this, linear_solver::HypreInput{matrix, std::move(constants)}};
      task.Execute();
      task.Commit();

      
    }


    // if constexpr (true/*false*/)
    if (update_directional) {
      dpl::sfor<3>([&, this](auto i) {

        matrix.diagonal = domain_diag_saved;
        std::vector<double> constants(cell_count, 0);

        FlowSolver::FacePressureSourceTerms<i>(model_, matrix.diagonal, constants);

        timesteps << (std::string("Matrix construction PERM ") + std::to_string(i)).c_str();

        task_queue_.Push(
          std::make_unique<DirectionalPressureTask<i>>(this, linear_solver::HypreInput{matrix, std::move(constants)}));
      });
    }

    // std::cout << std::endl;
    // timesteps >> std::cout;    
  }


  // size_t simulation_idx = 0;

  void FDWidget::EvaluateFlowDiagnostics(bool update_directional) {
    if (boundary_cond_.NoFlow()) {
      auto [min, max] = std::minmax_element(
        wells_.begin(), wells_.end(),
        [](const VerticalWell& l, const VerticalWell& r) { return l.pressure < r.pressure; });

      if (max->pressure - min->pressure < units::Pascal()) {
        std::cout << "Insignificant pressure drop between wells\n\n";
        return;
      }
    }

    try {
      EvaluateWellAndFacePressure(update_directional);
    }
    catch (std::exception& e) {
      std::cout << "\n<Exception>: " << e.what() << '\n';
    }


  }


  void FDWidget::UpdateVisual() {
    // RefreshTitleText();
    image_data_->Modified();
    threshold_->Modified();
    threshold_->Update();

// geometry_filter_->Update();
    // geometry_filter_->Update();
    // threshold_filter_->Update();
    // extract_edges_->Update();
    // outline_data_set_mapper_->Update();






    
    VisibleVolumeChanged();

    RefreshCubeAxesBounds();

    Render();

    
    // TimestepRecords time{"UpdateVisual"};

    // UpdateImageDataAndThreshold();

    //   const auto count = model_.CellCount();
    //   double poro_sum = 0.0;
    //   for (auto i = 0; i < count; ++i)
    //     if (model_.Domain(i) != model_.OuterDomain() && CellFilter()(i))
    //       poro_sum += model_[POROSITY][i];
    //   fd_summary_.pore_volume_filter = model_.CellSize().prod()*poro_sum;
    // }
    
    // time << "UpdateImageDataAndThreshold";

    // time << "UpdateCubeAxesBounds" >> std::cout << std::endl;

    // ResetPieChart();
  }

  void FDWidget::UpdateFull(bool update_directional) {
    EvaluateFlowDiagnostics(update_directional);
    
    // return;
    // HardRefreshPropertyTree();
    // UpdateVisual();
  }

  void FDWidget::RefreshCubeAxesBounds() {
    double bounds[6];
    renderer_->ComputeVisiblePropBounds(bounds);

    if (axes_.min_use_default) {
      axes_.min.x() = bounds[0]/scale_.x();
      axes_.min.y() = bounds[2]/scale_.y();
      axes_.min.z() = bounds[4]/scale_.z();
    }
    else {
      bounds[0] = axes_.min.x()*scale_.x();
      bounds[2] = axes_.min.y()*scale_.y();
      bounds[4] = axes_.min.z()*scale_.z();
    }

    if (axes_.max_use_default) {
      axes_.max.x() = bounds[1]/scale_.x();
      axes_.max.y() = bounds[3]/scale_.y();
      axes_.max.z() = bounds[5]/scale_.z();
    }
    else {
      bounds[1] = axes_.max.x()*scale_.x();
      bounds[3] = axes_.max.y()*scale_.y();
      bounds[5] = axes_.max.z()*scale_.z();
    }
    

    dpl::vector3d min{bounds[0], bounds[2], bounds[4]};

    auto size = dpl::vector3d{bounds[1], bounds[3], bounds[5]} - min;
    dpl::vector3i count = (size/size.min()*4).min(7.0);
    auto step = size/(count - 1);
    
    min = min/scale_;
    step = step/scale_;
    
    auto adjust_axis = [] (double& min, double& step, int& count) {
      auto value = step;
      auto multipler = std::pow(10.0, std::floor(std::log10(value)));
      
      auto max = (count - 1)*step + min;

      value /= multipler;
      
      value = 
        value < 2.5 ? 1.0 :
        value < 5.0 ? 2.5 :
        5.0;
      
      value *= multipler;

      min = std::ceil(min/value)*value;
      step = value;
      count = (max - min)/step + 1; 
    };

    for (auto i = 0; i < 3; ++i)
      adjust_axis(min[i], step[i], count[i]);
    
    min = min*scale_;
    step = step*scale_;

    constexpr auto margin_coef = 0.075;
    auto flat_factor = std::min({
      bounds[1] - bounds[0], bounds[3] - bounds[2], bounds[5] - bounds[4]})*margin_coef;

    for (auto i = 0; i < 3; ++i) {
      bounds[i*2] -= flat_factor;
      bounds[i*2 + 1] += flat_factor;
    }
    
    axes_.tidy.Build(bounds, min, step, count);
  }

  void FDWidget::AddVisualWells() {
    for (auto& well : wells_)
      VisualVerticalWell::AddActor(
        renderer_, rendering_settings_, cell_info_[PRESSURE].Lut(), well,
        visual_wells_.emplace_back(), scale_, VisualVerticalWell::VisualRadius(model_, scale_));
  }
  
  DisplayCellInfo* FDWidget::ActiveCellInfo() {
    const auto* name = image_data_->GetCellData()->GetScalars()->GetName();

    return cell_info_.first<DisplayCellInfo>([name](const DisplayCellInfo& cd) {
      return cd.vtk_field_name == name;
    });
  }

  
  
  

  

  void FDWidget::InitImageData() {
    image_data_->SetDimensions(model_.Dims() + 1);
    image_data_->SetSpacing(model_.CellSize());
    image_data_->SetOrigin(model_.Location().x(), model_.Location().y(), model_.Location().z());
    
    {
      image_data_->GetCellData()->AddArray(filter_ro_array_);
      threshold_->SetInputData(image_data_);
      threshold_->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, filter_ro_array_->GetName());
      threshold_->ThresholdBetween(1, 1);
      
      image_data_set_mapper_->SetInputData(threshold_->GetOutput());
      image_data_set_mapper_->UseLookupTableScalarRangeOn();
      image_data_set_mapper_->SetScalarModeToUseCellData();
      // image_data_set_mapper_->SetResolveCoincidentTopologyPolygonOffsetParameters(0, 1);
      // image_data_set_mapper_->SetResolveCoincidentTopologyToPolygonOffset();

      image_data_actor_->SetMapper(image_data_set_mapper_);
      image_data_actor_->GetProperty()->SetEdgeVisibility(rendering_settings_.grid.edges);
      image_data_actor_->GetProperty()->SetEdgeColor(0, 0, 0);

      image_data_actor_->GetProperty()->SetAmbient(0.5);
      image_data_actor_->GetProperty()->SetDiffuse(0.4);
      image_data_actor_->GetProperty()->BackfaceCullingOn();

      ScaleChanged += [this] { image_data_actor_->SetScale(scale_); };

      renderer_->AddActor(image_data_actor_);
      

      
      // image_data_wire_mapper_->SetInputData(image_data_);
      // image_data_wire_mapper_->UseLookupTableScalarRangeOff();
      // image_data_wire_mapper_->SetScalarModeToDefault();
      // image_data_wire_mapper_->SetResolveCoincidentTopologyPolygonOffsetParameters(1, 0.5);
      // image_data_wire_mapper_->SetResolveCoincidentTopologyToPolygonOffset();
      // vtkNew<vtkLookupTable> l;
      // l->SetNumberOfTableValues(1);
      // l->SetTableValue(0, 1.0, 0.0, 0.0);
      // l->Build();
      // image_data_wire_mapper_->SetLookupTable(l);
      // image_data_wire_actor_->SetMapper(image_data_wire_mapper_);
      
      // image_data_wire_actor_->GetProperty()->SetRepresentationToWireframe();
      
      // image_data_wire_actor_->GetProperty()->SetLineWidth(1);
      // image_data_wire_actor_->GetProperty()->SetColor(1.0,0.0,0.0);
      // image_data_wire_actor_->GetProperty()->SetEdgeColor(1.0,0.0,0.0);
      // image_data_wire_actor_->GetProperty()->FrontfaceCullingOn();
      // image_data_wire_actor_->GetProperty()->BackfaceCullingOn();
      // image_data_wire_actor_->GetProperty()->LightingOff();
      
      // renderer_->AddActor(image_data_wire_actor_);

      // geometry_filter_->OutputTrianglesOff();
      
      // edge_filter_->SetInputData(image_data_);
      // edge_filter_->SetClipFunction()

     //  threshold_filter_->SetInputData(image_data_);
     // threshold_filter_->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, "region" /*"domain"*/ /*"filter_array"*/);
     // threshold_filter_->ThresholdBetween(-100, 100);

      // geometry_filter_->SetInputData(image_data_);
      // geometry_filter_->CellClippingOn();
      
      // extract_edges_->SetInputData(threshold_filter_->GetOutput());
      // edge_filter_->SetExtent(0, 100, 0, 100, 0, 100);
      
      
      // edge_filter_->SetInputConnection(image_data_set_mapper_->GetOutputPort());
      // image_data_set_mapper_->Get
      // edge_filter_->SetInputData()

      // geometry_filter_->SetInputData(image_data_);
      // geometry_filter_->SetCellClipping(true);
      // geometry_filter_->Update();
      
      
      // this->extract_edges_->SetInputData(image_data_);
      // this->extract_edges_->ExtractAllEdgeTypesOn();


      // geometry_filter_->SetInputData(image_data_);
      // // geometry_filter_->SetExtent(0, 100, 0, 100, 0, 100);
      // // geometry_filter_->SetCellClipping(true);
      //
      // outline_data_set_mapper_->SetInputData(geometry_filter_->GetOutput());
      //
      // // vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
      // // table->SetNumberOfTableValues(1);
      // // table->Build();
      // // table->SetTableValue(0, 1.0, 0, 0);
      // // outline_data_set_mapper_->SetLookupTable(table);
      //
      // outline_actor_->SetMapper(outline_data_set_mapper_);
      //
      // // outline_actor_->GetProperty()->SetEdgeVisibility(true);
      //
      // outline_actor_->GetProperty()->SetEdgeColor(1.0, 0, 0);
      // outline_actor_->GetProperty()->SetColor(1.0, 0, 0);
      // // outline_actor_->GetProperty()->SetRepresentationToWireframe();
      // outline_actor_->GetProperty()->SetLineWidth(2);
      //
      //
      // renderer_->AddActor(outline_actor_);
    }
  }



  

  void FDWidget::InitScalarBar() {
    auto default_text_property = [this](vtkTextProperty* p) {
      p->SetFontSize(rendering_settings_.visual.font_size);
      p->BoldOff();
      p->ItalicOff();
      p->SetFontFile(rendering_settings_.font_path.string().c_str());
      p->SetFontFamily(VTK_FONT_FILE);
      p->SetColor(dpl::vector3d{0.15});
      p->ShadowOff();
    };

    scalar_bar_->rendering_settings = &rendering_settings_;

    scalar_bar_->SetUnconstrainedFontSize(true);
    scalar_bar_->SetVerticalTitleSeparation(10);
    scalar_bar_->GetAnnotationTextProperty()->SetJustificationToRight();
    scalar_bar_->SetAnnotationLeaderPadding(0);
    scalar_bar_->SetMaximumNumberOfColors(9);
    scalar_bar_->SetNumberOfLabels(5);
    default_text_property(scalar_bar_->GetTitleTextProperty());
    default_text_property(scalar_bar_->GetLabelTextProperty());
    default_text_property(scalar_bar_->GetAnnotationTextProperty());
    
    if (rendering_settings_.visual.scalar_bar.shape == "h_top_right" ||
        rendering_settings_.visual.scalar_bar.shape == "h_bottom_left")
      scalar_bar_->SetOrientationToHorizontal();

    scalar_bar_->SetVisibility(rendering_settings_.visual.scalar_bar.visible);
    

    auto directional_length = /*1.5**/19.0*rendering_settings_.visual.font_size*rendering_settings_.visual.scalar_bar.scale[0];
    auto directional_width = 2.5*rendering_settings_.visual.font_size*rendering_settings_.visual.scalar_bar.scale[1];
    
    if (scalar_bar_->GetOrientation() == VTK_ORIENT_VERTICAL) {
      scalar_bar_->SetMaximumWidthInPixels(directional_width);
      scalar_bar_->SetMaximumHeightInPixels(directional_length);
    }
    else {
      scalar_bar_->SetMaximumWidthInPixels(directional_length);
      scalar_bar_->SetMaximumHeightInPixels(directional_width);
      scalar_bar_->SetWidth(1.0); // Fixes the width, value is arbitrary.
    }

    // scalar_bar_->SetLayerNumber(20);
    
    renderer_->AddActor2D(scalar_bar_);
  }


  


  void FDWidget::InitDisplayProperties() {
    auto end_property_init = [this](DisplayCellInfo& cd) {
      cd.SetContext({
        &model_,
        &wells_,
        &boundary_cond_,
        image_data_,
        threshold_,
        image_data_set_mapper_,
        scalar_bar_
      });
      cd.EndInit();

      if (auto* cont_cd = dynamic_cast<ContiniousCellInfo*>(&cd)) {
        auto iter = rendering_settings_.visual.colour_ranges.find(cd.vtk_field_name);

        if (iter != rendering_settings_.visual.colour_ranges.end()) {
          auto [range, is_log] = iter->second;
          
          cont_cd->use_default_range = false;
          cont_cd->Lut()->SetTableRange(
            dpl::units::second{dpl::units::year{range.x()}},
            dpl::units::second{dpl::units::year{range.y()}});

          if (is_log)
            cont_cd->Lut()->SetScale(*is_log);

        }
      }
      // display_cell_data_.push_back(&dp);
    };


    
    {
      auto set_region_lut = [](vtkLookupTable* lut, int count, PetrophysicalSettings& petro) {
        lut->IndexedLookupOn();

        lut->SetNumberOfTableValues(count + 1);

        for (int i = 0; i < count; ++i) {
          auto& reg = petro.regions[i];

          lut->SetTableValue(i, reg.color.redF(), reg.color.greenF(), reg.color.blueF());
          lut->SetAnnotation(vtkVariant(i), reg.name);
        }

        lut->SetTableValue(count, 0.4, 0.4, 0.4);
        lut->SetAnnotation(vtkVariant(count), "Outer");

        lut->Modified();
      };

      auto set_domain_lut = [](vtkLookupTable* lut, int count, PetrophysicalSettings& petro) {
        lut->IndexedLookupOn();

        lut->SetNumberOfTableValues(petro.domains.size());

        for (auto& [i, domain] : petro.domains) {
          lut->SetTableValue(i, domain.color.redF(), domain.color.greenF(), domain.color.blueF());
          lut->SetAnnotation(vtkVariant(i), domain.name);
        }

        lut->SetTableRange(0, count - 1);
        lut->Modified();
      };


      if (stratigraphy_model_) {
        auto max_domain = 0, max_region = 0;
        for (auto& [d, _] : petrophysics_.domains)
          if (max_domain < d)
            max_domain = d;

        for (auto& [r, _] : petrophysics_.regions)
          if (max_region < r)
            max_region = r;


        
        set_region_lut(cell_info_[REGION].Lut(), max_region + 1 /*strat_model_->getOrderedSurfacesIndices().size() - 1*/, petrophysics_);
        set_domain_lut(cell_info_[DOMAIN].Lut(), max_domain + 1, petrophysics_);
      }
      else {
        set_region_lut(cell_info_[REGION].Lut(), 5, petrophysics_);
        set_domain_lut(cell_info_[DOMAIN].Lut(), 5, petrophysics_);
      }

      end_property_init(cell_info_[REGION]);
      end_property_init(cell_info_[DOMAIN]);
    }

    end_property_init(cell_info_[POROSITY]);
    cell_info_[POROSITY].TryUpdateDefaultLutRange();

      
    cell_info_[PERM_X].component = 0;
    cell_info_[PERM_X].vtk_field_name = "perm_x";
    cell_info_[PERM_X].caption = "Perm X";
    end_property_init(cell_info_[PERM_X]);
    cell_info_[PERM_X].TryUpdateDefaultLutRange();

    cell_info_[PERM_Y].component = 1;
    cell_info_[PERM_Y].vtk_field_name = "perm_y";
    cell_info_[PERM_Y].caption = "Perm Y";
    end_property_init(cell_info_[PERM_Y]);
    cell_info_[PERM_Y].TryUpdateDefaultLutRange();

    cell_info_[PERM_Z].component = 2;
    cell_info_[PERM_Z].vtk_field_name = "perm_z";
    cell_info_[PERM_Z].caption = "Perm Z";
    end_property_init(cell_info_[PERM_Z]);
    cell_info_[PERM_Z].TryUpdateDefaultLutRange();


    end_property_init(cell_info_[PRESSURE]);
    cell_info_[PRESSURE].TryUpdateDefaultLutRange();


    // if constexpr (direction_pressure_field) {
    //   pressure_directional_prop_[0].vtk_field_name = "pressure_x";
    //   pressure_directional_prop_[0].caption = "Pressure X";
    //
    //   pressure_directional_prop_[1].vtk_field_name = "pressure_y";
    //   pressure_directional_prop_[1].caption = "Pressure Y";
    //
    //   pressure_directional_prop_[2].vtk_field_name = "pressure_z";
    //   pressure_directional_prop_[2].caption = "Pressure Z";
    //
    //
    //   for (auto& press_dir : pressure_directional_prop_) {
    //     // pressure_directional_prop_[i].SetContext(&display_context_);
    //
    //     press_dir.units = "Pa";
    //     press_dir.display_unit_coef = units::Pascal();
    //     press_dir.display_format = "%.2f";
    //
    //     auto* lut = press_dir.Lut();
    //     PopulateLutRedWhiteBlue(lut);
    //
    //     end_property_init(press_dir);
    //   }
    //
    // }

    cell_info_[TOF_FORWARD].vtk_field_name = "tof_forward";
    cell_info_[TOF_FORWARD].scalar_bar_caption = "ToF from";
    cell_info_[TOF_FORWARD].caption = "ToF from";
    cell_info_[TOF_FORWARD].Lut()->SetScaleToLog10();
    end_property_init(cell_info_[TOF_FORWARD]);
    
    cell_info_[TOF_BACKWARD].vtk_field_name = "tof_backward";
    cell_info_[TOF_BACKWARD].scalar_bar_caption = "ToF to";
    cell_info_[TOF_BACKWARD].caption = "ToF to";
    cell_info_[TOF_BACKWARD].Lut()->SetScaleToLog10();
    end_property_init(cell_info_[TOF_BACKWARD]);
    
    cell_info_[TOF_TOTAL].vtk_field_name = "tof_total";
    cell_info_[TOF_TOTAL].caption = "ToF Total";
    cell_info_[TOF_TOTAL].Lut()->SetScaleToLog10();
    end_property_init(cell_info_[TOF_TOTAL]);
    
    cell_info_[TRACER_FORWARD].vtk_field_name = "tracer_forward";
    cell_info_[TRACER_FORWARD].caption = "Tracer from";
    end_property_init(cell_info_[TRACER_FORWARD]);

    cell_info_[TRACER_BACKWARD].vtk_field_name = "tracer_backward";
    cell_info_[TRACER_BACKWARD].caption = "Tracer to";
    end_property_init(cell_info_[TRACER_BACKWARD]);

    
    
    cell_info_[DARCY_VEL_H].vtk_field_name = "vel_h";
    cell_info_[DARCY_VEL_H].caption = "Velocity XY";
    cell_info_[DARCY_VEL_H].units = "m/d";
    cell_info_[DARCY_VEL_H].display_format = "%.1e";
    cell_info_[DARCY_VEL_H].Lut()->SetScaleToLog10();
    cell_info_[DARCY_VEL_H].SetFunctor([this](auto i) {
      return model_[DARCY_VEL_H][i]*dpl::units::coef<dpl::units::day>;
    });
    end_property_init(cell_info_[DARCY_VEL_H]);
    cell_info_[DARCY_VEL_H].TryUpdateDefaultLutRange();


    // cell_info_[DARCY_VEL_V].vtk_field_name = "vel_ver";
    // cell_info_[DARCY_VEL_V].caption = "Vel. vert.";
    // cell_info_[DARCY_VEL_V].units = "m/d";
    // cell_info_[DARCY_VEL_V].display_format = "%.1e";
    // cell_info_[DARCY_VEL_V].Lut()->SetScaleToLog10();
    // cell_info_[DARCY_VEL_V].SetFunctor([this](auto i) {
    //   return /*std::abs*/(model_[DARCY_VEL][i].y()*dpl::units::coef<dpl::units::day>);
    // });
    // end_property_init(cell_info_[DARCY_VEL_V]);
    // cell_info_[DARCY_VEL_V].TryUpdateDefaultLutRange();


    cell_info_[GRAVITY_NO].vtk_field_name = "grav_num";
    cell_info_[GRAVITY_NO].caption = "Gravity No";
    // cell_info_[GRAVITY_NO].units = "e-3";
    cell_info_[GRAVITY_NO].display_format = "%.1e";
    // const char* display_format = "%.0f";
    cell_info_[GRAVITY_NO].Lut()->SetScaleToLog10();
    cell_info_[GRAVITY_NO].SetFunctor([this](auto i) {

      return rendering_settings_.gravity_number.coef()*
        9.80665*model_[PERMEABILITY][i].z()/model_[DARCY_VEL_H][i];

      // return 1.0/*183.7//*L/H*/*(233.0)/*delta density*/*9.80665/*g*/*model_[PERMEABILITY][i].z()
      //   /(model_[DARCY_VEL][i].x()*dpl::units::pascal_second{6.0e-4}.value/**model_.viscosity.value*/);
    });
    end_property_init(cell_info_[GRAVITY_NO]);
    cell_info_[GRAVITY_NO].TryUpdateDefaultLutRange();
      
    
    image_data_->GetCellData()->SetActiveScalars(cell_info_[PRESSURE].vtk_field_name.c_str());
  }

  void FDWidget::SetActiveVisualWell(VisualVerticalWell& vw) {
    auto* prev = active_well_;

    if (!rendering_settings_.publishing) {
      if (prev)
        prev->SetTextBackground(dpl::vector3d{0.85});
      vw.SetTextBackground(dpl::vector3d{0.3, 0.8, 1.0});
    }

    active_well_ = &vw;
  }  

  void FDWidget::RefreshPerforationColours() {
    for (auto& well : visual_wells_) {
      double color[3];

      cell_info_[PRESSURE].Lut()->GetColor(well.input->pressure, color);
      well.perf_actor->GetProperty()->SetColor(color);
    }
  }

  void FDWidget::RemoveActiveWell() {
    renderer_->RemoveActor2D(active_well_->caption_actor);
    renderer_->RemoveActor(active_well_->above_perf_actor);
    renderer_->RemoveActor(active_well_->perf_actor);

    wells_.remove_if([this](auto& w) { return &w == active_well_->input; });
    visual_wells_.remove_if([this](auto& w) { return &w == active_well_; });

    active_well_ = nullptr;

    SetActiveVisualWell(*visual_wells_.rbegin());

    cell_info_[PRESSURE].TryUpdateDefaultLutRange();
    // pressure_property_.Lut()->Modified();

    UpdateFull(false);
  }

  void FDWidget::ViewportResized() {
    axes_.tidy.RefreshAxes();

    {
      auto compass_size_factor = 175.0;
      orientation_market_widget_->SetViewport(0.0, 0.0,
        compass_size_factor/ViewportWidth(), compass_size_factor/ViewportHeight());
    }
    
    if (scalar_bar_->GetOrientation() == VTK_ORIENT_VERTICAL)
      scalar_bar_->SetDisplayPosition(
        ViewportWidth() - scalar_bar_->GetMaximumWidthInPixels() - 4*rendering_settings_.visual.font_size,
        (ViewportHeight() - scalar_bar_->GetMaximumHeightInPixels())/2
      );
    else {
      if (rendering_settings_.visual.scalar_bar.shape == "h_top_right")
        scalar_bar_->SetDisplayPosition(
          ViewportWidth() - (scalar_bar_->GetMaximumWidthInPixels() + 3*rendering_settings_.visual.font_size),
          ViewportHeight() - (/*scalar_bar_->GetMaximumHeightInPixels() +*/ 3*rendering_settings_.visual.font_size)
        );
      else if (rendering_settings_.visual.scalar_bar.shape == "h_bottom_left") {
        scalar_bar_->SetDisplayPosition(
          (/*scalar_bar_->GetMaximumWidthInPixels() + */ 3*rendering_settings_.visual.font_size),
          (/*scalar_bar_->GetMaximumHeightInPixels() +*/ 3*rendering_settings_.visual.font_size)
        );
      }
      
    }
  }

  // void FDWidget::RefreshTitleText() {
  //   if (title_prop_item_) {
  //     auto text = title_prop_item_->Name() + " = " +
  //       title_prop_item_->GetDisplayValueQVariant(Qt::DisplayRole).toString().toStdString();
  //
  //
  //     title_mapper_->SetInput(text.c_str());
  //     title_mapper_->Modified();
  //     Render();
  //   }
  // }

  // void FDWidget::CameraCallback() {
  //   my_cube_axis.RefreshAxes();
  // }
  
  void FDWidget::HardRefreshPropertyTree() {
    // RefreshTitleText();

    // emit prop_tree_model_.dataChanged(QModelIndex(), QModelIndex());
    if (fd_property_tree_.TreeView())
      emit fd_property_tree_.TreeView()->model()->layoutChanged();
  }

  void FDWidget::PostPressureAssignUpdate() {
    cell_info_[PRESSURE].TryUpdateDefaultLutRange();
    RefreshPerforationColours();

    ++bound_cond_timestamp_;

    UpdateFull(false);
  }
}
