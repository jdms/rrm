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



#ifndef __S_MODELLER__
#define __S_MODELLER__

#include <iostream>
#include <vector>
#include <memory>

#include "stratmod/smodeller_primitives.hpp"

class STRATMODLIB_DLL_HANDLER SModeller
{
    public:

        /** 
         * \brief Default constructor.  
         * 
         * It's declararion is required because of the 
         * std::unique_ptr in the implementation.
         *
         **/
        SModeller(); 

        /**
         * \brief Default destructor.
         *
         * It's declararion is required because of the 
         * std::unique_ptr in the implementation.
         *
         **/
        ~SModeller(); 

        /** 
         * \brief Default copy constructor is deleted.  
         * 
         * It is unclear whether there is use for copy constructors. 
         *
         **/
        SModeller( const SModeller &m ) = delete;

        /** 
         * \brief Default move constructor.  
         * 
         * It's declararion is required because of the 
         * std::unique_ptr in the implementation.
         *
         **/
        SModeller( SModeller &&m ); 

        /** 
         * \brief Default assignment (copy) constructor is deleted.  
         * 
         * It is unclear whether there is use for copy constructors. 
         *
         **/
        SModeller& operator=( const SModeller &m ) = delete;

        /** 
         * \brief Default assignment (move) constructor.  
         * 
         * It's declararion is required because of the 
         * std::unique_ptr in the implementation.
         *
         **/
        SModeller& operator=( SModeller &&m ); 


        /** 
         * \brief Get number of pieces in which the width direction is discretized.
         *
         *
         * \return discretization number
         **/
        std::size_t getWidthDiscretization() const;

        /** 
         * \brief Get number of pieces in which the length direction is discretized.
         *
         * \return discretization number
         **/
        std::size_t getLengthDiscretization() const;

        void setOrigin( double x, double y, double z );

        bool setSize( double x, double y, double z );

        void getOrigin( double &x, double &y, double &z );

        void getSize( double &x, double &y, double &z );

        void clear();

        bool changeDiscretization( std::size_t width_discretization = 64, std::size_t depth_discretization = 64 );

        /* Query or modify the automatum state */


        ///////////////////////////////////////////////////////
        // New structure aware preserve methods
        ///////////////////////////////////////////////////////

        bool preserveAbove( std::vector<size_t> &bounding_surfaces_list );
        bool preserveBelow( std::vector<size_t> &bounding_surfaces_list );

        void stopPreserveAbove();
        void stopPreserveBelow();

        bool preserveAboveIsActive( std::vector<std::size_t> &bounding_surfaces_list );
        bool preserveBelowIsActive( std::vector<std::size_t> &bounding_surfaces_list );

        ///////////////////////////////////////////////////////

        void disableGeologicRules();

        void removeAbove();
        void removeAboveIntersection();

        void removeBelow();
        void removeBelowIntersection();


        bool canUndo();
        bool undo();

        bool canRedo();
        bool redo();

        bool destroyLastSurface();


        // Input & output 
        //

        std::vector<std::size_t> getSurfacesIndices();

        std::vector<std::size_t> getOrderedSurfacesIndices();
        
        bool createSurface( std::size_t surface_id, 
                const std::vector<double> &point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>() 
                );

        bool createLengthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool createLengthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool createWidthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool createWidthwiseExtrudedSurface( std::size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool tryCreateSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>() 
                );

        bool tryCreateLengthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool tryCreateLengthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool tryCreateWidthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data,
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(), 
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        bool tryCreateWidthwiseExtrudedSurface( std::size_t surface_id, std::vector<std::size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data, double cross_section, 
                const std::vector<double> &path_curve_point_data, 
                double fill_distance = 0.0,
                const std::vector<std::size_t> lower_bound_ids = std::vector<std::size_t>(),
                const std::vector<std::size_t> upper_bound_ids = std::vector<std::size_t>()
                );

        /* Get mesh, pcl and curves for visualization */
        /* For all combinations of int32_t, int64_t, float, and double */


        bool getMesh( std::size_t surface_id, std::vector<double> &vlist, std::vector<std::size_t> &flist );

        bool getWidthCrossSectionCurve( std::size_t surface_id, std::size_t width, std::vector<double> &vlist, std::vector<std::size_t> &elist );

        bool getLengthCrossSectionCurve( std::size_t surface_id, std::size_t lenght, std::vector<double> &vlist, std::vector<std::size_t> &elist );

        std::size_t getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::size_t> &element_list, std::vector<long int> &attribute_list );


        /* Change the model's properties */
        bool useDefaultCoordinateSystem();

        bool useOpenGLCoordinateSystem();


        /* Save and load models */
        bool saveBinary( std::string filename );

        bool loadBinary( std::string filename );

        bool saveJSON( std::string filename );

        bool loadJSON( std::string filename );

        ///////////////////////////////////////////////////////////////////////
        /* From SUtilities */
        ///////////////////////////////////////////////////////////////////////
        

        bool getNormalList( std::size_t surface_id, std::vector<double> &normals );
        bool getExtrusionPath( std::size_t surface_id, std::vector<double> &path_vertices );
        bool computeTetrahedralMeshVolumes( std::vector<double> &vlist );

        bool getVolumeAttributesFromPointList( const std::vector<double> &vcoords, std::vector<int> &regions);

        bool getBoundingSurfacesFromVolumeAttribute( std::size_t attribute_id, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound);
        std::vector<size_t> getSurfacesIndicesBelowPoint( double x, double y, double z );
        std::vector<size_t> getSurfacesIndicesAbovePoint( double x, double y, double z );

    private:
        std::unique_ptr<SModellerImplementation> pimpl_;

        SModellerImplementation& Impl();
        const SModellerImplementation& Impl() const;

        friend class SUtilities;
};

#endif
