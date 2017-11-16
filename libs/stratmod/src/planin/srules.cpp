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
                to_remove_surface->removeAbove(base_surface); 
                status |= true;
            }
    }
    else if ( defineAboveIsActive() ) 
    {
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
            to_remove_surface->removeAbove(base_surface); 
            status |= true;
        }
    }
    else if ( defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
            to_remove_surface->removeAbove(base_surface); 
            status |= true; 
        }
    }
    else 
    { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
        to_remove_surface->removeAbove(base_surface); 
        status |= true; 
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
                to_remove_surface->removeBelow(base_surface); 
                status |= true;
            }
    }

    else if ( defineAboveIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesAboveOrEqualsCheck(lower_bound_) ) { 
            to_remove_surface->removeBelow(base_surface); 
            status |= true;
        }
    }

    else if ( defineBelowIsActive() ) 
    { 
        if ( to_remove_surface->weakLiesBelowOrEqualsCheck(upper_bound_) ) { 
            to_remove_surface->removeBelow(base_surface); 
            status |= true; 
        }
    }

    else 
    { // if ( !defineAboveIsActive() && !defineBelowIsActive() ) { 
        to_remove_surface->removeBelow(base_surface); 
        status |= true; 
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

    Point3 origin = container[0]->getOrigin();
    Point3 lenght = container[0]->getLenght();

    bool out_of_boundaries = false;

    out_of_boundaries = out_of_boundaries || (p.x < origin.x) || (p.x > origin.x + lenght.x);
    out_of_boundaries = out_of_boundaries || (p.y < origin.y) || (p.y > origin.y + lenght.y);
    out_of_boundaries = out_of_boundaries || (p.z < origin.z) || (p.z > origin.z + lenght.z);

    if ( out_of_boundaries )
    {
        return descriptor;
    }

    bool lies_above_all_surfaces = true;

    for ( size_t i = 0; i < size(); ++i )
    {
        if ( operator[](i)->liesAbove(p) )
        {
            descriptor.push_back(i);
        }
        else
        {
            lies_above_all_surfaces = false;
        }
    }

    if ( lies_above_all_surfaces )
    {
        descriptor.clear();
    }

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
    Point3 origin = container[0]->getOrigin();
    Point3 lenght = container[0]->getLenght();

    bool out_of_boundaries = false;

    out_of_boundaries = out_of_boundaries || (p.x < origin.x) || (p.x > origin.x + lenght.x);
    out_of_boundaries = out_of_boundaries || (p.y < origin.y) || (p.y > origin.y + lenght.y);
    out_of_boundaries = out_of_boundaries || (p.z < origin.z) || (p.z > origin.z + lenght.z);

    if ( out_of_boundaries )
    {
        return descriptor;
    }

    bool lies_below_all_surfaces = true;

    for ( size_t i = 0; i < size(); ++i )
    {
        if ( operator[](i)->liesBelow(p) )
        {
            descriptor.push_back(i);
        }
        else
        {
            lies_below_all_surfaces = false;
        }
    }

    if ( lies_below_all_surfaces )
    {
        descriptor.clear();
    }

    return descriptor;
}

std::vector<size_t> SRules::getSurfacesAbovePoint( Point3 &&p )
{
    return getSurfacesBelowPoint(p);
}
