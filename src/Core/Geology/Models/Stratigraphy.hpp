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


#ifndef STRATIGRAPHY_HPP
#define STRATIGRAPHY_HPP

#include <vector>
#include <map>

#include <QtGlobal>


#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"




class Stratigraphy
{

    public:



        Stratigraphy(){ index = number_of_stratigraphies; number_of_stratigraphies++; }
		

        inline void addCurve( const float& d, Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }
        inline Curve2D* getCurve( const float& d ){ return &curves_list[ d ]; }
        inline void updateCurve( const float& d, Curve2D const& c ){ curves_list[ d ] = c; crosssections_list.push_back( d ); }


        inline std::vector< float > getCrossections(){ return crosssections_list; }


        inline bool isEmpty(){ return curves_list.empty(); }
        inline size_t getId(){ return index; }


        inline void updateSurface( const std::vector< float >& vertices,  const std::vector< size_t >& faces )
        {

            surface_vertices.clear();
            surface_faces.clear();

            surface_vertices.assign( vertices.begin(), vertices.end() );
            surface_faces.assign( faces.begin(), faces.end() );

        }
        inline void updateCurveWireframe( const std::vector< size_t >& edges )
        {
            curve_edges.clear();
            curve_edges.assign( edges.begin(), edges.end() );
        }


        inline std::vector< size_t >& getCurveEdges(){ return curve_edges; }

        inline std::vector< float >& getSurfaceVertices(){ return surface_vertices; }
        inline std::vector< size_t >& getSurfaceFaces(){ return surface_faces; }

        inline void clear()
        {
            crosssections_list.clear();

            curves_list.clear();
            curve_edges.clear();

            surface_vertices.clear();
            surface_faces.clear();
        }



	protected:
	

        size_t index;
        static size_t number_of_stratigraphies;

        std::vector< float > crosssections_list;

        std::map< float, Curve2D > curves_list;
        std::vector< size_t > curve_edges;

        std::vector< float > surface_vertices;
        std::vector< size_t > surface_faces;


};




#endif



