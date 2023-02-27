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

#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkNew.h>

namespace rrm::fd::tasks
{
  class RecoveryMapTask
  {
    friend struct MapAtItem;
    friend struct TaskTypeItem;
    
    vtkNew<vtkActor> surface_fill_;
    vtkNew<vtkActor> isolines_;
    vtkNew<vtkActor2D> isolabels_;
    
    core::property_editor::QPropertyTreeView* task_view_;
    FDWidget* fd_widget_;
    std::unique_ptr<QWidget> recovery_map_widget_;



    
    struct
    {
      dpl::vector2i steps = {3, 3};
      int type = 0; //  0 - pvi, 1 - time, 2 - flow rate
      double at_pvi = 1;
      double at_time = units::Year(10);
      std::string format = /*6*/".1f";
    
    
      struct
      {
        struct sim_result_point_data_type
        {
          std::vector<dpl::vector2d> sweep;
          double total_flow_rate;
        };
    
        double pore_volume;
        dpl::vector2i steps = 0;
        std::vector<sim_result_point_data_type> points_data;
    
        double x0, y0, wx, wy, z;
      } sim_results;
    } task_params_;



    static auto WellFlowRate(RegularGrid& input_model, VerticalWell& well, double* pressure) {      
      auto [x, y, z] = input_model.Dims();           
      auto [dx, dy, dz] = input_model.CellSize();
      auto viscosity = input_model.viscosity;
      
      dpl::vector3i idx_map(1, x, x*y);

      auto discrete_well = input_model.Discretise(well);

      auto flow_rate = 0.0;
      
      for (auto k = discrete_well.z0; k <= discrete_well.z1; ++k) {
        auto cell_idx = idx_map.dot({discrete_well.x, discrete_well.y, k});
        auto perm = input_model[PERM][cell_idx];                         

        flow_rate += (well.pressure - pressure[cell_idx])*
          2*boost::math::constants::pi<double>()*sqrt(perm.x()*perm.y())*dz/
          (viscosity*log(VerticalWell::DrainageRadius(dx, dy, perm.x(), perm.y())/well.wellbore_radius));        
      }

      return flow_rate;
    }
    
  public:

    RecoveryMapTask(){}
    
    void BeginInit(FDWidget* fd_widget);

    std::unique_ptr<QWidget> AcquireWidget() {
      return std::move(recovery_map_widget_);
    }
    
    void EndInit();
    
    void UpdateRecoveryMap();
    void EvaluateCurrentTask();
    
    
    void HardRefreshTaskTree() {
      emit task_view_->model()->layoutChanged();
    }
  };
}
