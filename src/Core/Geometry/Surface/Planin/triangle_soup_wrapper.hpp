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


#ifndef __TRIANGLE_SOUP_WRAPPER__
#define __TRIANGLE_SOUP_WRAPPER__

#include <vector> 
#include "core.hpp" 

// For the mesh utilities. 
#include <map>

// For std::referenc_wrapper
#include <functional> 

/* 
 * Class TriangleSoupWrapper for sanity. 
 * */

template<typename CoordinatesListType, typename FaceListType>
class TriangleSoupWrapper { 
    public: 
        using CoordinatesList = CoordinatesListType; 
        using FaceList = FaceListType;  

        TriangleSoupWrapper(CoordinatesListType &c, FaceListType &&f = FaceListType() ) : vertices_coordinates(c), face_list(f) {} 

        size_t ncoordinates() const { return vertices_coordinates.size(); }
        size_t nvertices() const { return vertices_coordinates.size()/3; }
        size_t ntriangles() const { return vertices_coordinates.size()/3; }
        size_t nfaces() const { return ntriangles(); }

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

            vertices_coordinates[ face*3 + 0 ] = face_indices[0]; 
            vertices_coordinates[ face*3 + 1 ] = face_indices[1]; 
            vertices_coordinates[ face*3 + 2 ] = face_indices[2]; 
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

    private: 
        CoordinatesList &vertices_coordinates; 
        FaceList &face_list; 
};



/* /1* Incomplete stuff. *1/ */ 

/* using MeshType = std::vector<float>; */ 

/* const unsigned int num_vertices_per_face = 3; */ 
/* using Face = std::array<unsigned int, num_vertices_per_face>; */ 

/* class lexicographicOrderOnFaces { */ 
/*     public: */ 
/*         bool operator()( Face lhface, Face rhface ) const { */ 
/*             std::sort( std::begin(lhface), std::end(lhface) ); */ 
/*             std::sort( std::begin(rhface), std::end(rhface) ); */ 

/*             bool result; */ 
/*             result = (lhface[0] < rhface[0]); */ 
/*             if (result) { */ 
/*                 return true; */
/*             } */
/*             result = (lhface[0] > rhface[0]); */ 
/*             if (result) { */ 
/*                 return false; */ 
/*             } */

/*             result = (lhface[1] < rhface[1]); */ 
/*             if (result) { */ 
/*                 return true; */
/*             } */
/*             result = (lhface[1] > rhface[1]); */ 
/*             if (result) { */ 
/*                 return false; */ 
/*             } */

/*             result = (lhface[2] < rhface[2]); */ 
/*             if (result) { */ 
/*                 return true; */
/*             } */
/*             result = (lhface[2] > rhface[2]); */ 
/*             if (result) { */ 
/*                 return false; */ 
/*             } */

/*             result = (lhface[3] < rhface[3]); */ 
/*             if (result) { */ 
/*                 return true; */
/*             } */
/*             result = (lhface[3] > rhface[3]); */ 
/*             if (result) { */ 
/*                 return false; */ 
/*             } */

/*             return false; */ 
/*         } */
/* }; */ 

/* using FaceContainer = std::map<Face, bool, lexicographicOrderOnFaces>; */ 

/* /1* class TriangleSoup { *1/ */ 
/* /1*     public: *1/ */ 
/* /1*         /2* Get a vertex index and returns the vertex coordinates. *2/ *1/ */
/* /1*         bool get_vertex( unsigned int vertex, Point3D &coordinates ) *1/ */ 
/* /1*         { *1/ */
/* /1*             if ( check() == false ) { *1/ */ 
/* /1*                 return false; *1/ */ 
/* /1*             } *1/ */

/* /1*             return true; *1/ */ 
/* /1*         } *1/ */


/* /1*         size_t nvertices() { return num_vertices; } *1/ */ 
/* /1*         size_t ntriangles() { return num_triangles; } *1/ */ 

/* /1*         bool check() *1/ */ 
/* /1*         { *1/ */ 
/* /1*             bool status = ( vertices_coordinates.size() == num_coordinates ); *1/ */ 
/* /1*             status &= ( num_vertices == num_coordinates/3 ); *1/ */ 
/* /1*             status &= ( num_triangles == num_vertices/3 ); *1/ */ 
/* /1*             status &= ( face_list.size() == num_triangles ); *1/ */ 
/* /1*             return status; *1/ */ 
/* /1*         } *1/ */

/* /1*         void add_triangle( Point3D p0, Point3D p1, Point3D p2 ) *1/ */ 
/* /1*         { *1/ */
/* /1*             std::vector<double> tmp = { p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, p2.x, p2.y, p2.z }; *1/ */ 
/* /1*             vertices_coordinates.insert( vertices_coordinates.end(), tmp.begin(), tmp.end() ); *1/ */ 

/* /1*             std::vector<size_t> new_face = { num_vertices, num_vertices+1, num_vertices+2 }; *1/ */ 
/* /1*             num_vertices += num_vertices_per_face; *1/ */ 
/* /1*             ++num_triangles; *1/ */ 
/* /1*         } *1/ */

/* /1*     private: *1/ */
/* /1*             std::vector<float> vertices_coordinates; *1/ */
/* /1*             std::vector<unsigned int> face_list; *1/ */ 

/* /1*             const size_t num_vertices_per_face_ = 3; *1/ */ 
/*             /1* size_t num_coordinates = 0; *1/ */ 
/*             /1* size_t num_vertices = 0; *1/ */ 
/*             /1* size_t num_triangles = 0; *1/ */
/* /1* }; *1/ */


#endif

