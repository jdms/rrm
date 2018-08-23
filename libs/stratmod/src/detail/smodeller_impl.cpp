
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "detail/planin/planin.hpp"

#include "smodeller_impl.hpp"

#include "testing_definitions.hpp"



bool SModellerImplementation::init()
{
    if ( got_origin_ == false ) { 
        origin_ = {{{ 0., 0., 0. }}};
    }

    if ( got_lenght_ == false ) { 
        lenght_ = {{{ 1., 1., 1. }}};
    }

    current_.state_ = State::RAI_SKETCHING;

    /* if ( max_discretization_level_ == 0 ) { */ 
    /*     setMaxDiscretizationLevel(7); */ 
    /*     requestChangeDiscretizationLevel( getMaxDiscretizationLevel(), getMaxDiscretizationLevel() ); */ 
    /* } */

    PlanarSurface::requestChangeDiscretization(
            static_cast<PlanarSurface::Natural>( discWidth_ ), 
            static_cast<PlanarSurface::Natural>( discLenght_ ) );

    /*         PlanarSurface::Coordinate::WIDTH, */
    /*         PlanarSurface::Coordinate::DEPTH, */ 
    /*         PlanarSurface::Coordinate::HEIGHT); */ 

    initialized_ = true; 

    return true;
}

Point3 SModellerImplementation::point3( double x, double y, double z )
{
    Point3 p;

    if ( default_coordinate_system_ )
    {
        p = {{{ x, y, z }}}; 
    }
    else
    {
        p = {{{ x, z, y }}}; 
    }

    return p;
}

void SModellerImplementation::getOrigin( double &x, double &y, double &z )
{
    if ( default_coordinate_system_ )
    {
        x = origin_.x;
        y = origin_.y;
        z = origin_.z;
    }
    else
    {
        x = origin_.x;
        y = origin_.z;
        z = origin_.y;
    }
}

void SModellerImplementation::getLenght( double &x, double &y, double &z )
{
    if ( default_coordinate_system_ )
    {
        x = lenght_.x;
        y = lenght_.y;
        z = lenght_.z;
    }
    else
    {
        x = lenght_.x;
        y = lenght_.z;
        z = lenght_.y;
    }
}

std::vector<size_t> SModellerImplementation::getSurfacesIndicesBelowPoint( double x, double y, double z )
{
    std::vector<size_t> surfaces_ids = {};
    Point3 p = point3(x, y, z);

    surfaces_ids = container_.getActiveSurfacesBelowPoint(p);
    size_t cid;
    /* std::cout << "getSurfacesIndicesBelowPoint\n"; */
    for ( auto &i : surfaces_ids )
    {
        getControllerIndex(i, cid);
        i = cid;
    }

    return surfaces_ids;
}

std::vector<size_t> SModellerImplementation::getSurfacesIndicesAbovePoint( double x, double y, double z )
{
    std::vector<size_t> surfaces_ids = {};
    Point3 p = point3(x, y, z);

    surfaces_ids = container_.getActiveSurfacesAbovePoint(p);
    size_t cid;
    /* std::cout << "getSurfacesIndicesAbovePoint\n"; */
    for ( size_t i = 0; i < surfaces_ids.size(); ++i )
    {
        getControllerIndex(surfaces_ids[i], cid);
        surfaces_ids[i] = cid;
    }

    return surfaces_ids;
}

#include <iostream>

bool SModellerImplementation::lastInsertedSurfaceIntersects( std::vector<ControllerSurfaceIndex> &intersected_surfaces )
{
    std::vector<ContainerSurfaceIndex> ids = {};
    bool status = container_.lastInsertedSurfaceIntersects(ids);

    if ( status == false )
    {
        return false;
    }

    intersected_surfaces.resize( ids.size() );
    ControllerSurfaceIndex cindex;

    for ( size_t i = 0; i < ids.size(); ++i )
    {
        status &= getControllerIndex(ids[i], cindex);
        intersected_surfaces[i] = cindex;
    }

    return status;
}

bool SModellerImplementation::getControllerIndex( const ContainerSurfaceIndex surface_id, ControllerSurfaceIndex &controller_id )
{
    for ( auto iter = dictionary_.begin(); iter != dictionary_.end(); ++iter )
    {
        if ( iter->second == surface_id )
        {
            controller_id = iter->first;
            /* std::cout << "Surf id: " << surface_id << " corresponds to cont id: " << controller_id << std::endl; */
            return true;
        }
    }

    return false;
}

bool SModellerImplementation::getSurfaceIndex( const size_t controller_index, size_t &index ) const
{
    auto iter = dictionary_.find(controller_index);  

    if ( iter == dictionary_.end() ) { 
        WARN( "getSurfaceIndex(...) could not find index in dictionary" ); 
        return false; 
    }
    else { 
        index = iter->second;
    }

    return true;
}

bool SModellerImplementation::getPlanarSurfaceId( const size_t controller_index, PlanarSurface::SurfaceId &surface_id )
{
    size_t index; 

    if ( getSurfaceIndex(controller_index, index) == false ) {
        return false; 
    }

    /* PlanarSurface::Ptr sptr(container_[index]); */
    surface_id = container_[index]->getID();

    return false; 
}

bool SModellerImplementation::parseTruncateSurfaces( std::vector<size_t> &lbounds, std::vector<size_t> &ubounds )
{
    bool status = true; 
    /* std::vector<size_t> lbounds, ubounds; */

    auto & lbound_indices = current_.truncate_lower_boundary_;
    auto & ubound_indices = current_.truncate_upper_boundary_; 

    for ( auto &i : lbound_indices ) 
    {
        size_t j;
        status &= getSurfaceIndex(i, j);

        if ( status ) { 
            lbounds.push_back(j); 
        }
    }

    for ( auto &i : ubound_indices )
    {
        size_t j;
        status &= getSurfaceIndex(i, j);

        if ( status ) { 
            ubounds.push_back(j); 
        }
    }

    /* if ( status == false ) */
    /* { */
    /*     return false; */ 
    /* } */

    return status;
}

bool SModellerImplementation::isInitialized()
{
    return initialized_; 
}

bool SModellerImplementation::insertSurface( const std::vector<double> &point_data, size_t surface_id, 
        const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids, 
        bool extruded_surface, bool orthogonally_oriented  )
{
    INFO( "Got into pimpl_->insertSurface(...)" );

    if ( isInitialized() == false ) { 
        WARN( "Modeller not initialized" );
        return false; 
    }

    if ( point_data.size() % 3 != 0 ) {
        WARN( "point_data.size() is not a multiple of 3" );
        return false;
    }

    current_.truncate_lower_boundary_ = lower_bound_ids; 
    current_.truncate_upper_boundary_ = upper_bound_ids; 

    std::vector<size_t> lbounds, ubounds;
    bool status = parseTruncateSurfaces(lbounds, ubounds); 
    if ( status == false )
    {
        WARN( "Failed when parsing the truncating surfaces" );
        return false; 
    }

    /* Create a surface */
    PlanarSurface::Ptr sptr = std::make_shared<PlanarSurface>(extruded_surface, orthogonally_oriented);

    sptr->setOrigin(origin_); 
    sptr->setLenght(lenght_); 

    const size_t dim = 3;
    for ( size_t i = 0; i < point_data.size(); i += dim ) { 
        /* if ( default_coordinate_system_ ) */
        /* { */
        /*     sptr->addPoint( {{{ point_data[i+0], point_data[i+1], point_data[i+2] }}} ); */ 
        /* } */
        /* else */
        /* { */
        /*     sptr->addPoint( {{{ point_data[i+0], point_data[i+2], point_data[i+1] }}} ); */ 
        /* } */

        sptr->addPoint( point3(point_data[i+0], point_data[i+1], point_data[i+2]) );
    }

    sptr->generateSurface(); 

    /* Execute selected Geologic Rule */
    status = commitSurface(sptr, surface_id, lbounds, ubounds); 

    if ( ( status == true ) && ( undoed_surfaces_stack_.empty() == false ) )
    {
        undoed_surfaces_stack_.clear();
        undoed_surfaces_indices_.clear();
        undoed_states_.clear(); 
    }

    return status;

}

bool SModellerImplementation::insertExtrusionAlongPath( size_t surface_id, 
            const std::vector<double> &cross_section_curve, double cross_section_depth,
            const std::vector<double> &path_curve,
            const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids, 
            bool orthogonally_oriented )
{
    INFO( "Got into pimpl_->insertSurface(...)" );

    if ( isInitialized() == false ) { 
        WARN( "Modeller not initialized" );
        return false; 
    }

    size_t dim = 2;

    if ( cross_section_curve.size() % dim != 0 ) {
        WARN( "cross_section_curve.size() is not a multiple of 2" );
        return false;
    }

    if ( path_curve.size() % dim != 0 ) {
        WARN( "path_curve.size() is not a multiple of 2" );
        return false;
    }

    current_.truncate_lower_boundary_ = lower_bound_ids; 
    current_.truncate_upper_boundary_ = upper_bound_ids; 

    std::vector<size_t> lbounds, ubounds;
    bool status = parseTruncateSurfaces(lbounds, ubounds); 
    if ( status == false )
    {
        WARN( "Failed when parsing the truncating surfaces" );
        return false; 
    }

    /* Create a surface */
    bool extruded_surface = true;
    PlanarSurface::Ptr sptr = std::make_shared<PlanarSurface>(extruded_surface, orthogonally_oriented);

    sptr->setOrigin(origin_); 
    sptr->setLenght(lenght_); 

    for ( size_t i = 0; i < cross_section_curve.size(); i += dim ) { 
        /* if ( default_coordinate_system_ ) */
        /* { */
        /*     sptr->addPoint( {{{ cross_section_curve[i+0], cross_section_curve[i+1], cross_section_curve[i+2] }}} ); */ 
        /* } */
        /* else */
        /* { */
        /*     sptr->addPoint( {{{ cross_section_curve[i+0], cross_section_curve[i+2], cross_section_curve[i+1] }}} ); */ 
        /* } */

        sptr->addPoint( point3(cross_section_curve[i+0], cross_section_curve[i+1], cross_section_depth) );
    }

    for ( size_t i = 0; i < path_curve.size(); i += dim )
    {
        sptr->addExtrusionPathPoint( path_curve[i+1], path_curve[i+0] );
    }
    sptr->setPathOrigin( cross_section_depth, 0. );

    sptr->generateSurface(); 

    /* Execute selected Geologic Rule */
    status = commitSurface(sptr, surface_id, lbounds, ubounds); 

    if ( ( status == true ) && ( undoed_surfaces_stack_.empty() == false ) )
    {
        undoed_surfaces_stack_.clear();
        undoed_surfaces_indices_.clear();
        undoed_states_.clear(); 
    }

    return status;
} 
        
bool SModellerImplementation::commitSurface( 
        PlanarSurface::Ptr &sptr, 
        size_t given_index, 
        std::vector<size_t> lbounds, 
        std::vector<size_t> ubounds
        )
{
    size_t index;
    bool status; 

    auto iter = dictionary_.find(given_index);
    if ( iter != dictionary_.end() )
    {
        std::cout << "Tried to insert surface with duplicated index.\n";
        return false;
    }

    if ( current_.state_ == State::SKETCHING )
    {
        /* std::cout << "Inserting surface " << given_index << "\n"; */
        /* std::cout << "Ubounds: "; */
        /* for ( auto s : ubounds ) */
        /* { */
            /* std::cout << s << ", "; */
        /* } */
        /* std::cout << "\n"; */

        /* std::cout << "Lbounds: "; */
        /* for ( auto s : lbounds ) */
        /* { */
            /* std::cout << s << ", "; */
        /* } */
        /* std::cout << "\n"; */

        status = container_.addSurface(sptr, index, ubounds, lbounds);
    }
    else
    {
        status = container_.addSurface(sptr, index); 
    }

    switch ( current_.state_ ) 
    { 
        case State::SKETCHING: 
            break; 

        case State::RA_SKETCHING: 
            if ( status == true )
                container_.removeAbove();
            break; 

        case State::RAI_SKETCHING:
            if ( status == true )
                container_.removeAboveIntersection(); 
            break;

        case State::RB_SKETCHING:
            if ( status == true )
                container_.removeBelow(); 
            break;

        case State::RBI_SKETCHING:
            if ( status == true )
                container_.removeBelowIntersection(); 
            break;

        default:
            status = false; 
            break;
    }

    if ( status == true ) { 
        dictionary_[given_index] = index; 
        inserted_surfaces_indices_.push_back(given_index); 
        /* std::cout << "Surface " << given_index << " was commited.\n"; */

        past_states_.push_back(current_);
    }

    INFO( "Inside commitSurface(...) " ); 
    REQUIRE( status == true );
    return status;
}

bool SModellerImplementation::popLastSurface()
{
    if ( container_.empty() )
    {
        return false; 
    }

    PlanarSurface::Ptr last_sptr; 
    container_.popLastSurface(last_sptr);

    size_t last_surface_index = inserted_surfaces_indices_.back(); 
    inserted_surfaces_indices_.pop_back(); 

    StateDescriptor last = past_states_.back();
    past_states_.pop_back();

    current_.bounded_above_ = last.bounded_above_;
    current_.bounded_below_ = last.bounded_below_;
    /* current_ = last; */ 
    enforceDefineRegion();

    /* pimpl_->undoed_surfaces_stack_.push_back(last_sptr); */ 
    /* pimpl_->undoed_surfaces_indices_.push_back(last_surface_index); */ 

    /* pimpl_->undoed_states_.push_back(last); */

    auto iter = dictionary_.find(last_surface_index); 
    dictionary_.erase(iter); 

    return true;
}

bool SModellerImplementation::preserveAbove( std::vector<size_t> &bounding_surfaces_list )
{
    ContainerSurfaceIndex index;
    std::vector<ContainerSurfaceIndex> surface_ids;

    for ( auto &surface_index : bounding_surfaces_list )
    {
        if ( getSurfaceIndex(surface_index, index) == false )
        {
            return false;
        }
        surface_ids.push_back(index);
    }

    current_.bounded_below_ = container_.defineAbove(surface_ids);
    if ( current_.bounded_below_ )
    {
        current_.upper_boundary_list_ = bounding_surfaces_list;
    }

    return current_.bounded_below_;
}

bool SModellerImplementation::preserveBelow( std::vector<size_t> &bounding_surfaces_list )
{
    ContainerSurfaceIndex index;
    std::vector<ContainerSurfaceIndex> surface_ids;

    for ( auto &surface_index : bounding_surfaces_list )
    {
        if ( getSurfaceIndex(surface_index, index) == false )
        {
            return false;
        }
        surface_ids.push_back(index);
    }

    current_.bounded_above_ = container_.defineBelow(surface_ids);
    if ( current_.bounded_above_ )
    {
        current_.lower_boundary_list_ = bounding_surfaces_list;
    }

    return current_.bounded_above_;
}

void SModellerImplementation::stopPreserveAbove()
{
    container_.stopDefineAbove();
}

void SModellerImplementation::stopPreserveBelow()
{
    container_.stopDefineBelow();
}

bool SModellerImplementation::preserveAboveIsActive( std::vector<std::size_t> &bounding_surfaces_list )
{
    if ( current_.bounded_below_ == false )
    {
        return false;
    }

    bounding_surfaces_list = current_.lower_boundary_list_;

    return true;
}

bool SModellerImplementation::preserveBelowIsActive( std::vector<std::size_t> &bounding_surfaces_list )
{
    if ( current_.bounded_above_ == false )
    {
        return false;
    }

    bounding_surfaces_list = current_.upper_boundary_list_;

    return true;
}

bool SModellerImplementation::createAboveIsActive()
{
    return current_.bounded_below_;
}

bool SModellerImplementation::createAboveIsActive( size_t &boundary_index )
{
    if ( current_.bounded_below_ )
    {
        boundary_index = current_.lower_boundary_;
    }

    return current_.bounded_below_;
}

bool SModellerImplementation::createBelowIsActive()
{
    return current_.bounded_above_; 
}

bool SModellerImplementation::createBelowIsActive( size_t &boundary_index )
{
    if ( current_.bounded_above_ )
    {
        boundary_index = current_.upper_boundary_;
    }

    return current_.bounded_above_; 
}

bool SModellerImplementation::createBelow( size_t surface_index )
{
    ContainerSurfaceIndex index;
    if ( getSurfaceIndex(surface_index, index) == false )
    {
        return false; 
    }

    // Do not accept surface as boundary if it was alread
    // used as a lower boundary
    if ( createAboveIsActive() == true )
    {
        if ( current_.lower_boundary_ == surface_index )
        {
            return false;
        }
    }
    current_.bounded_above_ = container_.defineBelow(index);
    current_.upper_boundary_ = surface_index;

    /* std::cout << "Upper boundary id: " << surface_index << std::endl; */

    return current_.bounded_above_; 
}

//
// brief:
// Clear any previous `createBelow()` call.
// Safe to call anytime.
//
void SModellerImplementation::stopCreateBelow()
{
    current_.bounded_above_ = false;
    container_.stopDefineBelow(); 
}

bool SModellerImplementation::createAbove( size_t surface_index )
{
    ContainerSurfaceIndex index;
    if ( getSurfaceIndex(surface_index, index) == false )
    {
        return false; 
    }

    // Do not accept surface as boundary if it was alread
    // used as an upper boundary
    if ( createBelowIsActive() == true )
    {
        if ( current_.upper_boundary_ == surface_index )
        {
            return false;
        }
    }

    current_.bounded_below_ = container_.defineAbove(index); 
    current_.lower_boundary_ = surface_index;

    TODO_UNUSED(0); // add logger for couts
    /* std::cout << "Lower boundary id: " << surface_index << std::endl; */

    return current_.bounded_below_; 
}


void SModellerImplementation::stopCreateAbove()
{
    current_.bounded_below_ = false;
    container_.stopDefineAbove(); 
}

bool SModellerImplementation::enforceDefineRegion()
{
    bool status = true; 

    if ( current_.bounded_above_ )
    {
        status &= createBelow(current_.upper_boundary_);
    }
    else
    {
        stopCreateBelow();
    }

    if ( current_.bounded_below_)
    {
        status &= createAbove(current_.lower_boundary_);
    }
    else
    {
        stopCreateAbove();
    }

    return status; 
}

