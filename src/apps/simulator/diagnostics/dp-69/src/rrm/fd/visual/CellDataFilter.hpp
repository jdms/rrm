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

#include <unordered_set>

namespace rrm::fd::visual
{
  struct CellDataFilter
  {
    const RegularGrid& m;
    
    explicit CellDataFilter(const RegularGrid& mdl) : m{mdl} {}
    
    std::unordered_set<int> domains;
    
    dpl::units::second max_tof_forward = {std::numeric_limits<double>::max()};
    dpl::units::second max_tof_backward = {std::numeric_limits<double>::max()};
    dpl::units::second max_tof_total = {std::numeric_limits<double>::max()};

    int slice_dim = 0; // 0 - x, 1 - y, 2 - z
    double slice_value = std::numeric_limits<double>::max();

    // -1 - lower bound, 1 - upper bound, 0 - disabled
    dpl::vector3i cutoff_direction {0}; 
    dpl::vector3d cutoff_planes {0};
    
    unsigned char operator()(const vtkIdType idx) const {
      auto pos = m.Location() + (Get3i(idx, m.Dims()) + 0.5)*m.CellSize();

      bool any_cutoff = false;
      bool cutoff = true;
      
      dpl::sfor<3>([&](auto i) {
        if (cutoff_direction[i] != 0) {
          any_cutoff = true;
          
          cutoff &= cutoff_direction[i] > 0
            ? pos[i] <= cutoff_planes[i]
            : pos[i] >= cutoff_planes[i];
        }
      });

      if (any_cutoff && cutoff)
        return false;
      
      if (slice_value != std::numeric_limits<double>::max()) {
        int slice_idx = (slice_value - m.Location()[slice_dim])/m.CellSize()[slice_dim];  
        if (slice_idx != Get3i(idx, m.Dims())[slice_dim])
          return false;
      }

      return
        domains.count(m.Petrophysics().region_to_domain[m[REGION][idx]]) 
        && m[TOF_FORWARD][idx] <= max_tof_forward
        && m[TOF_BACKWARD][idx] <= max_tof_backward
        && eval::TOF_TOTAL(m, idx) <= max_tof_total
      ;
    }
  };
}