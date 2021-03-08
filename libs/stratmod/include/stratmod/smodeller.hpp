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



#ifndef STRATMOD_SMODELLER_HPP
#define STRATMOD_SMODELLER_HPP

#include <iostream>
#include <vector>
#include <memory>

#include "stratmod/smodeller_primitives.hpp"

namespace stratmod {

/** 
 * @brief Creates consistent surface based models.
 *
 * This class provides the user API of the stratmod library.
 **/
class STRATMODLIB_DLL_HANDLER SModeller
{
    public:
        ///////////////////////////////////////////////////////////////////////
        //
        // Constructors, maintenance, etc.
        //
        ///////////////////////////////////////////////////////////////////////

        /** @brief Get reference to unique SModeller instance.
         **/
        static SModeller& Instance();

        /** 
         * @brief [[deprecated]] Public default constructor.  
         *
         * Be aware that the SModeller class is essentially a Singleton, 
         * explicit creation of SModeller objects will be deprecated in the
         * near future.
         **/
        SModeller(); 

        /**
         * @brief Default destructor.
         **/
        ~SModeller(); 

        /** 
         * @brief Copy constructor is deleted.  
         **/
        SModeller( const SModeller &m ) = delete;

        /** 
         * @brief Default move constructor.  
         **/
        SModeller( SModeller &&m ) = delete; 

        /** 
         * @brief Copy assignment constructor is deleted.  
         **/
        SModeller& operator=( const SModeller &m ) = delete;

        /** 
         * @brief Default move assignment constructor.  
         **/
        SModeller& operator=( SModeller &&m ) = delete; 

        /**
         * @brief Clear model.
         **/
        void clear();

        /**
         * @brief Save model to binary file..
         *
         * @param filename Name of file.
         *
         * @return True if model was saved correctly.
         **/
        bool saveBinary( std::string filename );

        /**
         * @brief Load model from binary file..
         *
         * @param filename Name of file.
         *
         * @return True if model was loaded correctly.
         **/
        bool loadBinary( std::string filename );

        /**
         * @brief Save model to json file..
         *
         * @param filename Name of file.
         *
         * @return True if model was saved correctly.
         **/
        bool saveJSON( std::string filename );

        /**
         * @brief Load model from json file..
         *
         * @param filename Name of file.
         *
         * @return True if model was loaded correctly.
         **/
        bool loadJSON( std::string filename );

        ///////////////////////////////////////////////////////////////////////
        //
        // Model geometry and discretization
        //
        ///////////////////////////////////////////////////////////////////////

        /**
         * @brief Set coordinate system such that x = width, y = length, z = height.
         *
         * Within the default coordinate system, all methods that receive or return 
         * vectors of triplets with geometry expect the list be organized as follows:
         *
         * {x_0, y_0, z_0, x_1, y_1, z_1, ...}
         **/
        bool useDefaultCoordinateSystem();

        /**
         * @brief Set coordinate system such that x = width, y = height, z = length.
         *
         * Within the OpenGL coordinate system, all methods that receive or return 
         * vectors of triplets with geometry expect the list be organized as follows:
         *
         * {x_0, z_0, y_0, x_1, z_1, y_1, ...}
         **/
        bool useOpenGLCoordinateSystem();

        /** 
         * @brief Get number of pieces in which the width direction is discretized.
         *
         * @return Discretization number
         **/
        std::size_t getWidthDiscretization() const;

        /** 
         * @brief Get number of pieces in which the length direction is discretized.
         *
         * @return Discretization number
         **/
        std::size_t getLengthDiscretization() const;

        /**
         * @brief Set model's bounding box origin.
         *
         * @param x,y,z Origin coordinates
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        void setOrigin( double x, double y, double z );

        /**
         * @brief Set model's bounding box size.
         *
         * @param x,y,z Size coordinates
         *
         * @return True if size is set correctly, i.e., x, y, z > 0
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        bool setSize( double x, double y, double z );

        /**
         * @brief Get model's bounding box origin.
         *
         * @param x,y,z Origin coordinates
         **/
        void getOrigin( double &x, double &y, double &z );

        /**
         * @brief Get model's bounding box size.
         *
         * @param x,y,z Size coordinates
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        void getSize( double &x, double &y, double &z );

        /**
         * @brief Change model's current discretization.
         *
         * @param width_discretization Discretization of widthwise cross-sections.
         * @param length_discretization Discretization of lengthwise cross-sections.
         *
         * @return True if discretization is set correctly, i.e., width_discretization, length_discretization > 0
         **/
        bool changeDiscretization( std::size_t width_discretization = 64, std::size_t length_discretization = 64 );


        ///////////////////////////////////////////////////////////////////////
        //
        // Geologic Operators:
        //
        // The following methods change the state of the modelling library.
        //
        ///////////////////////////////////////////////////////////////////////

        //
        // Operators that change newly inserted surfaces
        //

        /**
         * @brief PreserveAbove.
         *
         * Select the surfaces given by list bounding_surfaces_list 
         * as a lower boundary for surface creation, i.e., all parts of new surfaces
         * that lie below bounding_surfaces_list will be removed.
         *
         * @param bounding_surfaces_list Lower boundary for creation of new surfaces.
         *
         * @return True if bounding_surfaces_list is a valid lower boundary for PreserveAbove.
         **/
        bool preserveAbove( const std::vector<size_t> &bounding_surfaces_list );

        /**
         * @brief PreserveBelow.
         *
         * Select the surfaces given by list bounding_surfaces_list 
         * as an upper boundary for surface creation, i.e., all parts of new surfaces
         * that lie above bounding_surfaces_list will be removed.
         *
         * @param bounding_surfaces_list Upper boundary for creation of new surfaces.
         *
         * @return True if bounding_surfaces_list is a valid upper boundary for PreserveBelow.
         **/
        bool preserveBelow( const std::vector<size_t> &bounding_surfaces_list );

        /**
         * @brief Stop PreserveAbove.
         **/
        void stopPreserveAbove();

        /**
         * @brief Stop PreserveBelow.
         **/
        void stopPreserveBelow();

        /**
         * @brief Tells whether PreserveAbove is currently active.
         *
         * @param[out] bounding_surfaces_list List of surfaces indices of lower boundary.
         *
         * @return True PreserveAbove is active.
         **/
        bool preserveAboveIsActive( std::vector<std::size_t> &bounding_surfaces_list );

        /**
         * @brief Tells whether PreserveBelow is currently active.
         *
         * @param[out] bounding_surfaces_list List of surfaces indices of upper boundary.
         *
         * @return True PreserveBelow is active.
         **/
        bool preserveBelowIsActive( std::vector<std::size_t> &bounding_surfaces_list );

        //
        // Operators that change old, previously inserted surfaces
        //

        /**
         * @brief Apply RemoveAbove (RA) operator
         **/
        void removeAbove();

        /**
         * @brief Apply RemoveAboveIntersection (RAI) operator
         **/
        void removeAboveIntersection();

        /**
         * @brief Apply RemoveBelow (RB) operator
         **/
        void removeBelow();

        /**
         * @brief Apply RemoveBelowIntersection (RBI) operator
         **/
        void removeBelowIntersection();

        /**
         * @brief Disable RA, RB, RAI, RBI
         **/
        void disableGeologicRules();

        //
        // Change model by removing or restoring surfaces
        //

        /**
         * @brief Returns true is there is at least one surface in the model that can be undone
         **/
        bool canUndo();

        /**
         * @brief Undo the last surface insertion
         *
         * @return True is undo() was performed
         **/
        bool undo();

        /**
         * @brief Returns true is there is at least one surface in the undo stack that can be restored to the model
         **/
        bool canRedo();

        /**
         * @brief Redo the last undoed surface
         *
         * @return True is redo() was performed
         **/
        bool redo();

        /**
         * @brief Destroy last surface inserted in model (this operation can't be undone).
         **/
        bool destroyLastSurface();

        ///////////////////////////////////////////////////////////////////////
        //
        // Insert surfaces in model, query ids of surfaces already inserted
        // in model.
        //
        ///////////////////////////////////////////////////////////////////////

        /**
         * @brief Get list of surfaces indices in their insertion order
         **/
        std::vector<std::size_t> getSurfacesIndices();

        /**
         * @brief Get list of surfaces indices in their stratigraphic order
         **/
        std::vector<std::size_t> getOrderedSurfacesIndices();
        
        /**
         * @brief Create a new surface from input points and add it to the model.
         *
         * Creates an approximate interpolant as defined in paper:
         * WENDLAND H., RIEGER C., Approximate interpolation with applications 
         * to selecting smoothing parameters. Numerische Mathematik 101, 4 (2005), 
         * 729–748. 5
         *
         * See section 3 (subsection 3.2 in particular) of the paper for the criteria 
         * to pick the smoothing parameter (represented as the greek letter lambda in
         * the paper).  
         *
         * @param surface_id Index of surface.
         * @param point_data List of surface's sampled points (vector of triplets {width, length, height})
         * @param smoothing_parameter Set to 0 for interpolation, any value >0 creates an approximate interpolant
         * @param lower_bound_ids,upper_bound_ids Not used
         *
         * @return True New surface has been added to the model.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        bool createSurface( std::size_t surface_id, 
                const std::vector<double> &point_data,
                double smoothing_parameter = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>() 
                );

        /**
         * @brief Create a new surface linearly extruded along the model length and insert it to the model
         *
         * Creates an approximate interpolant as defined in paper:
         * WENDLAND H., RIEGER C., Approximate interpolation with applications 
         * to selecting smoothing parameters. Numerische Mathematik 101, 4 (2005), 
         * 729–748. 5
         *
         * See section 3 (subsection 3.2 in particular) of the paper for the criteria 
         * to pick the smoothing parameter (represented as the greek letter lambda in
         * the paper).  
         *
         * @param surface_id Index of surface.
         * @param cross_section_curve_point_data List of surface's sampled points (vector of tuples {width, height})
         * @param smoothing_parameter Set to 0 for interpolation, any value >0 creates an approximate interpolant
         * @param lower_bound_ids,upper_bound_ids Not used
         *
         * @return True New surface has been added to the model.
         **/
        bool createLengthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface extruded along the given path (defined along the lengthwise direction of the model)
         *
         * Creates an approximate interpolant as defined in paper:
         * WENDLAND H., RIEGER C., Approximate interpolation with applications 
         * to selecting smoothing parameters. Numerische Mathematik 101, 4 (2005), 
         * 729–748. 5
         *
         * See section 3 (subsection 3.2 in particular) of the paper for the criteria 
         * to pick the smoothing parameter (represented as the greek letter lambda in
         * the paper).  
         *
         * @param surface_id Index of surface.
         * @param cross_section_curve_point_data List of surface's sampled points (vector of tuples {width, height})
         * @param cross_section Position in model cross section lies
         * @param path_curve_point_data List of path's sampled points (vector of tuples {width, length})
         * @param smoothing_parameter Set to 0 for interpolation, any value >0 creates an approximate interpolant
         * @param lower_bound_ids,upper_bound_ids Not used
         *
         * @return True New surface has been added to the model.
         **/
        bool createLengthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface linearly extruded along the model width and insert it to the model
         *
         * Creates an approximate interpolant as defined in paper:
         * WENDLAND H., RIEGER C., Approximate interpolation with applications 
         * to selecting smoothing parameters. Numerische Mathematik 101, 4 (2005), 
         * 729–748. 5
         *
         * See section 3 (subsection 3.2 in particular) of the paper for the criteria 
         * to pick the smoothing parameter (represented as the greek letter lambda in
         * the paper).  
         *
         * @param surface_id Index of surface.
         * @param cross_section_curve_point_data List of surface's sampled points (vector of tuples {length, height})
         * @param smoothing_parameter Set to 0 for interpolation, any value >0 creates an approximate interpolant
         * @param lower_bound_ids,upper_bound_ids Not used
         *
         * @return True New surface has been added to the model.
         **/
        bool createWidthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface extruded along the given path (defined along the widthwise direction of the model)
         *
         * Creates an approximate interpolant as defined in paper:
         * WENDLAND H., RIEGER C., Approximate interpolation with applications 
         * to selecting smoothing parameters. Numerische Mathematik 101, 4 (2005), 
         * 729–748. 5
         *
         * See section 3 (subsection 3.2 in particular) of the paper for the criteria 
         * to pick the smoothing parameter (represented as the greek letter lambda in
         * the paper).  
         *
         * @param surface_id Index of surface.
         * @param cross_section_curve_point_data List of surface's sampled points (vector of tuples {length, height})
         * @param cross_section Position in model cross section lies
         * @param path_curve_point_data List of path's sampled points (vector of tuples {length, width})
         * @param smoothing_parameter Set to 0 for interpolation, any value >0 creates an approximate interpolant
         * @param lower_bound_ids,upper_bound_ids Not used
         *
         * @return True New surface has been added to the model.
         **/
        bool createWidthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface from input points and add it to the model, provided that it doesn't intersect other surfaces in the model.
         *
         * This method is meant to be used while geologic operators are disable, to test which operator would work best while inserting the new surface.
         *
         * @param intersected_surfaces List of surfaces previously inserted in model that new surface intersects.
         *
         * @return True New surface has been added to the model without intersecting other surfaces.
         *
         * @sa createSurface()
         **/
        bool tryCreateSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>() 
                );

        /**
         * @brief Create a new surface from input points and add it to the model, provided that it doesn't intersect other surfaces in the model.
         *
         * This method is meant to be used while geologic operators are disable, to test which operator would work best while inserting the new surface.
         *
         * @param intersected_surfaces List of surfaces previously inserted in model that new surface intersects.
         *
         * @return True New surface has been added to the model without intersecting other surfaces.
         *
         * @sa createLengthwiseExtrudedSurface()
         **/
        bool tryCreateLengthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface from input points and add it to the model, provided that it doesn't intersect other surfaces in the model.
         *
         * This method is meant to be used while geologic operators are disable, to test which operator would work best while inserting the new surface.
         *
         * @param intersected_surfaces List of surfaces previously inserted in model that new surface intersects.
         *
         * @return True New surface has been added to the model without intersecting other surfaces.
         *
         * @sa createLengthwiseExtrudedSurface()
         **/
        bool tryCreateLengthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface from input points and add it to the model, provided that it doesn't intersect other surfaces in the model.
         *
         * This method is meant to be used while geologic operators are disable, to test which operator would work best while inserting the new surface.
         *
         * @param intersected_surfaces List of surfaces previously inserted in model that new surface intersects.
         *
         * @return True New surface has been added to the model without intersecting other surfaces.
         *
         * @sa createWidthwiseExtrudedSurface()
         **/
        bool tryCreateWidthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /**
         * @brief Create a new surface from input points and add it to the model, provided that it doesn't intersect other surfaces in the model.
         *
         * This method is meant to be used while geologic operators are disable, to test which operator would work best while inserting the new surface.
         *
         * @param intersected_surfaces List of surfaces previously inserted in model that new surface intersects.
         *
         * @return True New surface has been added to the model without intersecting other surfaces.
         *
         * @sa createWidthwiseExtrudedSurface()
         **/
        bool tryCreateWidthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        ///////////////////////////////////////////////////////////////////////
        //
        // Get geometry and model properties for visualization and numerics.
        //
        ///////////////////////////////////////////////////////////////////////

        /**
         * @brief Get triangle mesh of a surface.
         *
         * @param surface_id Index of surface.
         * @param vlist Surface's vertex list (vector of triplets {width, length, height})
         * @param tlist Surface's triangle list (vector of triplets of indices into the vertex list)
         *
         * @return True if mesh was retrieved.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        bool getMesh( std::size_t surface_id, std::vector<double> &vlist, std::vector<std::size_t> &tlist );

        /**
         * @brief Get cross section curve of a surface along the width direction.
         *
         * @param surface_id Index of surface.
         * @param width Id of width cross-section curve lies.
         * @param vlist Surface's vertex list (vector of pairs {length, height})
         * @param flist Surface's triangle list (vector of pairs of indices into the vertex list)
         *
         * @return True if curve was retrieved.
         **/
        bool getWidthCrossSectionCurve( std::size_t surface_id, std::size_t width, std::vector<double> &vlist, std::vector<std::size_t> &elist );

        /**
         * @brief Get cross section curve of a surface along the length direction.
         *
         * @param surface_id Index of surface.
         * @param width Id of length cross-section curve lies.
         * @param vlist Surface's vertex list (vector of pairs {width, height})
         * @param flist Surface's triangle list (vector of pairs of indices into the vertex list)
         *
         * @return True if curve was retrieved.
         **/
        bool getLengthCrossSectionCurve( std::size_t surface_id, std::size_t lenght, std::vector<double> &vlist, std::vector<std::size_t> &elist );

        /**
         * @brief Get model's tetrahedral mesh.
         *
         * @param vertex_coordinates Model's vertex coordinates list (vector of triplets {width, length, height})
         * @param element_list Vector of tetrahedrals' indices list (vector of triplets of indices into the vertex list)
         * @param attribute_list Vector of regions tetrahedrals belong to
         *
         * @return Number of tetrahedrals in mesh.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        std::size_t getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::size_t> &element_list, std::vector<long int> &attribute_list );

        /**
         * @brief Get normal list of a surface.
         *
         * @param surface_id Index of surface.
         * @param normals Surface's normal list (vector of triplets with components {width, length, height} of normals)
         *
         * @return True if normals' list was retrieved.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        bool getNormalList( std::size_t surface_id, std::vector<double> &normals );

        /**
         * @brief Get extrusion curve path of a surface.
         *
         * @param surface_id Index of surface.
         * @param path_vertices Path abscissas and ordinates.
         *
         * @return True if surface has an extrusion path.
         **/
        bool getExtrusionPath( std::size_t surface_id, std::vector<double> &path_vertices );

        /**
         * @brief Compute volumes of regions.
         *
         * @param vlist List of volumes of regions.
         *
         * @return True if model has non-empty regions.
         **/
        bool computeTetrahedralMeshVolumes( std::vector<double> &vlist );

        /**
         * @brief Compute region ids from list of points.
         *
         * @param vcoords[in] Points' coordinates list (vector of triplets {width, length, height})
         * @param regions[out] Regions' ids corresponding to list of points.
         *
         * @return True if model has non-empty regions.
         **/
        bool getVolumeAttributesFromPointList( const std::vector<double> &vcoords, std::vector<int> &regions);

        /**
         * @brief Compute list of bounding surfaces given a region id.
         *
         * @param attribute_id Region id.
         * @param lower_bound[out] Ids of surfaces below the given region.
         * @param upper_bound[out] Ids of surfaces above the given region.
         *
         * @return True if model has non-empty regions.
         **/
        bool getBoundingSurfacesFromVolumeAttribute( std::size_t attribute_id, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound);

        /**
         * @brief Get ids of surfaces that lie below a given point.
         *
         * @param x,y,z Query point.
         *
         * @return List of surfaces' ids.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        std::vector<size_t> getSurfacesIndicesBelowPoint( double x, double y, double z );

        /**
         * @brief Get ids of surfaces that lie above a given point.
         *
         * @param x,y,z Query point.
         *
         * @return List of surfaces' ids.
         *
         * @sa useDefaultCoordinateSystem(), useOpenGLCoordinateSystem()
         **/
        std::vector<size_t> getSurfacesIndicesAbovePoint( double x, double y, double z );

    private:
        std::unique_ptr<SModellerImplementation> pimpl_;

        SModellerImplementation& Impl();
        const SModellerImplementation& Impl() const;

        friend class SUtilities;
};

} // namespace stratmod

using SModeller = stratmod::SModeller;

#endif

