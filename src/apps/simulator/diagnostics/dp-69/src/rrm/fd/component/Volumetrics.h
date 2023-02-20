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

#include <vtkPlaneSource.h>

namespace rrm::fd::component
{
  class Volumetrics
  {
    FDWidget* fd_widget_;
    
    vtkNew<vtkPlaneSource> plane_source_;
    vtkNew<vtkActor> plane_actor_;
    std::unordered_set<int> domains_;

    struct
    {
      double pore_above;
      double pore_below;
      double rock_above;
      double rock_below;
    } volume_;

    void InitFluidFluidContact();
  public:
    void SetDepth(double value);
    
    auto* PlaneActor() { return plane_actor_.Get(); }
    auto* PlaneSource() { return plane_source_.Get(); }
    auto& Domains() { return domains_; }
    
    void Init(FDWidget* w) {
      fd_widget_ = w;

      InitFluidFluidContact();
    }

    void RefreshVolumes();

    void InitPropertyTree();
  };
}
