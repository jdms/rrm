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

#include "srules.hpp" 
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
    VListType s1_points, s2_points, s3_points, sn_points; 
    VListType s1_vlist, s2_vlist, s3_vlist, sn_vlist; 

    using FListType = std::vector<unsigned long int>; 
    FListType s1_flist, s2_flist, s3_flist, sn_flist; 
    
    bool status = true; 
    
    status &= load_matlab_vector3<double>(s1_points, "ex5b_surface_1.mat" ); 
    status &= load_matlab_vector3<double>(s2_points, "ex5b_surface_2.mat" ); 
    status &= load_matlab_vector3<double>(s3_points, "ex5b_surface_3.mat" ); 
    status &= load_matlab_vector3<double>(sn_points, "ex5b_surface_n.mat" ); 

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


    /* */
    /* Build surfaces, apply rules and get meshes. */ 
    /* */ 

    auto t0 = std::chrono::high_resolution_clock::now(); 

    PlanarSurface::Ptr s1 = std::make_shared<PlanarSurface>();  
    for ( size_t i = 0; i < s1_points.size()/3; ++i ) 
    {
        s1->addPoint( get_point(s1_points, i) ); 
    }
    s1->generateSurface(); 

    PlanarSurface::Ptr s2 = std::make_shared<PlanarSurface>();  
    for ( size_t i = 0; i < s2_points.size()/3; ++i ) 
    {
        s2->addPoint( get_point(s2_points, i) ); 
    }
    s2->generateSurface(); 

    PlanarSurface::Ptr s3 = std::make_shared<PlanarSurface>();  
    for ( size_t i = 0; i < s3_points.size()/3; ++i ) 
    {
        s3->addPoint( get_point(s3_points, i) ); 
    }
    s3->generateSurface(); 

    PlanarSurface::Ptr sn = std::make_shared<PlanarSurface>();  
    for ( size_t i = 0; i < sn_points.size()/3; ++i ) 
    {
        sn->addPoint( get_point(sn_points, i) ); 
    }
    sn->generateSurface(); 

    auto t1 = std::chrono::high_resolution_clock::now(); 
    
    auto interpolationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t1-t0) ).count(); 
    std::cout << "It took " << interpolationtime << " milliseconds to load the data, to build and to evaluate the interpolant." << std::endl; 


    auto t2 = std::chrono::high_resolution_clock::now(); 

    std::size_t s1_index, s2_index, s3_index, sn_index; 

    SRules Surfaces; 
    Surfaces.addSurface(s1, s1_index); 
    Surfaces.addSurface(s2, s2_index); 
    Surfaces.defineBelow(); 
    Surfaces.addSurface(s3, s3_index); 
    Surfaces.addSurface(sn, sn_index); 
    Surfaces.removeBelow(); 
    Surfaces.stopDefineBelow(); 

    /* if ( s1->weakIntersectionCheck(sn) ) { */ 
    /*     s1->removeAbove(sn); */ 
    /* } */
    /* if ( s2->weakIntersectionCheck(sn) ) { */ 
    /*     s2->removeAbove(sn); */ 
    /* } */
    /* if ( s3->weakIntersectionCheck(sn) ) { */ 
    /*     s3->removeAbove(sn); */ 
    /* } */

    Surfaces[s1_index]->getVertexList(s1_vlist); 
    auto face_count1 = Surfaces[s1_index]->getFaceList(s1_flist); 

    Surfaces[s2_index]->getVertexList(s2_vlist); 
    auto face_count2 = Surfaces[s2_index]->getFaceList(s2_flist); 

    Surfaces[s3_index]->getVertexList(s3_vlist); 
    auto face_count3 = Surfaces[s3_index]->getFaceList(s3_flist); 

    Surfaces[sn_index]->getVertexList(sn_vlist); 
    auto face_countn = Surfaces[sn_index]->getFaceList(sn_flist); 

    auto t3 = std::chrono::high_resolution_clock::now(); 
    
    auto evaluationtime = std::chrono::duration_cast<std::chrono::milliseconds>( (t3-t2) ).count(); 
    std::cout << "It took " << evaluationtime << " milliseconds to apply rules, and to get: " 
        << face_count1 + face_count2 + face_count3 + face_countn  << " faces. " << std::endl; 
    

    status &= save_matlab_vector3<double>( s1_vlist, "ex5b_s1_vlist.mat" ); 
    status &= save_matlab_vector3<unsigned long int>( s1_flist, "ex5b_s1_flist.mat" ); 

    status &= save_matlab_vector3<double>( s2_vlist, "ex5b_s2_vlist.mat" ); 
    status &= save_matlab_vector3<unsigned long int>( s2_flist, "ex5b_s2_flist.mat" ); 

    status &= save_matlab_vector3<double>( s3_vlist, "ex5b_s3_vlist.mat" ); 
    status &= save_matlab_vector3<unsigned long int>( s3_flist, "ex5b_s3_flist.mat" ); 

    status &= save_matlab_vector3<double>( sn_vlist, "ex5b_sn_vlist.mat" ); 
    status &= save_matlab_vector3<unsigned long int>( sn_flist, "ex5b_sn_flist.mat" ); 

    if ( status == false ) { 
        std::cout << "Failed to save matlab data.\n\n"; 
        return -2; 
    }
    

    return 0; 
}


