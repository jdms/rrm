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


#ifndef __EXTRUSION_RULES_PROCESSOR__
#define __EXTRUSION_RULES_PROCESSOR__

#include <iostream>
#include <vector>
#include <map>


#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"

#include "stratigraphy_modeller.hpp" 



//TODO: create a base virtual class BaseRulesProcessor
namespace RRM
{
    class ExtrusionRulesProcessor {
    public:
        enum class State : int { 
            UNDEFINED = -1,
            SKETCHING,
            RA_SKETCHING, // Remove above
            RAI_SKETCHING, // Remove above intersection
            RB_SKETCHING, // Remove below
            RBI_SKETCHING, // Remove below intersection
            DA_SKETCHING, // Define above
            DB_SKETCHING, // Define below
            DR_SKETCHING // Define region
        };

        enum class Region : int {
            UNDEFINED = -1,
            NO_SELECTION,
            BOUNDED_ABOVE,
            BOUNDED_BELOW,
            BOUNDED
        };

        ExtrusionRulesProcessor();
        ~ExtrusionRulesProcessor() = default;

        ExtrusionRulesProcessor( const ExtrusionRulesProcessor& ) = delete;
        ExtrusionRulesProcessor& operator=( const ExtrusionRulesProcessor& ) = delete;

        ExtrusionRulesProcessor( const ExtrusionRulesProcessor&& ) = delete;
        ExtrusionRulesProcessor& operator=( const ExtrusionRulesProcessor&& ) = delete;

        /* DEPRECATED */
        bool isInitialized() const;

        std::vector<size_t> getActiveSurfaces(); 

        //
        // brief:
        // Verifies whether ir is possible to define a new 'drawing' region,
        // either above or below.
        // Argument: `eligible_surfaces` stores the indices of surfaces that can
        // be used as input either for an `defineAbove()` or a `defineBelow()`.
        // Return: true if at least one elegible surface was found.
        //
        /* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

        bool requestDefineAbove( std::vector<size_t> &eligible_surfaces );
        bool requestDefineBelow( std::vector<size_t> &eligible_surfaces );




        /* Change the model's properties */


        bool requestChangeDiscretizationLevel( size_t level_width, size_t level_height ); 

        void setOrigin( double opengl_x, double opengl_y, double opengl_z ); 

        bool setLenght( double opengl_x, double opengl_y, double opengl_z ); 


        /* Begin methods to interface with GUI */

        /* Initialization and clean up */

        bool init();

        void clear();


        /* Query or modify the automatum state */


        //
        // brief:
        // Define new input region above surface which index is `surface_index`.
        //
        bool defineAbove( size_t surface_index );


        //
        // brief:
        // Clear any previous `defineAbove()` call.
        // Safe to call anytime.
        //
        void stopDefineAbove();

        //
        // brief:
        // Define new input region below surface which index is `surface_index`.
        //
        bool defineBelow( size_t surface_index );

        //
        // brief:
        // Clear any previous `defineBelow()` call.
        // Safe to call anytime.
        //
        void stopDefineBelow();

        bool defineAboveIsActive(); 
        bool defineBelowIsActive();

        State currentGelogicRule();

        void removeAbove();
        void removeAboveIntersection();

        void removeBelow();
        void removeBelowIntersection(); 

        bool insertSurface( const std::vector<float> &point_data, size_t surface_id ); 

        bool insertSurface( const std::vector<float> &point_data, size_t surface_id, 
                const std::vector<size_t> lower_bound_ids, const std::vector<float> upper_bound_ids );

        template<typename CurveType>
        bool insertSurface( const CurveType &curve, size_t id );

        template<typename CurveType>
        bool insertSurface( const CurveType &curve, size_t id, 
                std::vector<size_t> lower_bound_ids, std::vector<size_t> upper_bound_ids ); 

        bool update( const State s ); 

        template<typename CurveType>
        bool update(const CurveType &curve, size_t id);

        template<typename CurveType>
        bool update(
                const CurveType &curve, 
                size_t id, 
                std::vector<size_t> lower_bound_ids, 
                std::vector<size_t> upper_bound_ids
            );

        bool canUndo();
        bool undo();

        bool canRedo();
        bool redo();


        /* Get mesh, pcl and curves for visualization */

        template<typename VertexList, typename FaceList>
        bool getInteriorPCL( VertexList &vlist, FaceList &flist ); 

        template<typename VertexList, typename FaceList>
        bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist );

        template<typename VertexList, typename EdgeList>
        bool getCurve( size_t surface_id, VertexList &vlist, EdgeList &elist ); 

        /* End methods to interface with GUI */

        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps = 1 );

    private:
        StratigraphyModeller modeller; 

        double origin_z_;

        enum class InternalState : int {
            UNDEFINED = -1,
            TRUNCATE_SKETCH,
            REMOVE_ABOVE,
            REMOVE_ABOVE_INTERSECTION,
            REMOVE_BELOW,
            REMOVE_BELOW_INTERSECTION
        };

        State current_state_;


        bool addSurface( 
                const std::vector<double> &curve, 
                size_t index, 
                std::vector<size_t> lb_id = std::vector<size_t>(), 
                std::vector<size_t> ub_id = std::vector<size_t>() 
            );
    };

    template<typename CurveType>
    bool ExtrusionRulesProcessor::update(const CurveType &curve, size_t id)
    {
        return update( curve, id, std::vector<size_t>(), std::vector<size_t>() ); 
    }
        
    template<typename CurveType>
    bool ExtrusionRulesProcessor::update(const CurveType &in_curve, size_t id, 
                std::vector<size_t> lower_bound_id, std::vector<size_t> upper_bound_id) 
    {
        if ( isInitialized() == false ) { 
            return false; 
        }

        std::vector<double> curve; //( 3*in_curve.size() ); 

        for ( size_t i = 0; i < in_curve.size(); ++i ) 
        {
            curve.push_back(in_curve[i].x());
            curve.push_back(in_curve[i].y());
            curve.push_back(origin_z_);
        }

        return addSurface( curve, id, lower_bound_id, upper_bound_id ); 
    }


    template<typename CurveType>
    bool insertSurface( const CurveType &curve, size_t id )
    {
        return update(curve, id);
    }

    template<typename CurveType>
    bool insertSurface( const CurveType &curve, size_t id, 
            std::vector<size_t> lower_bound_ids, std::vector<size_t> upper_bound_ids )
    {
        return update( curve, id, std::move(lower_bound_ids), std::move(upper_bound_ids) );
    }

    template<typename VertexList, typename FaceList>
    bool ExtrusionRulesProcessor::getMesh(size_t surface_index, VertexList &vlist, FaceList &flist)
    {
        return modeller.getMesh(surface_index, vlist, flist); 

    }

    template<typename VertexList, typename FaceList>
    bool ExtrusionRulesProcessor::getInteriorPCL( VertexList &vlist, FaceList &flist )
    {
        _RP_UNUSED(vlist); _RP_UNUSED(flist);
        return false;
    }

    template<typename VertexList, typename EdgeList>
    bool ExtrusionRulesProcessor::getCurve(size_t surface_id, VertexList &vlist, EdgeList &elist)
    {
        return modeller.getCrossSectionDepth(surface_id, vlist, elist, 0);
    }


} // End of Namespace RRM

#endif
