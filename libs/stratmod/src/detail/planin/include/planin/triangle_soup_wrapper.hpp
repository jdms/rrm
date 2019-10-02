/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
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


#ifndef __TRIANGLE_SOUP_WRAPPER__
#define __TRIANGLE_SOUP_WRAPPER__

#include <vector> 
#include <memory> 
#include <limits> 
#include <atomic>
#include <cmath>
#include "planin/core.hpp" 

// For the mesh utilities. 
#include <map>

// For std::referenc_wrapper
#include <functional> 


class lexicographicOrderOnVertices 
{ 
    public: 
        bool operator()( const Point3& lvertex, const Point3& rvertex ) const { 
            /* std::sort( std::begin(lvertex), std::end(lvertex) ); */ 
            /* std::sort( std::begin(rvertex), std::end(rvertex) ); */ 

            bool result; 
            result = (lvertex[0] - tolerance > rvertex[0]); 
            if (result) { 
                return false;
            }
            /* result = (lvertex[0] > rvertex[0] - tolerance); */ 
            /* if (result) { */ 
            /*     return false; */ 
            /* } */

            result = (lvertex[1] - tolerance > rvertex[1]); 
            if (result) { 
                return false;
            }
            /* result = (lvertex[1] > rvertex[1] - tolerance); */ 
            /* if (result) { */ 
            /*     return false; */ 
            /* } */

            result = (lvertex[2] - tolerance > rvertex[2]); 
            if (result) { 
                return false;
            }
            /* result = (lvertex[2] > rvertex[2] - tolerance); */ 
            /* if (result) { */ 
            /*     return false; */ 
            /* } */

            /* result = (lvertex[3] < rvertex[3] - tolerance); */ 
            /* if (result) { */ 
            /*     return true; */
            /* } */
            /* result = (lvertex[3] > rvertex[3] - tolerance); */ 
            /* if (result) { */ 
            /*     return false; */ 
            /* } */

            bool equal_v0 = std::fabs(lvertex[0] - rvertex[0]) < tolerance;
            bool equal_v1 = std::fabs(lvertex[1] - rvertex[1]) < tolerance;
            bool equal_v2 = std::fabs(lvertex[2] - rvertex[2]) < tolerance;

            // Enforce that x < x is false
            if (equal_v0 && equal_v1 && equal_v2)
            {
                return false; 
            }

            return true;
        }

        static void setTolerance( double t ) 
        { 
            tolerance = t > 0 ? t : tolerance; 
        } 

    private: 
        static double tolerance; 
};

/* double lexicographicOrderOnVertices::tolerance = std::numeric_limits<double>::epsilon(); */ 


/* 
 * Class TriangleSoupWrapper for sanity. 
 * */

template<typename CoordinatesListType, typename FaceListType>
class TriangleSoupWrapper { 
    public: 
        using CoordinatesList = CoordinatesListType; 
        using FaceList = FaceListType;  
        using Ptr = std::shared_ptr<TriangleSoupWrapper>; 

        /* TriangleSoupWrapper( CoordinatesListType &c ) : vertices_coordinates(c), pface_list( std::make_shared<FaceListType>() ), face_list(*pface_list) {} */ 
        TriangleSoupWrapper( CoordinatesListType &c, FaceListType &f ) : vertices_coordinates(c), face_list(f) {} 

        size_t ncoordinates() const { return vertices_coordinates.size(); }
        size_t nvertices() const { return vertices_coordinates.size()/3; }
        size_t nfaces() const { return face_list.size()/3; }
        size_t ntriangles() const { return nfaces(); }

        void clear() 
        {
            vertices_coordinates.clear(); 
            face_list.clear(); 
        }

        template<typename T>
        bool getVertex( size_t vertex, T& coord ) const 
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( vertex >= nvertices() ) { 
                return false; 
            }

            coord[0] = vertices_coordinates[ vertex*3 + 0 ]; 
            coord[1] = vertices_coordinates[ vertex*3 + 1 ]; 
            coord[2] = vertices_coordinates[ vertex*3 + 2 ]; 

            return true; 
        }

        template<typename T>
        bool setVertex( size_t vertex, const T& coord ) 
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( vertex >= nvertices() ) { 
                return false; 
            }

            vertices_coordinates[ vertex*3 + 0 ] = coord[0]; 
            vertices_coordinates[ vertex*3 + 1 ] = coord[1]; 
            vertices_coordinates[ vertex*3 + 2 ] = coord[2]; 

            return true; 
        }

        template<typename T>
        bool getFace( size_t face, T& face_indices ) const 
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( face >= nfaces() ) { 
                return false; 
            }

            face_indices[0] = vertices_coordinates[ face*3 + 0 ]; 
            face_indices[1] = vertices_coordinates[ face*3 + 1 ]; 
            face_indices[2] = vertices_coordinates[ face*3 + 2 ]; 
        }

        template<typename T>
        bool setFace( size_t face, const T& face_indices ) 
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( face >= nfaces() ) { 
                return false; 
            }

            face_list[ face*3 + 0 ] = face_indices[0]; 
            face_list[ face*3 + 1 ] = face_indices[1]; 
            face_list[ face*3 + 2 ] = face_indices[2]; 
        }

        template<typename T>
        void addVertex( const T& coords ) 
        {
            vertices_coordinates.push_back( coords[0] ); 
            vertices_coordinates.push_back( coords[1] ); 
            vertices_coordinates.push_back( coords[2] ); 
        }

        template<typename T>
        bool addFace( const T& face_indices ) 
        {
            bool status = true; 
            status &= ( face_indices[0] < nvertices() ); 
            status &= ( face_indices[1] < nvertices() ); 
            status &= ( face_indices[2] < nvertices() ); 

            if ( status == false ) { 
                return false; 
            }

            face_list.push_back( face_indices[0] ); 
            face_list.push_back( face_indices[1] ); 
            face_list.push_back( face_indices[2] ); 

            return true; 
        }

        bool weakCheck() const 
        {
            bool status = true; 
            status &= ( vertices_coordinates.size() % 3 == 0 ); 
            status &= ( face_list.size() % 3 == 0 ); 

            return status; 
        }

        bool append( const TriangleSoupWrapper& mesh ) 
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( mesh.weakCheck() == false ) { 
                return false; 
            }

            size_t old_face_list_size = face_list.size(); 
            size_t old_nvertices = nvertices(); 

            std::copy( mesh.vertices_coordinates.begin(), mesh.vertices_coordinates.end(), std::back_inserter(vertices_coordinates) ); 
            std::copy( mesh.face_list.begin(), mesh.face_list.end(), std::back_inserter(face_list) ); 

            for ( size_t i = old_face_list_size; i < face_list.size(); ++i ) 
            {
                face_list[i] += old_nvertices; 
            }

            return true; 
        }

        bool removeDuplicatedVertices( double tolerance )
        {
            if ( weakCheck() == false ) { 
                return false; 
            }

            if ( tolerance <= 0 ) { 
                return false; 
            }

            lexicographicOrderOnVertices::setTolerance(tolerance); 

            VertexContainer container; 
            Point3 v; 

            for ( size_t i = 0; i < nvertices(); ++i )
            {
                getVertex(i, v); 
                container[v].push_back(i); 
            }

            vertices_coordinates.clear(); 
            VertexMap dictionary; 
            unsigned long int new_index = 0; 

            for ( auto &iter : container )
            {
                for ( auto &old_index : iter.second ) 
                { 
                    dictionary[old_index] = new_index;
                }
                vertices_coordinates.push_back(iter.first[0]); 
                vertices_coordinates.push_back(iter.first[1]); 
                vertices_coordinates.push_back(iter.first[2]); 
                ++new_index; 
            }

            unsigned long vindex = 0; 
            bool status = true; 
            for ( size_t i = 0; i < face_list.size(); ++i )
            {
                vindex = face_list[i]; 
                if ( dictionary.find(vindex) == dictionary.end() ) { 
                    // Should never happen; 
                    status = false; 
                }
                face_list[i] = dictionary[vindex]; 
            }

            return status; 
        }

    protected: 
        CoordinatesList &vertices_coordinates; 
        /* std::shared_ptr<FaceList> pface_list; */ 
        FaceList &face_list; 

    private: 


        using VerticesIndices = std::vector<size_t>; 
        using VertexContainer = std::map<Point3, VerticesIndices, lexicographicOrderOnVertices>; 

        using VertexMap = std::map<unsigned long int, unsigned long int>; 
};


#endif

