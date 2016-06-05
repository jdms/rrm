/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#include <iostream>
#include <chrono>

#include "planar_surface.hpp" 
#include "interpolated_graph.hpp"
#include "test_and_debug.hpp" 
#include "matlab_utilities.hpp" 

int main( int argc, char **argv ) 
{
    UNUSED(argc); 
    UNUSED(argv); 

    /* Unity test. */ 
    /* test_interpolation(); */ 

    /* Do useful stuff. */
    using VListType = std::vector<double>; 
    VListType points, points_base_surface, vlist; 

    using FListType = std::vector<unsigned long int>; 
    FListType flist; 
    
    bool status = true; 
    
    status &= load_matlab_vector3<double>(points, "points.mat" ); 
    status &= load_matlab_vector3<double>(points_base_surface, "base_surface_points.mat" ); 

    if ( status == false ) { 
        std::cout << "Failed to load matlab data.\n\n"; 
        return -1; 
    }

    auto get_point = [] ( VListType &c, size_t index ) -> Point3 
    {
        Point3 p; 
        if ( index < c.size()/3 ) { 
            p[0] = c[3*index + 0]; 
            p[1] = c[3*index + 1]; 
            p[2] = c[3*index + 2]; 
        }

        return p; 
    }; 

    PlanarSurface::Ptr base_surface = std::make_shared<PlanarSurface>();  

    for ( size_t i = 0; i < points_base_surface.size()/3; ++i ) 
    {
        base_surface->addPoint( get_point(points_base_surface, i) ); 
    }
    base_surface->generateSurface(); 

    auto t0 = std::chrono::high_resolution_clock::now(); 

    PlanarSurface surface;  
    for ( size_t i = 0; i < points.size()/3; ++i ) 
    {
        surface.addPoint( get_point(points, i) ); 
    }
    surface.removeBelow(base_surface); 
    surface.generateSurface(); 

    auto t1 = std::chrono::high_resolution_clock::now(); 
    
    auto interpolationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t1-t0) ).count(); 
    std::cout << "It took " << interpolationtime << " milliseconds to load the data, to build and to evaluate the interpolant." << std::endl; 


    auto t2 = std::chrono::high_resolution_clock::now(); 
    surface.getVertexList(vlist); 
    surface.getFaceList(flist); 
    auto t3 = std::chrono::high_resolution_clock::now(); 
    
    auto evaluationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t3-t2) ).count(); 
    std::cout << "It took " << evaluationtime << " milliseconds to apply rules." << std::endl; 
    

    status &= save_matlab_vector3<double>( vlist, "vlist.mat" ); 
    status &= save_matlab_vector3<unsigned long int>( flist, "flist.mat" ); 

    if ( status == false ) { 
        std::cout << "Failed to save matlab data.\n\n"; 
        return -2; 
    }
    

    return 0; 
}


