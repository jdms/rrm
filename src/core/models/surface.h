/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef SURFACE_H
#define SURFACE_H

#include <vector>


#include "./core/definitions/constants.hpp"

/**
 *  A data structure to help to represent surfaces and handle its data
 */

class Surface
{

    public:


        /**
        * Constructor.
        */
        Surface() = default;


        /**
        * Constructor. Building a new surface given the vertices, faces, and optionally normals
        */
        Surface( const std::vector< double >& vs_, const std::vector< std::size_t >& fs_,
                 const std::vector< double >& ns_ = std::vector< double >() )
        {
            setVertices( vs_ );
            setFaces( fs_ );
            setNormals( ns_ );
        }


        /**
        * Method to return if the surface has any data
        * The surface is considered empty if it has no vertices
        * @return boolean return true if the surface has no vertices, and false otherwise
        */
        bool isEmpty() const
        {
            return data.vertices.empty();
        }


        /**
        * This method sets the vertices of the surface
        * @param vs_ a vector with the vertices coordinates of all vertices
        * @return void.
        */
        void setVertices( const std::vector< double >& vs_ )
        {
             data.vertices.clear();
             data.vertices.assign( vs_.begin(), vs_.end() );
        }



        /**
        * This method returns the vertices of the surface
        * @return std::vector< double > a vector with the vertices coordinates of all vertices
        */
        std::vector< double > getVertices() const
        {
             return data.vertices;
        }



        /**
        * This method sets the normals of the vertices of the surface
        * @param ns_ a vector with the coordinates of all normals
        * @return void.
        */
        void setNormals( const std::vector< double >& ns_ )
        {
             data.normals.clear();
             data.normals.assign( ns_.begin(), ns_.end() );
        }


        /**
        * This method returns the normals of the vertices of the surface
        * @return std::vector< double > a vector with the coordinates of all normals
        */
        std::vector< double > getNormals() const
        {
             return data.normals;
        }


        /**
        * This method sets the faces of the surface
        * @param fs_ a vector with the index of each vertice that defines a face for all faces of the surface
        * @return void.
        */
        void setFaces( const std::vector< std::size_t >& fs_ )
        {
             data.faces.clear();
             data.faces.assign( fs_.begin(), fs_.end() );
        }


        /**
        * This method returns the faces of the surface
        * @return std::vector< std::size_t > a vector with the index of each vertice that defines a face for all faces of the surface
        */
        std::vector< std::size_t > getFaces() const
        {
             return data.faces;
        }


        /**
        * This method remove all metadata from the surface
        * @return void.
        */
        void clear()
        {
            data.vertices.clear();
            data.faces.clear();
            data.normals.clear();
        }


    private:


        struct SurfaceData
        {
            std::vector< double > vertices;                                     /**< a vector of vertices coordinates for all vertices of the volume */
            std::vector< double > normals;                                      /**< a vector of coordinates for all normals of the vertices of the volume */
            std::vector< std::size_t > faces;                                   /**< a vector of indexes of the vertices that defines a face for each face of the volume */
        };

        SurfaceData data;                                                       /**< a data structure to hold the surface data  */

};

#endif // SURFACE_H
