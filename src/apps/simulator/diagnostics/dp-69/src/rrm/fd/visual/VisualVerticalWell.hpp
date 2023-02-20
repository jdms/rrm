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

#include "rrm/fd/VerticalWell.hpp"
#include "rrm/fd/RegularGrid.hpp"

#include <vtkRenderer.h>
#include <vtkNew.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkNamedColors.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor2D.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkDoubleArray.h>

#include <QString>



namespace rrm::fd
{
  class VisualVerticalWell
  {
    static constexpr double caption_overhead_ = 5;
    dpl::vector3d points_[3];

    dpl::vector3d scale_;
  
  public:
    static double VisualRadius(const RegularGrid& model, const dpl::vector3d& scale) {
      return sqrt(
        scale.x()*model.PhysicalSize().x()*
        scale.y()*model.PhysicalSize().y())/250.;
    }
    

    
    vtkNew<vtkActor2D> caption_actor;


    vtkNew<vtkDoubleArray> above_perf_array;   
    vtkNew<vtkTubeFilter> above_perf_tube_filter;
    vtkNew<vtkActor> above_perf_actor;

    vtkNew<vtkDoubleArray> perf_array;
    vtkNew<vtkTubeFilter> perf_tube_filter;
    vtkNew<vtkActor> perf_actor;

    VerticalWell* input;
    


    inline static auto DefaultTextOpacity = 0.75;
    
    void SetScale(dpl::vector3d scale, double visual_well_radius) {
      scale_ = scale;
      
      auto x = scale.x()*input->head_location.x();
      auto y = scale.y()*input->head_location.y();
      
      auto d0 = scale.z()*(input->head_location.z()/* + Overhead*/);
      auto d1 = scale.z()*(input->head_location.z() - input->perf_top);
      auto d2 = scale.z()*(input->head_location.z() - input->perf_top - input->perf_thickness);

      caption_actor->GetPositionCoordinate()->SetValue(x, y, d0 + caption_overhead_);

      points_[0] = {x, y, d0};
      points_[1] = {x, y, d1};
      points_[2] = {x, y, d2};

      above_perf_tube_filter->SetRadius(visual_well_radius);
      perf_tube_filter->SetRadius(1.75*visual_well_radius);
      
      above_perf_tube_filter->GetInputAlgorithm()->Modified();
      perf_tube_filter->GetInputAlgorithm()->Modified();
    }
    
    void SetVisibility(bool v) {
      caption_actor->SetVisibility(v);
      above_perf_actor->SetVisibility(v);
      perf_actor->SetVisibility(v);
    }
    
    void SetTextBackground(double* color, double opacity = DefaultTextOpacity) {
      auto mapper = static_cast<vtkTextMapper*>(caption_actor->GetMapper());      
      mapper->GetTextProperty()->SetBackgroundColor(color);
      mapper->GetTextProperty()->SetBackgroundOpacity(opacity);
      mapper->Modified();
    }
    
    void SetName(std::string name) {            
      auto mapper = static_cast<vtkTextMapper*>(caption_actor->GetMapper());      
      mapper->SetInput(QString::fromStdString(name).toUtf8().constData());
      mapper->Modified();
    }

    /// <summary>
    /// Head position
    /// </summary>
    dpl::vector3d GetPosition() {
      return points_[0]/scale_;
    }

    /// <summary>
    /// Head position
    /// </summary>
    void SetPosition(double* ptr) { SetPosition(ptr[0], ptr[1], ptr[2]); }
    
    void SetPosition(double x, double y, double z) {            


      

      
      caption_actor->GetPositionCoordinate()->SetValue(
        scale_*dpl::vector3d{x, y, z} + dpl::vector3d{0, 0, caption_overhead_});
      
      for (auto& p : points_) {
        p.x() = scale_.x()*x;
        p.y() = scale_.y()*y;
      }

      points_[0].z() = scale_.z()*(z);
      points_[1].z() = scale_.z()*(z - input->perf_top);
      points_[2].z() = scale_.z()*(z - input->perf_top - input->perf_thickness);
      
      
      above_perf_tube_filter->GetInputAlgorithm()->Modified();
      perf_tube_filter->GetInputAlgorithm()->Modified();
    }

    void SetPerforation(double top, double thickness) {
      auto z = input->head_location.z();
      points_[1].z() = scale_.z()*(z - top);
      points_[2].z() = scale_.z()*(z - top - thickness);

      above_perf_tube_filter->GetInputAlgorithm()->Modified();
      perf_tube_filter->GetInputAlgorithm()->Modified();
    }

    std::tuple<double, double> GetPerforation() {
      return std::make_tuple(
        (points_[0].z() - points_[1].z())/scale_.z(),
        (points_[1].z() - points_[2].z())/scale_.z());
    }
    
    static void AddActor(
      vtkRenderer* renderer,
      const StartupSettings& settings,
      vtkLookupTable* pressure_lut,
      VerticalWell& well,
      VisualVerticalWell& vtk_well,
      dpl::vector3d scale,
      double visual_well_radius) {

      auto x = scale.x()*well.head_location.x();
      auto y = scale.y()*well.head_location.y();
      
      auto d0 = scale.z()*(well.head_location.z()/* + Overhead*/);
      auto d1 = scale.z()*(well.head_location.z() - well.perf_top);
      auto d2 = scale.z()*(well.head_location.z() - well.perf_top - well.perf_thickness);

      vtk_well.input = &well;
      vtk_well.scale_ = scale;
      
      {
        vtkNew<vtkTextMapper> mapper;      
        mapper->SetInput(QString::fromStdString(well.name).toUtf8().constData());
        
        auto text_property = mapper->GetTextProperty();
        text_property->SetColor(dpl::vector3d{0.15});
        text_property->SetBackgroundColor(dpl::vector3d{0.85});
        text_property->SetBackgroundOpacity(DefaultTextOpacity);
        
        text_property->SetFontFamily(VTK_FONT_FILE);
        text_property->SetFontFile(settings.font_path.string().c_str());      
        text_property->SetFontSize(settings.visual.font_size);
        text_property->SetJustificationToCentered();
        text_property->SetVerticalJustificationToBottom();
        

        if (settings.publishing) {
          // text_property->SetColor(dpl::vector3d{0.15});
          // text_property->SetBackgroundColor(dpl::vector3d{0.85});
          text_property->SetBackgroundOpacity(1.0);
        }

        vtk_well.caption_actor->SetMapper(mapper);
        vtk_well.caption_actor->GetPositionCoordinate()->SetCoordinateSystemToWorld();
        vtk_well.caption_actor->GetPositionCoordinate()->SetValue(x, y, d0 + caption_overhead_);

        renderer->AddActor2D(vtk_well.caption_actor);              
      }

      
      vtk_well.points_[0] = {x, y, d0};
      vtk_well.points_[1] = {x, y, d1};
      vtk_well.points_[2] = {x, y, d2};
      
      {              
        vtk_well.above_perf_array->SetNumberOfComponents(3);
        vtk_well.above_perf_array->SetNumberOfTuples(2);                
        vtk_well.above_perf_array->SetArray(reinterpret_cast<double*>(vtk_well.points_), 6, 1);                       
        
        vtkNew<vtkPoints> points;        
        points->SetData(vtk_well.above_perf_array);

        vtkNew<vtkLineSource> line_source;      
        line_source->SetPoints(points);
        line_source->SetResolution(1);
        
        vtk_well.above_perf_tube_filter->SetRadius(/*sqrt(dx*dy)/8*1.25*/  visual_well_radius /* units::Metres(1.0*2)*/);
        vtk_well.above_perf_tube_filter->SetNumberOfSides(30);
        vtk_well.above_perf_tube_filter->CappingOn();               
        vtk_well.above_perf_tube_filter->SetInputConnection(line_source->GetOutputPort());
            
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(vtk_well.above_perf_tube_filter->GetOutputPort());
        
        vtk_well.above_perf_actor->SetMapper(mapper);
        vtk_well.above_perf_actor->GetProperty()->SetColor(0.4, 0.4, 0.4);  
        renderer->AddActor(vtk_well.above_perf_actor);        
      }
      
      {              
        vtk_well.perf_array->SetNumberOfComponents(3);
        vtk_well.perf_array->SetNumberOfTuples(2);                
        vtk_well.perf_array->SetArray(reinterpret_cast<double*>(vtk_well.points_ + 1), 6, 1);                       
        
        vtkNew<vtkPoints> points;
        points->SetData(vtk_well.perf_array);

        vtkNew<vtkLineSource> line_source;      
        line_source->SetPoints(points);
        line_source->SetResolution(1);
                          
        vtk_well.perf_tube_filter->SetRadius(/*sqrt(dx*dy)/4*1.25*//*units::Metres(1.75*2)*/ 1.75*visual_well_radius);
        vtk_well.perf_tube_filter->SetNumberOfSides(30);
        vtk_well.perf_tube_filter->CappingOn();        
        vtk_well.perf_tube_filter->SetInputConnection(line_source->GetOutputPort());
            
        vtkNew<vtkPolyDataMapper> mapper;
        mapper->SetInputConnection(vtk_well.perf_tube_filter->GetOutputPort());
        
        vtk_well.perf_actor->SetMapper(mapper);

        double color[3];            
        pressure_lut->GetColor(well.pressure, color);        
        vtk_well.perf_actor->GetProperty()->SetColor(color);  
        renderer->AddActor(vtk_well.perf_actor);                
      }
    }
  };
}

