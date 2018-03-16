/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLOWVISUALIZATIONCONTROLLER_H
#define FLOWVISUALIZATIONCONTROLLER_H

//#include "Model/CrossSection.hpp"

// OpenVolume Mesh
#include "core/models/openvolumemesh_defines.hpp"

// Flow Model
#include "flow_model/flow_model.hpp"

#include <vector>
#include <random>
#include <set>
#include <map>

#include <QWidget>
#include <QColor>
#include <QFileDialog>

#include "mesh.h"
#include "flow_diagnostics_interface.hpp"
#include "colormap.h"
#include <Eigen/Dense>

class FlowVisualizationController: public QWidget
{
    Q_OBJECT

    public:


        struct Color
        {
            int r = 255;
            int g = 0;
            int b = 0;
        };


        enum class  MESHING_METHOD{ CORNERPOINT, UNSTRUCTURED };
        enum class SaturationMethod{ PERREGION, APIGRAVITY };

        using TriangleMesh = FlowDiagnosticsInterface::TriangleMesh;
        using CurveMesh = FlowDiagnosticsInterface::CurveMesh;


        FlowVisualizationController( QWidget* parent = 0 );

        void readSkeletonFiles( const std::string& filename );
        void setSkeletonData( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
        void setSkeleton( const std::vector< TriangleMesh>& triangles_meshes, const std::vector< CurveMesh>& left_curves, const std::vector< CurveMesh >& right_curves,
                          const std::vector< CurveMesh> & front_curves, const std::vector< CurveMesh >& back_curves );


        void buildSurfaceSkeleton( std::vector< double >& positions, std::vector< unsigned int >& faces );

        void readInputParameters( const std::string& input_file );

        void generateCornerPoint();
        void generateUnstructured();

        void updateCornerPoint(std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces);
        void updateVolumetricMesh(
                std::vector< float >& raw_vertices, 
                std::vector<float>& normalized_vertices, 
                std::vector< unsigned int >& edges, 
                std::vector< unsigned int >& raw_faces,
                std::vector< unsigned int >& modified_faces
                );
        void getSurfacesFromCrossSection();

        void computeFlowProperties();

        void setPropertyArea(const int np,
                             const std::vector< double >& values,                         
                             const std::vector<double>& _saturations,                           
                             const std::map<int, std::pair<double, double> >& _permeability_gradients,
                             const std::map<int, std::pair<double, double> >& _porosity_gradiets);

        void getPropertyArea( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc );


        void getOilInPlace( std::string& _result);



        void setWellsValues(const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign, const std::vector<Eigen::Vector4d>& positions, std::vector< Eigen::Vector2d>& range);
        void getWellsValues(int& nw, std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign);


        /// @FIXEME September
        void setFluidProperty(double _viscosity, double _bo, const double& _oildensity, const std::pair<int, int>& _phase_method);


        bool isVolumetricBuilt() const;
        bool isUserInputOk() const;
        bool isSurfaceLoaded() const;
        bool arePropertiesComputed() const;

        void setVolumeDimensions(double width_, double height_, double depth_);

        void setCurrentColormap(ColorMap::COLORMAP cm);
        ColorMap::COLORMAP getCurrentColormap() const;

        void setSaturationMethod( const SaturationMethod& option );

        void clear();

        /// OpenVolumeMesh Interface	 ----- ---------------------------------------->>
        void loadPropertiesTetrahedron();
        void loadPropertiesHexahedron();

        void updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector<float>& _colors, double& _min, double& _max);
        void updateHexahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector<float>& _colors, double& _min, double& _max);

        std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> getPtrHexahedralMesh();
        std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> getPtrTetrahedralMesh();

        /// @TODO Later, move this fucntions to FlowvizualizationController
        Eigen::Affine3d FlowVisualizationController::getModelMatrix() const;

        void setSinglePhase(  );
		void setMultiPhase( SaturationMethod _method);

        void updateTetrahedonRegions( const std::vector< int >& regions_ , std::vector<double> &values_for_visualization_ );

    public slots:


        inline void setCurrentMethod( const FlowVisualizationController::MESHING_METHOD& t ){ current_method = t; }
        inline  FlowVisualizationController::MESHING_METHOD setCurrentMethod(){ return current_method; }

        /// @TODO June 2017
        void exportDerivedQuantities();
        void clearComputedQuantities();
        void getUpscalledPermeability(std::string& _result);
        void getPoreVolume();

        void exportSurfacetoVTK();
        void exportVolumetoVTK();
        void exportCornerPointtoVTK();
        void exportCornerPointtoGRDECL();
		void exportResultstoVTK();
		/// January 2018
		void exportMeshtoMSH();
		



    signals:

        void showToolbar( bool );

        void clearPropertiesMenu();
        void clearAll();

        /// Enable Mesh Build / Disable other
        void surfaceLoaded();
        /// Enable Compute Properties and Pores Volume / Disable other
        void unstructuredMeshBuilt();
        /// Enable Compute Properties / Disable other
        void cornerPointMeshBuilt();

        void propertiesComputed();





    private:

//        ProgressCounter counter;

        bool is_surface_loaded;
        bool is_volumetric_built;
        bool user_input_ok;
        bool are_properties_computed;

        MESHING_METHOD current_method;
        FlowDiagnosticsInterface code_interface;

        ColorMap colormap;
        ColorMap::COLORMAP current_colormap_;

        RRM::FlowModel flow_model_;

};


#endif // FLOWVISUALIZATIONCONTROLLER_H


