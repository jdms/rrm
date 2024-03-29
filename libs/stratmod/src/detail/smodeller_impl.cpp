
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "smodeller_impl.hpp"

#include "testing_definitions.hpp"

#include "planin/planin.hpp"

namespace stratmod {

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

    discWidth_ = 64;
    discLenght_ = 64;

    PlanarSurface::requestChangeDiscretization(
            static_cast<PlanarSurface::Natural>( discWidth_ ), 
            static_cast<PlanarSurface::Natural>( discLenght_ ) );

    /*         PlanarSurface::Coordinate::WIDTH, */
    /*         PlanarSurface::Coordinate::DEPTH, */ 
    /*         PlanarSurface::Coordinate::HEIGHT); */ 

    mesh_ = nullptr;
    i2v_region_map_ = {};

    initialized_ = true; 

    return true;
}

void SModellerImplementation::clear()
{
    /* size_t discWidth_ = 64, discLenght_ = 64; */ 
    /* size_t numI_, numJ_; */ 
    /* size_t max_discretization_level_; */ 
    /* size_t level_I_, level_J_; */

    /* bool default_coordinate_system_ = true; */

    container_ = SRules();
    dictionary_.clear(); 
    inserted_surfaces_indices_.clear();

    current_ = StateDescriptor(); 

    undoed_surfaces_stack_.clear();
    undoed_surfaces_indices_.clear();
    undoed_states_.clear(); 
    past_states_.clear();

    got_origin_ = false; 
    got_lenght_ = false; 

    mesh_ = nullptr;
    i2v_region_map_ = {};

    interpretations_.clear();

    model_id_ = RandomId::Get();

    init();
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

    surfaces_ids = container_.getSurfacesBelowPoint(p);
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

    surfaces_ids = container_.getSurfacesAbovePoint(p);
    size_t cid;
    /* std::cout << "getSurfacesIndicesAbovePoint\n"; */
    for ( size_t i = 0; i < surfaces_ids.size(); ++i )
    {
        getControllerIndex(surfaces_ids[i], cid);
        surfaces_ids[i] = cid;
    }

    return surfaces_ids;
}

std::vector<std::size_t> SModellerImplementation::getOrderedSurfacesIndices()
{
    std::vector<size_t> surfaces_ids;

    if ( buildTetrahedralMesh() == false )
    {
    /*     return surfaces_ids; */
    }

    mesh_->getOrderedSurfaceIndicesList(surfaces_ids);
    size_t cid;

    for ( size_t i = 0; i < surfaces_ids.size(); ++i )
    {
        getControllerIndex(surfaces_ids[i], cid);
        surfaces_ids[i] = cid;
    }

    return surfaces_ids;
}

bool SModellerImplementation::getBoundingSurfacesFromRegionID( std::size_t region_id, std::vector<size_t> &lower_bound_surfaces, std::vector<size_t> &upper_bound_surfaces)
{
    /* LegacyTetrahedralMeshBuilder mb(container_); */
    if ( buildTetrahedralMesh() == false )
    {
        return 0;
    }
    
    bool success = mesh_->mapRegionToBoundingSurfaces(region_id, lower_bound_surfaces, upper_bound_surfaces);
    if ( !success )
    {
        return false;
    }

    size_t cid;
    for ( size_t i = 0; i < lower_bound_surfaces.size(); ++i )
    {
        getControllerIndex(lower_bound_surfaces[i], cid);
        lower_bound_surfaces[i] = cid;
    }

    for ( size_t i = 0; i < upper_bound_surfaces.size(); ++i )
    {
        getControllerIndex(upper_bound_surfaces[i], cid);
        upper_bound_surfaces[i] = cid;
    }

    return true;
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

bool SModellerImplementation::getControllerIndexFromPlanarSurfaceId( const PlanarSurface::SurfaceId surface_id, size_t &controller_index )
{
    size_t index;
    bool success = container_.getSurfaceIndex(surface_id, index);

    success &= getControllerIndex(index, controller_index);
    if ( !success )
    {
        controller_index = std::numeric_limits<ControllerSurfaceIndex>::max();
    }

    return success;
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
        bool extruded_surface, bool orthogonally_oriented, double fill_distance )
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

    if ( fill_distance < 0 )
    {
        sptr->setMeshFillDistance();
    }
    else
    {
        sptr->setFillDistance(fill_distance);
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
        bool orthogonally_oriented, 
        double fill_distance )
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

        sptr->addPoint( Point3{ {{cross_section_curve[i+0], cross_section_depth, cross_section_curve[i+1]}} } );
    }

    for ( size_t i = 0; i < path_curve.size(); i += dim )
    {
        sptr->addExtrusionPathPoint( path_curve[i+1], path_curve[i+0] );
    }
    sptr->setPathOrigin( cross_section_depth, 0. );

    if ( fill_distance < 0 )
    {
        sptr->setMeshFillDistance();
    }
    else
    {
        sptr->setFillDistance(fill_distance);
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
                status = container_.removeAbove();
            break; 

        case State::RAI_SKETCHING:
            if ( status == true )
                status = container_.removeAboveIntersection(); 
            break;

        case State::RB_SKETCHING:
            if ( status == true )
                status = container_.removeBelow(); 
            break;

        case State::RBI_SKETCHING:
            if ( status == true )
                status = container_.removeBelowIntersection(); 
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

        mesh_ = nullptr;
        i2v_region_map_ = {};
    }
	else
	{
		container_.popLastSurface(sptr);
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

    // current_.bounded_above_ = last.bounded_above_;
    // // current_.upper_boundary_list_ = last.upper_boundary_list_;
    // current_.bounded_below_ = last.bounded_below_;
    // // current_.lower_boundary_list_ = last.lower_boundary_list_;
    current_ = last;
    enforceDefineRegion();

    /* pimpl_->undoed_surfaces_stack_.push_back(last_sptr); */ 
    /* pimpl_->undoed_surfaces_indices_.push_back(last_surface_index); */ 

    /* pimpl_->undoed_states_.push_back(last); */

    auto iter = dictionary_.find(last_surface_index); 
    dictionary_.erase(iter); 

    return true;
}

bool SModellerImplementation::canRedo()
{
    if ( undoed_surfaces_stack_.size() > 0 )
    {
        return true;
    }

    return false;
}

bool SModellerImplementation::canUndo()
{
    if ( container_.size() > 0 )
    {
        return true;
    }

    return false;
}

bool SModellerImplementation::popUndoStack()
{
    if ( canRedo() == false )
    {
        return false;
    }

    undoed_surfaces_stack_.pop_back(); 
    undoed_surfaces_indices_.pop_back();
    undoed_states_.pop_back();

    return true;
}

bool SModellerImplementation::undo()
{
    if ( canUndo() == false )
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
    current_.upper_boundary_list_ = last.upper_boundary_list_;
    current_.bounded_below_ = last.bounded_below_;
    current_.lower_boundary_list_ = last.lower_boundary_list_;
    // current_ = last; 
    enforceDefineRegion();

    undoed_surfaces_stack_.push_back(last_sptr); 
    undoed_surfaces_indices_.push_back(last_surface_index); 

    undoed_states_.push_back(last);

    auto iter = dictionary_.find(last_surface_index); 
    dictionary_.erase(iter); 

    // Cache was updated in call to SRules::popLastSurface()
    /* container_.updateCache(); */

    mesh_ = nullptr;
    i2v_region_map_ = {};

    return true;
}

bool SModellerImplementation::redo()
{
    if ( canRedo() == false )
    {
        return false;
    }

    PlanarSurface::Ptr undoed_sptr = undoed_surfaces_stack_.back(); 
    undoed_surfaces_stack_.pop_back(); 

    size_t surface_index = undoed_surfaces_indices_.back();
    undoed_surfaces_indices_.pop_back();

    /* StateDescriptor state_before_redo_ = current_; */
    current_ = undoed_states_.back();
    undoed_states_.pop_back();
    enforceDefineRegion();

    std::vector<size_t> lbounds, ubounds;
    bool status = parseTruncateSurfaces(lbounds, ubounds); 

    /* container_.updateCache(); */

    if ( status == true )
    {
        status = commitSurface(undoed_sptr, surface_index, lbounds, ubounds); 
    }


    /* bool status = commitSurface(undoed_sptr, surface_index, std::vector<size_t>(), std::vector<size_t>()); */

    /* current_ = state_before_redo_; */
    /* enforceDefineRegion(); */

    return status;
}

void SModellerImplementation::reloadModel()
{
    int counter = 0;
    while (canUndo())
    {
        undo();
        ++counter;
    }

    for (int i = 0; i < counter; ++i)
    {
        redo();
    }
}

bool SModellerImplementation::preserveAbove( std::vector<size_t> bounding_surfaces_list )
{
    ContainerSurfaceIndex index;
    std::vector<ContainerSurfaceIndex> surface_ids;
    /* stopPreserveAbove(); */

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
        current_.lower_boundary_list_ = bounding_surfaces_list;
    }
    else
    {
        current_.lower_boundary_list_ = {};
    }

    return current_.bounded_below_;
}

bool SModellerImplementation::preserveBelow( std::vector<size_t> bounding_surfaces_list )
{
    ContainerSurfaceIndex index;
    std::vector<ContainerSurfaceIndex> surface_ids;
    /* stopPreserveBelow(); */

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
        current_.upper_boundary_list_ = bounding_surfaces_list;
    }
    else
    {
        current_.upper_boundary_list_ = {};
    }

    return current_.bounded_above_;
}

void SModellerImplementation::stopPreserveAbove()
{
    current_.bounded_below_ = false;
    current_.lower_boundary_list_ = {};
    container_.stopDefineAbove();
}

void SModellerImplementation::stopPreserveBelow()
{
    current_.bounded_above_ = false;
    current_.upper_boundary_list_ = {};
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
        status &= preserveBelow(current_.upper_boundary_list_);
    }
    else
    {
        stopPreserveBelow();
    }

    if ( current_.bounded_below_)
    {
        status &= preserveAbove(current_.lower_boundary_list_);
    }
    else
    {
        stopPreserveAbove();
    }

    return status; 
}

bool SModellerImplementation::buildTetrahedralMesh()
{
    if ( mesh_ == nullptr )
    {
        mesh_ = std::make_shared<LegacyTetrahedralMeshBuilder>(container_);
    }

    return true;
}

void SModellerImplementation::fixLegacyRegionLoadFile()
{
    if ( mesh_ == nullptr )
    {
        mesh_ = std::make_shared<LegacyTetrahedralMeshBuilder>(container_);
    }
    auto i2v_region_map_points = mesh_->computeI2VRegionMap();

    reloadModel();

    std::map<std::size_t, std::map<int, int>> i2v_region_map_counts{};
    int rid;
    for (auto& [attribute, points] : i2v_region_map_points)
    {
        i2v_region_map_counts[attribute] = {};
        for (auto& p : points)
        {
            rid = static_cast<int>(container_.getSurfacesBelowPoint({{{p[0], p[1], p[2]}}}).size()) - 1;
            if (rid < 0)
                continue;

            auto iter = i2v_region_map_counts[attribute].find(rid);
            if (iter == i2v_region_map_counts[attribute].end())
            {
                i2v_region_map_counts[attribute].insert(std::make_pair(rid, 1));
            }
            else
            {
                auto& [region_id, counter] = *iter;
                ++counter;
            }
        }
    }

    /* std::map<std::size_t, int> i2v_region_map{}; */
    /* for (auto& [att, rmap] : i2v_region_map_counts) */
    /* { */
    /*     int votes = 0; */
    /*     for(auto iter = rmap.begin(); iter != rmap.end(); ++iter) */
    /*     { */
    /*         auto& [rid, counter] = *rmap.begin(); */
    /*         if (counter > votes) */
    /*         { */
    /*             i2v_region_map[att] = rid; */
    /*             votes = counter; */
    /*         } */
    /*     } */
    /* } */

    i2v_region_map_ = std::move(i2v_region_map_counts);

    return;
}

} // namespace stratmod
