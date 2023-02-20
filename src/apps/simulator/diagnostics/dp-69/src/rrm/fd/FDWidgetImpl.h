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

#include "rrm/fd/FDWidget.h"
#include "rrm/fd/component/DomainPartitioning.hpp"
#include "rrm/fd/component/LorenzPlot.hpp"
#include "rrm/fd/component/SweepPlot.hpp"
#include "rrm/fd/component/WellPartitioning.hpp"
#include "rrm/fd/component/Volumetrics.h"
// #include "rrm/fd/component/RecoveryMapTask.h"

namespace rrm::fd
{
  class FDWidgetImpl final : public FDWidget
  {
  // Q_OBJECT  

    component::LorenzPlot lorenz_plot_;
    component::SweepPlot sweep_plot_;
    component::DomainPartitioning domain_partitioning_;
    component::WellPartitioning well_partitioning_;
    component::Volumetrics volumetrics_;

    // QMenu context_menu_plot_; // TODO CHECK?

    // double scalar_bar_ratio_;
    // void SampleFromScalarBarAndUpdate(double ratio);
    // bool SampleScalarBar(dpl::vector2i pos, double& ratio);
    // double SampleScalarBar(vtkLookupTable* lut) const;
    // void SampleTofScalarBar();
    
    // dpl::vector3d position_picked_;
    // QPoint mouse_down_;
    // bool scalar_bar_sampling_ = false;
    // bool double_click_ = false;
    // bool mouse_pressed_on_well_ = false;
    // int mouse_pressed_region_ = -1;
    // bool mouse_moved_when_pressed_ = false;
    // void ViewportMouseEvent(QMouseEvent* e);
    // void UpdatePerforation(VisualVerticalWell* visual, dpl::vector2i display_coord);
    
    int screenshot_number_ = 0;
    void SaveScreenshot();
    void ExportWellsDialog();
    void ExportGridDialog();
    void LoadWellsDialog();
    void LoadWells(const nlohmann::json& j);
    
    
    
    
    void InitModel();
    void InitCompass();
    void InitMainMenu();
    
    void keyPressEvent(QKeyEvent* e) override;
    bool initialised_after_shown_ = false;
    void changeEvent(QEvent* event) override;
    void closeEvent(QCloseEvent* event) override {
      if (close_event_)
        close_event_();
    }
    
  public:
    void BeginInit();
    void EndInit();

     std::function<void()> close_event_;
  };
}
