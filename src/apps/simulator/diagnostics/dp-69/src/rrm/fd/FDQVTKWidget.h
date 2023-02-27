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

#include "dpl/colors.hpp"
#include "rrm/fd/FDWidget.h"
#include "rrm/fd/component/Volumetrics.h"

#if (VTK_MAJOR_VERSION == 8)
  #include <QVTKOpenGLWidget.h>
#elif (VTK_MAJOR_VERSION == 9)
  #include <QVTKOpenGLNativeWidget.h>
#endif

namespace rrm::fd
{
  inline constexpr auto Overhead = 20;
  
  #if (VTK_MAJOR_VERSION == 8)
    using FDQVTKWidgetBase = QVTKOpenGLWidget;
  #elif (VTK_MAJOR_VERSION == 9)
    using FDQVTKWidgetBase = QVTKOpenGLNativeWidget;
  #endif
  
  class FDQVTKWidget : public FDQVTKWidgetBase
  {
    FDWidget* fd_widget_;

    QMenu context_menu_3d_;
    QAction* action_ff_contact_;
    QAction* action_ff_contact_sep_;
    QAction* action_well_add_;
    QAction* action_well_add_remove_sep_;
    QAction* action_well_remove_;
    QAction* action_pressure_assign_;
    QAction* action_tof_reset_;
    QAction* action_tof_reset_all_;
    QAction* action_tof_assign_;
    QAction* action_switch_scale_;
    QAction* action_switch_scale_sep_;

    QAction* action_assign_value0_;
    QAction* action_assign_value1_;
    QAction* action_assign_pos0_;
    QAction* action_assign_pos1_;
    

    dpl::vector3d position_picked_;
    QPoint mouse_down_;
    bool scalar_bar_sampling_ = false;
    // // bool double_click_ = false;
    bool mouse_pressed_on_well_ = false;
    int mouse_pressed_region_ = -1;
    bool mouse_moved_when_pressed_ = false;

    bool mouse_over_show_ = false;
    vtkNew<vtkActor2D> mouse_over_actor_;

    vtkNew<vtkSphereSource> sphere_source_;
    vtkNew<vtkActor> sphere_actor_;

    void UpdatePerforation(VisualVerticalWell* visual, dpl::vector2i display_coord);
    void ViewportMouseEvent(QMouseEvent* e);
    void RightMouseButton(QMouseEvent* e);

    double scalar_bar_ratio_;
    void SampleFromScalarBarAndUpdate(double ratio);
    bool SampleScalarBar(dpl::vector2i pos, double& ratio);
    double SampleScalarBar(vtkLookupTable* lut) const;
    void SampleTofScalarBar();

    dpl::vector2i DisplayPosition(const QMouseEvent* e) const {
      return {e->x(), (fd_widget_->ViewportHeight() - 1) - e->y()};
    }

    void AddWellAtMouse(int x, int y);
    
  protected:
    bool event(QEvent* e) override {
      if (auto* mouse_event = dynamic_cast<QMouseEvent*>(e))
        ViewportMouseEvent(mouse_event);

      return FDQVTKWidgetBase::event(e);
    }
    
  public:
    bool MouseOverShow() { return mouse_over_show_; }
    void SetMouseOverShow(bool value) {
      mouse_over_show_ = value;
      if (!value)
        mouse_over_actor_->VisibilityOff();
    }
    
    
    void Init(FDWidget* fd_widget, component::Volumetrics* volumetrics) {
      fd_widget_ = fd_widget;



      {
        vtkNew<vtkTextMapper> mapper;      
        auto* tp = mapper->GetTextProperty();
        tp->SetColor(dpl::vector3d{1.0});
        tp->SetBackgroundColor(hsb_to_rgb(35, 70, 60));
        tp->SetBackgroundOpacity(1.0);
        tp->SetFontFamily(VTK_FONT_FILE);
        tp->SetFontFile(fd_widget_->RenderingSettings().font_path.string().c_str());      
        tp->SetFontSize(fd_widget_->RenderingSettings().visual.font_size);
        tp->SetLineSpacing(1.5);
        // text_property->SetJustificationToCentered();
        // text_property->SetVerticalJustificationToBottom();
        mouse_over_actor_->SetMapper(mapper);
        
        fd_widget_->Renderer()->AddActor2D(mouse_over_actor_);
        mouse_over_actor_->SetDisplayPosition(50, 50);
      }
      
      


      
      sphere_source_->SetRadius(3.0*
        VisualVerticalWell::VisualRadius(fd_widget_->Model(), fd_widget_->Scale())
      );
      sphere_source_->SetPhiResolution(10);
      sphere_source_->SetThetaResolution(10);
      sphere_source_->Update();


      vtkNew<vtkPolyDataMapper> mapper;
      mapper->SetInputData(sphere_source_->GetOutput());
      sphere_actor_->SetMapper(mapper);
      sphere_actor_->GetProperty()->SetColor(0.8, 0.8, 0.8);

      sphere_actor_->VisibilityOff();
      
      fd_widget_->Renderer()->AddActor(sphere_actor_);


      


      action_assign_pos0_ = context_menu_3d_.addAction("Set P0");
      action_assign_pos1_ = context_menu_3d_.addAction("Set P1");
      
      action_ff_contact_ = context_menu_3d_.addAction("Set Fluid-Fluid Contact");
      action_ff_contact_sep_ = context_menu_3d_.addSeparator();

      action_well_add_ = context_menu_3d_.addAction("Add Well");
      action_well_remove_ = context_menu_3d_.addAction("Remove Well");
      action_well_add_remove_sep_ = context_menu_3d_.addSeparator();

      action_switch_scale_ = context_menu_3d_.addAction("Switch Log/Linear");
      action_switch_scale_sep_ = context_menu_3d_.addSeparator();
      action_pressure_assign_ = context_menu_3d_.addAction("Assign to Current Well");

      action_assign_value0_ = context_menu_3d_.addAction("Set P0");
      action_assign_value1_ = context_menu_3d_.addAction("Set P1");
      
      action_tof_assign_ = context_menu_3d_.addAction("Set Threshold");
      action_tof_reset_ = context_menu_3d_.addAction("Reset Threshold");

      action_tof_reset_all_ = context_menu_3d_.addAction("Reset All Threshold");




      
      connect(action_ff_contact_, &QAction::triggered, [this, volumetrics] {
        volumetrics->PlaneActor()->SetVisibility(true);
        fd_widget_->HardRefreshPropertyTree(); // TODO:
        fd_widget_->RefreshCubeAxesBounds();   // Maybe not necessary
        volumetrics->SetDepth(position_picked_.z());
      });

      connect(action_well_add_, &QAction::triggered, [this] {
        auto pos = this->mapFromGlobal(/*QCursor::pos()*/context_menu_3d_.pos());
        /*static_cast<THIS_QVTKWidget*>(qvtk_widget_)*/this->AddWellAtMouse(pos.x(), pos.y());
      });

      connect(action_well_remove_, &QAction::triggered, [this] {
        fd_widget_->RemoveActiveWell();
      });

      connect(action_pressure_assign_, &QAction::triggered, [this] {
        fd_widget_->ActiveVisualWell()->input->pressure =
          SampleScalarBar(fd_widget_->CellInfo(PRESSURE).Lut());
        fd_widget_->PostPressureAssignUpdate();
      });


      
      {
        auto assigner_value = [this](bool is_value1) {
          return [this, is_value1] {
            if (fd_widget_->CellInfo(POROSITY).IsActive()) {
              auto* lut = fd_widget_->CellInfo(POROSITY).Lut();
              if (is_value1)
                fd_widget_->ActiveDomain()->grad.poro1 = SampleScalarBar(lut);
              else
                fd_widget_->ActiveDomain()->poro = SampleScalarBar(lut);
              fd_widget_->ActiveDomain()->grad.poro_log = lut->UsingLogScale();

              fd_widget_->PetrophysicsChanged({true, false});
            }
            else {
              PermeabilityCellInfo* p = nullptr;
              if (fd_widget_->CellInfo(PERM_X).IsActive())
                p = &fd_widget_->CellInfo(PERM_X);
              else if (fd_widget_->CellInfo(PERM_Y).IsActive())
                p = &fd_widget_->CellInfo(PERM_Y);
              else if (fd_widget_->CellInfo(PERM_Z).IsActive())
                p = &fd_widget_->CellInfo(PERM_Z);

              if (p) {
                auto* lut = p->Lut();

                if (is_value1)
                  fd_widget_->ActiveDomain()->grad.perm1[p->component] = SampleScalarBar(lut);
                else
                  fd_widget_->ActiveDomain()->perm[p->component] = SampleScalarBar(lut);
                fd_widget_->ActiveDomain()->grad.perm_log[p->component] = lut->UsingLogScale();

                fd_widget_->PetrophysicsChanged({false, true});
                
                // UpdateModelPoroPerm();
                //
                // p->DataArray()->Modified();
                // if (p->use_default_range)
                //   lut->SetTableRange(p->GetDefaultRange());
                //
                // UpdateFull(true);
              }
            }
          };
        };
        
        connect(action_assign_value0_, &QAction::triggered, assigner_value(false));
        connect(action_assign_value1_, &QAction::triggered, assigner_value(true));

        auto assigner_pos = [this, volumetrics](bool is_value1) {
          return [this, is_value1, volumetrics] {
            if (is_value1)
              fd_widget_->ActiveDomain()->grad.p1 = position_picked_;
            else
              fd_widget_->ActiveDomain()->grad.p0 = position_picked_;

            
            fd_widget_->UpdateModelPoroPerm();

            ContiniousCellInfo* props[4] {
              &fd_widget_->CellInfo(POROSITY),
              &fd_widget_->CellInfo(PERM_X), &fd_widget_->CellInfo(PERM_Y), &fd_widget_->CellInfo(PERM_Z)
            };

            for (auto* p : props) {
              p->DataArray()->Modified();
              p->TryUpdateDefaultLutRange();
            }

            volumetrics->RefreshVolumes();

            fd_widget_->UpdateFull(true);

          };
        };

        connect(action_assign_pos0_, &QAction::triggered, assigner_pos(false));
        connect(action_assign_pos1_, &QAction::triggered, assigner_pos(true));
      }
      


      
      

      connect(action_switch_scale_, &QAction::triggered, [this] {
        auto* active = fd_widget_->ActiveCellInfo();
        active->Lut()->SetScale(active->Lut()->GetScale() ? false : true);
        if (active == &fd_widget_->CellInfo(PRESSURE))
          fd_widget_->RefreshPerforationColours();

        fd_widget_->HardRefreshPropertyTree();
        fd_widget_->Render();
      });

      connect(action_tof_reset_, &QAction::triggered, [this] {
        if (fd_widget_->CellInfo(TOF_FORWARD).IsActive())
          fd_widget_->CellFilter().max_tof_forward = std::numeric_limits<double>::max();
        else if (fd_widget_->CellInfo(TOF_BACKWARD).IsActive())
          fd_widget_->CellFilter().max_tof_backward = std::numeric_limits<double>::max();
        else
          fd_widget_->CellFilter().max_tof_total = std::numeric_limits<double>::max();

        fd_widget_->HardRefreshPropertyTree();
        fd_widget_->UpdateVisual();
      });

      connect(action_tof_reset_all_, &QAction::triggered, [this] {
        fd_widget_->CellFilter().max_tof_forward = std::numeric_limits<double>::max();
        fd_widget_->CellFilter().max_tof_backward = std::numeric_limits<double>::max();
        fd_widget_->CellFilter().max_tof_total = std::numeric_limits<double>::max();

        fd_widget_->HardRefreshPropertyTree();
        fd_widget_->UpdateVisual();
      });

      connect(action_tof_assign_, &QAction::triggered, this, &FDQVTKWidget::SampleTofScalarBar);
    }
  };
}