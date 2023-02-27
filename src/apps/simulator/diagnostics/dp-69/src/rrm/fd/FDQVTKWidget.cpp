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

#include "rrm/fd/FDQVTKWidget.h"
#include "rrm/fd/core/SceneAlgebra.hpp"

#include <vtkCellPicker.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>

namespace rrm::fd
{
  void FDQVTKWidget::SampleFromScalarBarAndUpdate(double ratio) {
    if (fd_widget_->CellInfo(PRESSURE).IsActive()) {
      auto* lut = fd_widget_->CellInfo(PRESSURE).Lut();
      fd_widget_->ActiveVisualWell()->input->pressure = dpl::lerp(lut->UsingLogScale(), lut->GetTableRange(), ratio);
      fd_widget_->PostPressureAssignUpdate();
    }
    else if (fd_widget_->CellInfo(TOF_FORWARD).IsActive()) {
      auto* lut = fd_widget_->CellInfo(TOF_FORWARD).Lut();
      fd_widget_->CellFilter().max_tof_forward = dpl::lerp(lut->UsingLogScale(), lut->GetTableRange(), ratio);
      fd_widget_->HardRefreshPropertyTree();
      fd_widget_->UpdateVisual();
    }
    else if (fd_widget_->CellInfo(TOF_BACKWARD).IsActive()) {
      auto* lut = fd_widget_->CellInfo(TOF_BACKWARD).Lut();
      fd_widget_->CellFilter().max_tof_backward = dpl::lerp(lut->UsingLogScale(), lut->GetTableRange(), ratio);
      fd_widget_->HardRefreshPropertyTree();
      fd_widget_->UpdateVisual();
    }
    else if (fd_widget_->CellInfo(TOF_TOTAL).IsActive()) {
      auto* lut = fd_widget_->CellInfo(TOF_TOTAL).Lut();
      fd_widget_->CellFilter().max_tof_total = dpl::lerp(lut->UsingLogScale(), lut->GetTableRange(), ratio);
      fd_widget_->HardRefreshPropertyTree();
      fd_widget_->UpdateVisual();
    }
  }
  
  double FDQVTKWidget::SampleScalarBar(vtkLookupTable* lut) const {
    return dpl::lerp(lut->UsingLogScale(), lut->GetTableRange(), scalar_bar_ratio_);
  }

  bool FDQVTKWidget::SampleScalarBar(dpl::vector2i pos, double& ratio) {
    int rect[4];

    fd_widget_->ScalarBar()->GetScalarBarRect(rect, fd_widget_->Renderer());
    
    if (fd_widget_->ScalarBar()->GetOrientation() == VTK_ORIENT_VERTICAL) {
      if (ratio = static_cast<double>(pos.y() - rect[1])/rect[3]; rect[0] <= pos.x() && pos.x() <= rect[0] + rect[2])
        return 0 <= ratio && ratio <= 1;
    }
    else {
      if (ratio = static_cast<double>(pos.x() - rect[0])/rect[2]; rect[1] <= pos.y() && pos.y() <= rect[1] + rect[3])
        return 0 <= ratio && ratio <= 1;
    }

    return false;
  }

  void FDQVTKWidget::SampleTofScalarBar() {
    vtkLookupTable* lut;
    double* ptr;
    
    if (fd_widget_->CellInfo(TOF_FORWARD).IsActive()) {
      lut = fd_widget_->CellInfo(TOF_FORWARD).Lut();
      ptr = &fd_widget_->CellFilter().max_tof_forward.value;
    }
    else if (fd_widget_->CellInfo(TOF_BACKWARD).IsActive()) {
      lut = fd_widget_->CellInfo(TOF_BACKWARD).Lut();
      ptr = &fd_widget_->CellFilter().max_tof_backward.value;
    }
    else {
      lut = fd_widget_->CellInfo(TOF_TOTAL).Lut();
      ptr = &fd_widget_->CellFilter().max_tof_total.value;
    }

    *ptr = SampleScalarBar(lut);

    fd_widget_->HardRefreshPropertyTree();
    fd_widget_->UpdateVisual();
  }
  
  void FDQVTKWidget::RightMouseButton(QMouseEvent* e) {
    if (QApplication::mouseButtons() == Qt::RightButton) {
      // Mouse down 
      mouse_down_ = e->globalPos();
    }
    else if (auto globalPos = e->globalPos(); (mouse_down_ - globalPos).manhattanLength() < 4) {
      vtkNew<vtkPropPicker> picker;
      picker->Pick(e->x(), (fd_widget_->ViewportHeight() - 1) - e->y(), 0, fd_widget_->Renderer());
      auto* picked_prop = picker->GetViewProp();

      position_picked_ = dpl::vector3d{picker->GetPickPosition()}/fd_widget_->Scale();
      
      // ff_contact_.depth_to_be_assigned = picker->GetPickPosition()[2]/exaggeration_scale.z();

      for (auto* action : context_menu_3d_.actions())
        action->setVisible(false);

      // if (!picked_prop) {
        // action_copy_camera_->setVisible(true);
        // action_paste_camera_->setVisible(true);
        // action_camera_separator_->setVisible(true);

        // action_screenshot_make_->setVisible(true);
        // action_screenshot_folder_->setVisible(true);
        // action_screenshot_separator_->setVisible(true);
      // }

      if (fd_widget_->ScalarBar() == picked_prop) {
        if (SampleScalarBar(DisplayPosition(e), scalar_bar_ratio_)) {

          if (!fd_widget_->ActiveCellInfo()->PseudoDiscrete())
            action_switch_scale_->setVisible(true);
          
          if (fd_widget_->CellInfo(PRESSURE).IsActive()) {
            action_switch_scale_sep_->setVisible(true);
            action_pressure_assign_->setVisible(true);
          }
          else if (
            fd_widget_->CellInfo(POROSITY).IsActive() ||
            fd_widget_->CellInfo(PERM_X).IsActive() || fd_widget_->CellInfo(PERM_Y).IsActive() || fd_widget_->CellInfo(PERM_Z).IsActive()) {
            action_assign_value0_->setVisible(true);
            action_assign_value1_->setVisible(true);
          }
          else if (dynamic_cast<TofCellInfo*>(fd_widget_->ActiveCellInfo())) {
            action_switch_scale_sep_->setVisible(true);
            action_tof_reset_->setVisible(true);
            action_tof_reset_all_->setVisible(true);
            action_tof_assign_->setVisible(true);
          }
        }
      }
      else {
        bool show_ff_contact = false;

        action_well_add_->setVisible(true);
        action_well_add_remove_sep_->setVisible(true);

        if (fd_widget_->ImageDataActor() == picked_prop) {
          // action_export_grid_->setVisible(true);
          show_ff_contact = true;
        }
        else
          
          for (auto& vtk_well : fd_widget_->VisualWells()) {
            if (vtk_well.caption_actor == picked_prop || vtk_well.above_perf_actor == picked_prop) {
              fd_widget_->SetActiveVisualWell(vtk_well);
              fd_widget_->HardRefreshPropertyTree();

              fd_widget_->Render();

              action_well_remove_->setVisible(true);

              if (vtk_well.above_perf_actor == picked_prop)
                show_ff_contact = true;

              break;
            }

            if (vtk_well.perf_actor == picked_prop) {
              show_ff_contact = true;
              break;
            }
          }

        if (show_ff_contact) {
          if (fd_widget_->CellInfo(POROSITY).IsActive() ||
            fd_widget_->CellInfo(PERM_X).IsActive() || fd_widget_->CellInfo(PERM_Y).IsActive() || fd_widget_->CellInfo(PERM_Z).IsActive()) {
            action_assign_pos0_->setVisible(true);
            action_assign_pos1_->setVisible(true);
          }
          
          action_ff_contact_->setVisible(true);
          action_ff_contact_sep_->setVisible(true);
        }
      }


      for (auto* act : context_menu_3d_.actions())
        if (act->isVisible()) {
          context_menu_3d_.popup(globalPos);
          break;
        }
    }
  }

  

  void FDQVTKWidget::AddWellAtMouse(int x, int y) {
    const auto& scale = fd_widget_->Scale();
    const auto& model = fd_widget_->Model();
    auto* active_well = fd_widget_->ActiveVisualWell();

    // const auto* scalar_bar_ = fd_widget_->ScalarBar();
    
    auto p0 = model.Location() + model.PhysicalSize();
    
    if (active_well)
      p0.z() = active_well->input->head_location.z();
    else
      p0.z() += Overhead;

    auto proj = core::ProjectOnPlane(fd_widget_->Renderer(), {x, (height() - 1) - y}, scale*p0)/scale;

    proj.x() = std::clamp(proj.x(), scale.x()*model.MinX(), scale.x()*model.MaxX());
    proj.y() = std::clamp(proj.y(), scale.y()*model.MinY(), scale.y()*model.MaxY());
    
    auto& w = fd_widget_->Wells().emplace_back();
    w.head_location = proj;

    if (active_well) {
      w.perf_top = active_well->input->perf_top;
      w.perf_thickness = active_well->input->perf_thickness;

      w.name =
        boost::algorithm::ends_with(active_well->input->name, "(copy)")
          ? active_well->input->name
          : active_well->input->name + " (copy)";

      w.pressure = active_well->input->pressure;
      w.wellbore_radius = active_well->input->wellbore_radius;
    }
    else {
      w.head_location = {
        model.MinX() + 0.3 * model.PhysicalSize().x(),
        model.MinY() + 0.2 * model.PhysicalSize().y(),
        model.MinZ() + Overhead
      };

      w.perf_top = Overhead + 0.4 * model.PhysicalSize().z() + 0.01;
      w.perf_thickness = 0.6 * model.PhysicalSize().z() - 0.02;

      w.name = "Well";

      w.pressure = units::Bar(500);
      w.wellbore_radius = units::Feet(0.3);
    }

    auto& vtk_well = fd_widget_->VisualWells().emplace_back();
    VisualVerticalWell::AddActor(
      fd_widget_->Renderer(), fd_widget_->RenderingSettings(), fd_widget_->CellInfo(PRESSURE).Lut(), w, vtk_well,
      scale, VisualVerticalWell::VisualRadius(model, scale));

    fd_widget_->SetActiveVisualWell(vtk_well);

    fd_widget_->UpdateFull(false);
  }
  
  void FDQVTKWidget::UpdatePerforation(VisualVerticalWell* visual, dpl::vector2i display_coord) {
    const auto& scale = fd_widget_->Scale();
    
    if (mouse_pressed_region_ == 1) {
      auto* well = visual->input;

      auto depth = core::ProjectOnVerticalLine(fd_widget_->Renderer(), display_coord, scale*well->head_location)/scale.z();

      depth = std::clamp(depth, 0.0, well->perf_top + well->perf_thickness);

      if (!sphere_actor_->GetVisibility())
        sphere_actor_->VisibilityOn();
      sphere_source_->SetCenter(
        scale*dpl::vector3d{
          well->head_location.x(), well->head_location.y(), well->head_location.z() - depth});
      sphere_source_->Update();

      visual->SetPerforation(depth, (well->perf_top + well->perf_thickness) - depth);
    }
    else if (mouse_pressed_region_ == 2) {
      auto* well = visual->input;

      auto depth = core::ProjectOnVerticalLine(fd_widget_->Renderer(), display_coord, scale*well->head_location)/scale.z();

      if (depth < well->perf_top)
        depth = well->perf_top;

      if (!sphere_actor_->GetVisibility())
        sphere_actor_->VisibilityOn();
      sphere_source_->SetCenter(
        scale*dpl::vector3d{
          well->head_location.x(), well->head_location.y(), well->head_location.z() - depth});
      sphere_source_->Update();

      visual->SetPerforation(well->perf_top, depth - well->perf_top);
    }
  }
  
  void FDQVTKWidget::ViewportMouseEvent(QMouseEvent* e) {
    auto btn = e->button();

    const auto& scale = fd_widget_->Scale();
    auto* renderer = fd_widget_->Renderer();
    const auto& model = fd_widget_->Model();
    auto* active_well = fd_widget_->ActiveVisualWell();

    if (btn == Qt::NoButton) {
      // MouseMove            
      if (mouse_pressed_on_well_) {
        mouse_moved_when_pressed_ = true;

        if (mouse_pressed_region_ == 0) {
          auto p = core::ProjectOnPlane(renderer, DisplayPosition(e),
            scale*active_well->input->head_location)/scale;

          p.x() = std::clamp(p.x(), scale.x()*model.MinX(), scale.x()*model.MaxX());
          p.y() = std::clamp(p.y(), scale.y()*model.MinY(), scale.y()*model.MaxY());
            
          active_well->SetPosition(p);
        }
        else
          UpdatePerforation(active_well, {e->x(), (height() - 1) - e->y()});

        fd_widget_->Render();
      }
      else if (scalar_bar_sampling_) {
        double ratio;
        SampleScalarBar(DisplayPosition(e), ratio);
        SampleFromScalarBarAndUpdate(ratio);
      }


      if (mouse_over_show_) {
        auto dpos = DisplayPosition(e);
      
        vtkNew<vtkPropPicker> picker;
        picker->Pick(dpos.x(), dpos.y(), 0, renderer);


        if (picker->GetViewProp() == fd_widget_->ImageDataActor()) {
          vtkNew<vtkCellPicker> cell_picker;
          cell_picker->Pick(dpos.x(), dpos.y(), 0, renderer);

          auto* cell = fd_widget_->ActiveCellInfo();

          
          if (&fd_widget_->CellInfo(REGION) == cell) {
            static_cast<vtkTextMapper*>(mouse_over_actor_->GetMapper())->SetInput(
              fd_widget_->Petrophysics().regions[
                static_cast<int>(fd_widget_->Threshold()->GetOutput()->GetCellData()->GetScalars(
                  cell->vtk_field_name.c_str())->GetComponent(cell_picker->GetCellId(), 0))
              ].name.c_str());
          }
          else if (&fd_widget_->CellInfo(DOMAIN) == cell) {
            static_cast<vtkTextMapper*>(mouse_over_actor_->GetMapper())->SetInput(
              fd_widget_->Petrophysics().domains[
                static_cast<int>(fd_widget_->Threshold()->GetOutput()->GetCellData()->GetScalars(
                  cell->vtk_field_name.c_str())->GetComponent(cell_picker->GetCellId(), 0))
              ].name.c_str());
          }
          else if (auto* cont_cell = dynamic_cast<ContiniousCellInfo*>(cell)) {

            auto pos3d = dpl::vector3d_map{cell_picker->GetPickPosition()}/fd_widget_->Scale();
            
            std::stringstream ss;

            auto ijk = dpl::vector3i{(pos3d - fd_widget_->Model().Location())/fd_widget_->Model().CellSize()};

            
            auto dims = fd_widget_->Model().Dims();
            
            ss << std::fixed << std::setprecision(2) << '(' << pos3d << ")\n"
               << '[' << ijk << "]\n"
               << ijk.dot({1, dims.x(), dims.x()*dims.y()}) << '\n';

            static_cast<vtkTextMapper*>(mouse_over_actor_->GetMapper())->SetInput(
              (ss.str() +
                cont_cell->FormatFromSI(
                  fd_widget_->Threshold()->GetOutput()->GetCellData()->GetScalars(
                    cell->vtk_field_name.c_str())->GetComponent(cell_picker->GetCellId(), cont_cell->component),
                  &fd_widget_->CellInfo(PRESSURE) == cont_cell
                    ? "%.6f"
                    : cont_cell->display_format
                )).c_str()
            );  
          }
          else {
            double val = fd_widget_->Threshold()->GetOutput()->GetCellData()->GetScalars(
            cell->vtk_field_name.c_str())->GetTuple1(cell_picker->GetCellId());

            static_cast<vtkTextMapper*>(mouse_over_actor_->GetMapper())->SetInput(std::to_string(val).c_str());  
          }

          if (!mouse_over_actor_->GetVisibility())
            mouse_over_actor_->VisibilityOn();
        }
        else {
          mouse_over_actor_->VisibilityOff();
        }
        
        mouse_over_actor_->SetDisplayPosition(dpos.x() + 20, dpos.y() + 20);
        fd_widget_->Render();
      }
    }
    else if (btn == Qt::LeftButton) {
      if (QApplication::mouseButtons() == Qt::LeftButton) {
        // LeftButton pressed

        vtkNew<vtkPropPicker> picker;
        picker->Pick(e->x(), (height() - 1) - e->y(), 0, renderer);

        auto pick_prop = picker->GetViewProp();

        // if (double_click_) {
        //   double_click_ = false;
        //   return;
        // }

        if (e->modifiers() & Qt::ControlModifier &&
          (e->type() == QEvent::MouseButtonDblClick ||
            e->flags() == Qt::MouseEventCreatedDoubleClick) &&
          pick_prop != fd_widget_->ScalarBar()) {
          // double_click_ = true;

          AddWellAtMouse(e->x(), e->y());
        }
        else if (
          (e->type() == QEvent::MouseButtonDblClick ||
            e->flags() == Qt::MouseEventCreatedDoubleClick) &&
          pick_prop == fd_widget_->ImageDataActor()) {
          // double_click_ = true;
            
          vtkNew<vtkCellPicker> cell_picker;
          cell_picker->Pick(e->x(), (height() - 1) - e->y(), 0, renderer);
          
          auto domain_idx = static_cast<vtkIntArray*>(
            fd_widget_->Threshold()->GetOutput()->GetCellData()->GetScalars("domain"))->GetValue(cell_picker->GetCellId());

          fd_widget_->SetActiveDomain(&fd_widget_->Petrophysics().domains[domain_idx]);
          fd_widget_->HardRefreshPropertyTree();
        }
        else {
          if (pick_prop) {
            if (pick_prop == fd_widget_->ScalarBar()) {
              if (double ratio; 
                e->modifiers() == Qt::ControlModifier && SampleScalarBar(DisplayPosition(e), ratio)) {
                SampleFromScalarBarAndUpdate(ratio);
                scalar_bar_sampling_ = true;
                fd_widget_->Interactor()->Disable();
              }
            }
            else if (pick_prop == fd_widget_->ImageDataActor()) {
            }
            else {
              for (auto& vw : fd_widget_->VisualWells()) {
                if (vw.caption_actor == pick_prop || vw.above_perf_actor == pick_prop) {
                  fd_widget_->Interactor()->Disable();
                  mouse_pressed_on_well_ = true;
                  mouse_pressed_region_ = 0;

                  fd_widget_->SetActiveVisualWell(vw);
                  fd_widget_->HardRefreshPropertyTree();

                  fd_widget_->Render();
                  break;
                }

                if (vw.perf_actor == pick_prop /*|| vtk_well.above_perforation_actor_ == picked_prop*/) {
                  const auto well = vw.input;
                  const auto depth = core::ProjectOnVerticalLine(renderer, {e->x(), (height() - 1) - e->y()}, scale*well->head_location)/scale.z();
                  if (well->perf_thickness == 0 || abs(depth - well->perf_top) < abs(depth - (well->perf_top + well->perf_thickness)))
                    mouse_pressed_region_ = 1;
                  else
                    mouse_pressed_region_ = 2;

                  fd_widget_->Interactor()->Disable();
                  mouse_pressed_on_well_ = true;
                  mouse_moved_when_pressed_ = true;

                  UpdatePerforation(&vw, {e->x(), (height() - 1) - e->y()});

                  fd_widget_->SetActiveVisualWell(vw);
                  fd_widget_->HardRefreshPropertyTree();

                  fd_widget_->Render();
                  break;
                }
              }
            }
          }

        }
      }
      else {
        // LeftButton Released

        if (mouse_pressed_on_well_ && mouse_moved_when_pressed_) {
          if (mouse_pressed_region_ == 0) {
            // Well relocation
            auto* well = active_well->input;

            dpl::vector3d pos{active_well->GetPosition()};

            well->head_location.x() = pos.x();
            well->head_location.y() = pos.y();

            ++fd_widget_->bound_cond_timestamp_;
            fd_widget_->Render();

            fd_widget_->UpdateFull(false);
          }
          else if (mouse_pressed_region_ == 1 || mouse_pressed_region_ == 2) {
            // Well perforation change
            auto [top, thickness] = active_well->GetPerforation();
            active_well->input->perf_top = top;
            active_well->input->perf_thickness = thickness;

            sphere_actor_->VisibilityOff();

            ++fd_widget_->bound_cond_timestamp_;
            fd_widget_->Render();

            fd_widget_->UpdateFull(false);
          }
        }

        fd_widget_->Interactor()->Enable();
        scalar_bar_sampling_ = false;
        mouse_pressed_on_well_ = false;
        mouse_moved_when_pressed_ = false;
        mouse_pressed_region_ = -1;
      }
    }
    else if (btn == Qt::RightButton)
      RightMouseButton(e);
  }
}