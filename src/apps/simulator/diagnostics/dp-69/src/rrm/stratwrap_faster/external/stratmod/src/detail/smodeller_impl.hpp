/********************************************************************************/
/*                                                                              */
/* This file is part of the "Stratigraphy Modeller Library" (StratModLib)       */
/* Copyright (C) 2017, Julio Daniel Machado Silva.                              */
/*                                                                              */
/* StratModLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                   */
/* License as published by the Free Software Foundation; either                 */
/* version 3 of the License, or (at your option) any later version.             */
/*                                                                              */
/* StratModLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU            */
/* Lesser General Public License for more details.                              */
/*                                                                              */
/* You should have received a copy of the GNU Lesser General Public             */
/* License along with StratModLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,          */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                    */
/*                                                                              */
/********************************************************************************/


#ifndef STRATMOD_SMODELLER_IMPLEMENTATION_HPP
#define STRATMOD_SMODELLER_IMPLEMENTATION_HPP


#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

#include "detail/serialization_definitions.hpp"
#include "detail/testing_definitions.hpp"

#include "planin/planin.hpp"
#include "planin/mesh/linear_complexes.hpp"

#include "stratmod/model_interpretation.hpp"
#include "stratmod/smodeller.hpp"

namespace stratmod {

#define TODO_UNUSED(x) (void)(x)

/************************************************/
/* Auxiliar Data Types                          */
/************************************************/

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

enum class InternalState : int {
    UNDEFINED = -1,
    TRUNCATE_SKETCH,
    REMOVE_ABOVE,
    REMOVE_ABOVE_INTERSECTION,
    REMOVE_BELOW,
    REMOVE_BELOW_INTERSECTION
};

// New type names for clarity
using ControllerSurfaceIndex = std::size_t; 
using ContainerSurfaceIndex = std::size_t; 

struct StateDescriptor
{
    State state_ = State::UNDEFINED;

    bool bounded_above_ = false;
    ControllerSurfaceIndex upper_boundary_ = 0;
    std::vector<ControllerSurfaceIndex> upper_boundary_list_ = std::vector<ControllerSurfaceIndex>();

    bool bounded_below_ = false;
    ControllerSurfaceIndex lower_boundary_ = 0;
    std::vector<ControllerSurfaceIndex> lower_boundary_list_ = std::vector<ControllerSurfaceIndex>();

    std::vector<ControllerSurfaceIndex> truncate_lower_boundary_ = std::vector<ControllerSurfaceIndex>();
    std::vector<ControllerSurfaceIndex> truncate_upper_boundary_ = std::vector<ControllerSurfaceIndex>();

    template<typename Archive>
    void save( Archive &ar, const std::uint32_t version ) const
    {
        (void)(version);

        ar( state_,
            bounded_above_, upper_boundary_list_, 
            bounded_below_, lower_boundary_list_,
            truncate_lower_boundary_, 
            truncate_upper_boundary_
          );
    }

    template<typename Archive>
    void load( Archive &ar, const std::uint32_t version )
    {
        if ( version == 1 )
        {
            ar( state_,
                bounded_above_, upper_boundary_, 
                bounded_below_, lower_boundary_,
                truncate_lower_boundary_, 
                truncate_upper_boundary_
              );

            upper_boundary_list_ = { upper_boundary_ };
            lower_boundary_list_ = { lower_boundary_ };
        }
        else
        {
            ar( state_,
                bounded_above_, upper_boundary_list_, 
                bounded_below_, lower_boundary_list_,
                truncate_lower_boundary_, 
                truncate_upper_boundary_
              );
        }
    }

};


/************************************************/
/* Implementation Definition                    */
/************************************************/

struct SModellerImplementation 
{
    /************************************************/
    /* Internal Data                                */
    /************************************************/
    class SRules container_;

    // The history
    std::map<ControllerSurfaceIndex, ContainerSurfaceIndex> dictionary_;
    std::vector<ControllerSurfaceIndex> inserted_surfaces_indices_; 

    // Model properties
    Point3 origin_, lenght_; 

    size_t discWidth_ = 64, discLenght_ = 64; 
    size_t numI_, numJ_; 
    size_t max_discretization_level_; 
    size_t level_I_, level_J_;

    bool default_coordinate_system_ = true;

    // Class status
    bool initialized_ = false;
    bool got_origin_ = false; 
    bool got_lenght_ = false;

    StateDescriptor current_;

    // Undo stack
    std::vector<PlanarSurface::Ptr> undoed_surfaces_stack_; 
    std::vector<size_t> undoed_surfaces_indices_;
    std::vector<StateDescriptor> undoed_states_;
    std::vector<StateDescriptor> past_states_;

    // Volumetric information
    std::shared_ptr<LegacyTetrahedralMeshBuilder> mesh_;


    // Metadata and interpretation
    std::unordered_map<std::string, ModelInterpretation> interpretations_{};

    struct RandomId {
        std::default_random_engine re{};
        std::uniform_int_distribution<unsigned int> dist;

        RandomId()
        {
            unsigned int min = std::numeric_limits<unsigned int>::min();
            unsigned int max = std::numeric_limits<unsigned int>::max();
            dist = std::uniform_int_distribution<unsigned int>(min, max);

            auto now = std::chrono::system_clock::now();
            unsigned long int time = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
            re.seed(time);
        }

        ~RandomId() = default;

        static unsigned int Get()
        { 
            static RandomId instance;
            return instance.dist(instance.re); 
        };

        unsigned int get()
        {
            return dist(re);
        }
    };

    unsigned int model_id_ = RandomId::Get();


    /* template<typename Archive> */
    /* void save( Archive &ar, const std::uint32_t /1* version *1/ ) const */
    /* { */
    /*     ar( container_, */
    /*             dictionary_, */
    /*             inserted_surfaces_indices_, */
    /*             origin_, lenght_, */
    /*             discWidth_, discLenght_, */
    /*             numI_, numJ_, */
    /*             max_discretization_level_, */
    /*             level_I_, level_J_, */
    /*             default_coordinate_system_, */
    /*             initialized_, */
    /*             got_origin_, got_lenght_, */
    /*             current_, */
    /*             undoed_surfaces_stack_, */
    /*             undoed_surfaces_indices_, */
    /*             undoed_states_, */
    /*             past_states_, */
    /*             interpretations_ */
    /*       ); */
    /* } */

    template<typename Archive>
    void serialize( Archive &ar, const std::uint32_t version )
    {
        ar(
                container_,
                dictionary_,
                inserted_surfaces_indices_,
                origin_, lenght_,
                discWidth_, discLenght_,
                numI_, numJ_,
                max_discretization_level_,
                level_I_, level_J_,
                default_coordinate_system_,
                initialized_,
                got_origin_, got_lenght_,
                current_,
                undoed_surfaces_stack_,
                undoed_surfaces_indices_,
                undoed_states_,
                past_states_
          );

        if (version == 2)
        {
            ar(interpretations_);
            ar(model_id_);
        }
    }

    /************************************************/
    /* Methods                                      */
    /************************************************/

    bool init();

    void clear();

    /** Converts (x, y, z) triplet to PlanIn's Point3 accordingly to current coordinate system
     *
     *  If coordinate system is default, output will be Point3 {x, y, z}. 
     *  If coordinate system is OpenGL, output will be Point3 {x, z, y}.
     */
    Point3 point3( double x, double y, double z );

    void getOrigin( double &x, double &y, double &z );

    void getLenght( double &x, double &y, double &z );

    std::vector<size_t> getSurfacesIndicesBelowPoint( double x, double y, double z );

    std::vector<size_t> getSurfacesIndicesAbovePoint( double x, double y, double z );

    std::vector<std::size_t> getSurfacesIndices() const { return inserted_surfaces_indices_; }

    std::vector<std::size_t> getOrderedSurfacesIndices();

    bool getBoundingSurfacesFromRegionID( std::size_t region_id, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound);

    bool lastInsertedSurfaceIntersects( std::vector<ControllerSurfaceIndex> &intersected_surfaces_ids );

    bool getControllerIndex( const ContainerSurfaceIndex surface_id, ControllerSurfaceIndex &controller_id );

    bool getSurfaceIndex( const size_t controller_index, size_t &index ) const;

    bool getPlanarSurfaceId( const size_t controller_index, PlanarSurface::SurfaceId &surface_id );

    bool getControllerIndexFromPlanarSurfaceId( const PlanarSurface::SurfaceId surface_id, ControllerSurfaceIndex &controller_index );

    bool parseTruncateSurfaces( std::vector<size_t> &lbounds, std::vector<size_t> &ubounds );

    bool isInitialized();

    bool insertSurface( const std::vector<double> &point_data, size_t surface_id, 
            const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids, 
            bool extruded_surface = false, bool orthogonally_oriented = false, double fill_distance = -1.0 );

    bool insertExtrusionAlongPath( size_t surface_id, 
            const std::vector<double> &cross_section_curve, double cross_section_depth,
            const std::vector<double> &path_curve,
            const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids,
            bool orthogonally_oriented = false, 
            double fill_distance = -1.0 ); 

    bool commitSurface( PlanarSurface::Ptr &sptr, size_t given_index, 
            std::vector<size_t> lbounds, std::vector<size_t> ubounds );

    bool popLastSurface();

    bool canUndo();
    bool canRedo();

    bool popUndoStack();

    bool preserveAbove( std::vector<size_t> bounding_surfaces_list );
    bool preserveBelow( std::vector<size_t> bounding_surfaces_list );

    void stopPreserveAbove();
    void stopPreserveBelow();

    bool preserveAboveIsActive( std::vector<std::size_t> &bounding_surfaces_list );
    bool preserveBelowIsActive( std::vector<std::size_t> &bounding_surfaces_list );

    bool createAboveIsActive();

    bool createAboveIsActive( size_t &boundary_index );

    bool createBelowIsActive();

    bool createBelowIsActive( size_t &boundary_index );

    bool createBelow( size_t surface_index );

    void stopCreateBelow();

    bool createAbove( size_t surface_index );

    void stopCreateAbove();

    bool enforceDefineRegion();

    bool buildTetrahedralMesh();

    std::size_t numSurfaces() { return inserted_surfaces_indices_.size(); }

    int maxNumRegions() { return static_cast<int>(numSurfaces()) - 1; }

    template<typename VertexList>
        bool getVertexList( size_t surface_id, VertexList &vlist);

    template<typename VertexList, typename FaceList>
        bool getMesh( size_t surface_id, VertexList &vlist, FaceList &flist );

    template<typename VertexList, typename EdgeList>
        bool getCrossSectionWidth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t width );

    template<typename VertexList, typename EdgeList>
        bool getAdaptedCrossSectionAtConstantWidth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t width );

    template<typename VertexList, typename EdgeList>
        bool getCrossSectionDepth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t length );

    template<typename VertexList, typename EdgeList>
        bool getAdaptedCrossSectionAtConstantLength( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t length );
};



/************************************************/
/* Template Implementation                      */
/************************************************/

template<typename VertexList>
bool SModellerImplementation::getVertexList( size_t surface_id, VertexList &vlist)
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }
    INFO( "Getting vertex list for surface: " << index );

    return container_[index]->getVertexList(vlist); 
}

template<typename VertexList, typename FaceList>
bool SModellerImplementation::getMesh( size_t surface_id, VertexList &vlist, FaceList &flist )
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    /* return ( container_[index]->getMesh(vlist, flist) > 0 ); */ 
    bool success = (container_[index]->getFaceList(flist) > 0);
    if ( success )
    {
        success &= container_[index]->getVertexList(vlist);
    }

    return success;
}


template<typename VertexList, typename EdgeList>
bool SModellerImplementation::getCrossSectionWidth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t width )
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( container_[index] ); 

    PlanarSurface::Natural Nwidth = 2 * static_cast<PlanarSurface::Natural>(width);

    if ( Nwidth >= sptr->getNumX() )
    {
        return false;
    }

    vlist.resize( 2 * sptr->getNumY() );
    elist.clear(); 

    double height, previous_height; 
    bool status, previous_status; 

    /* TODO: have a look at those types and their conversions */
    using OutRealType = typename VertexList::value_type;
    using OutNaturalType = typename EdgeList::value_type;

    /* sptr->getCachedHeight( Nwidth, 0, height ); */
    sptr->getHeight( Nwidth, 0, height );
    vlist[0] = static_cast<OutRealType>( origin_.y );
    vlist[1] = static_cast<OutRealType>( height );

    bool has_curve = false;

    for ( PlanarSurface::Natural i = 1; i < sptr->getNumY(); ++i )
    {
        /* status          = sptr->getCachedHeight(Nwidth,     i, height); */
        status          = sptr->getHeight(Nwidth,     i, height);
        /* previous_status = sptr->getCachedHeight(Nwidth, i - 1, previous_height); */
        previous_status = sptr->getHeight(Nwidth, i - 1, previous_height);

        vlist[2*i + 0] = static_cast<OutRealType>( origin_.y + (double)(i) * lenght_.y / ( (double)(sptr->getNumY() - 1) ) );
        vlist[2*i + 1] = static_cast<OutRealType>( height );

        if ( status || previous_status )
        {
            elist.push_back( static_cast<OutNaturalType>(i - 1) );
            elist.push_back( static_cast<OutNaturalType>(i) );
            has_curve |= true;
        }
    }

    return has_curve;
}


template<typename VertexList, typename EdgeList>
bool SModellerImplementation::getCrossSectionDepth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t length )
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( container_[index] ); 

    PlanarSurface::Natural Ndepth = 2 * static_cast<PlanarSurface::Natural>(length);

    if ( Ndepth >= sptr->getNumY() )
    {
        return false;
    }

    vlist.resize( 2 * sptr->getNumX() );
    elist.clear(); 

    double height, previous_height; 
    bool status, previous_status; 

    /* TODO: have a look at those types and their conversions */
    using OutRealType = typename VertexList::value_type;
    using OutNaturalType = typename EdgeList::value_type;

    /* sptr->getCachedHeight(0, Ndepth, height); */
    sptr->getHeight(0, Ndepth, height);
    vlist[0] = static_cast<OutRealType>( origin_.x );
    vlist[1] = static_cast<OutRealType>( height );

    bool has_curve = false;

    for ( PlanarSurface::Natural i = 1; i < sptr->getNumX(); ++i )
    {
        /* status          = sptr->getCachedHeight(    i, Ndepth, height); */
        status          = sptr->getHeight(    i, Ndepth, height);
        /* previous_status = sptr->getCachedHeight(i - 1, Ndepth, previous_height); */
        previous_status = sptr->getHeight(i - 1, Ndepth, previous_height);

        vlist[2*i + 0] = static_cast<OutRealType>( origin_.x + (double)(i) * lenght_.x / ( (double)(sptr->getNumX() - 1) ) );
        vlist[2*i + 1] = static_cast<OutRealType>( height );

        if ( status || previous_status )
        {
            elist.push_back( static_cast<OutNaturalType>( i - 1 ) );
            elist.push_back( static_cast<OutNaturalType>( i ) );
            has_curve |= true;
        }
    }

    return has_curve;
}

template<typename VertexList, typename EdgeList>
bool SModellerImplementation::getAdaptedCrossSectionAtConstantWidth( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t width )
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( container_[index] ); 

    PlanarSurface::Natural Nwidth = 2 * static_cast<PlanarSurface::Natural>(width);

    if ( Nwidth >= sptr->getNumX() )
    {
        return false;
    }

    if ( sptr->getNumY() <= 0 )
    {
        return false;
    }

    vlist.resize( 2 * sptr->getNumY() );
    elist.clear(); 

    double height, previous_height, abscissa, ordinate, previous_abscissa, previous_ordinate; 
    bool status, previous_status; 
    PlanarSurface::SurfaceId bsid, pbsid;
    size_t bindex; //, intersection_point_index;

    /* TODO: have a look at those types and their conversions */
    using OutRealType = typename VertexList::value_type;
    /* using OutNaturalType = typename EdgeList::value_type; */

    /* auto computeIntersection = [&]( PlanarSurface intersected, std::vector<double> &intersected_vlist, PlanarSurface base, &base_vlist) -> void */ 
    /* { */
    /*     std::vector<double> vertices = {}; */

    /*     double sa_0_abs, sa_1_abs, sb_0_abs, sb_1_abs; */
    /*     double sa_0_ord, sa_1_ord, sb_0_ord, sb_1_ord; */

    /*     size_t i; */
    /*     sptr->getRawHeight(Nwidth, i, sa_0_abs); */
    /*     sptr->getRawHeight(Nwidth, i-1, sa_1_abs); */

    /*     sptr->getRawHeight(Nwidth, i, sa_0_ord); */
    /*     sptr->getRawHeight(Nwidth, i-1, sa_1_ord); */
    /* }; */

    /* sptr->getCachedHeight( Nwidth, 0, height ); */
    bool has_curve = false;
    size_t num_segments = 0;

    EdgeList current;

    VertexList intersectec_vlist;
    intersectec_vlist.resize(4);

    /* std::vector<double> intersected_vlist = {0., 0., 0., 0., 0., 0.}; */ 
    /* std::vector<double> base_vlist = {0., 0., 0., 0., 0., 0.}; */

    size_t source = 0;
    size_t sink = 1;
    for ( PlanarSurface::Natural i = 1; i < sptr->getNumY(); i += 1 )
    {
        source = i-1;
        sink = i;

        previous_status = sptr->getHeight( Nwidth, source, previous_height, pbsid );
        sptr->getRawHeight( Nwidth, source, previous_height );
        previous_abscissa = static_cast<OutRealType>( origin_.y + (double)(i-1) * lenght_.y / ( (double)(sptr->getNumY() - 1) ) );
        previous_ordinate = static_cast<OutRealType>( previous_height );
        vlist[2*source + 0] = previous_abscissa;
        vlist[2*source + 1] = previous_ordinate;

        status = sptr->getHeight( Nwidth, sink, height, bsid );
        sptr->getRawHeight( Nwidth, sink, height );
        abscissa = static_cast<OutRealType>( origin_.y + (double)(i) * lenght_.y / ( (double)(sptr->getNumY() - 1) ) );
        ordinate = static_cast<OutRealType>( height );
        vlist[2*sink + 0] = abscissa;
        vlist[2*sink + 1] = ordinate;

        Segment<VertexList, 2> segment(vlist, source, sink, previous_status, status);

        if ( !previous_status != !status )
        {
            if ( !previous_status && status )
            {
                container_.getSurfaceIndex(pbsid, bindex);
            }
            else if ( previous_status && !status )
            {
                container_.getSurfaceIndex(bsid, bindex);
            }

            container_[bindex]->getRawHeight(Nwidth, source, previous_height);
            intersectec_vlist[0] = previous_abscissa;
            intersectec_vlist[1] = previous_height;

            container_[bindex]->getRawHeight(Nwidth, sink, height);
            intersectec_vlist[2] = abscissa;
            intersectec_vlist[3] = height;

            Segment<VertexList, 2> intersected(intersectec_vlist, 0, 1);
            //intersection_point_index = segment.computeVerticalIntersection( intersected );
            segment.computeVerticalIntersection( intersected );
        }

        num_segments = segment.getConnectivity(current);
        if ( num_segments > 0 )
        {
            std::copy( current.begin(), current.end(), std::back_inserter(elist) );
        }
        
        has_curve |= (num_segments > 0);
        /* status          = sptr->getHeight(Nwidth,     i, height, bsid); */
        /* sptr->getRawHeight(Nwidth, i, height); */

        /* previous_status = sptr->getHeight(Nwidth, i - 1, previous_height, pbsid); */
    }

    return has_curve;
}

template<typename VertexList, typename EdgeList>
bool SModellerImplementation::getAdaptedCrossSectionAtConstantLength( size_t surface_id, VertexList &vlist, EdgeList &elist, size_t length )
{
    size_t index; 
    if ( getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    PlanarSurface::Ptr sptr( container_[index] ); 

    PlanarSurface::Natural Nlength = 2 * static_cast<PlanarSurface::Natural>(length);

    if ( Nlength >= sptr->getNumY() )
    {
        return false;
    }

    if ( sptr->getNumX() <= 0 )
    {
        return false;
    }

    vlist.resize( 2 * sptr->getNumX() );
    elist.clear(); 

    double height, previous_height, abscissa, ordinate, previous_abscissa, previous_ordinate; 
    bool status, previous_status; 
    PlanarSurface::SurfaceId bsid, pbsid;
    size_t bindex; //, intersection_point_index;

    /* TODO: have a look at those types and their conversions */
    using OutRealType = typename VertexList::value_type;
    /* using OutNaturalType = typename EdgeList::value_type; */

    /* auto computeIntersection = [&]( PlanarSurface intersected, std::vector<double> &intersected_vlist, PlanarSurface base, &base_vlist) -> void */ 
    /* { */
    /*     std::vector<double> vertices = {}; */

    /*     double sa_0_abs, sa_1_abs, sb_0_abs, sb_1_abs; */
    /*     double sa_0_ord, sa_1_ord, sb_0_ord, sb_1_ord; */

    /*     size_t i; */
    /*     sptr->getRawHeight(Nlength, i, sa_0_abs); */
    /*     sptr->getRawHeight(Nlength, i-1, sa_1_abs); */

    /*     sptr->getRawHeight(Nlength, i, sa_0_ord); */
    /*     sptr->getRawHeight(Nlength, i-1, sa_1_ord); */
    /* }; */

    /* sptr->getCachedHeight( Nlength, 0, height ); */
    bool has_curve = false;
    size_t num_segments = 0;

    EdgeList current;

    VertexList intersectec_vlist;
    intersectec_vlist.resize(4);

    /* std::vector<double> intersected_vlist = {0., 0., 0., 0., 0., 0.}; */ 
    /* std::vector<double> base_vlist = {0., 0., 0., 0., 0., 0.}; */

    size_t source = 0;
    size_t sink = 1;
    for ( PlanarSurface::Natural i = 1; i < sptr->getNumX(); i += 1 )
    {
        source = i-1;
        sink = i;

        previous_status = sptr->getHeight( source, Nlength, previous_height, pbsid );
        sptr->getRawHeight( source, Nlength, previous_height );
        previous_abscissa = static_cast<OutRealType>( origin_.x + (double)(i-1) * lenght_.x / ( (double)(sptr->getNumX() - 1) ) );
        previous_ordinate = static_cast<OutRealType>( previous_height );
        vlist[2*source + 0] = previous_abscissa;
        vlist[2*source + 1] = previous_ordinate;

        status = sptr->getHeight( sink, Nlength, height, bsid );
        sptr->getRawHeight( sink, Nlength, height );
        abscissa = static_cast<OutRealType>( origin_.x + (double)(i) * lenght_.x / ( (double)(sptr->getNumX() - 1) ) );
        ordinate = static_cast<OutRealType>( height );
        vlist[2*sink + 0] = abscissa;
        vlist[2*sink + 1] = ordinate;

        /* std::cout << "\nSegment " << i-1 */ 
        /*     << ": [( " << previous_abscissa << ", " << previous_ordinate << "), " */
        /*     << "( " << abscissa << ", " << ordinate << ")] "; */


        Segment<VertexList, 2> segment(vlist, source, sink, previous_status, status);

        // != is equivalent to xor in c++
        size_t cid;
        if ( !previous_status != !status )
        {
            /* std::cout << "-- truncated segment: "; */
            if ( !previous_status && status )
            {
                container_.getSurfaceIndex(pbsid, bindex);
                getControllerIndex(bindex, cid);
                /* std::cout << "\n<-- Surface " << surface_id << " was intersected by surface: " */ 
                    /* << "cid: " << cid << ", index: " << bindex << ", sID: " << pbsid << "\n"; */
                /* std::cout << "first point is invalid"; */
            }
            else if ( previous_status && !status )
            {
                container_.getSurfaceIndex(bsid, bindex);
                getControllerIndex(bindex, cid);
                /* std::cout << "\n--> Surface " << surface_id << " was intersected by surface: " */ 
                    /* << "cid: " << cid << ", index: " << bindex << ", sID: " << bsid << "\n"; */
                /* std::cout << "second point is invalid"; */
            }

            container_[bindex]->getRawHeight(source, Nlength, previous_height);
            intersectec_vlist[0] = previous_abscissa;
            intersectec_vlist[1] = previous_height;

            container_[bindex]->getRawHeight(sink, Nlength, height);
            intersectec_vlist[2] = abscissa;
            intersectec_vlist[3] = height;

            Segment<VertexList, 2> intersected(intersectec_vlist, 0, 1);
            /* intersection_point_index = segment.computeVerticalIntersection( intersected ); */
            segment.computeVerticalIntersection( intersected );
        }

        num_segments = segment.getConnectivity(current);
        if ( num_segments > 0 )
        {
            std::copy( current.begin(), current.end(), std::back_inserter(elist) );
        }

        /* std::cout << "\n     num_segments: " << num_segments << ", connectivity: (" << current[0] << ", " << current[1] << ") \n"; */

        has_curve |= (num_segments > 0);

        /* status          = sptr->getHeight(Nlength,     i, height, bsid); */
        /* sptr->getRawHeight(Nlength, i, height); */

        /* previous_status = sptr->getHeight(Nlength, i - 1, previous_height, pbsid); */
    }

    return has_curve;
}

} // namespace stratmod

CEREAL_CLASS_VERSION(stratmod::StateDescriptor, 2);
CEREAL_CLASS_VERSION(stratmod::SModellerImplementation, 2);

#endif

