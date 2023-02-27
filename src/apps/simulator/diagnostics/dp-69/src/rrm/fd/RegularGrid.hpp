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

#include "dpl/soa.hpp"
#include "dpl/static_vector.hpp"

#include "rrm/fd/General.hpp"
#include "rrm/fd/VerticalWell.hpp"

#include <vtkLine.h>

#ifdef DOMAIN
#undef DOMAIN
#endif

#define STATIC_KEY(name) \
  inline constexpr struct name##_TYPE {} name;

namespace rrm::fd
{
  // ReSharper disable CppInconsistentNaming
  STATIC_KEY(REGION)
  STATIC_KEY(DOMAIN)
  STATIC_KEY(POROSITY)
  STATIC_KEY(PERMEABILITY)
  STATIC_KEY(PRESSURE)
  STATIC_KEY(TOF_FORWARD)
  STATIC_KEY(TOF_BACKWARD)
  STATIC_KEY(TOF_TOTAL)
  STATIC_KEY(TRACER_FORWARD)
  STATIC_KEY(TRACER_BACKWARD)

  inline constexpr auto& PERM = PERMEABILITY;
  inline constexpr auto& PORO = POROSITY;

  STATIC_KEY(PERM_X)
  STATIC_KEY(PERM_Y)
  STATIC_KEY(PERM_Z)
  
  // STATIC_KEY(TRACER)

  // STATIC_KEY(DARCY_VEL) // horizontal, vertical
  STATIC_KEY(DARCY_VEL_H)
  // STATIC_KEY(DARCY_VEL_V)
  STATIC_KEY(GRAVITY_NO)

  namespace eval
  {
    template <typename CellData, typename Integral>
    auto TOF_TOTAL(const CellData& d, const Integral i) { return d[TOF_FORWARD][i] + d[TOF_BACKWARD][i]; }
  }
  // ReSharper restore CppInconsistentNaming
  
  using flow_diagnostics_soa = dpl::soa<
    TOF_FORWARD_TYPE, double,
    TOF_BACKWARD_TYPE, double,
    TRACER_FORWARD_TYPE, double,
    TRACER_BACKWARD_TYPE, double,
    DARCY_VEL_H_TYPE, double
  >;
  
  // All units are in SI system.   
   
  struct DiscreteWell {
    int x, y;   // lateral location
    int z0, z1; // perforation zone z0 < z < z1
  };


  template<typename Integral>
  dpl::vector3i Get3i(Integral idx, const dpl::vector3i& dims) {
    auto [x, y, z] = dims;
    auto xy = x*y;

    auto k = idx/xy;
    auto j = (idx - k*xy)/x;
    auto i = idx - k*xy - j*x;

    return {i, j, k};
  }
  
  class RegularGrid
  {
    dpl::vector3i dims_; // number of cells       
    dpl::vector3d location_ = 0;
    dpl::vector3d physical_size_ = 0;

    dpl::soa<
      REGION_TYPE, int,
      POROSITY_TYPE, double,
      PERMEABILITY_TYPE, dpl::vector3d,
      PRESSURE_TYPE, double,
      TOF_FORWARD_TYPE, double,
      TOF_BACKWARD_TYPE, double,
      TRACER_FORWARD_TYPE, double,
      TRACER_BACKWARD_TYPE, double,
      DARCY_VEL_H_TYPE, double
    > cell_data_;
   
    int outer_domain;

    PetrophysicalSettings* petphys_ = nullptr;
  public:
    
    dpl::units::pascal_second viscosity;

    DiscreteWell Discretise(const VerticalWell& well) const {
      auto [x, y, z] = dims_;           
      auto [dx, dy, dz] = physical_size_/dims_;

      using std::clamp;

      // NOLINTBEGIN(cppcoreguidelines-narrowing-conversions, clang-diagnostic-float-conversion)
      return {
        clamp<int>((well.head_location.x() - MinX())/dx, 0, x - 1),      
        clamp<int>((well.head_location.y() - MinY())/dy, 0, y - 1), 
        clamp<int>((well.head_location.z() - well.perf_top - well.perf_thickness - MinZ())/dz, 0, z - 1),
        clamp<int>((well.head_location.z() - well.perf_top                       - MinZ())/dz, 0, z - 1)                     
      };
      // NOLINTEND(cppcoreguidelines-narrowing-conversions, clang-diagnostic-float-conversion)
    }
    
    void SetDimensions(const dpl::vector3i& dims) {
      dims_ = dims;
      cell_data_.resize(dims.prod());
    }
    
    void SetPoroPermByDomains(PetrophysicalSettings& petrophysics, dpl::vector3d scale) {
      petphys_ = &petrophysics;
      outer_domain = petrophysics.domains.size() - 1;

      auto cell_size = physical_size_/dims_;

      for (int i = 0, cell_count = dims_.prod(); i < cell_count; i++) {
        auto pos = location_ + (Get3i(i, dims_) + 0.5)*cell_size; // TODO: can be optimised by 3-dim loops
        
        auto& domain = petrophysics.domains[Domain(i)];

        double t;

        vtkLine::DistanceToLine(pos*scale, domain.grad.p0*scale, domain.grad.p1*scale, t);
        
        if (domain.grad.poro1)
          cell_data_[POROSITY][i] = dpl::lerp<dpl::extrapolant::flat>(
            domain.grad.poro_log, domain.poro, *domain.grad.poro1, t);
        else
          cell_data_[POROSITY][i] = domain.poro;

        dpl::sfor<3>([&](auto dim) {
          if (domain.grad.perm1[dim])
            cell_data_[PERMEABILITY][i][dim] = dpl::lerp<dpl::extrapolant::flat>(
              domain.grad.perm_log[dim], domain.perm[dim], *domain.grad.perm1[dim], t);
          else
            cell_data_[PERMEABILITY][i][dim] = domain.perm[dim];
        });
        
      }
    }

    auto& Petrophysics() const {
      return *petphys_;
    }

    auto OuterDomain() const {
      return outer_domain;
    }
    

    // void SetViscocity(double viscocity) {
    //   viscosity_ = viscocity;
    // }

    // auto Viscosity() const { return viscosity_; }

    
    double MinX() const { return location_.x(); }
    double MinY() const { return location_.y(); }
    double MinZ() const { return location_.z(); }
    

    double MaxX() const { return location_.x() + physical_size_.x(); }
    double MaxY() const { return location_.y() + physical_size_.y(); }
    double MaxZ() const { return location_.z() + physical_size_.z(); }


    auto& Location() const { return location_; }
    void SetLocation(const dpl::vector3d& v) {
      location_ = v;
    }

    auto& PhysicalSize() const { return physical_size_; }
    void SetPhysicalSize(const dpl::vector3d& v) {
      physical_size_ = v;
    }
    
    auto& Dims() const { return dims_; }

    auto CellSize() const { return physical_size_/dims_; }
   
    auto CellCount() const {
      return dims_.prod();
    }

    template<typename Key>
    auto& operator[](Key k) const {
      return cell_data_[k];
    }

    // template<typename Integral>
    // auto& operator[](DOMAIN_TYPE, Integral i) const {
    //   return this->Domain(i);
    // }

    template<typename Integral>
    int Domain(Integral i) const {
      return petphys_->region_to_domain[cell_data_[REGION][i]];
    }

    // template<typename Integral>
    // auto TofTotal(Integral i) {
    //   return cell_data_[TOF_FORWARD][i] + cell_data_[TOF_BACKWARD][i];
    // }
  };
}
