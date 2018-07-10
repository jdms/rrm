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



#ifndef REGIONS_H
#define REGIONS_H

#include <iostream>
#include <string>
#include <vector>

#include "object.h"
#include "./core/definitions/constants.hpp"


struct Domain
{
    std::set< std::size_t > regions_set;
};


class Regions: public Object
{
    public:

        Regions();
        Regions( const Regions& reg_ );
        Regions& operator=(const Regions& reg_);
        ~Regions();


        void getTriangleCells( std::vector< std::size_t >& cells_ ) const;

        ///====================================================================

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;

        void setTetrahedralCells( const std::vector< std::size_t >& cells_ );
        void getTetrahedralCells( std::vector< std::size_t >& cells_ ) const;
        void clearCells();

        void setVertices( const std::vector< double >& vertices_  );
        void getVertices( std::vector< double >& vertices_  ) const;

        ///====================================================================


        void setPoint( double x_, double y_, double z_ );
        void getPoint( double& x_, double& y_, double& z_ ) const;

        void setName( const std::string& name_ );
        std::string getName() const;

        void setVisible( const bool status_ );
        bool isVisible() const;



        void clearVertices();

        void setMaxMin( double maxx_, double maxy_, double maxz_,
                                double minx_, double miny_, double minz_ );
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                                double& minx_, double& miny_, double& minz_ ) const;


        void setColor( int r_, int g_, int b_ );
        void getColor( int& r_, int& g_, int& b_ ) const;


        void clear();
        void initialize();



    private:

        struct  Point { double x, y, z; } max, min;

        struct Color
        {
          int r = 255;
          int g = 0;
          int b = 0;
        };


        std::size_t index;
        Point center;

        bool is_visible;
        std::string name;

        std::vector< std::size_t > index_cells;
        std::vector< double > vertices;

        Color color;
};

#endif // REGIONS_H
