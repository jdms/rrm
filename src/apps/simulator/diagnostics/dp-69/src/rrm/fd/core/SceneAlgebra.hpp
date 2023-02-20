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
// #include "rrm/fd/VerticalWell.hpp"

#include "dpl/static_vector.hpp"

#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkLine.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkRenderer.h>



namespace rrm::fd::core
{
  // display - (left-to-right, bottom-to-top) coordinates
  
  // Depth from the well head
  inline double ProjectOnVerticalLine(vtkRenderer* renderer, dpl::vector2i display, dpl::vector3d p0) {
    auto* camera = renderer->GetActiveCamera();
    dpl::vector3d_map camera_pos{camera->GetPosition()};

    dpl::vector3d p_minus_k{p0.x(), p0.y(), p0.z() - 1};

    double t;
    vtkLine::DistanceToLine(camera_pos, p0, p_minus_k, t);

    vtkNew<vtkCoordinate> coord;
    coord->SetCoordinateSystemToDisplay();
    coord->SetValue(display.x(), display.y(), 0.0);

    dpl::vector3d_map near_plane_proj{coord->GetComputedWorldValue(renderer)};

    dpl::vector3d plane_proj;

    vtkPlane::IntersectWithLine(
      camera->GetParallelProjection()
        ? near_plane_proj + camera->GetViewPlaneNormal()
        : camera_pos, /* p0 */
      near_plane_proj, /* p1 */
      camera_pos - dpl::vector3d{p0.x(), p0.y(), p0.z() - t}, /* plane normal */
      p0, /* point on the plane */
      t, /* dummy */
      plane_proj /* projection on the plane*/);

    vtkLine::DistanceToLine(plane_proj, p0, p_minus_k, t);
    return t/*/scale.z()*/;
  }


   inline dpl::vector3d ProjectOnPlane(vtkRenderer* renderer, dpl::vector2i display, dpl::vector3d p0, dpl::vector3d normal = dpl::vector3d{0, 0, 1}) {
    vtkNew<vtkCoordinate> coord;
    coord->SetCoordinateSystemToDisplay();
    coord->SetValue(display.x(), display.y(), 0.0);

    auto* camera = renderer->GetActiveCamera();

    dpl::vector3d_map near_plane_proj{coord->GetComputedWorldValue(renderer)};

    double t;
    dpl::vector3d plane_proj;
    vtkPlane::IntersectWithLine(
      camera->GetParallelProjection()
        ? static_cast<double*>(near_plane_proj + camera->GetViewPlaneNormal())
        : camera->GetPosition(), /* p0 */
      near_plane_proj, /* p1 */
      normal, /* plane normal */
      p0, /* point on the plane */
      t, /* dummy */
      plane_proj /* projection on the plane*/);



    
    // plane_proj.x() = std::clamp(plane_proj.x(),
    //   scale_.x()*cartesian_model_.MinX(),
    //   scale_.x()*cartesian_model_.MaxX());
    //
    // plane_proj.y() = std::clamp(plane_proj.y(),
    //   scale_.y()*cartesian_model_.MinY(),
    //   scale_.y()*cartesian_model_.MaxY());

    

    return plane_proj;
  }
}
