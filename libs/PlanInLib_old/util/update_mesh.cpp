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

#include "interpolated_graph.hpp"
#include "test_and_debug.hpp" 
#include "matlab_utilities.hpp" 

template<typename T>
void load_vertices( T &vertices, T &vx, T &vy, T &vz ) 
{
    vertices.clear(); 
    for ( size_t i = 0; i < vx.size(); ++i )
    {
        vertices.push_back(vx[i]); 
        vertices.push_back(vy[i]); 
        vertices.push_back(vz[i]); 
    }
}

template<typename T>
void unload_vertices( T &vertices, T &vx, T &vy, T &vz ) 
{
    vx.clear(); vy.clear(); vz.clear();  
    for ( size_t i = 0; i < vertices.size()/3; ++i )
    {
        vx.push_back(vertices[3*i + 0]); 
        vy.push_back(vertices[3*i + 1]); 
        vz.push_back(vertices[3*i + 2]); 
    }
}

int main( int argc, char **argv ) 
{
    UNUSED(argc); 
    UNUSED(argv); 

    /* Unity test. */ 
    /* test_interpolation(); */ 

    /* Do useful stuff. */
    using Container = std::vector<double>; 
    Container points, vertices, vx, vy, vz; 
    
    bool status = true; 
    size_t nX, nY, nZ; 
    
    status &= load_matlab_vector3<double>(points, "points.mat" ); 
    status &= load_matlab_grid<double>(vx, vy, vz, nX, nY, nZ, "vertices.mat" ); 
    if ( status == false ) { 
        std::cout << "Failed to load matlab data.\n\n"; 
        return -1; 
    }

    auto get_point = [] ( Container &c, size_t index ) -> Point3 
    {
        Point3 p; 
        if ( index < c.size()/3 ) { 
            p[0] = c[3*index + 0]; 
            p[1] = c[3*index + 1]; 
            p[2] = c[3*index + 2]; 
        }

        return p; 
    }; 

    auto t0 = std::chrono::high_resolution_clock::now(); 
    InterpolatedGraph g; 
    for ( size_t i = 0; i < points.size()/3; ++i ) 
    {
        g.addPoint( get_point(points, i) ); 
    }

    g.generateSurface(); 
    auto t1 = std::chrono::high_resolution_clock::now(); 
    
    auto interpolationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t1-t0) ).count(); 
    std::cout << "It took " << interpolationtime << " milliseconds to load the data and build an interpolant." << std::endl; 

    load_vertices(vertices, vx, vy, vz); 

    auto t2 = std::chrono::high_resolution_clock::now(); 
    g.getRawHeightMap(vertices); 
    auto t3 = std::chrono::high_resolution_clock::now(); 
    
    auto evaluationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t3-t2) ).count(); 
    std::cout << "It took " << evaluationtime << " milliseconds to evaluate the interpolant." << std::endl; 
    
    unload_vertices(vertices, vx, vy, vz); 

    status &= save_matlab_grid<double>(vx, vy, vz, nX, nY, nZ, "output_vertices.mat"); 
    if ( status == false ) { 
        std::cout << "Failed to save matlab data.\n\n"; 
        return -2; 
    }
    

    return 0; 
}


