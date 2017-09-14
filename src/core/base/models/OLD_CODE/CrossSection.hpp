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


#ifndef CROSSSECTION_HPP
#define CROSSSECTION_HPP

#include <vector>
#include <map>

#include "Boundary.hpp"
#include "Stratigraphy.hpp"
#include "Intersection.hpp"


class CrossSection
{

    public:

        CrossSection(){ boundary = nullptr; }

        inline bool isEmpty(){ return curves.empty(); }
        inline bool hasBoundary(){ if( boundary == nullptr ) return false; return true; }


        inline void setBoundary( Boundary* const& bd ){ boundary = bd; }
        inline Boundary* getBoundary(){ return boundary; }


        inline void setDepth( const float& d ){ depth = d; }
        inline float getDepth(){ return depth; }


        inline void addStratigraphy( Stratigraphy* const& strat )
        {
            Curve2D* c = strat->getCurve( depth );
            curves.push_back( c );

            stratigraphies[ c ] = strat;
        }


        inline Stratigraphy* getStratigraphy( const unsigned int& id )
        {
            Curve2D* c = curves[ id ];
            return stratigraphies[ c ];
        }

        inline void getIntersections( Stratigraphy* const& strat, std::vector< Curve2D* >& sec_curves, std::vector< std::vector < Point2D > >& points )
        {
            Curve2D* c = strat->getCurve( depth );
            intersections.getIntersections( c, sec_curves, points );
        }


        inline void clear()
        {
            curves.clear();
            stratigraphies.clear();
        }


    protected:


        float depth;
        Boundary* boundary;

        Intersection intersections;
        std::vector< Curve2D* > curves;
        std::map< Curve2D*, Stratigraphy* > stratigraphies;




};


#endif
