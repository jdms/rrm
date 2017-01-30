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


#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <vector>
#include <map>

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"


class Intersection
{

public:



    Intersection(){}

    inline void addIntersection( Curve2D* const& c1, Curve2D* const& c2, const std::vector< Point2D >& points )
    {
        ResultIntersection res_c1;
        res_c1.second = c2;
        res_c1.intersection_points = points;

        std::vector< ResultIntersection > resultsc1_list;
        resultsc1_list.push_back( res_c1 );

        intersections[ c1 ] = resultsc1_list;



        ResultIntersection res_c2;
        res_c2.second = c1;
        res_c2.intersection_points = points;

        std::vector< ResultIntersection > resultsc2_list;
        resultsc2_list.push_back( res_c2 );

        intersections[ c2 ] = resultsc2_list;

    }

    inline void getIntersections( Curve2D* const& c, std::vector< Curve2D* >& curves, std::vector< std::vector < Point2D > >& points )
    {
        std::vector< ResultIntersection > res = intersections[ c ];

        for( size_t i = 0; i < res.size(); ++i )
        {
            curves.push_back( res[ i ].second );
            points.push_back( res[ i ].intersection_points );
        }

    }

    inline void clear(){ intersections.clear(); }




protected:


    struct ResultIntersection
    {
        Curve2D* second;
        std::vector< Point2D > intersection_points;
    };


    std::map< Curve2D*,  std::vector< ResultIntersection > > intersections;



};


#endif

