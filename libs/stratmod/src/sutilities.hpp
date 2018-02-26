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

#include "win_dll_export_handler.hpp"

#include "smodeller.hpp"



class STRATMODLIB_DLL_HANDLER SUtilities
{
    public:
        SUtilities( SModeller &m );

        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps, size_t active_cross_section = 0 );

        size_t getNumVerticesWidth();
        size_t getNumVerticesDepth();

        bool getExtrusionPath( std::size_t surface_id, std::vector<double> &path_vertices );

        bool getNormalList( std::size_t surface_id, std::vector<double> &normals );

        bool getFrontBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getBackBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getLeftBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        bool getRightBoundary3DCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist);
        
        bool exportToTetgen( std::string filename );
        bool exportToVTK( std::string filename );

        bool getTetrahedralMeshRegions( const std::vector<double> &vcoords, const std::vector<size_t> &elements, std::vector<int> &regions);

        bool liesBetweenBoundarySurfaces(double x, double y, double z);

        std::vector<size_t> getSurfacesIndicesBelowPoint(double x, double y, double z);
        std::vector<size_t> getSurfacesIndicesAbovePoint(double x, double y, double z);

    private:
        SModeller &model_;

        bool getWidthCrossSectionCurveVertices( std::size_t surface_id,  std::size_t width, std::vector< std::vector<double> > vertices_coordinates );
        bool getLengthCrossSectionCurveVertices( std::size_t surface_id, std::size_t lenght, std::vector< std::vector<double> > vertices_coordinates );

        bool getWidthCrossSectionCurve( size_t surface_id, size_t width, std::vector<double> &vlist, std::vector<size_t> &elist );
        bool getLengthCrossSectionCurve( size_t surface_id, size_t lenght, std::vector<double> &vlist, std::vector<size_t> &elist );

        bool computeLateralBoundingCurves( std::vector<std::vector<double>> &vlist, std::vector<std::vector<size_t>> &elist );

};

#endif

