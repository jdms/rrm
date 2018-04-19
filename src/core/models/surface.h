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


class Surface
{

    public:

        Surface() = default;

        Surface( const std::vector< double >& vs_, const std::vector< std::size_t >& fs_,
                 const std::vector< double >& ns_ = std::vector< double >() )
        {
            setVertices( vs_ );
            setFaces( fs_ );
            setNormals( ns_ );
        }


        bool isEmpty() const
        {
            return data.vertices.empty();
        }


        void setVertices( const std::vector< double >& vs_ )
        {
             data.vertices.clear();
             data.vertices.assign( vs_.begin(), vs_.end() );
        }

        std::vector< double > getVertices() const
        {
             return data.vertices;
        }



        void setNormals( const std::vector< double >& ns_ )
        {
             data.normals.clear();
             data.normals.assign( ns_.begin(), ns_.end() );
        }

        std::vector< double > getNormals() const
        {
             return data.normals;
        }



        void setFaces( const std::vector< std::size_t >& fs_ )
        {
             data.faces.clear();
             data.faces.assign( fs_.begin(), fs_.end() );
        }

        std::vector< std::size_t > getFaces() const
        {
             return data.faces;
        }



        void clear()
        {
            data.vertices.clear();
            data.faces.clear();
            data.normals.clear();
        }




    private:


        struct SurfaceData
        {
            std::vector< double > vertices;
            std::vector< double > normals;
            std::vector< std::size_t > faces;
        };

        SurfaceData data;

};

#endif // SURFACE_H
