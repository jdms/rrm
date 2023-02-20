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

#include "rrm/fd/RegularGrid.hpp"
#include "rrm/fd/core/linear_solver/HypreInput.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/range/adaptor/reversed.hpp>

#undef pascal

namespace rrm::fd
{
   
  class FlowSolver
  {       
            
  public:
    template <typename Avg = HarmonicAverage>
    static linear_solver::SparseMatrix PressureCellMatrix(RegularGrid& model, const Avg& avg = Avg{}) {
      auto [x, y, z] = model.Dims();
      auto [dx, dy, dz] = model.CellSize();

      auto xy = x*y;

      linear_solver::SparseMatrix matrix{xy*z};            

      auto i_term = -(dy*dz)/model.viscosity/dx;
      auto j_term = -(dz*dx)/model.viscosity/dy;
      auto k_term = -(dx*dy)/model.viscosity/dz;

      const auto& perm = model[PERM];
      
      for (int k = 0, idx = 0; k < z; ++k) 
        for (int j = 0; j < y; ++j)        
          for (int i = 0; i < x; ++i, ++idx) {
            if (i < x - 1) {
              auto next_idx = idx + 1;
              matrix.AddDifferenceCoefs(idx, next_idx, avg(perm[idx].x(), perm[next_idx].x())*i_term);         
            }

            if (j < y - 1) {
              auto next_idx = idx + x;
              matrix.AddDifferenceCoefs(idx, next_idx, avg(perm[idx].y(), perm[next_idx].y())*j_term);     
            }

            if (k < z - 1) {
              auto next_idx = idx + xy;
              matrix.AddDifferenceCoefs(idx, next_idx, avg(perm[idx].z(), perm[next_idx].z())*k_term);     
            }
          }

      return matrix;
    }

    static void DarcyVelocityHorizontal(
      const RegularGrid& model,
      const double* pressure,
      const BoundaryConditions& bc,
      const std::list<VerticalWell>& wells,
      double* darcy_vel) {

      const auto& dims = model.Dims();
      auto [x, y, z] = dims;
      dpl::vector3i idx_map(1, x, x*y);
      
      {
        dpl::vector3i ijk{0};
        auto& [i, j, k] = ijk;
        i1d idx;

        // const auto& perm = model[PERM];

        auto term = -1.0/model.viscosity/model.CellSize();
        
        for (k = 0, idx = 0; k < z; ++k) 
          for (j = 0; j < y; ++j)        
            for (i = 0; i < x; ++i, ++idx) {
              dpl::vector3d comp;
              
              dpl::sfor<2 /*3*/>([&](auto dim) {
                // HarmonicAverage avg;
                //
                // darcy_vel[idx][dim] =
                //   
                //   ((
                //   ijk[dim] == 0
                //     ? perm[idx][dim]*(term[dim]*2)*(
                //       pressure[idx] - (bc.values[dim*2]/* && filter(idx)*/ ? *bc.values[dim*2] : pressure[idx]))
                //     : avg(perm[idx - idx_map[dim]][dim], perm[idx][dim])*term[dim]*(
                //       pressure[idx] - pressure[idx - idx_map[dim]])) +
                //
                //   (
                //   ijk[dim] == dims[dim] - 1
                //   ? perm[idx][dim]*(term[dim]*2)*(
                //       (bc.values[dim*2 + 1]/* && filter(idx)*/ ? *bc.values[dim*2 + 1] : pressure[idx]) - pressure[idx])
                //   : avg(perm[idx][dim], perm[idx + idx_map[dim]][dim])*term[dim]*(
                //       pressure[idx + idx_map[dim]] - pressure[idx])))/2;

                // darcy_vel[idx][dim] =
                //   ijk[dim] == 0
                //     ? perm[idx][dim]*(term[dim]*2)*(
                //       pressure[idx] - (bc.values[dim*2]/* && filter(idx)*/ ? *bc.values[dim*2] : pressure[idx]))
                //     : avg(perm[idx - idx_map[dim]][dim], perm[idx][dim])*term[dim]*(
                //       pressure[idx] - pressure[idx - idx_map[dim]]);

                // darcy_vel[idx][dim] = ijk[dim] == dims[dim] - 1
                //   ? perm[idx][dim]*(term[dim]*2)*(
                //       (bc.values[dim*2 + 1]/* && filter(idx)*/ ? *bc.values[dim*2 + 1] : pressure[idx]) - pressure[idx])
                //   : avg(perm[idx][dim], perm[idx + idx_map[dim]][dim])*term[dim]*(
                //       pressure[idx + idx_map[dim]] - pressure[idx]);

                comp[dim] =
                  model[PERM][idx][dim]*term[dim]*(
                    (ijk[dim] == dims[dim] - 1
                       ? (bc.values[dim*2 + 1] /*&& filter(idx) */? *bc.values[dim*2 + 1] : pressure[idx])
                       : (pressure[idx] + pressure[idx + idx_map[dim]])/2.0) -
                    (ijk[dim] == 0
                       ? (bc.values[dim*2] /*&& filter(idx)*/ ? *bc.values[dim*2] : pressure[idx])
                       : (pressure[idx - idx_map[dim]] + pressure[idx])/2.0));
              });
              
              darcy_vel[idx] = dpl::vector2d{comp[0], comp[1]}.length();
              // {
              //   dpl::vector2d{comp[0], comp[1]}.length(),
              //   std::abs(comp[2])
              // };
            }
      }

      {
        auto [dx, dy, dz] = model.CellSize();
        
        for (const auto& well : wells) {                    
          auto disc_well = model.Discretise(well);                    
          
          for (int k = disc_well.z0; k <= disc_well.z1; ++k) {                        
            i1d idx = idx_map.dot({disc_well.x, disc_well.y, k});
            auto perm = model[PERM][idx];
            auto r0 = VerticalWell::DrainageRadius(dx, dy, perm.x(), perm.y());
            
            darcy_vel[idx]/*[0]*/ +=
              abs(sqrt(perm.x()*perm.y())*(well.pressure - pressure[idx])/
                (r0*log(well.wellbore_radius/r0)*model.viscosity.value));
          }          
        }
      }
    }
    
    
    static void WellPressureSourceTerms(
      RegularGrid& model, std::list<VerticalWell>& wells,
      std::vector<double>& diag_coefs, std::vector<double>& constants) {
      auto [x, y, z] = model.Dims();           
      auto [dx, dy, dz] = model.CellSize();
      auto viscosity = model.viscosity;
      
      dpl::vector3i idx_map(1, x, x*y);

      for (auto& well : wells) {
        auto discrete_well = model.Discretise(well);

        for (auto k = discrete_well.z0; k <= discrete_well.z1; ++k) {
          auto cell_idx = idx_map.dot({discrete_well.x, discrete_well.y, k});

          auto perm = model[PERM][cell_idx];                         
          auto coef =
            (2*boost::math::constants::pi<double>()*sqrt(perm.x()*perm.y())*dz)
            /(viscosity.value*std::log(VerticalWell::DrainageRadius(dx, dy, perm.x(), perm.y())/well.wellbore_radius));                    

          diag_coefs[cell_idx] += coef;          
          constants[cell_idx] += coef*well.pressure;                                 
        }          
      }
    }


    template<int face/*, typename Filter*/>
    static double FaceFlowRate(RegularGrid& model, double pressure/*, const Filter& filter*/) {
      auto dims = model.Dims();           
      auto cell_size = model.CellSize();

      dpl::vector3i idx_map{1, dims.x(), dims.x()*dims.y()};


      // static constexpr auto i_rel = dpl::face_cubic<face>::dim;
      // static constexpr bool is_upper = dpl::face_cubic<face>::is_upper;

      
      auto i_rel = dpl::face_cubic<face>::dim;
      auto j_rel = i_rel.next();
      auto k_rel = j_rel.next();

      dpl::vector3i idx3d;

      auto& j = idx3d[j_rel];
      auto& k = idx3d[k_rel];

      auto j_count = dims[j_rel];
      auto k_count = dims[k_rel];
      
      idx3d[i_rel] = dpl::face_cubic<face>::is_upper ? dims[i_rel] - 1 : 0;

      double in_rate = 0.0;

      auto no_perm_coef = -(cell_size[j_rel]*cell_size[k_rel])/model.viscosity/(cell_size[i_rel]/2);   
      
      for (k = 0; k < k_count; ++k)
        for (j = 0; j < j_count; ++j) {
          auto owner_idx = idx_map.dot(idx3d);
          /*if (filter(owner_idx))*/
            in_rate -= (model[PRESSURE][owner_idx] - pressure)*-model[PERM][owner_idx][i_rel]*no_perm_coef;
        }

      return in_rate;
    }

    
    
    template<int dim, int face/*, typename Filter*/>
    static void FacePressureSourceTerms(
      RegularGrid& model, std::vector<double>& diag_coefs, std::vector<double>& constants,
      double pressure/*, const Filter& filter*/) {
      auto dims = model.Dims();           
      auto cell_size = model.CellSize();

      dpl::vector3i idx_map{1, dims.x(), dims.x()*dims.y()};

      static constexpr auto i_rel = dpl::sdim<3, dim>{};
      static constexpr auto j_rel = i_rel.next();
      static constexpr auto k_rel = j_rel.next();

      dpl::vector3i idx3d;

      auto& j = idx3d[j_rel];
      auto& k = idx3d[k_rel];

      auto j_count = dims[j_rel];
      auto k_count = dims[k_rel];

      idx3d[i_rel] = face == 0 ? 0 : dims[i_rel] - 1;

      auto no_perm_coef = -(cell_size[j_rel]*cell_size[k_rel])/model.viscosity/(cell_size[i_rel]/2);

      for (k = 0; k < k_count; ++k)
        for (j = 0; j < j_count; ++j) {
          auto owner_idx = idx_map.dot(idx3d);
          /*if (filter(owner_idx)) */{
            auto coef = model[PERM][owner_idx][i_rel]*no_perm_coef;

            diag_coefs[owner_idx] -= coef;          
            constants[owner_idx] -= coef*pressure;
          }
        }
    }
    
    template<int dim>
    static void FacePressureSourceTerms(
      RegularGrid& model, std::vector<double>& diag_coefs, std::vector<double>& constants) {
      auto dims = model.Dims();           
      auto cell_size = model.CellSize();

      dpl::vector3i idx_map{1, dims.x(), dims.x()*dims.y()};

      static constexpr auto i_rel = dpl::sdim<3, dim>{};
      static constexpr auto j_rel = i_rel.next();
      static constexpr auto k_rel = j_rel.next();

      dpl::vector3i idx3d;

      auto& i = idx3d[i_rel];
      auto& j = idx3d[j_rel];
      auto& k = idx3d[k_rel];

      auto i_last = dims[i_rel] - 1;
      auto j_count = dims[j_rel];
      auto k_count = dims[k_rel];

      auto no_perm_coef = -(cell_size[j_rel]*cell_size[k_rel])/model.viscosity/(cell_size[i_rel]/2);

      for (k = 0; k < k_count; ++k)
        for (j = 0; j < j_count; ++j) {
          {
            i = 0;

            auto owner_idx = idx_map.dot(idx3d);
            auto coef = model[PERM][owner_idx][i_rel]*no_perm_coef;

            diag_coefs[owner_idx] -= coef;          
            constants[owner_idx] -= coef/**inlet*/; // 1 Pascal
          }

          {
            i = i_last;

            auto owner_idx = idx_map.dot(idx3d);
            auto coef = model[PERM][owner_idx][i_rel]*no_perm_coef;

            diag_coefs[owner_idx] -= coef;          
            // constants[owner_idx] -= coef*outlet; // 0 Pressure
          }
        }
    }

    template <typename Pressure, typename Avg = HarmonicAverage>
    static auto TransportDomainMatrices(RegularGrid& model, const Pressure& p, const Avg& avg = Avg{}) {
      auto [x, y, z] = model.Dims();           
      auto [dx, dy, dz] = model.CellSize();
      auto visc = model.viscosity;

      auto xy = x*y;      
      
      std::tuple xyz_as_tuple{xy*z};
      
      std::pair<linear_solver::SparseMatrix, linear_solver::SparseMatrix> matrices{
        std::piecewise_construct, xyz_as_tuple, xyz_as_tuple
      };      
      
      auto& [mat_f, mat_b] = matrices;     

      size_t no_flow_faces = 0;
      
      // NOLINTBEGIN(clang-diagnostic-shorten-64-to-32)
      auto add_coefs_two_way = [&](auto idx, auto next_idx, auto coef) {
        if (coef == 0.0) {
          ++no_flow_faces;
        }
        else if (coef > 0) {
          mat_f.AddDiagCoef(idx, coef);              
          mat_f.SetOffDiagCoef(next_idx, idx, -coef);  

          mat_b.AddDiagCoef(next_idx, coef);
          mat_b.SetOffDiagCoef(idx, next_idx, -coef);   
        }
        else {
          mat_f.AddDiagCoef(next_idx, -coef);
          mat_f.SetOffDiagCoef(idx, next_idx, coef);

          mat_b.AddDiagCoef(idx, -coef);                                              
          mat_b.SetOffDiagCoef(next_idx, idx, coef); 
        }
      };
      // NOLINTEND(clang-diagnostic-shorten-64-to-32)

      
      // constexpr double dp_epsilon = 1.0*dpl::units::pascal{1};
      // constexpr double epsilon = 0;//1e-6; // 1 cm3/s

      const auto& perm = model[PERM];

      size_t idx = 0;
      for (int k = 0; k < z; ++k) 
        for (int j = 0; j < y; ++j)        
          for (int i = 0; i < x; ++i, ++idx) {            
            if (i < x - 1) {
              auto next_idx = idx + 1;

              add_coefs_two_way(idx, next_idx, 
                -avg(perm[idx].x(), perm[next_idx].x())/visc*(p[next_idx] - p[idx])/dx*(dy*dz));
            }

            if (j < y - 1) {
              auto next_idx = idx + x;
              
              add_coefs_two_way(idx, next_idx, 
                -avg(perm[idx].y(), perm[next_idx].y())/visc*(p[next_idx] - p[idx])/dy*(dz*dx));
            }

            if (k < z - 1) {
              auto next_idx = idx + xy;

              add_coefs_two_way(idx, next_idx,
                -avg(perm[idx].z(), perm[next_idx].z())/visc*(p[next_idx] - p[idx])/dz*(dx*dy));
            }
          }

      // if (no_flow_faces)
      //   std::cout << "no flow faces: " << no_flow_faces << '\n';
      
      return matrices;
    }

    static void TransportWellTerms(
      RegularGrid& model,
      const std::list<VerticalWell>& wells,
      const double* pressure,
      double* diag_for, double* tracer_src_for,
      double* diag_back, double* tracer_src_back) {

      auto [x, y, z] = model.Dims();           
      auto [dx, dy, dz] = model.CellSize();
      auto viscosity = model.viscosity;

      dpl::vector3i idx_map{1, x, x*y};

      int inj_well_idx = 0, prod_well_idx = 0;      

      for (const auto& well : wells) {                    
        auto disc_well = model.Discretise(well);                    

        if (well.pressure > pressure[idx_map.dot({disc_well.x, disc_well.y, disc_well.z0})])
          ++inj_well_idx;
        else
          ++prod_well_idx;

        for (int k = disc_well.z0; k <= disc_well.z1; ++k) {                        
          auto cell_idx = idx_map.dot({disc_well.x, disc_well.y, k});
          auto perm = model[PERM][cell_idx];

          auto rate =              
            (well.pressure - pressure[cell_idx])*
            -(2*boost::math::constants::pi<double>()*sqrt(perm.x()*perm.y())*dz)/
            (viscosity.value*log(VerticalWell::DrainageRadius(dx, dy, perm.x(), perm.y())/well.wellbore_radius));                    

          if (rate > 0) { // producer
            diag_for[cell_idx] += rate;
            tracer_src_back[cell_idx] += prod_well_idx*rate;   // Multiple wells in the same cell for accumulation
          }
          else { // injector  // Tracer BC                                                                                  
            tracer_src_for[cell_idx] -= inj_well_idx*rate;
            diag_back[cell_idx] -= rate;
          }            
        }          
      }
    }

    template<int face>
    static void TransportFaceTerms(
      RegularGrid& model,
      const double* cell_pressure,
      const double face_pressure,
      std::vector<double>& diag_forward, std::vector<double>& diag_backward) {
      auto dims = model.Dims();           
      auto cell_size = model.CellSize();

      dpl::vector3i idx_map{1, dims.x(), dims.x()*dims.y()};
      
      static constexpr auto i_rel = dpl::face_cubic<face>::dim;
      static constexpr auto j_rel = i_rel.next();
      static constexpr auto k_rel = j_rel.next();

      static constexpr bool is_upper = dpl::face_cubic<face>::is_upper;
      
      dpl::vector3i idx3d;

      auto& j = idx3d[j_rel];
      auto& k = idx3d[k_rel];

      auto j_count = dims[j_rel];
      auto k_count = dims[k_rel];

      idx3d[i_rel] = is_upper ? dims[i_rel] - 1 : 0;

      auto no_perm_coef = -(cell_size[j_rel]*cell_size[k_rel])/model.viscosity/(cell_size[i_rel]/2);

      for (k = 0; k < k_count; ++k)
        for (j = 0; j < j_count; ++j) {
          auto owner_idx = idx_map.dot(idx3d);
          /*if (filter(owner_idx))*/ {
            if constexpr (is_upper) {
              if (double rate = (face_pressure - cell_pressure[owner_idx])*model[PERM][owner_idx][i_rel]*no_perm_coef;
                rate > 0)
                diag_forward[owner_idx] += rate;
              else
                diag_backward[owner_idx] -= rate;
            }
            else {
              if (double rate = (cell_pressure[owner_idx] - face_pressure)*model[PERM][owner_idx][i_rel]*no_perm_coef;
                rate > 0)
                diag_backward[owner_idx] += rate;
              else
                diag_forward[owner_idx] -= rate;
            }
          }
        }
    }


    

    static auto TopologicalSort(const linear_solver::SparseMatrix& m) {
      using namespace boost;

      adjacency_list<listS, vecS, directedS> flux_graph(m.nrows);
      
      int row_idx = 0;
      for (auto& off_coefs : m.off_diag_coefs) {
        for (auto [off_coef_idx, _] : off_coefs)
          add_edge(off_coef_idx, row_idx, flux_graph);

        ++row_idx;
      }
            
      std::vector<int> topo_sorted(m.nrows);
      topological_sort(flux_graph, topo_sorted.rbegin());
      
      return topo_sorted;
    }


    static void EvaluateTransport(
      RegularGrid& model, std::list<VerticalWell>& wells, BoundaryConditions bc,
      double* pressure, flow_diagnostics_soa& fd, TimestepRecords& timesteps) {
      
      auto [mat_f, mat_b] = TransportDomainMatrices(model, pressure);
            
      timesteps <<
        "upwind matrix construction";

      std::vector<HYPRE_Int> topo_sort_f;
      
      try {
        topo_sort_f = TopologicalSort(mat_f);
      }
      catch (boost::not_a_dag&) {
        std::cout << "not DAG flow field\n";
      }
      catch (...) {
        std::cout << "topological sort exception\n";
      }

      timesteps << "flux graph construction & topological sort";


      auto cell_count = model.CellCount();
      std::vector<double> tracer_src_f(cell_count, 0);
      std::vector<double> tracer_src_b(cell_count, 0);

      auto& diag_f = mat_f.diagonal;
      auto& diag_b = mat_b.diagonal;

      TransportWellTerms(model, wells, pressure,
        diag_f.data(), tracer_src_f.data(), diag_b.data(), tracer_src_b.data());

      if (!bc.NoFlow()) {
        dpl::sfor<6>([&](auto i) {
          if (bc.values[i])
            FlowSolver::TransportFaceTerms<i>(model, pressure, *bc.values[i], diag_f, diag_b);
        });
        
        // auto add_face_terms = [&](auto filter) {
        //   dpl::sfor<6>([&, this](auto i) {
        //     if (bc.values[i])
        //       FlowSolver::TransportFaceTerms<i>(model, pressure, *bc.values[i], filter, diag_f, diag_b);
        //   });
        // };
        //
        // if (bc.include_outer)
        //   add_face_terms([](auto) { return true; });
        // else
        //   add_face_terms([&](auto i) { return model.Domain(i) != model.OuterDomain(); });
      }

      timesteps << "source terms";

      auto dv = model.CellSize().prod();


      // std::vector<dpl::vector3d> darcy_vel(cell_count);

      DarcyVelocityHorizontal(model, pressure, bc, wells, fd[DARCY_VEL_H].get());
      
      // if (bc.include_outer)
      //   DarcyVelocity(model, pressure, bc, [](auto) { return true; }, darcy_vel.data());
      // else
      //   DarcyVelocity(model, pressure, bc, [&](auto i) { return model.Domain(i) != model.OuterDomain(); }, darcy_vel.data());
      
      // for (decltype(cell_count) i = 0; i < cell_count; ++i) {
      //   fd[DARCY_VEL][i] = {
      //     dpl::vector2d{darcy_vel[i].x(), darcy_vel[i].y()}.length(),
      //     /*std::abs*/(darcy_vel[i].z())};
      //     // diag_f[i];
      //
      //   // auto diff = abs(diag_f[i] - diag_b[i]);
      //   // if (diff > 1e-7) {
      //   //   std::cout << diff << '\n';
      //   //   int p = 3;
      //   // }
      // }
      
      for (auto row_idx : topo_sort_f) {                           
        auto diag_coef = diag_f[row_idx]; // total outflux?

        if (diag_coef == 0) {
          std::cout << "ZERO DIAG";
          continue;
        }
                
        auto inflow_tracer = tracer_src_f[row_idx] /*source term*/; // free term 0
        auto inflow_tof = model[PORO][row_idx]*dv /*free term*/; // source term 0

        for (auto [off_idx, off_coef] : mat_f.GetOffDiagCoefs(row_idx)) {
          inflow_tracer -= off_coef*fd[TRACER_FORWARD][off_idx];
          inflow_tof -= off_coef*fd[TOF_FORWARD][off_idx];
        }                
                
        fd[TRACER_FORWARD][row_idx] = inflow_tracer/diag_coef;
        fd[TOF_FORWARD][row_idx] = inflow_tof/diag_coef;
      }
      

      for (auto row_idx : boost::adaptors::reverse(topo_sort_f)) {
        auto diag_coef = diag_b[row_idx];

        if (diag_coef == 0) {
          std::cout << "ZERO DIAG";
          continue;
        }

        auto inflow_tracer = tracer_src_b[row_idx];
        auto inflow_tof = model[PORO][row_idx]*dv;

        for (auto [off_idx, off_coef] : mat_b.GetOffDiagCoefs(row_idx)) {
          inflow_tracer -= off_coef*fd[TRACER_BACKWARD][off_idx];
          inflow_tof -= off_coef*fd[TOF_BACKWARD][off_idx];
        }

        fd[TRACER_BACKWARD][row_idx] = inflow_tracer/diag_coef;
        fd[TOF_BACKWARD][row_idx] = inflow_tof/diag_coef;
      }
      
      timesteps << "manual solution (summation)";              

      // timesteps >> cout;
    }

    

    static void EvaluateDiagnosticsCurves(const RegularGrid& model, FlowSummary& result) {            
      const auto cell_count = model.CellCount();

      result.lorenz.resize(cell_count + 1);
      result.sweep.resize(cell_count + 1);
      
      std::vector<int> idx_total(cell_count);
       
      const auto dv = model.CellSize().prod();
      
      double flow_capacity_total = 0.0;

      result.pore_volume = 0.0;
      result.pore_volume_excluding_outer = 0.0;
      
      for (int i = 0; i < cell_count; ++i) {
        const auto pv = model[PORO][i]*dv;
        result.pore_volume += pv;
        if (model.Domain(i) != model.OuterDomain())
          result.pore_volume_excluding_outer += pv;
        flow_capacity_total += pv/eval::TOF_TOTAL(model, i);

        idx_total[i] = i;
      }

      std::sort(idx_total.begin(), idx_total.end(), [&model](auto l_idx, auto r_idx) {
        return eval::TOF_TOTAL(model, l_idx) < eval::TOF_TOTAL(model, r_idx);
      });

      double storage_acc = 0.0;
      double flow_acc = 0.0;
      
      result.lorenz_coef = -1.0;

      result.lorenz[0] = 0;
      result.sweep[0] = 0;
      
      for (int i = 0; i < cell_count; ++i) {
        const auto j = idx_total[i];
        const auto pv = model[PORO][j]*dv;
        storage_acc += pv;
        flow_acc += pv/eval::TOF_TOTAL(model, j);

        result.lorenz[i + 1] = {storage_acc/result.pore_volume, flow_acc/flow_capacity_total};
        result.lorenz_coef +=
          (result.lorenz[i + 1].x() - result.lorenz[i].x())*
          (result.lorenz[i + 1].y() + result.lorenz[i].y());
      }

      for (auto i = 0; i < cell_count - 1; ++i) {                
        auto td = 1.0/(result.lorenz[i + 2] - result.lorenz[i + 1]).slope();        
        result.sweep[i + 1] = {td, result.lorenz[i + 1].x() + (1 - result.lorenz[i + 1].y())*td};
      }
      
      result.sweep[cell_count] = result.sweep[cell_count - 1];
      result.sweep[cell_count].x() += 1e-12;
    }
  };










  


  

  class PressureSolveTask : public AbstractTask
  {
  protected:
    TimestepRecords timesteps_{"background task", false};

    FDWidget* widget_;

    linear_solver::HypreInput input_;
    std::vector<HYPRE_Real> pressure_;

    void Execute() override {
      timesteps_.Record();      
      pressure_ = input_.Solve();
      timesteps_ << "pressure solve";
    }

    bool Acceptable() {
      return widget_->Model().CellCount() == pressure_.size();
    }

  public:

    PressureSolveTask(FDWidget* const widget, linear_solver::HypreInput&& input)
      : widget_(widget),
      input_(std::move(input)) {}

    // explicit PermeabilityTask(FDWidget* const widget) : widget_(widget) {}
  };








  template<int dim>
  class DirectionalPressureTask : public PressureSolveTask
  {
    double UpscalePermeability() {
      auto& model = widget_->Model();
      
      auto dims = model.Dims();           

      dpl::vector3i idx_map{1, dims.x(), dims.x()*dims.y()};

      static constexpr auto i_rel = dpl::sdim<3, dim>{};
      static constexpr auto j_rel = i_rel.next();
      static constexpr auto k_rel = j_rel.next();

      dpl::vector3i idx3d;

      idx3d[i_rel] = 0;
      auto& j = idx3d[j_rel];
      auto& k = idx3d[k_rel];

      // auto i_last = dims[i_rel] - 1;
      auto j_count = dims[j_rel];
      auto k_count = dims[k_rel];

      double in_rate = 0.0;

      
      for (k = 0; k < k_count; ++k)
        for (j = 0; j < j_count; ++j) {
          auto owner_idx = idx_map.dot(idx3d);
          in_rate -= (pressure_[owner_idx] - 1.0)*input_.constants[owner_idx];
        }
      
      const auto& phys_size = model.PhysicalSize();
      
      return -in_rate*model.viscosity*phys_size[i_rel]/(phys_size[j_rel]*phys_size[k_rel]*(0.0 - 1.0));
    }
    
    void Commit() override {
      // return;
      
      if (!Acceptable())
        return;

      widget_->GetFlowSummary().upscaled_perm[dim] = UpscalePermeability();
      widget_->HardRefreshPropertyTree();
      

      // if constexpr (FDWidget::direction_pressure_field) {
      //   widget_->PressureDirectionalProperty<dim>().data = std::move(pressure_);
      //   widget_->PressureDirectionalProperty<dim>().Refresh();
      //
      //   widget_->UpdateImageDataAndThreshold(/*false*/); //_TODO cannot be false
      // }

      widget_->Render();
    }

  public:

    DirectionalPressureTask(FDWidget* const widget, linear_solver::HypreInput&& input)
      : PressureSolveTask(widget, std::move(input)) {}
  };


  
  
  class WellPressureTask : public PressureSolveTask
  {
    size_t bound_cond_timestamp_;
    
    flow_diagnostics_soa fd_;

    // std::unordered_map<VerticalWell*, WellSimulationResult> result_;
    
  public:
  // protected:
    void Execute() override {
      if (/*true */widget_->bound_cond_timestamp_ == bound_cond_timestamp_) {
        PressureSolveTask::Execute();       

        fd_.resize(widget_->Model().CellCount());
        for (auto i = 0, cell_count = widget_->Model().CellCount(); i < cell_count; ++i) {
          fd_[TOF_FORWARD][i] = std::numeric_limits<double>::max();
          fd_[TOF_BACKWARD][i] = std::numeric_limits<double>::max();
          fd_[TRACER_FORWARD][i] = std::numeric_limits<double>::max();
          fd_[TRACER_BACKWARD][i] = std::numeric_limits<double>::max();
          // model[DARCY_VEL][i] = fd_[DARCY_VEL][i];
        }
        
        // fd_.assign(widget_->Model().CellCount(), std::numeric_limits<double>::max());        
        
        FlowSolver::EvaluateTransport(widget_->Model(), widget_->wells_, widget_->boundary_cond_, pressure_.data(), fd_, timesteps_);
      }
    }

  // private:
    void Commit() override {
      if (!Acceptable() || widget_->bound_cond_timestamp_ != bound_cond_timestamp_)
        return;

      timesteps_ << "--\n  begin commit";            

      auto& model = widget_->Model();

      for (auto i = 0, cell_count = model.CellCount(); i < cell_count; ++i) {
        model[PRESSURE][i] = pressure_[i];
        model[TOF_FORWARD][i] = fd_[TOF_FORWARD][i];
        model[TOF_BACKWARD][i] = fd_[TOF_BACKWARD][i];
        model[TRACER_FORWARD][i] = fd_[TRACER_FORWARD][i];
        model[TRACER_BACKWARD][i] = fd_[TRACER_BACKWARD][i];
        model[DARCY_VEL_H][i] = fd_[DARCY_VEL_H][i];
      }

      auto inj_count = 0, prod_count = 0;
      {
        auto idx_map = dpl::vector3i{1, model.Dims().x(), model.Dims().x()*model.Dims().y()};
        auto [dx, dy, dz] = model.CellSize();

        widget_->flow_summary_.well_results.clear();
        
        for (auto& well : widget_->wells_) {                    
          auto disc_well = model.Discretise(well);                    

          auto breakthrough_time = std::numeric_limits<double>::max();

          dpl::vector3i ijk;
          auto flow_rate_sign = well.pressure - pressure_[idx_map.dot({disc_well.x, disc_well.y, disc_well.z0})];
          auto update_breakthrough_time = [&] {
            breakthrough_time = 
              std::min(breakthrough_time, 
                (flow_rate_sign < 0 ? model[TOF_FORWARD] : model[TOF_BACKWARD])[idx_map.dot(ijk)]);
          };
          
          {
            auto [x, y, z] = model.Dims();
            auto i_count = std::min(x, disc_well.x + 2);
            auto j_count = std::min(y, disc_well.y + 2);
            
            for (ijk.z() = disc_well.z0; ijk.z() <= disc_well.z1; ++ijk.z()) 
            for (ijk.x() = std::max(0, disc_well.x - 1); ijk.x() < i_count; ++ijk.x())
            for (ijk.y() = std::max(0, disc_well.y - 1); ijk.y() < j_count; ++ijk.y()) {
              if (std::abs(ijk.x() - disc_well.x) + std::abs(ijk.y() - disc_well.y) < 2)
                update_breakthrough_time();
            }
          
            ijk.x() = disc_well.x;
            ijk.y() = disc_well.y;
            
            ijk.z() = std::max(0, disc_well.z0 - 1);
            update_breakthrough_time();
          
            ijk.z() = std::min(z, disc_well.z1 + 2) - 1;
            update_breakthrough_time();
          }
          
          
          auto flow_rate = 0.0;
          for (auto k = disc_well.z0; k <= disc_well.z1; ++k) {
            auto cell_idx = idx_map.dot({disc_well.x, disc_well.y, k});
            auto perm = model[PERM][cell_idx];                         
            auto coef =
              (2*boost::math::constants::pi<double>()*sqrt(perm.x()*perm.y())*dz)
              /(model.viscosity*log(VerticalWell::DrainageRadius(dx, dy, perm.x(), perm.y())/well.wellbore_radius));                    

            flow_rate += (well.pressure - pressure_[cell_idx])*coef;
          }

          widget_->flow_summary_.well_results[&well] = {
            flow_rate,
            breakthrough_time
          };

          if (flow_rate < 0)
            ++prod_count;
          else
            ++inj_count;
        }
      }


      auto& bc = widget_->boundary_cond_;


      dpl::sfor<6>([&, this](auto i) {
        widget_->flow_summary_.face_flow_rate[i] = 0;
      });
      
      if (!bc.NoFlow()) {
        dpl::sfor<6>([&, this](auto i) {
          if (bc.values[i])
            widget_->flow_summary_.face_flow_rate[i] = FlowSolver::FaceFlowRate<i>(model, *bc.values[i]);
        });
        
        // if (
        //   auto add_face_source_terms = [&, this](const auto& filter) {
        //     dpl::sfor<6>([&, this](auto i) {
        //       if (bc.values[i]) {
        //         auto flow_rate = FlowSolver::FaceFlowRate<i/2, i%2>(model, *bc.values[i], filter);
        //
        //         widget_->flow_summary_.face_flow_rate[i] = flow_rate;
        //       }
        //     });
        //   };
        //   bc.include_outer)
        //   add_face_source_terms([](auto) { return true; });
        // else
        //   add_face_source_terms([&, this](auto i) {
        //     return model.Domain(i) != model.OuterDomain();
        //   });
      }

      // widget_->flow_summary_.well_results = std::move(result_);
      
      timesteps_ << "properties set";
      
      widget_->cell_info_[TOF_FORWARD].TryUpdateDefaultLutRange();
      widget_->cell_info_[TOF_BACKWARD].TryUpdateDefaultLutRange();
      widget_->cell_info_[TOF_TOTAL].TryUpdateDefaultLutRange();
      widget_->cell_info_[DARCY_VEL_H].TryUpdateDefaultLutRange();
      // widget_->cell_info_[DARCY_VEL_V].TryUpdateDefaultLutRange();
      widget_->cell_info_[GRAVITY_NO].TryUpdateDefaultLutRange();
      
      widget_->cell_info_[TRACER_FORWARD].Lut()->SetTableRange(inj_count == 0 ? 0 : 0.999, inj_count);
      widget_->cell_info_[TRACER_BACKWARD].Lut()->SetTableRange(prod_count == 0 ? 0 : 0.999, prod_count);

      timesteps_ << "tof modified & tracer range";
      

      {
        FlowSolver::EvaluateDiagnosticsCurves(model, widget_->flow_summary_);

        // 1 m3/s = 543439.65056533 bbl/day 

        widget_->flow_summary_.total_inj_flow_rate = 0;
        for (auto& [_, wr] : widget_->flow_summary_.well_results)
          if (!wr.IsProducer())
            widget_->flow_summary_.total_inj_flow_rate += wr.flow_rate;

        for (const auto flow_rate : widget_->flow_summary_.face_flow_rate)
          if (flow_rate > 0)
            widget_->flow_summary_.total_inj_flow_rate += flow_rate;
      }


      
      widget_->FlowDiagnosticsChanged();
      
      widget_->UpdateVisual();
      
      timesteps_ << "UpdateVisual & FlowDiagnosticsChanged";
      
      widget_->HardRefreshPropertyTree();

      timesteps_ << "HardRefreshPropertyTree";

      timesteps_ >> std::cout << std::endl;
    }

    WellPressureTask(FDWidget* const widget, linear_solver::HypreInput&& input)
      : PressureSolveTask(widget, std::move(input)), bound_cond_timestamp_(widget->bound_cond_timestamp_) {}
  };
  
}
