/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 2.1 of the License, or (at your option) any later version.         */
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


#include "srules.hpp" 

/* #include <iostream> */ 
/* using namespace std; */ 


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

bool SRules::setBoundingBox( const Point3 &origin, const Point3 &lenght ) 
{
    bool status = PlanarSurface::setLenght(lenght); 
    if ( status == false ) { 
        return false; 
    }

    PlanarSurface::setOrigin(origin); 
    
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

    if ( define_above_ == true ) {
        /* cout << "Someone was defined above!\n"; */ 
        sptr->removeBelow(lower_bound_); 
    }

    if ( define_below_ == true ) { 
        /* cout << "Someone was defined below!\n"; */ 
        sptr->removeAbove(upper_bound_); 
    }

    surface_index = container.size(); 
    dictionary[ sptr->getID() ] = surface_index; 
    container.push_back(sptr); 

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

    bool intersects_other_surface = false; 
    for ( auto &s : container ) 
    { 
        if ( s->surfaceIsSet() == true ) 
            if ( s->getID() != sptr->getID() ) 
                if ( s->weakIntersectionCheck(sptr) == true ) 
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
    lower_bound_ = nullptr; 
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
    upper_bound_ = nullptr;
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

    bool status = false; 
    for ( auto s : container ) 
    {
        if ( !s->weakLiesBelowCheck(sptr) )
        if ( s->surfaceIsSet() == true  ) 
        {
            status |= boundaryAwareRemoveAbove(sptr, s); 
        }
    }

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

    bool status = false; 
    ContainerType intersection_seeds {sptr}; 
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
                        }
                    }

                    // Check if the valid part of surface s intersects surface seed
                    else if ( s->weakIntersectionCheck(seed) ) 
                    { 
                        status |= boundaryAwareRemoveAbove(sptr, s); 
                        intersection_seeds.push_back(s); 
                        intersected_surfaces.insert( s->getID() ); 
                    }
                }
            }
        }
    } while ( !intersection_seeds.empty() ); 

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

    bool status = false; 
    for ( auto s : container ) 
    {
        if ( !s->weakLiesAboveCheck(sptr) )
        if ( s->surfaceIsSet() == true ) 
        { 
            status |= boundaryAwareRemoveBelow(sptr, s); 
        }
    }

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

    bool status = false; 
    ContainerType intersection_seeds {sptr}; 
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
                        }
                    }

                    // Check if the valid part of surface s intersects surface seed
                    else if ( s->weakIntersectionCheck(seed) ) 
                    { 
                        status |= boundaryAwareRemoveBelow(sptr, s); 
                        intersection_seeds.push_back(s); 
                        intersected_surfaces.insert( s->getID() ); 
                    }
                }
            }
        }
    } while ( !intersection_seeds.empty() ); 

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

    lower_bound_ = sptr; 
    define_above_ = true; 
    return true; 
}

bool SRules::defineBelow( PlanarSurface::Ptr sptr ) 
{ 
    if ( isValidSurface(sptr) == false ) { 
        stopDefineBelow(); 
        return false; 
    }

    if ( weakEntireSurfaceCheck(sptr) == false ) { 
        stopDefineBelow(); 
        return false; 
    }

    upper_bound_ = sptr; 
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
    if ( surface_index >= size() )
    {
        return false;
    }

    return defineAbove( container[surface_index] );
}

bool SRules::defineBelow( std::size_t surface_index )
{
    if ( surface_index >= size() )
    {
        return false;
    }

    return defineBelow( container[surface_index] );
}

bool SRules::weakEntireSurfaceCheck( const PlanarSurface::Ptr &s ) 
{
    bool status = false; 
    /* cout << "Check for entire surface: "; */ 

    if ( define_above_ && define_below_ ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, upper_bound_ ); 
        /* cout << " defined above and below: " << status << endl; */ 

    }
    else if ( define_above_ ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, PlanarSurface::Ptr() ); 
        /* cout << " defined above: " << status << endl; */ 
    }
    else if ( define_below_ ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( PlanarSurface::Ptr(), upper_bound_ ); 
        /* cout << " defined below: " << status << endl; */ 
    }
    else 
    {
        status |= s->weakEntireSurfaceCheck(); 
    }

    return status; 
}

bool SRules::boundaryAwareRemoveAbove( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface )  
{
    bool status = false; 

    if ( define_above_ && define_below_ ) 
    { 
        if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) )  
            if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                to_remove_surface->removeAbove(base_surface); 
                status |= true;
            }
    }
    else if ( define_above_ ) 
    {
        if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) ) { 
            to_remove_surface->removeAbove(base_surface); 
            status |= true;
        }
    }
    else if ( define_below_ ) 
    { 
        if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
            to_remove_surface->removeAbove(base_surface); 
            status |= true; 
        }
    }
    else 
    { // if ( !define_above_ && !define_below_ ) { 
        to_remove_surface->removeAbove(base_surface); 
        status |= true; 
    }

    return status; 
}

bool SRules::boundaryAwareRemoveBelow( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface )  
{
    bool status = false; 

    if ( define_above_ && define_below_ ) 
    { 
        if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) )  
            if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
                to_remove_surface->removeBelow(base_surface); 
                status |= true;
            }
    }

    else if ( define_above_ ) 
    { 
        if ( to_remove_surface->weakLiesAboveCheck(lower_bound_) ) { 
            to_remove_surface->removeBelow(base_surface); 
            status |= true;
        }
    }

    else if ( define_below_ ) 
    { 
        if ( to_remove_surface->weakLiesBelowCheck(upper_bound_) ) { 
            to_remove_surface->removeBelow(base_surface); 
            status |= true; 
        }
    }

    else 
    { // if ( !define_above_ && !define_below_ ) { 
        to_remove_surface->removeBelow(base_surface); 
        status |= true; 
    }

    return status; 
}

