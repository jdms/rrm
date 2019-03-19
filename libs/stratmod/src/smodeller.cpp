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



#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <fstream>

#include "detail/planin/planin.hpp"

#include "stratmod/smodeller.hpp"
#include "detail/smodeller_impl.hpp"
#include "detail/serialization_definitions.hpp"

#include "detail/testing_definitions.hpp"


/*****************************/
/* Actual Implementation */ 
/*****************************/


// Default methods 

SModeller::SModeller() : 
    pimpl_( new SModellerImplementation() ) 
{
    pimpl_->init();
} 

SModeller::~SModeller() = default; 

SModellerImplementation& SModeller::Impl()
{
    return *pimpl_;
}

const SModellerImplementation& SModeller::Impl() const
{
    return *pimpl_;
}

std::vector<size_t> SModeller::getSurfacesIndices()
{
    return pimpl_->inserted_surfaces_indices_; 
}

std::vector<std::size_t> SModeller::getOrderedSurfacesIndices()
{
    return pimpl_->getOrderedSurfacesIndices();
}

/* // create[Above,Below] is DEPRECATED */
/* // */
/* // brief: */
/* // Verifies whether ir is possible to define a new 'drawing' region, */
/* // either above or below. */
/* // Argument: `eligible_surfaces` stores the indices of surfaces that can */
/* // be used as input either for an `defineAbove()` or a `defineBelow()`. */
/* // Return: true if at least one elegible surface was found. */
/* // */
/* [[deprecated]] */
/* bool requestCreateAbove( std::vector<std::size_t> &eligible_surfaces ); */

/* bool SModeller::requestCreateAbove( std::vector<size_t> &eligible_surfaces ) */
/* { */
    /* if ( pimpl_->container_.empty() ) */
    /* { */
    /*     return false; */ 
    /* } */

    /* eligible_surfaces.clear(); */ 
    /* ControllerSurfaceIndex output_index; */ 
    /* ContainerSurfaceIndex index; */ 

    /* for ( size_t i = 0; i < pimpl_->inserted_surfaces_indices_.size(); ++i ) */
    /* { */
    /*     output_index = pimpl_->inserted_surfaces_indices_[i]; */ 
    /*     if ( pimpl_->getSurfaceIndex(output_index, index) == false ) */
    /*         continue; */ 

    /*     if ( pimpl_->container_.weakEntireSurfaceCheck(index) ) */
    /*     { */
    /*         if ( pimpl_->createBelowIsActive() ) */
    /*         { */
    /*             /1* std::cout << "Define Below is active.\n"; *1/ */ 
    /*             ContainerSurfaceIndex boundary_index; */ 

    /*             if ( pimpl_->getSurfaceIndex(pimpl_->current_.upper_boundary_, boundary_index) == false ) */
    /*             { */
    /*                 /1* std::cout << "Surface id: " << index << ", boundary id: " << boundary_index << std::endl; *1/ */ 
    /*                 /1* std::cout << "Got wrong id. \n"; *1/ */ 
    /*                 continue; */
    /*             } */

    /*             /1* std::cout << "Surface id: " << index << ", boundary id: " << boundary_index << std::endl; *1/ */ 

    /*             if ( index == boundary_index ) */
    /*             { */
    /*                 continue; */
    /*             } */

    /*             if ( pimpl_->container_[index]->weakLiesBelowOrEqualsCheck(pimpl_->container_[boundary_index]) ) */
    /*             { */
    /*                 eligible_surfaces.push_back(output_index); */ 
    /*             } */
    /*             else */
    /*             { */
    /*                 /1* std::cout << " --> Not eligible\n"; *1/ */ 
    /*             } */
    /*         } */
    /*         else */
    /*         { */
    /*             eligible_surfaces.push_back(output_index); */ 
    /*         } */

    /*     } */
    /* } */

    /* return !eligible_surfaces.empty(); */ 
/* } */

/* [[deprecated]] */
/* bool requestCreateBelow( std::vector<std::size_t> &eligible_surfaces ); */

/* bool SModeller::requestCreateBelow( std::vector<size_t> &eligible_surfaces ) */
/* { */
    /* if ( pimpl_->container_.empty() ) */
    /* { */
    /*     return false; */ 
    /* } */

    /* eligible_surfaces.clear(); */ 
    /* ControllerSurfaceIndex output_index; */ 
    /* ContainerSurfaceIndex index; */ 

    /* for ( size_t i = 0; i < pimpl_->inserted_surfaces_indices_.size(); ++i ) */
    /* { */
    /*     output_index = pimpl_->inserted_surfaces_indices_[i]; */ 
    /*     if ( pimpl_->getSurfaceIndex(output_index, index) == false ) */
    /*         continue; */ 

    /*     if ( pimpl_->container_.weakEntireSurfaceCheck(index) ) */
    /*     { */
    /*         if ( pimpl_->createAboveIsActive() ) */
    /*         { */
    /*             /1* std::cout << "Define Below is active.\n"; *1/ */ 
    /*             ContainerSurfaceIndex boundary_index; */ 

    /*             if ( pimpl_->getSurfaceIndex(pimpl_->current_.lower_boundary_, boundary_index) == false ) */
    /*             { */
    /*                 /1* std::cout << "Surface id: " << index << ", boundary id: " << boundary_index << std::endl; *1/ */ 
    /*                 /1* std::cout << "Got wrong id. \n"; *1/ */ 
    /*                 continue; */
    /*             } */

    /*             /1* std::cout << "Surface id: " << index << ", boundary id: " << boundary_index << std::endl; *1/ */ 
    /*             if ( index == boundary_index ) */
    /*             { */
    /*                 continue; */
    /*             } */

    /*             if ( pimpl_->container_[index]->weakLiesAboveOrEqualsCheck(pimpl_->container_[boundary_index]) ) */
    /*             { */
    /*                 eligible_surfaces.push_back(output_index); */ 
    /*             } */
    /*             else */
    /*             { */
    /*                 /1* std::cout << " --> Not eligible\n"; *1/ */ 
    /*             } */
    /*         } */
    /*         else */
    /*         { */
    /*             eligible_surfaces.push_back(output_index); */ 
    /*         } */

    /*     } */
    /* } */

    /* return !eligible_surfaces.empty(); */ 
/* } */


        /* Change the model's properties */
bool SModeller::useDefaultCoordinateSystem()
{
    /* if ( pimpl_->container_.size() > 0 ) */
    /* { */
    /*     return false; */
    /* } */

    PlanarSurface::setOutputCoordinatesOrdering( 
            PlanarSurface::Coordinate::WIDTH,
            PlanarSurface::Coordinate::HEIGHT, 
            PlanarSurface::Coordinate::DEPTH
            );

    pimpl_->default_coordinate_system_ = true;

    return true;
}

bool SModeller::useOpenGLCoordinateSystem()
{
    /* if ( pimpl_->container_.size() > 0 ) */
    /* { */
    /*     return false; */
    /* } */

    PlanarSurface::setOutputCoordinatesOrdering( 
            PlanarSurface::Coordinate::WIDTH,
            PlanarSurface::Coordinate::DEPTH, 
            PlanarSurface::Coordinate::HEIGHT
            ); 

    pimpl_->default_coordinate_system_ = false;

    return true;
}


/**
 * \brief Change models discretization if model is empty
 *
 * \param width_discretization - discretization in the width direction
 * \param length_discretization - discretization in the length direction
 *
 * \return true if discretization was changed
 */
/* [[deprecated]] */
/* bool tryChangeDiscretization( std::size_t width_discretization = 64, std::size_t depth_discretization = 64 ); */

/* bool SModeller::tryChangeDiscretization( size_t width, size_t length ) */
/* { */
/*     if ( pimpl_->container_.size() > 0 ) */
/*     { */
/*         return false; */
/*     } */

/*     pimpl_->discWidth_ = width; */ 
/*     pimpl_->discLenght_ = length; */ 

/*     PlanarSurface::requestChangeDiscretization(width, length); */

/*     return true; */
/* } */

bool SModeller::changeDiscretization( size_t width, size_t length )
{
    if ( (width == 0) || (length == 0) )
    {
        return false;
    }

    // WEIRD: without the following (superfluous) test, changeDiscretization()
    // was crashing if width and length were equal to the model's discretization
    if ( (width == pimpl_->discWidth_) && (length == pimpl_->discLenght_) )
    {
        return true;
    }

    int counter = 0;
    while ( canUndo() )
    {
        undo();
        ++counter;
    }

    PlanarSurface::requestChangeDiscretization(width, length);

    pimpl_->discWidth_ = width; 
    pimpl_->discLenght_ = length; 
    /* pimpl_->container_.updateCache(); */

    while ( counter > 0 )
    {
        redo();
        --counter;
    }

    /* TODO: review use and conversion of types */
    /* /1* PlanarSurface::requestChangeDiscretization((PlanarSurface::Natural)(pimpl_->discWidth_), PlanarSurface::Natural(pimpl_->discLenght_)); *1/ */
    /* return pimpl_->container_.changeDiscretization(width, length); */

    return true;
}


size_t SModeller::getWidthDiscretization() const
{
    return pimpl_->discWidth_;
}

size_t SModeller::getLengthDiscretization() const
{
    return pimpl_->discLenght_;
}

void SModeller::setOrigin( double x, double y, double z )
{
    /* if ( pimpl_->default_coordinate_system_ ) */
    /* { */
    /*     pimpl_->origin_.x = x; */ 
    /*     pimpl_->origin_.y = y; */ 
    /*     pimpl_->origin_.z = z; */ 
    /* } */
    /* else */
    /* { */
    /*     pimpl_->origin_.x = x; */ 
    /*     pimpl_->origin_.y = z; */ 
    /*     pimpl_->origin_.z = y; */ 
    /* } */

    pimpl_->origin_ = pimpl_->point3(x, y, z);

    pimpl_->got_origin_ = true; 
}


bool SModeller::setSize( double x, double y, double z )
{
    if ( ( x > 0 ) && ( y > 0 ) && ( z > 0 ) ) 
    { 
        /* if ( pimpl_->default_coordinate_system_ ) */
        /* { */
        /*     pimpl_->lenght_.x = x; */ 
        /*     pimpl_->lenght_.y = y; */ 
        /*     pimpl_->lenght_.z = z; */ 
        /* } */
        /* else */
        /* { */
        /*     pimpl_->lenght_.x = x; */ 
        /*     pimpl_->lenght_.y = z; */ 
        /*     pimpl_->lenght_.z = y; */ 
        /* } */

        pimpl_->lenght_ = pimpl_->point3(x, y, z);

        pimpl_->got_lenght_ = true; 

        return true;
    }

    return false; 
}

void SModeller::getOrigin( double &x, double &y, double &z )
{
    pimpl_->getOrigin(x, y, z);
}


void SModeller::getSize( double &x, double &y, double &z )
{
    pimpl_->getLenght(x, y, z);
}

        /* Begin methods to interface with GUI */

        /* Initialization and clean up */

void SModeller::clear()
{
    pimpl_->clear();

    /* pimpl_->container_.clear(); */ 
    /* pimpl_->dictionary_.clear(); */ 
    /* pimpl_->inserted_surfaces_indices_.clear(); */

    /* pimpl_->current_ = StateDescriptor(); */ 

    /* pimpl_->undoed_surfaces_stack_.clear(); */
    /* pimpl_->undoed_states_.clear(); */ 
    /* pimpl_->past_states_.clear(); */

    /* pimpl_->got_origin_ = false; */ 
    /* pimpl_->got_lenght_ = false; */ 

    /* pimpl_->mesh_ = nullptr; */

    /* pimpl_->init(); */
}



        /* Query or modify the automatum state */


/* // */
/* // brief: */
/* // Define new input region above surface which index is `surface_index`. */
/* // */
/* // */
/* // brief: */
/* // Define new input region above surface which index is `surface_index`. */
/* // */
/* [[deprecated]] */
/* bool createAbove( std::size_t surface_index ); */

/* bool SModeller::createAbove( size_t surface_index ) */
/* { */
/*     return pimpl_->createAbove(surface_index); */ 
/* } */


/* // */
/* // brief: */
/* // Clear any previous `defineAbove()` call. */
/* // Safe to call anytime. */
/* // */
/* [[deprecated]] */
/* void stopCreateAbove(); */

        /* // */
        /* // brief: */
        /* // Clear any previous `createAbove()` call. */
        /* // Safe to call anytime. */
        /* // */
/* void SModeller::stopCreateAbove() */
/* { */
    /* pimpl_->stopCreateAbove(); */
/* } */


/* // */
/* // brief: */
/* // Define new input region below surface which index is `surface_index`. */
/* // */
/* [[deprecated]] */
/* bool createBelow( std::size_t surface_index ); */

/* // */
/* // brief: */
/* // Define new input region below surface which index is `surface_index`. */
/* // */
/* bool SModeller::createBelow( size_t surface_index ) */
/* { */
/*     return pimpl_->createBelow(surface_index); */ 
/* } */

/* // */
/* // brief: */
/* // Clear any previous `defineBelow()` call. */
/* // Safe to call anytime. */
/* // */
/* [[deprecated]] */
/* void stopCreateBelow(); */

/* // */
/* // brief: */
/* // Clear any previous `defineBelow()` call. */
/* // Safe to call anytime. */
/* // */
/* void SModeller::stopCreateBelow() */
/* { */
/*     pimpl_->stopCreateBelow(); */
/* } */


/* [[deprecated]] */
/* bool createAboveIsActive( std::size_t &boundary_index ); */

/* bool SModeller::createAboveIsActive( size_t &boundary_index ) */
/* { */
/*     return pimpl_->createAboveIsActive(boundary_index); */
/* } */

/* [[deprecated]] */
/* bool createBelowIsActive( std::size_t &boundary_index ); */

/* bool SModeller::createBelowIsActive( size_t &boundary_index ) */  
/* { */
/*     return pimpl_->createBelowIsActive(boundary_index); */
/* } */

bool SModeller::preserveAbove( std::vector<size_t> &bounding_surfaces_list )
{
    return pimpl_->preserveAbove(bounding_surfaces_list);
}

bool SModeller::preserveBelow( std::vector<size_t> &bounding_surfaces_list )
{
    return pimpl_->preserveBelow(bounding_surfaces_list);
}

void SModeller::stopPreserveAbove()
{
    return pimpl_->stopPreserveAbove();
}

void SModeller::stopPreserveBelow()
{
    return pimpl_->stopPreserveBelow();
}

bool SModeller::preserveAboveIsActive( std::vector<std::size_t> &bounding_surfaces_list )
{
    return pimpl_->preserveAboveIsActive(bounding_surfaces_list);
}

bool SModeller::preserveBelowIsActive( std::vector<std::size_t> &bounding_surfaces_list )
{
    return pimpl_->preserveBelowIsActive(bounding_surfaces_list);
}

void SModeller::disableGeologicRules()
{
    pimpl_->current_.state_ = State::SKETCHING;
}

void SModeller::removeAbove()
{
    pimpl_->current_.state_ = State::RA_SKETCHING;
}

void SModeller::removeAboveIntersection()
{
    pimpl_->current_.state_ = State::RAI_SKETCHING; 
}

        
void SModeller::removeBelow()
{
    pimpl_->current_.state_ = State::RB_SKETCHING;
}

void SModeller::removeBelowIntersection()
{
    pimpl_->current_.state_ = State::RBI_SKETCHING; 
} 


/* bool SModeller::createSurface( size_t surface_id, const std::vector<double> &point_data ) */
/* { */
/*     INFO( "Got into SModeller::createSurface(...)" ); */

/*     bool extruded_surface = false; */ 
/*     return pimpl_->insertSurface(point_data, surface_id, std::vector<size_t>(), std::vector<size_t>(), extruded_surface); */
/* } */

bool SModeller::createSurface( size_t surface_id, const std::vector<double> &point_data, 
                const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    bool extruded_surface = false; 
    return pimpl_->insertSurface(point_data, surface_id, lower_bound_ids, upper_bound_ids, extruded_surface); 
}

/* bool SModeller::createExtrudedSurface( size_t surface_id, const std::vector<double> &point_data ) */
/* { */
/*     bool extruded_surface = true; */ 
/*     return pimpl_->insertSurface(point_data, surface_id, std::vector<size_t>(), std::vector<size_t>(), extruded_surface); */
/* } */

bool SModeller::createLengthwiseExtrudedSurface( size_t surface_id, const std::vector<double> &point_data, 
                const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    bool orthogonally_oriented = false; 
    size_t cross_section_depth = 0;
    std::vector<double> empty_path = std::vector<double>();

    return pimpl_->insertExtrusionAlongPath(surface_id, 
            point_data, cross_section_depth, 
            empty_path, 
            lower_bound_ids, upper_bound_ids, orthogonally_oriented); 
}


bool SModeller::createLengthwiseExtrudedSurface( size_t surface_id, 
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data, 

        const std::vector<size_t> lower_bound_ids,
        const std::vector<size_t> upper_bound_ids
        )
{
    bool orthogonally_oriented = false; 
    return pimpl_->insertExtrusionAlongPath(surface_id, cross_section_curve_point_data, cross_section_depth, path_curve_point_data, lower_bound_ids, upper_bound_ids, orthogonally_oriented);
}


bool SModeller::createWidthwiseExtrudedSurface( size_t surface_id, const std::vector<double> &point_data, 
                const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    bool orthogonally_oriented = true; 
    size_t cross_section_depth = 0;
    std::vector<double> empty_path = std::vector<double>();

    return pimpl_->insertExtrusionAlongPath(surface_id, 
            point_data, cross_section_depth, 
            empty_path, 
            lower_bound_ids, upper_bound_ids, orthogonally_oriented); 
}


bool SModeller::createWidthwiseExtrudedSurface( size_t surface_id, 
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data, 

        const std::vector<size_t> lower_bound_ids,
        const std::vector<size_t> upper_bound_ids
        )
{
    bool orthogonally_oriented = true; 
    return pimpl_->insertExtrusionAlongPath(surface_id, cross_section_curve_point_data, cross_section_depth, path_curve_point_data, lower_bound_ids, upper_bound_ids, orthogonally_oriented);
}


bool SModeller::tryCreateLengthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data, 

        const std::vector<size_t> lower_bound_ids,
        const std::vector<size_t> upper_bound_ids
        )
{
    bool status = createLengthwiseExtrudedSurface(surface_id, cross_section_curve_point_data, cross_section_depth, path_curve_point_data, lower_bound_ids, upper_bound_ids);

    if ( status == false )
    {
        return false;
    }

    pimpl_->lastInsertedSurfaceIntersects(intersected_surfaces);

    if ( ! intersected_surfaces.empty() )
    {
        pimpl_->popLastSurface();

        return false;
    }

    return true;
}

bool SModeller::tryCreateWidthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
        const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
        const std::vector<double> &path_curve_point_data, 

        const std::vector<size_t> lower_bound_ids,
        const std::vector<size_t> upper_bound_ids
        )
{
    bool status = createWidthwiseExtrudedSurface(surface_id, cross_section_curve_point_data, cross_section_depth, path_curve_point_data, lower_bound_ids, upper_bound_ids);

    if ( status == false )
    {
        return false;
    }

    pimpl_->lastInsertedSurfaceIntersects(intersected_surfaces);

    if ( ! intersected_surfaces.empty() )
    {
        pimpl_->popLastSurface();

        return false;
    }

    return true;
}


/* bool SModeller::tryCreateSurface( size_t surface_id, const std::vector<double> &point_data, std::vector<size_t> &intersected_surfaces ) */
/* { */
/*     return tryCreateSurface(surface_id, point_data, std::vector<size_t>(), std::vector<size_t>(), intersected_surfaces); */
/* } */

bool SModeller::tryCreateSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces, 
        const std::vector<double> &point_data, 
        const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    /* State current = pimpl_->current_.state_; */
    /* pimpl_->current_.state_ = State::SKETCHING; */

    /* std::cout << "Trying to create a surface...\n"; */ 
    bool status = createSurface(surface_id, point_data, lower_bound_ids, upper_bound_ids); 

    /* pimpl_->current_.state_ = current; */ 

    if ( status == false )
    {
        return false;
    }

    pimpl_->lastInsertedSurfaceIntersects(intersected_surfaces);

    if ( ! intersected_surfaces.empty() )
    {
        /* std::cout << "But it intersected another surface...\n"; */
        pimpl_->popLastSurface();

        return false;
    }

    /* std::cout << "Success!\n"; */

    return true;
}

/* bool SModeller::tryCreateExtrudedSurface( size_t surface_id, const std::vector<double> &point_data, std::vector<size_t> &intersected_surfaces ) */
/* { */
/*     return tryCreateExtrudedSurface(surface_id, point_data, std::vector<size_t>(), std::vector<size_t>(), intersected_surfaces); */
/* } */

bool SModeller::tryCreateLengthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
        const std::vector<double> &point_data,
        const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    /* State current = pimpl_->current_.state_; */
    /* pimpl_->current_.state_ = State::SKETCHING; */

    bool status = createLengthwiseExtrudedSurface(surface_id, point_data, lower_bound_ids, upper_bound_ids); 

    /* pimpl_->current_.state_ = current; */ 

    if ( status == false )
    {
        return false;
    }

    pimpl_->lastInsertedSurfaceIntersects(intersected_surfaces);

    if ( ! intersected_surfaces.empty() )
    {
        pimpl_->popLastSurface();

        return false;
    }

    return true;
}

bool SModeller::tryCreateWidthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
        const std::vector<double> &point_data,
        const std::vector<size_t> lower_bound_ids, const std::vector<size_t> upper_bound_ids )
{
    /* State current = pimpl_->current_.state_; */
    /* pimpl_->current_.state_ = State::SKETCHING; */

    bool status = createWidthwiseExtrudedSurface(surface_id, point_data, lower_bound_ids, upper_bound_ids); 

    /* pimpl_->current_.state_ = current; */ 

    if ( status == false )
    {
        return false;
    }

    pimpl_->lastInsertedSurfaceIntersects(intersected_surfaces);

    if ( ! intersected_surfaces.empty() )
    {
        pimpl_->popLastSurface();

        return false;
    }

    return true;
}

bool SModeller::canUndo()
{
    if ( pimpl_->container_.size() > 0 )
    {
        return true;
    }

    return false;
}

bool SModeller::undo()
{
    if ( canUndo() == false )
    {
        return false; 
    }

    PlanarSurface::Ptr last_sptr; 
    pimpl_->container_.popLastSurface(last_sptr);

    size_t last_surface_index = pimpl_->inserted_surfaces_indices_.back(); 
    pimpl_->inserted_surfaces_indices_.pop_back(); 

    StateDescriptor last = pimpl_->past_states_.back();
    pimpl_->past_states_.pop_back();

    pimpl_->current_.bounded_above_ = last.bounded_above_;
    pimpl_->current_.upper_boundary_list_ = last.upper_boundary_list_;
    pimpl_->current_.bounded_below_ = last.bounded_below_;
    pimpl_->current_.lower_boundary_list_ = last.lower_boundary_list_;
    // pimpl_->current_ = last; 
    pimpl_->enforceDefineRegion();

    pimpl_->undoed_surfaces_stack_.push_back(last_sptr); 
    pimpl_->undoed_surfaces_indices_.push_back(last_surface_index); 

    pimpl_->undoed_states_.push_back(last);

    auto iter = pimpl_->dictionary_.find(last_surface_index); 
    pimpl_->dictionary_.erase(iter); 

    // Cache was updated in call to SRules::popLastSurface()
    /* pimpl_->container_.updateCache(); */

    pimpl_->mesh_ = nullptr;

    return true;
}

/* [[deprecated]] */
/* bool popUndoStack(); */

/* bool SModeller::popUndoStack() */
/* { */
/*     return pimpl_->popUndoStack(); */
/* } */

bool SModeller::canRedo()
{
    if ( pimpl_->undoed_surfaces_stack_.size() > 0 )
    {
        return true;
    }

    return false;
}

bool SModeller::redo()
{
    if ( canRedo() == false )
    {
        return false;
    }

    PlanarSurface::Ptr undoed_sptr = pimpl_->undoed_surfaces_stack_.back(); 
    pimpl_->undoed_surfaces_stack_.pop_back(); 

    size_t surface_index = pimpl_->undoed_surfaces_indices_.back();
    pimpl_->undoed_surfaces_indices_.pop_back();

    /* StateDescriptor state_before_redo_ = pimpl_->current_; */
    pimpl_->current_ = pimpl_->undoed_states_.back();
    pimpl_->undoed_states_.pop_back();
    pimpl_->enforceDefineRegion();

    std::vector<size_t> lbounds, ubounds;
    bool status = pimpl_->parseTruncateSurfaces(lbounds, ubounds); 

    /* pimpl_->container_.updateCache(); */

    if ( status == true )
    {
        status = pimpl_->commitSurface(undoed_sptr, surface_index, lbounds, ubounds); 
    }


    /* bool status = commitSurface(undoed_sptr, surface_index, std::vector<size_t>(), std::vector<size_t>()); */

    /* pimpl_->current_ = state_before_redo_; */
    /* pimpl_->enforceDefineRegion(); */

    return status;
}

bool SModeller::destroyLastSurface()
{
    if ( !canUndo() )
    {
        return false;
    }

    undo();
    return pimpl_->popUndoStack(); // must return true
}

/* [[deprecated]] */
/* bool getVertexList( std::size_t surface_id, std::vector<float> &vlist ); */

/* bool SModeller::getVertexList( size_t surface_id, std::vector<float> &vlist ) */
/* { */
/*     return pimpl_->getVertexList(surface_id, vlist); */
/* } */

/* [[deprecated]] */
/* bool getVertexList( std::size_t surface_id, std::vector<double> &vlist ); */

/* bool SModeller::getVertexList( size_t surface_id, std::vector<double> &vlist ) */
/* { */
/*     return pimpl_->getVertexList(surface_id, vlist); */
/* } */

/* [[deprecated]] */
/* bool getMesh( std::size_t surface_id, std::vector<float> &vlist, std::vector<unsigned int> &flist ); */

/* bool SModeller::getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<unsigned int> &flist ) */
/* { */
/*     return pimpl_->getMesh(surface_id, vlist, flist); */
/* } */

bool SModeller::getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist )
{
    return pimpl_->getMesh(surface_id, vlist, flist);
}

/* [[deprecated]] */
/* bool getWidthCrossSectionCurve( std::size_t surface_id, std::size_t width, std::vector<float> &vlist, std::vector<unsigned int> &elist ); */

/* bool SModeller::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<unsigned int> &elist ) */
/* { */
/*     return pimpl_->getCrossSectionWidth(surface_id, vlist, elist, width); */
/*     /1* return pimpl_->getAdaptedCrossSectionAtConstantWidth(surface_id, vlist, elist, width); *1/ */
/* } */

bool SModeller::getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    return pimpl_->getCrossSectionWidth(surface_id, vlist, elist, width);
    /* return pimpl_->getAdaptedCrossSectionAtConstantWidth(surface_id, vlist, elist, width); */
}

/* [[deprecated]] */
/* bool getLengthCrossSectionCurve( std::size_t surface_id, std::size_t lenght, std::vector<float> &vlist, std::vector<unsigned int> &elist ); */

/* bool SModeller::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<float> &vlist, std::vector<unsigned int> &elist ) */
/* { */
/*     return pimpl_->getCrossSectionDepth(surface_id, vlist, elist, length); */
/*     /1* return pimpl_->getAdaptedCrossSectionAtConstantLength(surface_id, vlist, elist, length); *1/ */
/* } */

bool SModeller::getLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist )
{
    return pimpl_->getCrossSectionDepth(surface_id, vlist, elist, length);
    /* return pimpl_->getAdaptedCrossSectionAtConstantLength(surface_id, vlist, elist, length); */
}


/* [[deprecated]] */
/* std::size_t getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::vector<std::size_t>> &element_list ); */

/* std::size_t SModeller::getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector< std::vector<std::size_t> > &element_list ) */
/* { */
/*     /1* TetrahedralMeshBuilder mb(pimpl_->container_); *1/ */
/*     if ( pimpl_->buildTetrahedralMesh() == false ) */
/*     { */
/*         return 0; */
/*     } */

/*     bool status = true; */ 
/*     size_t num_elements; */

/*     status &= (pimpl_->mesh_->getVertexCoordinates(vertex_coordinates) > 0); */

/*     if ( status == false ) */
/*     { */
/*         return 0; */
/*     } */


/*     num_elements = pimpl_->mesh_->getTetrahedronList(element_list); */

/*     return num_elements; */
/* } */

std::size_t SModeller::getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::size_t> &element_list, std::vector<long int> &attribute_list )
{
    /* TetrahedralMeshBuilder mb(pimpl_->container_); */
    if ( pimpl_->buildTetrahedralMesh() == false )
    {
        return 0;
    }

    bool status = true;
    size_t num_elements;

    status &= (pimpl_->mesh_->getVertexCoordinates(vertex_coordinates) > 0);

    if ( status == false )
    {
        return 0;
    }

    num_elements = pimpl_->mesh_->getTetrahedronList(element_list, attribute_list);

    return num_elements;
}

bool SModeller::getNormalList( std::size_t surface_id, std::vector<double> &normal_list )
{
    size_t index; 
    if ( pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    bool status = pimpl_->container_[index]->getNormalList(normal_list);

    return status;

}

bool SModeller::getExtrusionPath( std::size_t surface_id, std::vector<double> &path_vertex_list )
{
    size_t index; 
    if ( pimpl_->getSurfaceIndex(surface_id, index) == false )
    {
        return false; 
    }

    return pimpl_->container_[index]->getPathVertexList(path_vertex_list);
}

bool SModeller::computeTetrahedralMeshVolumes( std::vector<double> &vlist )
{
    if ( pimpl_->buildTetrahedralMesh() == false )
    {
        return false;
    }

    return pimpl_->mesh_->getRegionVolumeList(vlist);
}

bool SModeller::getVolumeAttributesFromPointList( const std::vector<double> &vcoords, std::vector<int> &attribute_list)
{
    if ( vcoords.size() % 3 != 0 )
    {
        return false;
    }

    size_t num_points = vcoords.size()/3;

    Point3 p;
    std::vector<Point3> point_list(num_points);

    for ( size_t i = 0; i < num_points; ++i )
    {
        p = pimpl_->point3(vcoords[3*i + 0], vcoords[3*i + 1], vcoords[3*i + 2]);
        point_list[i] = p;
    }

    if ( pimpl_->buildTetrahedralMesh() == false )
    {
        return false;
    }

    bool status = pimpl_->mesh_->mapPointsToAttributes(point_list, attribute_list);

    return status;
}

bool SModeller::getBoundingSurfacesFromVolumeAttribute( std::size_t attribute_id, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound)
{
    return pimpl_->getBoundingSurfacesFromRegionID(attribute_id, lower_bound, upper_bound);
}

std::vector<size_t> SModeller::getSurfacesIndicesBelowPoint( double x, double y, double z )
{
    return pimpl_->getSurfacesIndicesBelowPoint(x, y, z);
}

std::vector<size_t> SModeller::getSurfacesIndicesAbovePoint( double x, double y, double z )
{
    return pimpl_->getSurfacesIndicesAbovePoint(x, y, z);
}


#if defined(BUILD_WITH_SERIALIZATION)
bool SModeller::saveBinary( std::string filename )
{
        std::ofstream ofs(filename, std::ios::binary);
        /* std::ofstream ofs(filename); */

        if ( !ofs.good() )
        {
            return false; 
        }

        cereal::PortableBinaryOutputArchive oarchive(ofs);
        /* cereal::XMLOutputArchive oarchive(ofs); */
        /* cereal::JSONOutputArchive oarchive(ofs); */

        unsigned int version = 1;

        try 
        {
            oarchive( cereal::make_nvp("SModeller version", version), 
                      cereal::make_nvp("SModeller data", *pimpl_) 
                    );
        }
        catch( const std::exception &e )
        {
            std::cerr << "Exception caught while trying to save file: " << e.what() << std::endl << std::flush;

            return false;
        }
        catch(...)
        {
            std::cerr << "Unknown exception caught in method SModeller::saveBinary(...)\n\n" << std::flush;

            return false;
        }

        return true;
}

bool SModeller::saveJSON( std::string filename )
{
        /* std::ofstream ofs(filename, std::ios::binary); */
        std::ofstream ofs(filename);

        if ( !ofs.good() )
        {
            return false; 
        }

        /* cereal::PortableBinaryOutputArchive oarchive(ofs); */
        /* cereal::XMLOutputArchive oarchive(ofs); */
        cereal::JSONOutputArchive oarchive(ofs);

        unsigned int version = 1;

        try 
        {
            oarchive( cereal::make_nvp("SModeller version", version), 
                      cereal::make_nvp("SModeller data", *pimpl_) 
                    );
        }
        catch( const std::exception &e )
        {
            std::cerr << "Exception caught while trying to save file: " << e.what() << std::endl << std::flush;

            return false;
        }
        catch(...)
        {
            std::cerr << "Unknown exception caught in method SModeller::saveJSON(...)\n\n" << std::flush;

            return false;
        }

        return true;
}

bool SModeller::loadBinary( std::string filename )
{
        clear();

        std::ifstream ifs(filename, std::ios::binary);
        /* std::ifstream ifs(filename); */

        if ( !ifs.good() )
        {
            return false;
        }

        cereal::PortableBinaryInputArchive iarchive(ifs);
        /* cereal::XMLInputArchive iarchive(ifs); */
        /* cereal::JSONInputArchive iarchive(ifs); */

        unsigned int version;

        std::cout << "Trying to load binary file: " + filename + " >> ";
        try
        {
            iarchive( version, *pimpl_ );
        }
        catch( const std::exception &e )
        {
            std::cout << "failure\n";
            std::cerr << "Exception caught while trying to load file: " << e.what() << std::endl << std::flush;
            clear();

            return false;
        }
        catch(...)
        {
            std::cout << "failure\n";
            std::cerr << "Unknown exception caught in method SModeller::loadBinary(...)\n\n" << std::flush;
            clear();

            return false;
        }
        std::cout << "success\n" << std::flush;


        return true;
}

bool SModeller::loadJSON( std::string filename )
{
        clear();

        std::ifstream ifs(filename);

        if ( !ifs.good() )
        {
            return false;
        }

        /* cereal::XMLInputArchive iarchive(ifs); */
        cereal::JSONInputArchive iarchive(ifs);

        unsigned int version;

        std::cout << "Trying to load JSON file: " + filename + " >> ";
        try
        {
            iarchive( version, *pimpl_ );
        }
        catch( const std::exception &e )
        {
            std::cout << "failure\n";
            std::cerr << "Exception caught while trying to load file: " << e.what() << std::endl << std::flush;
            clear();

            return false;
        }
        catch(...)
        {
            std::cout << "failure\n";
            std::cerr << "Unknown exception caught in method SModeller::loadJSON(...)\n\n" << std::flush;
            clear();

            return false;
        }
        std::cout << "success\n" << std::flush;


        return true;
}

#else //defined(BUILD_WITH_SERIALIZATION)
bool SModeller::saveBinary( std::string )
{
    return false;
}

bool SModeller::loadBinary( std::string )
{
    return false;
}

bool SModeller::saveJSON( std::string )
{
    return false;
}

bool SModeller::loadBinary( std::string )
{
    return false;
}

#endif //defined(BUILD_WITH_SERIALIZATION)
