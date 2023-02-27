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

#include <optional>

#include <nlohmann/json.hpp>

#include "dpl/static_vector.hpp"
#include "dpl/units.hpp"

namespace rrm::fd 
{
  struct BoundaryConditions {
    // bool include_outer = true;

    /**
     * \brief x_min, x_max, y_min, ...
     */
    std::array<std::optional<dpl::units::pascal>, 6> values;

    bool NoFlow() {
      for (int i = 0; i < 6; ++i)
        if (values[i])
          return false;

      return true;
    }
  };
  
  struct VerticalWell
  {
    std::string name;

    dpl::vector3d head_location;

    
    /**
     * \brief Measured depth from well head location    
     */
    double perf_top;    
    double perf_thickness;


    dpl::units::metre wellbore_radius;  // wellbore radius
    dpl::units::pascal pressure; // bottomhole (flowing) pressure


        
    
    
    /**
     * Schwabe, K. and J. Brand (1967). "Prediction of Reservoir Behavior Using Numerical Simulators." Journal of Petroleum Technology 19(9): 1163-&.
     *   
     * Peaceman, D. W. (1983). Interpretation of Well-Block Pressures in Numerical Reservoir Simulation with Non-Square Grid Blocks and Anisotropic Permeability
     *
     */    
    static auto DrainageRadius(double dx, double dy, double kx, double ky) {
      const auto sqrt_kykx = sqrt(ky/kx);
      const auto sqrt_kxky = sqrt(kx/ky);
      
      return 0.28*sqrt(sqrt_kykx*dx*dx+sqrt_kxky*dy*dy)/(sqrt(sqrt_kykx) + sqrt(sqrt_kxky));
    }






    void from_json(const nlohmann::json& json) {
      name = json["name"];

      json["location"].get_to(head_location);

      {
        auto& x = json["perforation_md"];
        x[0].get_to(perf_top);
        x[1].get_to(perf_thickness);
      }

      json["wellbore_radius"].get_to(wellbore_radius);

      json["pressure"].get_to(pressure);
    }

    nlohmann::json to_json() {
      return {
        {"name", name},
        {"location", {
          head_location.x(),
          head_location.y(),
          head_location.z()}},
        {"perforation_md", {perf_top, perf_thickness}},
        {"wellbore_radius", wellbore_radius/*.value*/},
        {"pressure", pressure}
      };
    }
  };






  struct WellSimulationResult
  {
    // bool is_producer;
    double flow_rate;
    double breakthrough_time;

    bool IsProducer() {
      return flow_rate < 0;
    }
    // double tracer_fraction;    
  };
  
}
