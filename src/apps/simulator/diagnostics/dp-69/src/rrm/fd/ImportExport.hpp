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
#include "rrm/fd/core/property_editor/PropertyEntities.hpp"
#include "rrm/fd/core/QExtendedPieChartView.hpp"

#include <QLineSeries>

namespace rrm::fd
{
  
  inline void ExportReport(
    std::list<VerticalWell>& wells,
    std::unordered_map<VerticalWell*, WellSimulationResult>& well_results,
    QtCharts::QPieSeries* inj,
    QtCharts::QPieSeries* prod,
    QtCharts::QLineSeries* lorenz,
    QtCharts::QLineSeries* pvipvp,
    core::property_editor::PropertyCategory* summary,
    double lorenz_coef,
    dpl::vector3d ups_perm,
    std::string filename) {

    std::ofstream file(filename);

    auto prec2 = setprecision(2);
    
    {
      std::stringstream names, x, y, z, perf_top, perf_base, pressure;

      names << ',';
      x << "\"X, m\",";
      y << "\"Y, m\",";
      perf_top << "\"Perforation top, m\",";
      perf_base << "\"Perforation base, m\",";
      pressure << "\"Pressure, bar\",";


      // auto k = std::fixed << prec;
      
      
      for (auto& w : wells) {
        names << w.name << ','; 
        x << std::fixed << prec2 << w.head_location.x() << ',';
        y << std::fixed << prec2 << w.head_location.y() << ',';
        perf_top << std::fixed << prec2 << w.head_location.z() - w.perf_top << ',';
        perf_base << std::fixed << prec2 << w.head_location.z() - w.perf_top - w.perf_thickness << ',';
        pressure << std::fixed << prec2 << w.pressure/units::Bar() << ',';
      }

      file << names.str() << '\n'
           << x.str() << '\n'
           << y.str() << '\n'
           << perf_top.str() << '\n'
           << perf_base.str() << '\n'
           << pressure.str() << '\n';
    }

      
    {
      {
        std::stringstream inj_ratio;
        

        file << '\n'
             << "Injector tracer";
          
        inj_ratio << "Proportion";

        for (auto* slice : inj->slices()) {
          file << ',' << static_cast<core::QCustomPieSlice*>(slice)->name;//static_cast<VerticalWell*>(static_cast<QCustomPieSlice*>(slice)->data)->name;
          inj_ratio << ',' << std::fixed << prec2 << slice->percentage();
        }
        
        file << '\n' << inj_ratio.str() /*<< '\n'*/;
      }

      {
        file << "\n\"Flow rate, Mbbl/day\"";

        for (auto& w : wells) {
          auto& r = well_results[&w];
          if (!r.IsProducer())
            file << ',' << boost::format("%.2f") % std::abs(r.flow_rate/units::Barrel(1e3)*units::Day());
        }
      }

      {
        file << "\n\"Breakthrough, yrs\"";

        for (auto& w : wells) {
          auto& r = well_results[&w];
          if (!r.IsProducer())
            file << ',' << boost::format("%.2f") % (r.breakthrough_time/units::Year());
        }
      }

      file << '\n';
    }


    {
      {
        std::stringstream prod_ratio;
        
        file << '\n'
             << "Producer tracer";

        prod_ratio << "Proportion";

        for (auto* slice : prod->slices()) {
          file << ',' << static_cast<core::QCustomPieSlice*>(slice)->name;//static_cast<VerticalWell*>(static_cast<QCustomPieSlice*>(slice)->data)->name;
          prod_ratio << ',' << std::fixed << prec2 << slice->percentage();
        }

        file << "\n" << prod_ratio.str() /*<< '\n'*/;

      }
      
      {
        file << "\n\"Flow rate, Mbbl/d\"";

        for (auto& w : wells) {
          auto& r = well_results[&w];
          if (r.IsProducer())
            file << ',' << boost::format("%.2f") % std::abs(r.flow_rate/units::Barrel(1e3)*units::Day());
        }
      }

      {
        file << "\n\"Breakthrough, yrs\"";

        for (auto& w : wells) {
          auto& r = well_results[&w];
          if (r.IsProducer())
            file << ',' << boost::format("%.2f") % (r.breakthrough_time/units::Year());
        }
      }

      file << '\n';
    }


    file << "\n";
    
    for (auto* x : summary->children)
      if (auto* item = dynamic_cast<core::property_editor::PropertyItem*>(x))
        file << "\"" << item->Name() << "\",\""
          << item->GetDisplayValueQVariant(Qt::DisplayRole).toString().toStdString() << "\"\n";

    // {
    //   file << '\n'
    //        << ",\"Kx, mD\",\"Ky, mD\",\"Kz, mD\",,Lorenz coef\n"
    //        << ',' << std::fixed << prec2 << ups_perm.x()/units::MilliDarcy()
    //        << ',' << std::fixed << prec2 << ups_perm.y()/units::MilliDarcy()
    //        << ',' << std::fixed << prec2 << ups_perm.z()/units::MilliDarcy()
    //        << ",," << lorenz_coef << '\n';
    // }
    
    {
      file << std::fixed << setprecision(6)
           << '\n'
           << "Production,PVI,PVP,,,Storage capacity,Flow Capacity\n";

      auto count = std::max(pvipvp->points().size(), lorenz->points().size());
      
      for (auto i = 0; i < count; ++i) {
        if (i < pvipvp->points().size())
          file << ',' << pvipvp->points()[i].x() << ',' << pvipvp->points()[i].y();
        else
          file << ",,";

        file << ",,";
        
        if (i < lorenz->points().size())
          file << ',' << lorenz->points()[i].x() << ',' << lorenz->points()[i].y();
        else
          file << ",,";

        file << '\n';
      }
    }
  }

  inline void ExportPetrelWellHeads(std::list<VerticalWell>& wells, std::string filename) {
    std::ofstream file(filename);

    file << 
R"(# Petrel well head
VERSION 1
BEGIN HEADER
Name
UWI
Well symbol
Surface X
Surface Y
KB
TD (MD)
Cost
Spud date
Simulation Name
Operator
TWT auto
FLOAT,Well datum value
FLOAT,Ambient temperature
FLOAT,Heat transfer coefficient
END HEADER
)";

    for (auto& w : wells) {
      file << boost::format("\"%s\" \"\" 3 %d %d %d %d -999 \"NULL\" \"\" \"\" -999 -999 -999 -999\n")
        % w.name
        % w.head_location.x() % w.head_location.y() % w.head_location.z()
        % (w.perf_top + w.perf_thickness);
    }
  }


  /**
   * \brief Export ECLIPSE ASCII grid | GRDECL
   */
  inline void ExportGRDECL(RegularGrid& model, std::string filename) {
    std::ofstream file(filename);

    file << "GRIDUNIT\n  METRES /\n\n";

    auto dims = model.Dims();

    auto add = [&file](auto vec) { file << vec[0] << " " << vec[1] << " " << vec[2] << " "; };

    file << "SPECGRID\n  ";
    add(dims);
    // file << "/\n\nCOORD";


    file << "/\n\n";
    
    file << boost::format("DX\n  %i*%f /\n\n") % dims.prod() % model.CellSize().x();
    file << boost::format("DY\n  %i*%f /\n\n") % dims.prod() % model.CellSize().y();
    file << boost::format("DZ\n  %i*%f /\n\n") % dims.prod() % model.CellSize().z();
    
    file << "COORD";

    auto [dx, dy, dz] = model.CellSize();

    auto top_depth = 0;//-cartesian_model_.PhysicalSize().z();
    auto bottom_depth = top_depth + model.PhysicalSize().z();
    
    for (auto j = 0; j <= dims.y(); ++j)
      for (auto i = 0; i <= dims.x(); ++i) {
        file << "\n  ";
        add(dpl::vector3d{/*min_x + */i*dx, /*min_y + */j*dy, bottom_depth});
        file << "\n  ";
        add(dpl::vector3d{/*min_x + */i*dx, /*min_y + */j*dy, top_depth});
      }

    file << "\n  /\n\n";

    file << "ZCORN";

    for (auto k = 0; k < dims.z(); ++k) {
      file << "\n  " << 4*dims.x()*dims.y() << "*" << top_depth + k*dz
        << "\n  " << 4*dims.x()*dims.y() << "*" << top_depth + (k + 1)*dz;
    }

    dpl::vector3i map{1, dims.x(), dims.x()*dims.y()};

    auto add_property = [&file, &map, &dims](const char* name, auto func) {
      file << "\n  /\n\n";

      file << name;

      for (auto k = 0; k < dims.z(); ++k)
        for (auto j = 0; j < dims.y(); ++j)
          for (auto i = 0; i < dims.x(); ++i)
            file << "\n  " <<  func(map.dot({i, j, (dims.z() - 1) - k}));
    };


    add_property("PORO", [&](auto idx) { return model[POROSITY][idx]; });
    add_property("PERMX", [&](auto idx) { return model[PERMEABILITY][idx].x()/units::MilliDarcy(); });
    add_property("PERMY", [&](auto idx) { return model[PERMEABILITY][idx].y()/units::MilliDarcy(); });
    add_property("PERMZ", [&](auto idx) { return model[PERMEABILITY][idx].z()/units::MilliDarcy(); });
    add_property("PRESSURE", [&](auto idx) { return model[PRESSURE][idx]/units::Bar();} );

    add_property("OPERNUM", [&](auto idx) {
      auto domain = model.Domain(idx);
      return domain == model.OuterDomain() ? 0 : domain + 1;
      });

    add_property("ACTNUM", [&](auto idx) {
      auto domain = model.Domain(idx);
      return domain == model.OuterDomain() ? 0 : 1;
    });

    file << "\n  /";
  }
}
