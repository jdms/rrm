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

#include "smodeller_primitives.hpp"

class STRATMODLIB_DLL_HANDLER SModeller
{
    public:

        /** 
         * \brief Default construction.  
         * 
         * It's declararion is required because of the 
         * std::unique_ptr in the implementation.
         *
         **/
        SModeller(); 

        /**
         * \brief Default destructor.
         *
         **/
        ~SModeller(); 

        SModeller( const SModeller &m ) = delete;
        SModeller( SModeller &&m ); 

        SModeller& operator=( const SModeller &m ) = delete;
        SModeller& operator=( SModeller &&m ); 


        // TODO: check whether name is correct
        size_t getWidthDiscretization() const;

        // TODO: check whether name is correct
        size_t getLengthDiscretization() const;

        bool tryChangeDiscretization( size_t width_discretization = 64, size_t depth_discretization = 64 );

        void setOrigin( double x, double y, double z );

        // TODO: change lenght to size
        bool setSize( double x, double y, double z );

        void getOrigin( double &x, double &y, double &z );

        // TODO: change lenght to size
        void getSize( double &x, double &y, double &z );

        void clear();

        /* Query or modify the automatum state */

        //
        // brief:
        // Verifies whether ir is possible to define a new 'drawing' region,
        // either above or below.
        // Argument: `eligible_surfaces` stores the indices of surfaces that can
        // be used as input either for an `defineAbove()` or a `defineBelow()`.
        // Return: true if at least one elegible surface was found.
        //
        bool requestCreateAbove( std::vector<size_t> &eligible_surfaces );

        bool requestCreateBelow( std::vector<size_t> &eligible_surfaces );

        //
        // brief:
        // Define new input region above surface which index is `surface_index`.
        //
        bool createAbove( size_t surface_index );


        //
        // brief:
        // Clear any previous `defineAbove()` call.
        // Safe to call anytime.
        //
        void stopCreateAbove();

        //
        // brief:
        // Define new input region below surface which index is `surface_index`.
        //
        bool createBelow( size_t surface_index );

        //
        // brief:
        // Clear any previous `defineBelow()` call.
        // Safe to call anytime.
        //
        void stopCreateBelow();

        bool createAboveIsActive( size_t &boundary_index );
        bool createBelowIsActive( size_t &boundary_index );

        void disableGeologicRules();

        void removeAbove();
        void removeAboveIntersection();

        void removeBelow();
        void removeBelowIntersection();


        bool canUndo();
        bool undo();

        bool canRedo();
        bool redo();


        // Input & output 
        //

        std::vector<size_t> getSurfacesIndices();
        
        /* bool createSurface( size_t surface_id, const std::vector<double> &point_data ); */

        bool createSurface( size_t surface_id, 
                const std::vector<double> &point_data,
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(), 
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>() 
                );

        /* bool createExtrudedSurface( size_t surface_id, const std::vector<double> &point_data ); */

        // TODO: createExtrudedSurfaceAlongLenght ?
        // TODO: createLenghtwiseExtrudedSurface ?
        bool createLengthwiseExtrudedSurface( size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data,
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(),
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>()
                );

        bool createLengthwiseExtrudedSurface( size_t surface_id, 
                const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
                const std::vector<double> &path_curve_point_data, 
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(),
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>()
                );

        /* bool tryCreateSurface( size_t surface_id, const std::vector<double> &point_data, std::vector<size_t> &intersected_surfaces ); */

        bool tryCreateSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
                const std::vector<double> &point_data,
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(), 
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>() 
                );

        /* bool tryCreateExtrudedSurface( size_t surface_id, const std::vector<double> &point_data, std::vector<size_t> &intersected_surfaces ); */

        bool tryCreateLengthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data,
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(), 
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>()
                );

        bool tryCreateLengthwiseExtrudedSurface( size_t surface_id, std::vector<size_t> &intersected_surfaces,
                const std::vector<double> &cross_section_curve_point_data, double cross_section_depth, 
                const std::vector<double> &path_curve_point_data, 
                const std::vector<size_t> lower_bound_ids = std::vector<size_t>(),
                const std::vector<size_t> upper_bound_ids = std::vector<size_t>()
                );

        /* Get mesh, pcl and curves for visualization */
        /* For all combinations of int32_t, int64_t, float, and double */

        bool getVertexList( size_t surface_id, std::vector<float> &vlist );
        bool getVertexList( size_t surface_id, std::vector<double> &vlist );

        bool getMesh( size_t surface_id, std::vector<float> &vlist, std::vector<size_t> &flist );
        bool getMesh( size_t surface_id, std::vector<double> &vlist, std::vector<size_t> &flist );

        
        bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<size_t> &elist );
        bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist );

        // TODO: change Depth to Lenght
        bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<float> &vlist, std::vector<size_t> &elist );
        bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<double> &vlist, std::vector<size_t> &elist );

        std::size_t getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::vector<std::size_t>> &element_list );


        /* Change the model's properties */
        bool useDefaultCoordinateSystem();

        bool useOpenGLCoordinateSystem();


        /* Save and load models */
        bool saveBinary( std::string filename );

        bool loadBinary( std::string filename );


    protected:
        bool changeDiscretization( size_t width_discretization = 64, size_t depth_discretization = 64 );

    private:
        std::unique_ptr<SModellerImplementation> pimpl_;

        friend class SUtilities;
};

#endif

