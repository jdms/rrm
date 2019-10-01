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



#ifndef __S_UTILITIES__
#define __S_UTILITIES__



#include <iostream>
#include <vector>
#include <memory>

#include "stratmod/smodeller.hpp"



class STRATMODLIB_DLL_HANDLER SUtilities
{
    public:
        SUtilities( SModeller &m );

        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps, size_t active_cross_section = 0 );

        size_t getNumVerticesWidth();
        size_t getNumVerticesDepth();

        bool getVertexList( std::size_t surface_id, std::vector<float> &vlist );
        bool getVertexList( std::size_t surface_id, std::vector<double> &vlist );

        std::size_t getUniqueFacesList( std::size_t surface_id, std::vector<std::size_t> &flist );

        // Duplicated in SModeller
        bool getExtrusionPath( std::size_t surface_id, std::vector<double> &path_vertices );

        // Duplicated in SModeller
        bool getNormalList( std::size_t surface_id, std::vector<double> &normals );

        // Duplicated in SModeller
        bool getRegionVolumeList( std::vector<double> &vlist );

        bool getIntersectingSurfaceIndices( size_t surface_id, std::vector<size_t> &intersecting_surfaces_indices );

        bool getAdaptedWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<float> &vlist, std::vector<size_t> &elist );

        bool getAdaptedWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist );

        bool getAdaptedLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<float> &vlist, std::vector<size_t> &elist );

        bool getAdaptedLengthCrossSectionCurve( size_t surface_id, size_t length, std::vector<double> &vlist, std::vector<size_t> &elist );

        bool getFrontBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getBackBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getLeftBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getRightBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        
        bool exportToTetgen( std::string filename );
        bool exportToVTK( std::string filename );

        // Duplicated in SModeller
        bool getBoundingSurfacesFromRegionID( std::size_t region_id, std::vector<size_t> &lower_bound, std::vector<size_t> &upper_bound);

        // Improved in SModeller
        bool getTetrahedralMeshRegions( const std::vector<double> &vcoords, const std::vector<size_t> &elements, std::vector<int> &regions);

        std::size_t getTetrahedralMesh( std::vector<double> &vertex_coordinates, std::vector<std::vector<std::size_t>> &element_list );

        bool liesBetweenBoundarySurfaces(double x, double y, double z);

        // Duplicated in SModeller
        std::vector<size_t> getSurfacesIndicesBelowPoint(double x, double y, double z);
        // Duplicated in SModeller
        std::vector<size_t> getSurfacesIndicesAbovePoint(double x, double y, double z);

        bool getQuadMesh( std::size_t surface_id, std::vector<double> &points, std::vector<bool> &valid_points, std::size_t &num_width, std::size_t &num_length );

    private:
        SModeller &model_;

        bool getWidthCrossSectionCurveVertices( std::size_t surface_id,  std::size_t width, std::vector< std::vector<double> > vertices_coordinates );
        bool getLengthCrossSectionCurveVertices( std::size_t surface_id, std::size_t lenght, std::vector< std::vector<double> > vertices_coordinates );

        bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist );
        bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<double> &vlist, std::vector<size_t> &elist );

        bool computeLateralBoundingCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist );

};

#endif

