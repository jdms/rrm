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
        if ( lower_bound_.expired() == true )
        {
            lower_bound_ = PlanarSurface::WeakPtr();
            define_above_ = false;
        }
    }

    return define_above_;
}

bool SRules::defineBelowIsActive()
{
    if ( define_below_ == true )
    {
        if ( upper_bound_.expired() == true )
        {
            upper_bound_ = PlanarSurface::WeakPtr();
            define_below_ = false;
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

    if ( defineAboveIsActive() == true ) {
        /* cout << "Someone was defined above!\n"; */ 
        sptr->removeBelow(lower_bound_); 
    }

    if ( defineBelowIsActive() == true ) { 
        /* cout << "Someone was defined below!\n"; */ 
        sptr->removeAbove(upper_bound_); 
    }

    surface_index = container.size(); 
    dictionary[ sptr->getID() ] = surface_index; 
    container.push_back(sptr); 

    sptr->updateCache();

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

    remove_above_surfaces = getLowerBound(remove_above_surfaces);
    remove_below_surfaces = getUpperBound(remove_below_surfaces);

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
    lower_bound_ = PlanarSurface::WeakPtr(); 
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
    upper_bound_ = PlanarSurface::WeakPtr();
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
        if ( s->surfaceIsSet() == true  ) 
        if ( !s->weakLiesBelowCheck(sptr) )
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
                    /* else if ( s->weakCompleteIntersectionCheck(seed) ) */ 
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
        if ( s->surfaceIsSet() == true ) 
        if ( !s->weakLiesAboveCheck(sptr) )
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
                    /* else if ( s->weakCompleteIntersectionCheck(seed) ) */ 
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

    lower_bound_ = PlanarSurface::WeakPtr(sptr); 
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

    upper_bound_ = PlanarSurface::WeakPtr(sptr); 
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
        if ( auto sptr = upper_bound_.lock() )
        {
            sptr->getHeight(p2, ub_height);
            /* std::cout << "Upper boundary height: " << ub_height << std::endl << std::flush; */
        }
        /* std::cout << "Upper boundary (bbox) height: " << ub_height << std::endl << std::flush; */
    }

    lb_height = origin.z;
    if ( defineAboveIsActive() )
    {
        if( auto sptr = lower_bound_.lock() )
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
    bool status = false; 
    /* cout << "Check for entire surface: "; */ 

    if ( defineAboveIsActive() && defineBelowIsActive() ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, upper_bound_ ); 
        /* cout << " defined above and below: " << status << endl; */ 

    }
    else if ( defineAboveIsActive() ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( lower_bound_, PlanarSurface::WeakPtr() ); 
        /* cout << " defined above: " << status << endl; */ 
    }
    else if ( defineBelowIsActive() ) 
    {
        status |= s->weakBoundedEntireSurfaceCheck( PlanarSurface::WeakPtr(), upper_bound_ ); 
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

    if ( defineAboveIsActive() && defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) )  
            if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
                /* to_remove_surface->updateCache(); */
                to_remove_surface->removeAbove(base_surface); 
                status |= true;
            }
    }
    else if ( defineAboveIsActive() ) 
    {
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
            /* to_remove_surface->updateCache(); */
            to_remove_surface->removeAbove(base_surface); 
            status |= true;
        }
    }
    else if ( defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
            /* to_remove_surface->updateCache(); */
            to_remove_surface->removeAbove(base_surface); 
            status |= true; 
        }
    }
    else 
    { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
        /* to_remove_surface->updateCache(); */
        to_remove_surface->removeAbove(base_surface); 
        status |= true; 
    }

    if ( status == true )
    {
        to_remove_surface->updateCache();
    }

    return status; 
}

bool SRules::boundaryAwareRemoveBelow( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface )  
{
    bool status = false; 

    if ( defineAboveIsActive() && defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) )  
            if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
                /* to_remove_surface->updateCache(); */
                to_remove_surface->removeBelow(base_surface); 
                status |= true;
            }
    }

    else if ( defineAboveIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
            /* to_remove_surface->updateCache(); */
            to_remove_surface->removeBelow(base_surface); 
            status |= true;
        }
    }

    else if ( defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
            /* to_remove_surface->updateCache(); */
            to_remove_surface->removeBelow(base_surface); 
            status |= true; 
        }
    }

    else 
    { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
        /* to_remove_surface->updateCache(); */
        to_remove_surface->removeBelow(base_surface); 
        status |= true; 
    }

    if ( status == true )
    {
        to_remove_surface->updateCache();
    }

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

    bool out_of_boundaries = !liesInsideBoundingBox(p);

    if ( out_of_boundaries )
    {
        return descriptor;
    }

    /* bool lies_above_all_surfaces = true; */

    for ( size_t i = 0; i < size(); ++i )
    {
        if ( operator[](i)->liesAbove(p) )
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

    bool out_of_boundaries = !liesInsideBoundingBox(p);

    if ( out_of_boundaries )
    {
        /* std::cout << "Point is out of boundaries.\n"; */
        return descriptor;
    }

    /* bool lies_below_all_surfaces = true; */

    for ( size_t i = 0; i < size(); ++i )
    {
        if ( operator[](i)->liesBelow(p) )
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
        if ( auto sptr = upper_bound_.lock() )
        {
            sptr->getHeight(p2, ub_height);
        }
    }

    lb_height = origin.z;
    if ( defineAboveIsActive() )
    {
        if ( auto sptr = lower_bound_.lock() )
        {
            sptr->getHeight(p2, lb_height);
        }
    }

    for ( size_t i = 0; i < size(); ++i )
    {
        /* std::cout << "Processing surface: " << i << std::flush; */
        operator[](i)->getHeight(p2, height);

        if ( ( height >= lb_height ) && ( height <= ub_height ) )
        {
            if ( operator[](i)->liesAbove(p) )
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
        if ( auto sptr = upper_bound_.lock() )
        {
            sptr->getHeight(p2, ub_height);
        }
    }

    lb_height = origin.z;
    if ( defineAboveIsActive() )
    {
        if ( auto sptr = lower_bound_.lock() )
        {
            sptr->getHeight(p2, lb_height);
        }
    }


    for ( size_t i = 0; i < size(); ++i )
    {
        operator[](i)->getHeight(p2, height);

        if ( ( height >= lb_height ) && ( height <= ub_height ) )
        {
            if ( operator[](i)->liesBelow(p) )
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
    std::multimap<TriangleHeights, size_t> dictionary;
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
                dictionary.insert(std::make_pair(th, s));
            }

            for ( auto iter = dictionary.begin(); iter != dictionary.end(); ++iter )
            {
                ordered_ids.push_back( iter->second );
            }

            if ( !ordered_ids.empty() )
            {
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
    std::multimap<TriangleHeights, size_t> dictionary;
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
                dictionary.insert(std::make_pair(th, s));
            }

            for ( auto iter = dictionary.begin(); iter != dictionary.end(); ++iter )
            {
                ordered_ids.push_back( iter->second );
            }

            if ( !ordered_ids.empty() )
            {
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

void SRules::updateCache()
{
        for ( auto &s : container )
        {
            s->updateCache();
        }
}

