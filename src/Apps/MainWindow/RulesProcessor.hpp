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


#ifndef __RULES_PROCESSOR__
#define __RULES_PROCESSOR__

#include <iostream>
#include <vector>
#include <map>


#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"

//#include "stratigraphy_modeller.hpp"



//TODO: create a base virtual class BaseRulesProcessor
namespace RRM
{
    class RulesProcessor {

    public:

        inline RulesProcessor(){}
        inline ~RulesProcessor(){}

        inline std::vector<std::size_t> getSurfaces(){ return std::vector<std::size_t>(); }

        //
        // brief:
        // Verifies whether ir is possible to define a new 'drawing' region,
        // either above or below.
        // Argument: `eligible_surfaces` stores the indices of surfaces that can
        // be used as input either for an `defineAbove()` or a `defineBelow()`.
        // Return: true if at least one elegible surface was found.
        //
        /* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

        inline bool requestCreateAbove( std::vector<size_t> &eligible_surfaces ){ return false; }
        inline bool requestCreateBelow( std::vector<size_t> &eligible_surfaces ){ return false; }




        /* Change the model's properties */



        inline bool setLowResolution(){ return false; }

        inline bool setMediumResolution(){ return false; }

        inline bool setHighResolution(){ return false; }


        inline void setOrigin( double opengl_x, double opengl_y, double opengl_z ){}

        inline bool setLenght( double opengl_x, double opengl_y, double opengl_z ){ return false; }


        /* Begin methods to interface with GUI */


        /* Clean up */
        inline void clear(){}


        /* Query or modify the automatum state */


        //
        // brief:
        // Define new input region above surface which index is `surface_index`.
        //
        inline bool defineAbove( size_t surface_index ){ return false; }


        //
        // brief:
        // Clear any previous `defineAbove()` call.
        // Safe to call anytime.
        //
        inline void stopDefineAbove(){}

        //
        // brief:
        // Define new input region below surface which index is `surface_index`.
        //
        inline bool defineBelow( size_t surface_index ){ return false; }

        //
        // brief:
        // Clear any previous `defineBelow()` call.
        // Safe to call anytime.
        //
        inline void stopDefineBelow(){}

        inline bool defineAboveIsActive(){ return false; }
        inline bool defineBelowIsActive(){ return false; }


        inline void removeAbove(){}
        inline void removeAboveIntersection(){}

        inline void removeBelow(){}
        inline void removeBelowIntersection(){}


        template<typename CurveType>
        inline bool createSurface( size_t surface_index, const std::vector< std::tuple< CurveType, std::size_t  > > &curves ){ return false; }


        template<typename CurveType>
        inline bool createChannel( size_t surface_index, const CurveType &cross_section, const CurveType &path ){ return false; }


        inline bool canUndo(){ return false; }
        inline bool undo(){ return false; }

        inline bool canRedo(){ return false; }
        inline bool redo(){ return false; }


        /* Get mesh, pcl and curves for visualization */


        template<typename VertexList, typename FaceList>
        inline bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist ){ return false; }

        template<typename VertexList, typename EdgeList>
        inline bool getCrossSection( size_t surface_id, std::size_t depth, VertexList &vlist, EdgeList &elist ){ return false; }



        /* End methods to interface with GUI */


    private:

//        StratigraphyModeller modeller;

    };

} // End of Namespace RRM

#endif
