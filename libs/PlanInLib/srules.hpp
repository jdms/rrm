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


#ifndef __SRULES_HPP__
#define __SRULES_HPP__

#include <vector>
#include <unordered_map>

#include "planar_surface.hpp" 

/** 
 * \class SRules \anchor SRules
 * 
 * \brief Simple stratigraphic rules. 
 *
 * This class implements a container and a basic set of stratigraphic rules, 
 * which are applied to the container's surfaces. 
 *
 */
class SRules
{
    public: 
        /** \brief The container's type. A simple vector of PlanarSurfaces's (smart) shared pointers.*/ 
        using ContainerType = std::vector<PlanarSurface::Ptr>; 

        /** \brief Type of the dictionary that translates PlanarSurface's SurfaceIds to this->container indices. */ 
        using MapType = std::unordered_map<PlanarSurface::SurfaceId, size_t>; 


        /** \brief Clear the container. 
         */
        void clear(); 

        /** \brief Overloading of 'operator[]'. 
         *
         * \param surface_index: index of desired surface. 
         * \return A reference to the surface's shared pointer at index 'surface_index'. 
         */
        PlanarSurface::Ptr &operator[]( std::size_t surface_index ); 

        /** \brief Container's size. 
         *
         * \return Size of the container. 
         */
        std::size_t size(); 

        bool empty(); 
        
        /** \brief Translates a PlanarSurface's SurfaceId to the surface's index in the container. 
         *
         * \param id: surface's id. 
         * \param surface_index: index of the surface in the container, output. 
         *
         * \return Whether the surface was found in the container or not. 
         */ 
        bool getSurfaceIndex( const PlanarSurface::SurfaceId id, size_t &surface_index ); 


        /** \brief Set the bounding box for all PlanarSurfaces
         *
         * \param origin: bounding box's origin. 
         * \param length: bounding box's length. 
         *
         * \return Whether the bounding box was set or not. 
         */ 
        /* TODO: move logic for handling the bounding box from class PlanarSurface to here. */
        bool setBoundingBox( const Point3 &origin, const Point3 &lenght ); 

        /** \brief Add a new surface to the container. 
         *
         * \param sptr: reference to a shared pointer, pointing to the surface. Warning: the surface is moved from this pointer. 
         *
         * \return Whether the surface was correctly inserted or not. The surface will not be inserted in the container if 
         * it (the surface) is not defined. 
         */
        bool addSurface( PlanarSurface::Ptr &sptr ); 

        /** \brief Add a new surface to the container. 
         *
         * \param sptr: reference to a shared pointer, pointing to the surface. Warning: the surface is moved from this pointer. 
         * \param surface_index: index of the new surface in the container. 
         *
         * \return Whether the surface was correctly inserted or not. The surface will not be inserted in the container if 
         * it (the surface) is not defined. 
         */
        bool addSurface( PlanarSurface::Ptr &sptr, std::size_t &surface_index ); 

        /** \brief Add a new surface to the container. 
         *
         * \param sptr: reference to a shared pointer, pointing to the surface. Warning: the surface is moved from this pointer. 
         * \param surface_index: index of the new surface in the container. 
         * \param remove_above_surfaces: list of surface's indices from which the inserted surface should be "removed above". 
         * \param remove_below_surfaces: list of surface's indices from which the inserted surface should be "removed below". 
         *
         * \return Whether the surface was correctly inserted or not. The surface will neither be inserted in the container if 
         * it (the surface) is not defined, nor if any of the gelological rules fail to apply.  
         */
        bool addSurface( 
                PlanarSurface::Ptr &sptr, 
                std::size_t &surface_index, 
                std::vector<size_t> remove_above_surfaces, 
                std::vector<size_t> remove_below_surfaces 
            ); 

        /** \brief Returns a list with all surfaces that intersect the last surface added to the container. 
         *
         * \param intersection_list: Vector with the indices of the intersecting surfaces. 
         *
         * \return False if the container was empty, True otherwise. 
         */
        bool lastInsertedSurfaceIntersects( std::vector<std::size_t> &intersection_list ); 

        /** \brief Returns a list with all surfaces that intersect the given surface. 
         *
         * \param sptr: Reference to shared pointer pointing to the given surface. 
         * \param intersection_list: Vector with the indices of the intersecting surfaces. 
         *
         * \return False if the container was empty, True otherwise. 
         */
        bool getIntersectionList( const PlanarSurface::Ptr &sptr, std::vector<std::size_t> &intersection_list ); 

        /** \brief Remove last added surface to the container. 
         *
         * \return False if the container was empty, True otherwise. 
         */
        bool popLastSurface(); 

        /** \brief Remove given surface from the container. 
         *
         * \param surface: shared pointer pointing to the surface to be removed from the container. 
         *
         * \return False if the container was empty, True otherwise. 
         */
        bool popLastSurface( PlanarSurface::Ptr &surface ); 


        /** \brief Disalow surfaces from being defined below the last inserted surface.  
         */
        bool defineAbove(); 
        /** \brief Stop the 'defineAbove' restriction. 
         */ 
        void stopDefineAbove();  


        /** \brief Disalow surfaces from being defined above the last inserted surface.  
         */
        bool defineBelow(); 
        /** \brief Stop the 'defineBelow' restriction. 
         */ 
        void stopDefineBelow(); 


        /** \brief Stratigraphic rule.
         */
        bool removeAbove(); 

        /** \brief Stratigraphic rule.
         */
        bool removeAboveIntersection(); 

        /** \brief Stratigraphic rule.
         */
        bool removeBelow(); 

        /** \brief Stratigraphic rule.
         */
        bool removeBelowIntersection(); 


    public:
        bool removeAbove( PlanarSurface::Ptr sptr ); 
        bool removeAboveIntersection( PlanarSurface::Ptr sptr ); 

        bool removeBelow( PlanarSurface::Ptr sptr ); 
        bool removeBelowIntersection( PlanarSurface::Ptr sptr ); 

        bool defineAbove( PlanarSurface::Ptr sptr ); 
        bool defineBelow( PlanarSurface::Ptr sptr ); 

        bool removeAbove( std::size_t surface_index ); 
        bool removeAboveIntersection( std::size_t surface_index ); 

        bool removeBelow( std::size_t surface_index ); 
        bool removeBelowIntersection( std::size_t surface_index ); 

        bool defineAbove( std::size_t surface_index ); 
        bool defineBelow( std::size_t surface_index ); 

        bool weakEntireSurfaceCheck( std::size_t surface_index ); 


    private: 
        ContainerType container; 
        MapType dictionary; 

        PlanarSurface::WeakPtr lower_bound_; 
        bool define_above_ = false; 

        PlanarSurface::WeakPtr upper_bound_; 
        bool define_below_ = false; 

        bool defineAboveIsActive();
        bool defineBelowIsActive();

        bool isValidSurface( const PlanarSurface::Ptr &sptr ); 
        bool weakEntireSurfaceCheck( const PlanarSurface::Ptr &sptr ); 

        bool isValidSurfaceForInsertion( const PlanarSurface::Ptr &sptr, std::size_t &surface_index ); 

        bool boundaryAwareRemoveAbove( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface ); 
        bool boundaryAwareRemoveBelow( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface ); 
}; 

#endif 
