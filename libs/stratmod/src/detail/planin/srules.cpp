/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/


#include <fstream>

#include "srules.hpp" 
/* #include "util/prettyprint.hpp" */


#if defined( BUILD_WITH_SERIALIZATION )
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"

bool SRules::saveBinary( const std::string &filename )
{
    std::ofstream ofs(filename, std::ios::binary);

    if ( !ofs.good() )
    {
        return false; 
    }

    cereal::PortableBinaryOutputArchive oarchive(ofs);

    try 
    {
        oarchive( cereal::make_nvp("SRules container", *this) );
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl << std::flush;

        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught in method SRules::saveBinary(...)\n\n" << std::flush;

        return false;
    }

    return true;
}

bool SRules::loadBinary( const std::string &filename )
{
    std::ifstream ifs(filename, std::ios::binary);

    if ( !ifs.good() )
    {
        return false;
    }

    cereal::PortableBinaryInputArchive iarchive(ifs);

    try
    {
        iarchive( *this );
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl << std::flush;
        clear();

        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught in method SRules::loadBinary(...)\n\n" << std::flush;

        return false;
    }


    return true;
}

bool SRules::saveXML( const std::string &filename )
{
    std::ofstream ofs(filename);

    if ( !ofs.good() )
    {
        return false; 
    }

    cereal::XMLOutputArchive oarchive(ofs);

    try 
    {
        oarchive( cereal::make_nvp("SRules container", *this) );
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl << std::flush;

        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught in method SRules::saveXML(...)\n\n" << std::flush;

        return false;
    }

    return true;
}

bool SRules::loadXML( const std::string &filename )
{
    std::ifstream ifs(filename);

    if ( !ifs.good() )
    {
        return false;
    }

    cereal::XMLInputArchive iarchive(ifs);

    try
    {
        iarchive( *this );
    }
    catch( const std::exception &e )
    {
        std::cerr << e.what() << std::endl << std::flush;
        clear();

        return false;
    }
    catch(...)
    {
        std::cerr << "Unknown exception caught in method SRules::loadXML(...)\n\n" << std::flush;

        return false;
    }


    return true;
}

#else /* defined( BUILD_WITH_SERIALIZATION ) */
bool SRules::saveBinary( const std::string & )
{
    return false;
}


bool SRules::loadBinary( const std::string & )
{
    return false;
}

bool SRules::saveXML( const std::string & )
{
    return false;
}


bool SRules::loadXML( const std::string & )
{
    return false;
}

#endif /* defined( BUILD_WITH_SERIALIZATION ) */

/* #include <iostream> */ 
/* using namespace std; */ 


bool SRules::changeDiscretization(size_t numX, size_t numY)
{
    bool has_changed = PlanarSurface::requestChangeDiscretization(numX, numY);

    if ( has_changed )
    {
        for ( auto &s : container )
        {
            s->updateDiscretization();
        }
    }

    return has_changed;
}

bool SRules::empty() 
{ 
    return container.empty(); 
}

std::size_t SRules::size() 
{ 
    return container.size(); 
}

void SRules::clear()
{
    container.clear();
    dictionary.clear(); 
    define_above_ = false; 
    define_below_ = false; 
}

PlanarSurface::Ptr& SRules::operator[]( std::size_t surface_index ) 
{ 
    return container[surface_index]; 
}

bool SRules::defineAboveIsActive()
{
    if ( define_above_ == true )
    {
        define_above_ = false;
        for ( auto &lower_bound : lower_bound_ )
            if ( lower_bound.expired() == false )
            {
                define_above_ = true;
            }
    }

    return define_above_;
}

bool SRules::defineBelowIsActive()
{
    if ( define_below_ == true )
    {
        define_below_ = false;
        for ( auto &upper_bound : upper_bound_ )
            if ( upper_bound.expired() == false )
            {
                define_below_ = true;
            }
    }

    return define_below_;
}

bool SRules::isValidSurface( const PlanarSurface::Ptr &sptr ) { 
    return ( ( sptr != nullptr ) && ( sptr->surfaceIsSet() != false ) ); 
}

bool SRules::isValidSurfaceForInsertion( const PlanarSurface::Ptr &sptr, size_t &surface_index ) 
{

    // Ignore invalid surfaces. 
    if ( isValidSurface(sptr) == false ) { 
        return false; 
    }
    /* cout << "Got a surface!\n"; */ 

    // Cannot add the same surface twice. 
    auto iter = dictionary.find( sptr->getID() ); 
    if ( iter != dictionary.end() ) { 
        surface_index = iter->second; 
        return false; 
    }

    return true; 
}

void SRules::setOrigin( Point3 o )
{
    PlanarSurface::setOrigin(o);

    updateDiscretization();
    updateCache();
}

bool SRules::setLength( Point3 p )
{
    bool success = PlanarSurface::setLenght(p);

    if (success)
    {
        updateDiscretization();
        updateCache();
    }

    return success;
}

bool SRules::setBoundingBox( const Point3 &origin, const Point3 &lenght ) 
{
    bool status = PlanarSurface::setLenght(lenght); 
    if ( status == false ) { 
        return false; 
    }

    PlanarSurface::setOrigin(origin); 

    updateDiscretization();
    updateCache();

    return status;
}

bool SRules::addSurface( PlanarSurface::Ptr &sptr, size_t &surface_index ) 
{ 

    if ( isValidSurfaceForInsertion(sptr, surface_index) == false ) { 
        return false; 
    }

    // 
    // The surface is logically acceptable. 
    //

    if ( defineAboveIsActive() == true ) {
        /* cout << "Someone was defined above!\n"; */ 
        std::vector<PlanarSurface::SurfaceId> lb_plannar_surface_ids;
        for ( auto &s : lower_bound_ids_ )
        {
            lb_plannar_surface_ids.push_back(container[s]->getID());
        }

        for ( auto &lower_bound : lower_bound_ )
            sptr->removeBelow(lower_bound, lb_plannar_surface_ids); 
    }

    if ( defineBelowIsActive() == true ) { 
        /* cout << "Someone was defined below!\n"; */ 
        std::vector<PlanarSurface::SurfaceId> ub_planar_surface_ids;
        for ( auto &s : upper_bound_ids_ )
        {
            ub_planar_surface_ids.push_back(container[s]->getID());
        }

        for ( auto &upper_bound : upper_bound_ )
            sptr->removeAbove(upper_bound, ub_planar_surface_ids); 
    }

    surface_index = container.size(); 
    dictionary[ sptr->getID() ] = surface_index; 
    container.push_back(sptr); 

    updateDiscretization();
    updateCache();

    return true; 
}

bool SRules::addSurface( PlanarSurface::Ptr &sptr ) 
{
    size_t unused_index; 
    return addSurface(sptr, unused_index); 
}


bool SRules::addSurface( 
        PlanarSurface::Ptr &sptr, 
        std::size_t &surface_index, 
        std::vector<size_t> remove_above_surfaces, 
        std::vector<size_t> remove_below_surfaces 
        ) 
{
    if ( isValidSurfaceForInsertion(sptr, surface_index) == false ) { 
        return false; 
    }

    // 
    // The surface is logically acceptable. Now check if the list 
    // with surfaces to bound the current surface is valid. 
    //

    /* // Alternative way of checking if the surface's lists are valid: */ 
    /* auto isInvalid = [&] ( size_t i ) { return ( i >= this->size() ); }; */ 
    /* if ( std::any_of( remove_above_surfaces.begin(), remove_above_surfaces.end(), isInvalid ) ) { */ 
    /*     return false; */ 
    /* } */

    if ( !remove_above_surfaces.empty() ) {
        for ( auto &i : remove_above_surfaces ) { 
            if ( i >= size() ) { 
                return false; 
            }
        }
    }

    if ( !remove_below_surfaces.empty() ) {
        for ( auto &i : remove_below_surfaces ) { 
            if ( i >= size() ) { 
                return false; 
            }
        }
    }

    /* remove_above_surfaces = getLowerBound(remove_above_surfaces); */
    /* remove_below_surfaces = getUpperBound(remove_below_surfaces); */

    if ( !remove_above_surfaces.empty() ) {
        for ( auto &i : remove_above_surfaces ) { 
            sptr->removeAbove( container[i] ); 
        }
    }


    if ( !remove_below_surfaces.empty() ) {
        for ( auto &i : remove_below_surfaces ) { 
            sptr->removeBelow( container[i] ); 
        }
    }

    return addSurface(sptr, surface_index); 
}


bool SRules::lastInsertedSurfaceIntersects( std::vector<std::size_t> &intersection_list )
{
    auto last_inseted_surface = container.back(); 
    return getIntersectionList(last_inseted_surface, intersection_list); 
}

bool SRules::getIntersectionList( const PlanarSurface::Ptr &sptr, std::vector<std::size_t> &intersection_list )
{
    size_t intersected_surface_index; 

    // What happens if sptr intersects either the lower_bound_ or the upper_bound_? 
    // This has to be considered for truncate 
    //
    bool intersects_other_surface = false; 
    for ( auto &s : container ) 
    { 
        if ( s->surfaceIsSet() == true ) 
            if ( s->getID() != sptr->getID() ) 
                if ( s->weakIntersectionCheck(sptr) == true ) 
                    /* if ( s->weakCompleteIntersectionCheck(sptr) == true ) */ 
                { 
                    getSurfaceIndex( s->getID(), intersected_surface_index ); 
                    intersection_list.push_back( intersected_surface_index ); 
                    intersects_other_surface |= true; 
                }
    }

    return intersects_other_surface; 
}

bool SRules::getSurfaceIndex( const PlanarSurface::SurfaceId id, size_t &surface_index )
{
    auto iter = dictionary.find(id); 
    if ( iter == dictionary.end() ) { 
        return false; 
    }

    surface_index =  iter->second; 
    return true; 
}

bool SRules::popLastSurface( PlanarSurface::Ptr &surface )
{
    if ( container.empty() ) { 
        return false; 
    }

    dictionary.erase( container.back()->getID() ); 
    // Make sure that we make a *copy* (or call a move operation, if available)
    // of the last surface (and not of the last surface's pointer)
    // before popping it from the container.
    surface = std::make_shared<PlanarSurface>( std::move(*container.back()) );
    surface->clearBoundingLists(); 

    container.pop_back(); 
    for ( auto &sptr : container ) { 
        sptr->pruneBoundingLists(); 
    }

    updateCache();

    return true; 
}

bool SRules::popLastSurface() 
{
    PlanarSurface::Ptr unused_pointer; 
    return popLastSurface(unused_pointer); 
}

bool SRules::defineAbove()
{
    if ( container.empty() ) { 
        return false; 
    }

    return defineAbove( container.back() ); 
}

void SRules::stopDefineAbove() { 
    lower_bound_ = std::vector<PlanarSurface::WeakPtr>(); 
    lower_bound_ids_ = {};
    sup_lower_bound_ids_ = {};
    define_above_ = false; 
}

bool SRules::defineBelow() 
{
    if ( container.empty() ) { 
        return false; 
    }

    return defineBelow( container.back() ); 
}

void SRules::stopDefineBelow() { 
    upper_bound_ = std::vector<PlanarSurface::WeakPtr>();
    upper_bound_ids_ = {};
    inf_upper_bound_ids_ = {};
    define_below_ = false; 
}

bool SRules::removeAbove() 
{ 
    if ( container.empty() ) { 
        return false; 
    }

    return removeAbove( container.back() );
}

bool SRules::removeAboveIntersection() 
{ 
    if ( container.empty() ) { 
        return false; 
    }

    return removeAboveIntersection( container.back() );
}

bool SRules::removeBelow() 
{ 
    if ( container.empty() ) { 
        return false; 
    }

    return removeBelow( container.back() );
}

bool SRules::removeBelowIntersection() 
{ 
    if ( container.empty() ) { 
        return false; 
    }

    return removeBelowIntersection( container.back() );
}


bool SRules::removeAbove( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        return false; 
    }

    std::set<PlanarSurface::SurfaceId> modified_surfaces{};
    bool status = true; 
    for ( auto s : container ) 
    {
        if ( s->surfaceIsSet() == true  ) 
            if ( !s->weakLiesBelowCheck(sptr) )
            {
                status |= boundaryAwareRemoveAbove(sptr, s);
                modified_surfaces.insert( s->getID() );
            }
    }

    for ( auto sid : modified_surfaces )
    {
        std::size_t surface_id;
        if ( getSurfaceIndex(sid, surface_id ) )
            container[surface_id]->markCacheUnfresh();
    }
    updateCache();

    return status; 
}

/* bool removeAboveIntersection( PlanarSurface::Ptr sptr ) { */ 
/*     if ( isValidSurface(sptr) == false ) { */ 
/*         return false; */ 
/*     } */

/*     if ( weakEntireSurfaceCheck(sptr) == false ) { */ 
/*         return false; */ 
/*     } */

/*     bool status = false; */ 
/*     for ( auto s : container ) */ 
/*     { */
/*         if ( s->surfaceIsSet() == true ) */ 
/*             if ( s->weakIntersectionCheck(sptr) ) { */ 
/*                 status |= boundaryAwareRemoveAbove(sptr, s); */ 
/*             } */
/*     } */

/*     return status; */ 
/* } */

bool SRules::removeAboveIntersection( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        return false; 
    }

    bool status = true; 
    ContainerType intersection_seeds {sptr};
    std::set<PlanarSurface::SurfaceId> modified_surfaces{};
    std::set<PlanarSurface::SurfaceId> intersected_surfaces {sptr->getID()}; 
    PlanarSurface::Ptr seed; 

    do { 
        seed = intersection_seeds.back(); 
        intersection_seeds.pop_back(); 

        for ( auto s : container ) 
        {
            if ( s->surfaceIsSet() == true ) 
            { 
                // Check if surface s wasn't added to the intersection_seeds list already. 
                auto iter = intersected_surfaces.find( s->getID() ); 
                if ( iter == intersected_surfaces.end() ) 
                {
                    // Check if surface s depends on current intersection seed
                    if ( s->checkIfDependsOn( seed->getID() ) ) 
                    {
                        if ( s->weakLiesBelowCheck(sptr) == false ) 
                        {
                            status |= boundaryAwareRemoveAbove(sptr, s); 
                            intersection_seeds.push_back(s); 
                            intersected_surfaces.insert( s->getID() );
                            modified_surfaces.insert( s->getID() );
                        }
                    }

                    // Check if the valid part of surface s intersects surface seed
                    else if ( s->weakIntersectionCheck(seed) ) 
                        /* else if ( s->weakCompleteIntersectionCheck(seed) ) */ 
                    { 
                        status |= boundaryAwareRemoveAbove(sptr, s); 
                        intersection_seeds.push_back(s); 
                        intersected_surfaces.insert( s->getID() );
                        modified_surfaces.insert( s->getID() );
                    }
                }
            }
        }
    } while ( !intersection_seeds.empty() ); 

    if (!lower_bound_.empty())
    {
        ContainerType new_lbounds{};
        for ( auto s : container )
        {
            for (auto sid : sup_lower_bound_ids_)
            {
                auto bsptr = container[sid];
                if ((s->getID() == sptr->getID())
                        || s->checkIfDependsOn(sptr->getID())
                        || isInBoundList(s->getID(), lower_bound_))
                    continue;

                if (s->checkIfInLowerBound(bsptr->getID()) && !isInBoundList(s->getID(), upper_bound_))
                {
                    if( s->weakCompleteIntersectionCheck(bsptr) && !s->weakLiesAboveOrEqualsCheck(sptr) )
                    {
                        new_lbounds.push_back(s);
                    }
                }
            }
        }

        for (auto s : new_lbounds)
        {
            sptr->removeBelow(s);
        }
    }

    for ( auto sid : modified_surfaces )
    {
        std::size_t surface_id;
        if ( getSurfaceIndex(sid, surface_id ) )
            container[surface_id]->markCacheUnfresh();
    }
    updateCache();

    return status; 
}

bool SRules::removeBelow( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        return false; 
    }

    std::set<PlanarSurface::SurfaceId> modified_surfaces{};
    bool status = true; 
    for ( auto s : container ) 
    {
        if ( s->surfaceIsSet() == true ) 
            if ( !s->weakLiesAboveCheck(sptr) )
            { 
                status |= boundaryAwareRemoveBelow(sptr, s);
                modified_surfaces.insert( s->getID() );
            }
    }

    for ( auto sid : modified_surfaces )
    {
        std::size_t surface_id;
        if ( getSurfaceIndex(sid, surface_id ) )
            container[surface_id]->markCacheUnfresh();
    }
    updateCache();

    return status; 
}

/* bool removeBelowIntersection( PlanarSurface::Ptr sptr ) { */ 
/*     if ( isValidSurface(sptr) == false ) { */ 
/*         return false; */ 
/*     } */

/*     if ( weakEntireSurfaceCheck(sptr) == false ) { */ 
/*         return false; */ 
/*     } */

/*     bool status = false; */ 
/*     for ( auto s : container ) */ 
/*     { */
/*         if ( s->surfaceIsSet() ) */ 
/*             if ( s->weakIntersectionCheck(sptr) ) { */ 
/*                 status |= boundaryAwareRemoveBelow(sptr, s); */ 
/*             } */
/*     } */

/*     return status; */ 
/* } */

bool SRules::removeBelowIntersection( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        return false; 
    }

    bool status = true; 
    ContainerType intersection_seeds {sptr};
    std::set<PlanarSurface::SurfaceId> modified_surfaces{};
    std::set<PlanarSurface::SurfaceId> intersected_surfaces {sptr->getID()}; 
    PlanarSurface::Ptr seed; 

    do { 
        seed = intersection_seeds.back(); 
        intersection_seeds.pop_back(); 

        for ( auto s : container ) 
        {
            if ( s->surfaceIsSet() == true ) 
            { 
                // Check if surface s wasn't added to the intersection_seeds list already. 
                auto iter = intersected_surfaces.find( s->getID() ); 
                if ( iter == intersected_surfaces.end() ) 
                {
                    // Check if surface s depends on current intersection seed
                    if ( s->checkIfDependsOn( seed->getID() ) ) 
                    {
                        if ( s->weakLiesAboveCheck(sptr) == false ) 
                        {
                            status |= boundaryAwareRemoveBelow(sptr, s); 
                            intersection_seeds.push_back(s); 
                            intersected_surfaces.insert( s->getID() );
                            modified_surfaces.insert( s->getID() );
                        }
                    }

                    // Check if the valid part of surface s intersects surface seed
                    else if ( s->weakIntersectionCheck(seed) ) 
                        /* else if ( s->weakCompleteIntersectionCheck(seed) ) */ 
                    { 
                        status |= boundaryAwareRemoveBelow(sptr, s); 
                        intersection_seeds.push_back(s); 
                        intersected_surfaces.insert( s->getID() );
                        modified_surfaces.insert( s->getID() );
                    }
                }
            }
        }
    } while ( !intersection_seeds.empty() ); 

    if (!upper_bound_.empty())
    {
        ContainerType new_ubounds{};
        for ( auto s : container )
        {
            for (auto sid : inf_upper_bound_ids_)
            {
                auto bsptr = container[sid];
                if ((s->getID() == sptr->getID())
                        || s->checkIfDependsOn(sptr->getID())
                        || isInBoundList(s->getID(), upper_bound_))
                    continue;

                if (s->checkIfInUpperBound(bsptr->getID()) && !isInBoundList(s->getID(), lower_bound_))
                {
                    if( s->weakCompleteIntersectionCheck(bsptr) && !s->weakLiesBelowOrEqualsCheck(sptr) )
                    {
                        new_ubounds.push_back(s);
                    }
                }
            }
        }

        for (auto s : new_ubounds)
        {
            sptr->removeAbove(s);
        }
    }

    for ( auto sid : modified_surfaces )
    {
        std::size_t surface_id;
        if ( getSurfaceIndex(sid, surface_id ) )
            container[surface_id]->markCacheUnfresh();
    }
    updateCache();

    return status; 
}

bool SRules::defineAbove( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        stopDefineAbove(); 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        stopDefineAbove(); 
        return false; 
    }

    lower_bound_ = { PlanarSurface::WeakPtr(sptr) }; 
    define_above_ = true; 
    return true; 
}

bool SRules::defineAbove( std::vector<PlanarSurface::Ptr> &bounding_surfaces ) 
{ 
    std::vector<PlanarSurface::WeakPtr> wptr_surfaces;
    bool has_bounding_surfaces = false;

    for ( auto &sptr : bounding_surfaces )
        if ( isValidSurface(sptr) ) { 
            has_bounding_surfaces = true;
            wptr_surfaces.push_back( PlanarSurface::WeakPtr( sptr ) );
        }

    if ( has_bounding_surfaces == false )
    {
        stopDefineAbove();
        /* std::cout << "SRules::defineAbove(): failed to get surfaces.\n"; */
        return false;
    }

    if ( weakUpperBoundedEntireSurfaceListCheck(bounding_surfaces) == false ) { 
        stopDefineAbove(); 
        return false; 
    }

    lower_bound_ = wptr_surfaces; 
    define_above_ = true; 
    return true; 
}

bool SRules::defineBelow( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) 
    { 
        stopDefineBelow(); 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) 
    { 
        stopDefineBelow(); 
        return false; 
    }

    upper_bound_ = { PlanarSurface::WeakPtr(sptr) }; 
    define_below_ = true; 
    return true; 
}

bool SRules::defineBelow( std::vector<PlanarSurface::Ptr> &bounding_surfaces ) 
{ 
    std::vector<PlanarSurface::WeakPtr> wptr_surfaces;
    bool has_bounding_surfaces = false;

    for ( auto &sptr : bounding_surfaces )
        if ( isValidSurface(sptr) ) { 
            has_bounding_surfaces = true;
            wptr_surfaces.push_back( PlanarSurface::WeakPtr( sptr ) );
        }

    if ( has_bounding_surfaces == false )
    {
        stopDefineBelow();
        /* std::cout << "SRules::defineBelow(): failed to get surfaces.\n"; */
        return false;
    }

    if ( weakLowerBoundedEntireSurfaceListCheck(bounding_surfaces) == false ) { 
        stopDefineBelow(); 
        return false; 
    }

    upper_bound_ = wptr_surfaces; 
    define_below_ = true; 
    return true; 
}

bool SRules::removeAbove( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return removeAbove( container[surface_index] );
}

bool SRules::removeAboveIntersection( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return removeAboveIntersection( container[surface_index] );
}

bool SRules::removeBelow( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return removeBelow( container[surface_index] );
}

bool SRules::removeBelowIntersection( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return removeBelowIntersection( container[surface_index] );
}

bool SRules::defineAbove( std::size_t surface_index )
{
    /* if ( surface_index >= size() ) */
    /* { */
    /*     return false; */
    /* } */

    /* return defineAbove( container[surface_index] ); */

    std::vector<size_t> surface_indices = {surface_index};
    return defineAbove(surface_indices);
}

bool SRules::defineBelow( std::size_t surface_index )
{
    /* if ( surface_index >= size() ) */
    /* { */
    /*     return false; */
    /* } */

    /* return defineBelow( container[surface_index] ); */

    std::vector<size_t> surface_indices = {surface_index};
    return defineBelow(surface_indices);
}

bool SRules::defineAbove( std::vector<size_t> surface_indices )
{
    std::vector<PlanarSurface::Ptr> surfaces;
    /* std::cout << "SRules::defineAbove(): " << surface_indices << std::endl; */

    if ( lower_bound_ids_ != surface_indices )
    {
        lower_bound_ids_ = surface_indices;
        sup_lower_bound_ids_ = getUpperBound(surface_indices);
    }

    /* std::cout << "Lbound surfaces ids: " << sup_lower_bound_ids_ << std::endl; */

    for ( auto &sid : sup_lower_bound_ids_ )
    {
        if ( sid >= size() )
        {
            stopDefineAbove();
            return false;
        }

        surfaces.push_back(container[sid]);
    }

    return defineAbove(surfaces);
}

bool SRules::defineBelow( std::vector<size_t> surface_indices )
{
    std::vector<PlanarSurface::Ptr> surfaces;
    /* std::cout << "SRules::defineBelow(): " << surface_indices << std::endl; */

    if ( upper_bound_ids_ != surface_indices )
    {
        upper_bound_ids_ = surface_indices;
        inf_upper_bound_ids_ = getLowerBound(surface_indices);
    }
    /* std::cout << "Ubound surfaces ids: " << inf_upper_bound_ids_ << std::endl; */

    for ( auto &sid : inf_upper_bound_ids_ )
    {
        if ( sid >= size() )
        {
            stopDefineBelow();
            return false;
        }

        surfaces.push_back(container[sid]);
    }

    return defineBelow(surfaces);
}

bool SRules::liesInsideBoundingBox( const Point3 &p )
{
    Point3 origin = container[0]->getOrigin();
    Point3 lenght = container[0]->getLenght();

    bool lies_inside_boundaries = true;

    lies_inside_boundaries &= (p.x >= origin.x) && (p.x <= origin.x + lenght.x);
    lies_inside_boundaries &= (p.y >= origin.y) && (p.y <= origin.y + lenght.y);
    lies_inside_boundaries &= (p.z >= origin.z) && (p.z <= origin.z + lenght.z);

    return lies_inside_boundaries;
}

bool SRules::liesInsideBoundingBox( Point3 &&p )
{
    return liesInsideBoundingBox(p);
}

bool SRules::liesBetweenBoundarySurfaces( const Point3 &p )
{
    Point3 origin = container[0]->getOrigin();
    Point3 lenght = container[0]->getLenght();

    double lb_height, ub_height;
    Point2 p2 = {{{ p.x, p.y }}};

    ub_height = origin.z + lenght.z;
    if ( defineBelowIsActive() )
    {
        for ( auto &ub_wptr : upper_bound_ )
            if ( auto sptr = ub_wptr.lock() )
            {
                sptr->getHeight(p2, ub_height);
                /* std::cout << "Upper boundary height: " << ub_height << std::endl << std::flush; */
            }
        /* std::cout << "Upper boundary (bbox) height: " << ub_height << std::endl << std::flush; */
    }

    lb_height = origin.z;
    if ( defineAboveIsActive() )
    {
        for ( auto &lb_wptr : lower_bound_ )
            if( auto sptr = lb_wptr.lock() )
            {
                sptr->getHeight(p2, lb_height);
                /* std::cout << "Lower boundary height: " << lb_height << std::endl << std::flush; */
            }
        /* std::cout << "Lower boundary (bbox) height: " << lb_height << std::endl << std::flush; */
    }

    bool lies_inside_boundaries = true;

    lies_inside_boundaries &= (p.x >= origin.x ) && (p.x <= origin.x + lenght.x);
    lies_inside_boundaries &= (p.y >= origin.y ) && (p.y <= origin.y + lenght.y);
    lies_inside_boundaries &= (p.z >= lb_height) && (p.z <= ub_height          );

    return lies_inside_boundaries;
}

bool SRules::liesBetweenBoundarySurfaces( Point3 &&p )
{
    return liesBetweenBoundarySurfaces(p);
}

bool SRules::weakEntireSurfaceCheck( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return weakEntireSurfaceCheck( container[surface_index] ); 
}

bool SRules::weakEntireSurfaceCheck( const PlanarSurface::Ptr &s ) 
{
    //
    // Changed to always return true to allow the creation of volumes bounded
    // by non-entire surfaces.  Will leave the old implementation available in
    // case of need.
    //

    return true;

    bool status = false; 
    /* cout << "Check for entire surface: "; */ 

    if ( defineAboveIsActive() && defineBelowIsActive() ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, upper_bound_ ); 
        /* cout << " defined above and below: " << status << endl; */ 

    }
    else if ( defineAboveIsActive() ) 
    {
        auto upper_bound = std::vector<PlanarSurface::WeakPtr>();
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, upper_bound ); 
        /* cout << " defined above: " << status << endl; */ 
    }
    else if ( defineBelowIsActive() ) 
    {
        auto lower_bound = std::vector<PlanarSurface::WeakPtr>();
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound, upper_bound_ ); 
        /* cout << " defined below: " << status << endl; */ 
    }
    else 
    {
        status |= s->weakEntireSurfaceCheck(); 
    }

    return status; 
}

bool SRules::weakEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &/* surfaces */)
{ 
    //
    // Changed to always return true to allow the creation of volumes bounded
    // by non-entire surfaces.  Will leave the old implementation available in
    // case of need.
    //

    return true;

/*     if ( surfaces.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     std::vector<PlanarSurface::Ptr> lower_bound; */
/*     bool has_lower_boundary = false; */
/*     if ( define_above_ ) */
/*     { */
/*         for ( auto &wptr : lower_bound_ ) */
/*         { */
/*             if ( auto sptr = wptr.lock() ) */
/*                 if ( (sptr != nullptr) && sptr->surfaceIsSet() ) */
/*                 { */
/*                     lower_bound.push_back(sptr); */
/*                     has_lower_boundary = true; */
/*                 } */
/*         } */
/*     } */

/*     std::vector<PlanarSurface::Ptr> upper_bound; */
/*     bool has_upper_boundary = false; */ 
/*     if ( define_below_ ) */
/*     { */
/*         for ( auto &wptr : upper_bound_ ) */
/*         { */
/*             if ( auto sptr = wptr.lock() ) */
/*                 if ( (sptr != nullptr) && sptr->surfaceIsSet() ) */
/*                 { */
/*                     upper_bound.push_back(sptr); */
/*                     has_upper_boundary = true; */
/*                 } */
/*         } */
/*     } */

/*     bool isEntireSurface = true; */ 
/*     bool point_status, status, lstatus, ustatus; */ 

/*     // bug */
/*     auto origin = surfaces.front()->PlanarSurface::getOrigin(); */
/*     auto lenght = surfaces.front()->PlanarSurface::getLenght(); */

/*     const double lb = origin.z; */  
/*     const double ub = origin.z + lenght.z; */  

/*     double height, lheight, uheight; ; */ 
/*     double min_height = origin.z + lenght.z; */
/*     double max_height = origin.z; */

/*     auto num_vertices_omp = surfaces.front()->getNumVertices(); */ 
/*     auto tolerance = surfaces.front()->getTolerance(); */

/*     // VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type */ 
/*     // #pragma omp parallel for shared(lower_surfaces, upper_surfaces) firstprivate(ub, lb, num_vertices_omp, has_lower_boundary, has_upper_boundary) private(status, lstatus, ustatus, height, lheight, uheight) default(none) reduction(&&: isEntireSurface) */ 
/*     for ( long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i ) */ 
/*     { */
/*         status = false; */
/*         point_status = false; */

/*         for ( auto &sptr : surfaces ) */
/*         { */
/*             point_status = sptr->getHeight(i, height); */ 
/*             if ( point_status == false ) */
/*             { */ 
/*                 if ( std::fabs(lb - height) <= tolerance ) { */
/*                     point_status = true; */ 
/*                 } */

/*                 else if ( std::fabs(height - ub) <= tolerance ) { */ 
/*                     point_status = true; */ 
/*                 } */
/*             } */

/*             if ( max_height < height ) */
/*                 max_height = height; */

/*             if ( min_height > height ) */
/*                 min_height = height; */

/*             status |= point_status; */
/*         } */

/*         if ( status == false ) */ 
/*         { */ 
/*             if ( has_lower_boundary ) */ 
/*             { */ 
/*                 for ( auto &lower_surface : lower_bound ) */
/*                 { */
/*                     lstatus = lower_surface->getHeight(i, lheight); */ 
/*                     /1* std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "lstatus value: " << lstatus; *1/ */ 
/*                     // which height to compare? */
/*                     if ( lstatus && ( std::fabs(lheight - min_height) <= tolerance ) ) { */
/*                         status |= true; */ 
/*                     } */
/*                     else { */ 
/*                         /1* std::cout << " missed index: " << i << ", distance: " << height - lheight << ", tolerance: " << tolerance << std::endl; *1/ */ 
/*                     } */
/*                 } */
/*             } */
/*             if ( has_upper_boundary ) */ 
/*             { */
/*                 for ( auto &upper_surface : upper_bound ) */
/*                 { */
/*                     ustatus = upper_surface->getHeight(i, uheight); */ 
/*                     // which height to compare? */
/*                     if ( ustatus && ( std::fabs(max_height - uheight) <= tolerance ) ) { */ 
/*                         status |= true; */ 
/*                     } */
/*                 } */
/*             } */
/*         } */

/*         isEntireSurface = isEntireSurface && status; */ 
/*     } */

/*     return isEntireSurface; */ 
} 

bool SRules::weakLowerBoundedEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &/* surfaces */)
{ 
    //
    // Changed to always return true to allow the creation of volumes bounded
    // by non-entire surfaces.  Will leave the old implementation available in
    // case of need.
    //

    return true;

/*     if ( surfaces.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     std::vector<PlanarSurface::Ptr> lower_bound; */
/*     bool has_lower_boundary = false; */
/*     if ( define_above_ ) */
/*     { */
/*         for ( auto &wptr : lower_bound_ ) */
/*         { */
/*             if ( auto sptr = wptr.lock() ) */
/*                 if ( (sptr != nullptr) && sptr->surfaceIsSet() ) */
/*                 { */
/*                     lower_bound.push_back(sptr); */
/*                     has_lower_boundary = true; */
/*                 } */
/*         } */
/*     } */

/*     bool isEntireSurface = true; */ 
/*     bool point_status, status, lstatus; */ 

/*     // bug */
/*     auto origin = surfaces.front()->PlanarSurface::getOrigin(); */
/*     auto lenght = surfaces.front()->PlanarSurface::getLenght(); */

/*     const double lb = origin.z; */  
/*     const double ub = origin.z + lenght.z; */  

/*     double height, lheight, min_height, max_height; */

/*     auto num_vertices_omp = surfaces.front()->getNumVertices(); */ 
/*     auto tolerance = surfaces.front()->getTolerance(); */

/*     // VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type */ 
/*     #pragma omp parallel for shared(lower_bound, surfaces) firstprivate(ub, lb, num_vertices_omp, has_lower_boundary, tolerance, origin, lenght) private(status, lstatus, point_status, height, lheight, min_height, max_height) default(none) reduction(&&: isEntireSurface) */ 
/*     for (long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i) */
/*     { */
/*         status = false; */
/*         point_status = false; */
/*         min_height = origin.z + lenght.z; */
/*         max_height = origin.z; */

/*         for (auto &sptr : surfaces) */
/*         { */
/*             point_status = sptr->getHeight(i, height); */
/*             if (point_status == false) */
/*             { */
/*                 if (std::fabs(lb - height) <= tolerance) { */
/*                     point_status = true; */
/*                 } */

/*                 else if (std::fabs(height - ub) <= tolerance) { */
/*                     point_status = true; */
/*                 } */
/*             } */

/*             if (max_height < height) */
/*                 max_height = height; */

/*             if (min_height > height) */
/*                 min_height = height; */

/*             status |= point_status; */
/*         } */

/*         if (status == false) */
/*         { */
/*             if (has_lower_boundary) */
/*             { */
/*                 for (auto &lower_surface : lower_bound) */
/*                 { */
/*                     lstatus = lower_surface->getHeight(i, lheight); */
/*                     // std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(8) << "lstatus value: " << lstatus; */
/*                     // which height to compare? */
/*                     if (lstatus && (std::fabs(lheight - min_height) <= tolerance)) { */
/*                         status |= true; */
/*                     } */
/*                     else { */
/*                         // std::cout << " missed index: " << i << ", distance: " << height - lheight << ", tolerance: " << tolerance << std::endl; */
/*                     } */
/*                 } */
/*             } */
/*         } */

/*         isEntireSurface = isEntireSurface && status; */
/*     } */

/*     return isEntireSurface; */ 
} 

bool SRules::weakUpperBoundedEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &/* surfaces */)
{ 
    //
    // Changed to always return true to allow the creation of volumes bounded
    // by non-entire surfaces.  Will leave the old implementation available in
    // case of need.
    //

    return true;

/*     if ( surfaces.empty() ) */
/*     { */
/*         return false; */
/*     } */

/*     std::vector<PlanarSurface::Ptr> upper_bound; */
/*     bool has_upper_boundary = false; */ 
/*     if ( define_below_ ) */
/*     { */
/*         for ( auto &wptr : upper_bound_ ) */
/*         { */
/*             if ( auto sptr = wptr.lock() ) */
/*                 if ( (sptr != nullptr) && sptr->surfaceIsSet() ) */
/*                 { */
/*                     upper_bound.push_back(sptr); */
/*                     has_upper_boundary = true; */
/*                 } */
/*         } */
/*     } */

/*     bool isEntireSurface = true; */ 
/*     bool point_status, status, ustatus; */ 

/*     // bug */
/*     auto origin = surfaces.front()->PlanarSurface::getOrigin(); */
/*     auto lenght = surfaces.front()->PlanarSurface::getLenght(); */

/*     const double lb = origin.z; */  
/*     const double ub = origin.z + lenght.z; */  

/*     double height, uheight, min_height, max_height; */

/*     auto num_vertices_omp = surfaces.front()->getNumVertices(); */ 
/*     auto tolerance = surfaces.front()->getTolerance(); */

/*     // VS2013 error C3016: index variable in OpenMP 'for' statement must have signed integral type */ 
/*     #pragma omp parallel for shared(surfaces, upper_bound) firstprivate(ub, lb, num_vertices_omp, has_upper_boundary, tolerance, origin, lenght) private(status, point_status, ustatus, height, uheight, min_height, max_height) default(none) reduction(&&: isEntireSurface) */ 
/*     for (long int i = 0; i < static_cast<long int>(num_vertices_omp); ++i) */
/*     { */
/*         status = false; */
/*         point_status = false; */
/*         min_height = origin.z + lenght.z; */
/*         max_height = origin.z; */

/*         for (auto &sptr : surfaces) */
/*         { */
/*             point_status = sptr->getHeight(i, height); */
/*             if (point_status == false) */
/*             { */
/*                 if (std::fabs(lb - height) <= tolerance) { */
/*                     point_status = true; */
/*                 } */

/*                 else if (std::fabs(height - ub) <= tolerance) { */
/*                     point_status = true; */
/*                 } */
/*             } */

/*             if (max_height < height) */
/*                 max_height = height; */

/*             if (min_height > height) */
/*                 min_height = height; */

/*             status |= point_status; */
/*         } */


/*         if (status == false) */
/*         { */
/*             if (has_upper_boundary) */
/*             { */
/*                 for (auto &upper_surface : upper_bound) */
/*                 { */
/*                     ustatus = upper_surface->getHeight(i, uheight); */
/*                     // which height to compare? */
/*                     if (ustatus && (std::fabs(max_height - uheight) <= tolerance)) { */
/*                         status |= true; */
/*                     } */
/*                 } */
/*             } */
/*         } */

/*         isEntireSurface = isEntireSurface && status; */
/*     } */

/*     return isEntireSurface; */ 
} 

bool SRules::boundaryAwareRemoveAbove( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface )  
{
    bool status = false; 

    if ( defineAboveIsActive() && defineBelowIsActive() ) 
    { 
        if ( !isInBoundList(to_remove_surface->getID(), lower_bound_) )
        if ( !isInBoundList(to_remove_surface->getID(), upper_bound_) )
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) )  
            if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
                std::vector<PlanarSurface::SurfaceId> ub_planar_surface_ids;
                for ( auto &s : upper_bound_ids_ )
                {
                    ub_planar_surface_ids.push_back(container[s]->getID());
                }


                to_remove_surface->updateCache();
                to_remove_surface->removeAbove(base_surface, ub_planar_surface_ids); 
                status |= true;
            }
    }
    else if ( defineAboveIsActive() ) 
    {
        if ( !isInBoundList(to_remove_surface->getID(), lower_bound_) )
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
            to_remove_surface->updateCache();
            to_remove_surface->removeAbove(base_surface); 
            status |= true;
        }
    }
    else if ( defineBelowIsActive() ) 
    { 
        if ( !isInBoundList(to_remove_surface->getID(), upper_bound_) )
        if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
            std::vector<PlanarSurface::SurfaceId> ub_planar_surface_ids;
            for ( auto &s : upper_bound_ids_ )
            {
                ub_planar_surface_ids.push_back(container[s]->getID());
            }

            to_remove_surface->updateCache();
            to_remove_surface->removeAbove(base_surface, ub_planar_surface_ids); 
            status |= true; 
        }
    }
    else 
    { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
        to_remove_surface->updateCache();
        to_remove_surface->removeAbove(base_surface); 
        status |= true; 
    }

    to_remove_surface->updateCache();
    return status; 
}

    bool SRules::boundaryAwareRemoveBelow( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface )  
    {
        bool status = false; 
        /* std::cout << "Inside SRules::boundaryAwareRemoveBelow(), status is " << status << std::endl; */
        /* std::cout << "BaseSurface = " << base_surface->getID() << ", ToRemove = " << to_remove_surface->getID() << std::endl; */
        if ( defineAboveIsActive() && defineBelowIsActive() ) 
        { 
            /* std::cout << "defineAboveIsActive() && defineBelowIsActive(); "; */
            if ( !isInBoundList(to_remove_surface->getID(), lower_bound_) )
            if ( !isInBoundList(to_remove_surface->getID(), upper_bound_) )
            if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) )  
                if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
                    std::vector<PlanarSurface::SurfaceId> lb_plannar_surface_ids;
                    for ( auto &s : lower_bound_ids_ )
                    {
                        lb_plannar_surface_ids.push_back(container[s]->getID());
                    }

                    to_remove_surface->updateCache();
                    to_remove_surface->removeBelow(base_surface, lb_plannar_surface_ids); 
                    status |= true;
                    /* std::cout << "weakLies[Above,Below](); "; */
                }
            /* std::cout << "status is " << status << std::endl; */
        }

        else if ( defineAboveIsActive() ) 
        { 
            /* std::cout << "defineAboveIsActive(); "; */
            if ( !isInBoundList(to_remove_surface->getID(), lower_bound_) )
            if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
                std::vector<PlanarSurface::SurfaceId> lb_plannar_surface_ids;
                for ( auto &s : lower_bound_ids_ )
                {
                    lb_plannar_surface_ids.push_back(container[s]->getID());
                }

                to_remove_surface->updateCache();
                to_remove_surface->removeBelow(base_surface, lb_plannar_surface_ids); 
                status |= true;
                /* std::cout << "weakLiesAbove(); "; */
            }
            /* std::cout << "status is " << status << std::endl; */
        }

        else if ( defineBelowIsActive() ) 
        { 
            /* std::cout << "defineBelowIsActive(); "; */
            if ( !isInBoundList(to_remove_surface->getID(), upper_bound_) )
            if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
                to_remove_surface->updateCache();
                to_remove_surface->removeBelow(base_surface); 
                status |= true; 
                /* std::cout << "weakLiesBelow(); "; */
            }
            /* std::cout << "status is " << status << std::endl; */
        }

        else 
        { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
            to_remove_surface->updateCache();
            to_remove_surface->removeBelow(base_surface); 
            status |= true; 
            /* std::cout << "defineAboveIsActive() && defineBelowIsActive(); status is true"; */
        }

        to_remove_surface->updateCache();
        return status; 
        }

        std::vector<size_t> SRules::getSurfacesBelowPoint( const Point3 &p )
        {
            std::vector<size_t> descriptor = {};

            if ( empty() )
            {
                return descriptor;
            }

            descriptor.reserve( size() );

            /* // TODO (BUG): this method is not supposed to check whether point lies inside domain or not */
            /* bool out_of_boundaries = !liesInsideBoundingBox(p); */

            /* if ( out_of_boundaries ) */
            /* { */
            /*     return descriptor; */
            /* } */

            /* bool lies_above_all_surfaces = true; */
            PlanarSurface::PointCache cache;
            cache.reserve( size() );
            auto [tindex, bcoords] = container.front()->linear_graph_.baricentricCoordinates({p.x, p.y});

            for ( size_t i = 0; i < size(); ++i )
            {
                if ( operator[](i)->liesAbove(tindex, bcoords, p.z, cache) )
                {
                    descriptor.push_back(i);
                }
                /* else */
                /* { */
                /*     lies_above_all_surfaces = false; */
                /* } */
            }

            /* if ( lies_above_all_surfaces ) */
            /* { */
            /*     descriptor.clear(); */
            /* } */

            return descriptor;

            /* PlanarSurface::Natural min_surface_id = container[0]->getID(); */
            /* int region_id = 0; */
            /* bool above_all_surfaces = true; */

            /* for ( auto &sptr : container ) */
            /* { */
            /*     if ( min_surface_id > sptr->getID() ) */
            /*     { */
            /*         min_surface_id = sptr->getID(); */
            /*     } */

            /*     if ( sptr->liesAbove(p) ) */
            /*     { */
            /*         region_id += static_cast<int>(std::pow( 2.0, static_cast<double>(sptr->getID()) )); */
            /*     } */
            /*     else */
            /*     { */
            /*         above_all_surfaces &= false; */
            /*     } */
            /* } */

            /* if ( above_all_surfaces ) */
            /* { */
            /*     return 0; */
            /* } */

            /* region_id /= static_cast<int>(std::pow( 2.0, static_cast<double>(min_surface_id) )); */

            /* return region_id; */
        }

        std::vector<size_t> SRules::getSurfacesBelowPoint( Point3 &&p )
        {
            return getSurfacesBelowPoint(p);
        }

        std::vector<size_t> SRules::getSurfacesAbovePoint( const Point3 &p )
        {
            std::vector<size_t> descriptor = {};

            if ( empty() )
            {
                return descriptor;
            }

            descriptor.reserve( size() );

            /* // TODO (BUG): this method is not supposed to check whether point lies inside domain or not */
            /* bool out_of_boundaries = !liesInsideBoundingBox(p); */

            /* if ( out_of_boundaries ) */
            /* { */
            /*     /1* std::cout << "Point is out of boundaries.\n"; *1/ */
            /*     return descriptor; */
            /* } */

            /* bool lies_below_all_surfaces = true; */

            PlanarSurface::PointCache cache;
            cache.reserve( size() );
            auto [tindex, bcoords] = container.front()->linear_graph_.baricentricCoordinates({p.x, p.y});

            for ( size_t i = 0; i < size(); ++i )
            {
                if ( operator[](i)->liesBelow(tindex, bcoords, p.z, cache) )
                {
                    descriptor.push_back(i);
                }
                /* else */
                /* { */
                /*     lies_below_all_surfaces = false; */
                /* } */
            }

            /* if ( lies_below_all_surfaces ) */
            /* { */
            /*     descriptor.clear(); */
            /* } */

            return descriptor;
        }

        std::vector<size_t> SRules::getSurfacesAbovePoint( Point3 &&p )
        {
            return getSurfacesAbovePoint(p);
        }

        std::vector<size_t> SRules::getActiveSurfacesBelowPoint( const Point3 &p )
        {
            std::vector<size_t> descriptor = {};

            if ( empty() )
            {
                return descriptor;
            }

            Point3 origin = container[0]->getOrigin();
            Point3 lenght = container[0]->getLenght();

            /* std::cout << "Is point out of model?"; */
            bool out_of_boundaries = !liesBetweenBoundarySurfaces(p);

            if ( out_of_boundaries )
            {
                /* std::cout << " Yes.\n" << std::flush; */
                return descriptor;
            }
            /* std::cout << " No.\n" << std::flush; */

            descriptor.reserve( size() );

            double height, lb_height, ub_height;
            Point2 p2 = {{{ p.x, p.y }}};

            ub_height = origin.z + lenght.z;
            if ( defineBelowIsActive() )
            {
                for ( auto &upper_bound : upper_bound_ )
                    if ( auto sptr = upper_bound.lock() )
                    {
                        double height;
                        sptr->getHeight(p2, height);
                        if ( height < ub_height )
                            ub_height = height;
                    }
            }

            lb_height = origin.z;
            if ( defineAboveIsActive() )
            {
                for ( auto &lower_bound : lower_bound_ )
                    if ( auto sptr = lower_bound.lock() )
                    {
                        double height;
                        sptr->getHeight(p2, height);
                        if ( height > lb_height )
                            lb_height = height;
                    }
            }

            PlanarSurface::PointCache cache;
            cache.reserve( size() );

            for ( size_t i = 0; i < size(); ++i )
            {
                /* std::cout << "Processing surface: " << i << std::flush; */
                operator[](i)->getHeight(p2, height);

                if ( ( height >= lb_height ) && ( height <= ub_height ) )
                {
                    if ( operator[](i)->liesAbove(p, cache) )
                    {
                        /* std::cout << "-> point lies above surface" << std::endl << std::flush; */
                        descriptor.push_back(i);
                    }
                    /* else */ 
                    /* { */
                    /* std::cout << "-> point DOESN'T lie above surface" << std::endl << std::flush; */
                    /* } */
                }
                /* else */
                /* { */
                /* std::cout << "-> surface lies outside of active area" << std::endl << std::flush; */
                /* } */

            }

            /* if ( lies_above_all_surfaces ) */
            /* { */
            /*     descriptor.clear(); */
            /* } */

            return descriptor;

            /* PlanarSurface::Natural min_surface_id = container[0]->getID(); */
            /* int region_id = 0; */
            /* bool above_all_surfaces = true; */

            /* for ( auto &sptr : container ) */
            /* { */
            /*     if ( min_surface_id > sptr->getID() ) */
            /*     { */
            /*         min_surface_id = sptr->getID(); */
            /*     } */

            /*     if ( sptr->liesAbove(p) ) */
            /*     { */
            /*         region_id += static_cast<int>(std::pow( 2.0, static_cast<double>(sptr->getID()) )); */
            /*     } */
            /*     else */
            /*     { */
            /*         above_all_surfaces &= false; */
            /*     } */
            /* } */

            /* if ( above_all_surfaces ) */
            /* { */
            /*     return 0; */
            /* } */

            /* region_id /= static_cast<int>(std::pow( 2.0, static_cast<double>(min_surface_id) )); */

            /* return region_id; */
        }

        std::vector<size_t> SRules::getActiveSurfacesBelowPoint( Point3 &&p )
        {
            return getActiveSurfacesBelowPoint(p);
        }

        std::vector<size_t> SRules::getActiveSurfacesAbovePoint( const Point3 &p )
        {
            std::vector<size_t> descriptor = {};

            if ( empty() )
            {
                return descriptor;
            }

            descriptor.reserve( size() );
            Point3 origin = container[0]->getOrigin();
            Point3 lenght = container[0]->getLenght();

            bool out_of_boundaries = !liesBetweenBoundarySurfaces(p);

            if ( out_of_boundaries )
            {
                /* std::cout << "Point is out of boundaries.\n"; */
                return descriptor;
            }

            double height, lb_height, ub_height;
            Point2 p2 = {{{ p.x, p.y }}};

            ub_height = origin.z + lenght.z;
            if ( defineBelowIsActive() )
            {
                for ( auto &upper_bound : upper_bound_ )
                    if ( auto sptr = upper_bound.lock() )
                    {
                        double height;
                        sptr->getHeight(p2, height);
                        if ( height < ub_height )
                            ub_height = height;
                    }
            }

            lb_height = origin.z;
            if ( defineAboveIsActive() )
            {
                for ( auto &lower_bound : lower_bound_ )
                    if ( auto sptr = lower_bound.lock() )
                    {
                        double height;
                        sptr->getHeight(p2, height);
                        if ( height > lb_height )
                            lb_height = height;
                    }
            }

            PlanarSurface::PointCache cache;
            cache.reserve( size() );

            for ( size_t i = 0; i < size(); ++i )
            {
                operator[](i)->getHeight(p2, height);

                if ( ( height >= lb_height ) && ( height <= ub_height ) )
                {
                    if ( operator[](i)->liesBelow(p, cache) )
                    {
                        descriptor.push_back(i);
                    }
                }
            }

            /* if ( lies_below_all_surfaces ) */
            /* { */
            /*     descriptor.clear(); */
            /* } */

            return descriptor;
        }

        std::vector<size_t> SRules::getActiveSurfacesAbovePoint( Point3 &&p )
        {
            return getActiveSurfacesAbovePoint(p);
        }

        std::vector<size_t> SRules::getLowerBound( std::vector<size_t> surface_ids )
        {
            /* std::cout << "Inside SRules::getLowerBound()\n"; */
            std::vector<size_t> lbound;

            for ( auto &s : surface_ids )
            {
                if ( s >= size() )
                {
                    return lbound;
                }
            }

            if ( surface_ids.size() <= 1 )
            {
                lbound = surface_ids;
                return lbound;
            }

            // TODO: change for parallelization
            std::set<size_t> lbound_set;

            std::vector<size_t> ordered_ids;
            std::map<TriangleHeights, size_t> dictionary;
            TriangleHeights th;
            size_t numBlocks = PlanarSurface::getDiscretizationX() * PlanarSurface::getDiscretizationY();
            size_t numTriangPerBlock = 8;

            for ( size_t bindex = 0; bindex < numBlocks; ++bindex )
            {
                for ( size_t tpos = 0; tpos < numTriangPerBlock; ++tpos )
                {
                    ordered_ids.clear();
                    dictionary.clear();

                    for ( auto s : surface_ids )
                    {
                        th = -container[s]->getTriangleHeightsFromPositionInBlock(tpos, bindex);
                        try
                        {
                            dictionary.insert(std::make_pair(th, s));
                        }
                        catch(...)
                        {
                            std::cerr << "DERROR: MSVC tags TH ordering as invalid\n";
                        }
                    }
                    /* std::cout << "For pair (b, t) " << "(" << bindex << ", " << tpos << ")" << " ordered indices are:"; */

                    for ( auto iter = dictionary.begin(); iter != dictionary.end(); ++iter )
                    {
                        /* std::cout << " " << iter->second; */
                        ordered_ids.push_back( iter->second );
                    }

                    if ( !ordered_ids.empty() )
                    {
                        /* std::cout << "; picked: " << ordered_ids.back() << std::endl; */
                        lbound_set.insert( ordered_ids.back() );
                    }
                }
            }

            for ( size_t e : lbound_set )
            {
                lbound.push_back(e);
            }

            return lbound;
        }

        std::vector<size_t> SRules::getUpperBound( std::vector<size_t> surface_ids )
        {
            /* std::cout << "Inside SRules::getUpperBound()\n"; */
            std::vector<size_t> ubound;

            for ( auto &s : surface_ids )
            {
                if ( s >= size() )
                {
                    return ubound;
                }
            }

            if ( surface_ids.size() <= 1 )
            {
                ubound = surface_ids;
                return ubound;
            }


            // TODO: change for parallelization
            std::set<size_t> ubound_set;

            std::vector<size_t> ordered_ids;
            std::map<TriangleHeights, size_t> dictionary;
            TriangleHeights th;
            size_t numBlocks = PlanarSurface::getDiscretizationX() * PlanarSurface::getDiscretizationY();
            size_t numTriangPerBlock = 8;

            for ( size_t bindex = 0; bindex < numBlocks; ++bindex )
            {
                for ( size_t tpos = 0; tpos < numTriangPerBlock; ++tpos )
                {
                    ordered_ids.clear();
                    dictionary.clear();

                    for ( auto s : surface_ids )
                    {
                        th = container[s]->getTriangleHeightsFromPositionInBlock(tpos, bindex);
                        try
                        {
                            dictionary.insert(std::make_pair(th, s));
                        }
                        catch(...)
                        {
                            std::cerr << "DERROR: MSVC tags TH ordering as invalid\n";
                        }
                    }
                    /* std::cout << "For pair (b, t) " << "(" << bindex << ", " << tpos << ")" << " ordered indices are:"; */

                    for ( auto iter = dictionary.begin(); iter != dictionary.end(); ++iter )
                    {
                        /* std::cout << " " << iter->second; */
                        ordered_ids.push_back( iter->second );
                    }

                    if ( !ordered_ids.empty() )
                    {
                        /* std::cout << "; picked: " << ordered_ids.back() << std::endl; */
                        ubound_set.insert( ordered_ids.back() );
                    }
                }
            }

            for ( size_t e : ubound_set )
            {
                ubound.push_back(e);
            }

            return ubound;
        }

        void SRules::updateDiscretization()
        {
            for ( auto &s : container )
            {
                s->updateDiscretization();
            }
        }

        void SRules::updateCache()
        {
            for ( auto &s : container )
            {
                s->updateCache();
            }
        }

