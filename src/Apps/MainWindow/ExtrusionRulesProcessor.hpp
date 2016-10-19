#ifndef __EXTRUSION_RULES_PROCESSOR__
#define __EXTRUSION_RULES_PROCESSOR__

#include <iostream>
#include <vector>
#include <map>


#include "Model/GeologicRules.hpp"
#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"

#include "./libs/PlanInLib/srules.hpp"
#include "./libs/PlanInLib/planar_surface.hpp"
#include "./libs/PlanInLib/core.hpp"




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
        bool requestDefineRegion( std::vector<size_t> &eligible_surfaces );

        bool requestDefineAbove( std::vector<size_t> &eligible_surfaces );
        bool requestDefineBelow( std::vector<size_t> &eligible_surfaces );




        /* Change the model's properties */

        size_t getMaxDiscretizationLevel() const;  

        bool setMaxDiscretizationLevel( size_t level ); 

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

    private:
        class SRules container_;

        // New type names for clarity
        using ControllerSurfaceIndex = size_t; 
        using ContainerSurfaceIndex = size_t; 

        // The history
        std::map<ControllerSurfaceIndex, ContainerSurfaceIndex> dictionary_;
        std::vector<ControllerSurfaceIndex> inserted_surfaces_indices_; 

        // Model properties
        Point3 origin_, lenght_; 

        size_t numI_, numJ_; 
        size_t max_discretization_level_; 
        size_t level_I_, level_J_;

        // Class status
        bool initialized_ = false;
        bool got_origin_ = false; 
        bool got_lenght_ = false;


        enum class InternalState : int {
            UNDEFINED = -1,
            TRUNCATE_SKETCH,
            REMOVE_ABOVE,
            REMOVE_ABOVE_INTERSECTION,
            REMOVE_BELOW,
            REMOVE_BELOW_INTERSECTION
        };

        struct StateDescriptor
        {
            State state_ = State::UNDEFINED;
            bool bounded_above_ = false;
            size_t upper_boundary_ = 0;
            bool bounded_below_ = false;
            size_t lower_boundary_ = 0;
        };
        StateDescriptor current_;

        // Undo stack
        std::vector<PlanarSurface::Ptr> undoed_surfaces_stack_; 
        std::vector<size_t> undoed_surfaces_indices_;
        std::vector<StateDescriptor> undoed_states_;
        std::vector<StateDescriptor> past_states_;

        // Helper methods
        void requestChangeDiscretization( size_t discretization_WIDTH, size_t discretization_DEPTH ); 

        bool getSurfaceIndex( const size_t controller_index, size_t &container_index ) const;

        bool getSurfaceID( const size_t controller_index, PlanarSurface::SurfaceId &surface_id );

        bool addSurface( 
                const std::vector<Point3> &curve, 
                size_t index, 
                std::vector<size_t> lb_id = std::vector<size_t>(), 
                std::vector<size_t> ub_id = std::vector<size_t>() 
            );

        void registerState(ControllerSurfaceIndex given_index, ContainerSurfaceIndex index); 

        bool enforceDefineRegion(); 

        bool commitSurface( 
                PlanarSurface::Ptr &sptr, 
                size_t index, 
                std::vector<size_t> lb_id, 
                std::vector<size_t> ub_id
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

        std::vector<Point3> curve( in_curve.size() ); 
        Point3 p;

        for ( size_t i = 0; i < curve.size(); ++i ) 
        {
            p.x = in_curve[i].x();
            p.z = in_curve[i].y();
            p.y = origin_.depth;

            curve[i] = p;
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
        size_t index; 
        if ( getSurfaceIndex(surface_index, index) == false )
        {
            return false; 
        }

        return container_[index]->getMesh(vlist, flist); 
    }

    template<typename VertexList, typename FaceList>
    bool ExtrusionRulesProcessor::getInteriorPCL( VertexList &vlist, FaceList &flist )
    {
        return false;
    }

    template<typename VertexList, typename EdgeList>
    bool ExtrusionRulesProcessor::getCurve(size_t surface_id, VertexList &vlist, EdgeList &elist)
    {
        size_t index; 
        if ( getSurfaceIndex(surface_id, index) == false )
        {
            return false; 
        }

        PlanarSurface::Ptr sptr( container_[index] ); 

        vlist.resize( 2 * sptr->getNumX() );
        elist.clear(); 

        double height, previous_height; 
        bool status, previous_status; 

        sptr->getHeight(0, 0, height);
        vlist[0] = origin_.x;
        vlist[1] = height;

        for ( size_t i = 1; i < sptr->getNumX(); ++i )
        {
            status          = sptr->getHeight(    i, 0, height);
            previous_status = sptr->getHeight(i - 1, 0, previous_height);

            vlist[2*i + 0] = origin_.x + (float)(i) * lenght_.x / ( (float)(sptr->getNumX() - 1) );
            vlist[2*i + 1] = height;

            if ( status || previous_status )
            {
                elist.push_back(i - 1);
                elist.push_back(i);
            }
        }

        return true;
    }


} // End of Namespace RRM

#endif
