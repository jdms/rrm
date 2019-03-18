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


#ifndef __SRULES_HPP__
#define __SRULES_HPP__

#include <vector>
#include <unordered_map>
#include <cstdint>

#include "serialization_primitives.hpp"
#include "planar_surface.hpp" 

/** 
 * \class SRules \anchor SRules
 * 
 * \brief Simple stratigraphic rules aware surfaces container. 
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

        /** \brief Change surfaces' discretization. 
         *
         * \param numX: number of intervals to discretize the width of the model
         * \param numY: number of intervals to discretize the length of the model
         * \return True if the discretizations were changed. 
         */
        bool changeDiscretization(size_t numX, size_t numY);

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
        
        /** \brief Query whether the container is empty. 
         *
         * \return True if the container is empty. 
         */
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

        std::vector<size_t> getSurfacesBelowPoint( const Point3 &p );

        std::vector<size_t> getSurfacesBelowPoint( Point3 &&p );

        std::vector<size_t> getSurfacesAbovePoint( const Point3 &p );

        std::vector<size_t> getSurfacesAbovePoint( Point3 &&p );

        // return empty descriptor if outside Bounding Box
        std::vector<size_t> getActiveSurfacesBelowPoint( const Point3 &p );

        // return empty descriptor if outside Bounding Box
        std::vector<size_t> getActiveSurfacesBelowPoint( Point3 &&p );

        // return empty descriptor if outside Bounding Box
        std::vector<size_t> getActiveSurfacesAbovePoint( const Point3 &p );

        // return empty descriptor if outside Bounding Box
        std::vector<size_t> getActiveSurfacesAbovePoint( Point3 &&p );

        std::vector<size_t> getLowerBound( std::vector<size_t> surface_ids );

        std::vector<size_t> getUpperBound( std::vector<size_t> surface_ids );

        template<typename VertexList, typename FaceList, typename NormalList>
        size_t getAdaptedMesh( size_t surface_id, VertexList &vlist, FaceList &flist, NormalList &nlist );

        template<typename VertexList, typename FaceList>
        bool getLowerBoundary( const std::vector<VertexList> &vlists, const std::vector<FaceList> &flists, VertexList &boundary_vlist, FaceList &boundary_flist );

        template<typename VertexList, typename FaceList>
        bool getUpperBoundary( const std::vector<VertexList> &vlists, const std::vector<FaceList> &flists, VertexList &boundary_vlist, FaceList &boundary_flist );

        bool saveBinary( const std::string &filename );

        bool loadBinary( const std::string &filename );

        bool saveXML( const std::string &filename );

        bool loadXML( const std::string &filename );


    public:
        bool removeAbove( PlanarSurface::Ptr sptr ); 
        bool removeAboveIntersection( PlanarSurface::Ptr sptr ); 

        bool removeBelow( PlanarSurface::Ptr sptr ); 
        bool removeBelowIntersection( PlanarSurface::Ptr sptr ); 

        bool defineAbove( PlanarSurface::Ptr sptr ); 
        bool defineBelow( PlanarSurface::Ptr sptr ); 

        bool defineAbove( std::vector<PlanarSurface::Ptr> &bounding_surfaces ); 
        bool defineBelow( std::vector<PlanarSurface::Ptr> &bounding_surfaces ); 

        bool removeAbove( std::size_t surface_index ); 
        bool removeAboveIntersection( std::size_t surface_index ); 

        bool removeBelow( std::size_t surface_index ); 
        bool removeBelowIntersection( std::size_t surface_index ); 

        bool defineAbove( std::size_t surface_index ); 
        bool defineBelow( std::size_t surface_index ); 

        bool defineAbove( std::vector<size_t> surface_indices );
        bool defineBelow( std::vector<size_t> surface_indices );

        bool weakEntireSurfaceCheck( std::size_t surface_index ); 

        bool liesInsideBoundingBox( const Point3 &p );

        bool liesInsideBoundingBox( Point3 &&p );

        bool liesBetweenBoundarySurfaces( const Point3 &p );

        bool liesBetweenBoundarySurfaces( Point3 &&p );

        void updateDiscretization();

        void updateCache();

    private: 
        ContainerType container; 
        MapType dictionary; 

        std::vector<std::size_t> lower_bound_ids_ = {};
        std::vector<std::size_t> sup_lower_bound_ids_ = {};
        std::vector<PlanarSurface::WeakPtr> lower_bound_;
        bool define_above_ = false; 

        std::vector<std::size_t> upper_bound_ids_ = {};
        std::vector<std::size_t> inf_upper_bound_ids_ = {};
        std::vector<PlanarSurface::WeakPtr> upper_bound_;
        bool define_below_ = false;

        bool defineAboveIsActive();
        bool defineBelowIsActive();

        bool isValidSurface( const PlanarSurface::Ptr &sptr );
        bool weakEntireSurfaceCheck( const PlanarSurface::Ptr &sptr );
        bool weakEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &surfaces );

        bool weakLowerBoundedEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &surfaces );
        bool weakUpperBoundedEntireSurfaceListCheck( const std::vector<PlanarSurface::Ptr> &surfaces );

        bool isValidSurfaceForInsertion( const PlanarSurface::Ptr &sptr, std::size_t &surface_index );

        bool boundaryAwareRemoveAbove( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface );
        bool boundaryAwareRemoveBelow( const PlanarSurface::Ptr &base_surface, PlanarSurface::Ptr &to_remove_surface );

        // Cereal provides an easy way to serialize objects
        friend class cereal::access;

        template<typename Archive>
        void save( Archive &ar, const std::uint32_t version ) const;

        template<typename Archive>
        void load( Archive &ar, const std::uint32_t version );

        /* template<typename Archive> */
        /* void serialize( Archive &ar, const std::uint32_t version ); */
}; 

template<typename VertexList, typename FaceList, typename NormalList>
size_t SRules::getAdaptedMesh( size_t surface_id, VertexList &vlist, FaceList &flist, NormalList &nlist )
{
    UNUSED(surface_id);
    UNUSED(vlist);
    UNUSED(flist);
    UNUSED(nlist);

    if ( surface_id >= this->size() )
    {
        return false;
    }

    size_t face_count = 0;

    container[surface_id]->getVertexList(vlist);
    flist.clear(); 

    /* std::cout << "Getting face list: \n"; */ 

    // For all triangles in discretization
    //     Consider:
    //         a) triangle is empty:
    //             return null;
    //         b) triangle is valid:
    //             return triangle;
    //         c) triangle has one invalid vertex:
    //             return case 1;
    //         d) triangle has two invalid vertices, truncated by the same surface:
    //             return case 2;
    //         e) triangle has two invalid vertices, truncated by different surfaces:
    //             return triangle; (?, try something different for visualization?)
    //
    //      If got triangles from "case 1" or case 2" update vertex list and normal list
    //
    //      function getTriangles( triangle data[in], face list[out], vertex list[out], normal list[out] ) -> num triangles(integer)
    //
    //      function computeIntersection( segment1, segment2) -> parameter(real)


    /* unsigned int face_count = 0; 0 */
    /* for ( Natural i = 0; i < nX_ - 1; i += 2 ) { */ 
    /*     for ( Natural j = 0; j < nY_ - 1; j += 2 ) { */ 
    /*     } */
    /* } */

    return face_count; 
}

template<typename VertexList, typename FaceList>
bool SRules::getLowerBoundary( const std::vector<VertexList> &vlists, const std::vector<FaceList> &flists, VertexList &boundary_vlist, FaceList &boundary_flist )
{
    if ( vlists.size() != flists.size() )
    {
        return false;
    }

    size_t num_elements = vlists[0].size();
    for ( size_t i = 1; i < vlists.size(); ++i )
        if ( vlists[i].size() != num_elements )
            return false;

    boundary_vlist = vlists[0];
    for ( size_t i = 1; i < vlists.size(); ++i )
    {
        for ( size_t j = 0; j < num_elements; ++j )
        {
            if ( boundary_vlist[j] > vlists[i][j] )
            {
                boundary_vlist[j] = vlists[i][j];
            }
        }
    }

    return container[0]->mergeFaceLists(flists, boundary_flist);
}

template<typename VertexList, typename FaceList>
bool SRules::getUpperBoundary( const std::vector<VertexList> &vlists, const std::vector<FaceList> &flists, VertexList &boundary_vlist, FaceList &boundary_flist )
{
    if ( vlists.size() != flists.size() )
    {
        return false;
    }

    size_t num_elements = vlists[0].size();
    for ( size_t i = 1; i < vlists.size(); ++i )
        if ( vlists[i].size() != num_elements )
            return false;

    boundary_vlist = vlists[0];
    for ( size_t i = 1; i < vlists.size(); ++i )
    {
        for ( size_t j = 0; j < num_elements; ++j )
        {
            if ( boundary_vlist[j] < vlists[i][j] )
            {
                boundary_vlist[j] = vlists[i][j];
            }
        }
    }

    return container[0]->mergeFaceLists(flists, boundary_flist);
}


#if defined( BUILD_WITH_SERIALIZATION )
    #include "cereal/types/vector.hpp"
    #include "cereal/types/unordered_map.hpp"

    template<typename Archive>
    void SRules::save( Archive &ar, const std::uint32_t version ) const
    {
        (void)(version);
        ar(
            container, 
            dictionary,
            lower_bound_, 
            define_above_, 
            upper_bound_, 
            define_below_
          );
    }

    template<typename Archive>
    void SRules::load( Archive &ar, const std::uint32_t version )
    {
        if ( version == 1 )
        {
            PlanarSurface::WeakPtr lbound, ubound;

            ar(
                container, 
                dictionary,
                lbound, 
                define_above_, 
                ubound, 
                define_below_
          );
                lower_bound_ = { lbound };
                upper_bound_ = { ubound };
        }
        else
        {
            ar(
                container, 
                dictionary,
                lower_bound_, 
                define_above_, 
                upper_bound_, 
                define_below_
            );
        }

        updateCache();
    }

    CEREAL_CLASS_VERSION(SRules, 2);

#else
    /* template<typename Archive> */
    /* void SRules::serialize( Archive &, const std::uint32_t ) {} */

    template<typename Archive>
    void SRules::save( Archive &/* ar */, const std::uint32_t /* version */ ) const {}

    template<typename Archive>
    void SRules::load( Archive &/* ar */, const std::uint32_t /* version */ ) {}

#endif /* BUILD_WITH_SERIALIZATION */

#endif 
