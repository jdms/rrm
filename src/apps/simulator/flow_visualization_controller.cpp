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

#include "flow_visualization_controller.h"

FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    is_surface_loaded = false;
    is_volumetric_built = false;
    are_properties_computed = false;
    user_input_ok = false;

    current_method = MESHING_METHOD::UNSTRUCTURED;
    current_colormap_ = ColorMap::COLORMAP::COOL_TO_WARM;

    //code_interface.loadDefaultValues(1);
}

void FlowVisualizationController::readSkeletonFiles( const std::string& filename )
{
    if( is_surface_loaded == true ) return;
    code_interface.readSkeletonFile( filename );
}

void FlowVisualizationController::buildSurfaceSkeleton( std::vector< double >& positions, std::vector< unsigned int >& faces )
{
    unsigned int surfaces_number = 0;
    std::vector< unsigned int > nu_list;
    std::vector< unsigned int > nv_list;

    code_interface.getSurfaceSkeleton( surfaces_number, nu_list, nv_list, positions );

    unsigned int number_lines_ = nu_list.size();
    int offset = 0;

    for( unsigned int k = 0; k < number_lines_; ++k  )
    {

        int nu_number = nu_list[ k ];
        int nv_number = nv_list[ k ];


        for(   int j = 0; j < nv_number - 1; ++j )
        {
            for(  int i = 0; i < nu_number - 1 ; ++i )
            {

                unsigned int id0 = j*nu_number + i + offset;
                unsigned int id1 = j*nu_number + ( i + 1 ) + offset;
                unsigned int id2 = ( j + 1 )*nu_number + ( i + 1 ) + offset;
                unsigned int id3 = ( j + 1 )*nu_number + i  + offset;

                faces.push_back( id0 );
                faces.push_back( id1 );
                faces.push_back( id2 );
                faces.push_back( id3 );


            }
        }
        offset += nu_number*nv_number;

    }

    is_surface_loaded = true;
}

void FlowVisualizationController::readInputParameters( const std::string& input_file )
{
    //code_interface.readUserInput( input_file );//no user input funtion needed
    user_input_ok = true;
}

void FlowVisualizationController::generateCornerPoint()
{
    if (is_surface_loaded == false || user_input_ok == false)
    {
        return;
    }

    code_interface.buildCPGVolumetricMesh();
    is_volumetric_built = true;
}

void FlowVisualizationController::generateUnstructured()
{
    //if (is_surface_loaded == false || user_input_ok == false)
    //{
    //    return;
    //}

    code_interface.buildVolumetricMesh();
    is_volumetric_built = true;
}

void FlowVisualizationController::updateCornerPoint(std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces)
{
     code_interface.getCPGVolumeVertices( vertices );
     code_interface.getCPGVolumeEdges( edges );
     code_interface.getCPGVolumeCells( faces);
     flow_model_.createHexahedonMesh(vertices, faces);

     flow_model_.uploadHexahedron(faces, vertices);
     //ptr_mesh = std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d>(flow_model_.getPtrHexahedralMesh());

     is_volumetric_built = true;
}

void FlowVisualizationController::updateVolumetricMesh(std::vector< float >& raw_vertices, std::vector<float>& normalized_vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& raw_faces, std::vector< unsigned int >& modified_faces)
{


    code_interface.getVolumeVertices( raw_vertices );
    normalized_vertices = raw_vertices;

    code_interface.getVolumeEdges( edges );
    code_interface.getVolumeCells( raw_faces );
    modified_faces = raw_faces;

    std::cout << " FlowVisualizationController vertices " << normalized_vertices.size() << std::endl;

    flow_model_.createTetrahedonMesh(normalized_vertices, modified_faces);

    flow_model_.uploadTetrahedron(modified_faces, normalized_vertices);
    //ptr_mesh = std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d>(flow_model_.getPtrTetrahedralMesh());

    is_volumetric_built = true;

}

void FlowVisualizationController::setSkeletonData( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{

    std::vector< unsigned int> nu_uint( nu.begin(), nu.end() );
    std::vector< unsigned int> nv_uint( nv.begin(), nv.end() );

    unsigned int surfaces_number = ( unsigned int) nu.size();


    code_interface.setSkeletonData( surfaces_number, nu_uint, nv_uint, points );
}




void FlowVisualizationController::setSkeleton( const std::vector< TriangleMesh>& triangles_meshes, const std::vector< CurveMesh>& left_curves, const std::vector< CurveMesh >& right_curves,
                                               const std::vector< CurveMesh> & front_curves, const std::vector< CurveMesh >& back_curves )
{

    code_interface.setSkeleton( triangles_meshes, left_curves, right_curves, front_curves, back_curves );
    is_surface_loaded = true;
}


bool FlowVisualizationController::isVolumetricBuilt() const
{
    return this->is_volumetric_built;
}

bool FlowVisualizationController::isUserInputOk() const
{
    return this->user_input_ok;
}

bool FlowVisualizationController::isSurfaceLoaded() const
{
    return this->is_surface_loaded;
}

bool FlowVisualizationController::arePropertiesComputed() const
{
    return are_properties_computed;
}

void FlowVisualizationController::setVolumeDimensions(double width_, double height_, double depth_)
{
    code_interface.setVolumeDimensions(width_, height_, depth_);
}

void FlowVisualizationController::computeFlowProperties()
{

    //if (is_volumetric_built == false || is_surface_loaded == false)
    //{
    //    return;
    //}

    code_interface.init();
    code_interface.computeProperties();

    emit clearPropertiesMenu();

    are_properties_computed = true;

}

void FlowVisualizationController::setPropertyArea(const int np,
                                                  const std::vector< double >& values,
                                                  const std::vector<double>& _saturations,
                                                  const std::map<int,std::pair<double, double> >& _permeability_gradients,
                                                  const std::map<int,std::pair<double, double> >& _porosity_gradients)
{
    code_interface.setNumberofRegions(np);
    for( int i = 0; i < np; ++i )
    {
        /// @FIXME September
        
        code_interface.setRegion(i,
                                values[3 * i],
                                values[3 * i + 1],
                                values[3 * i + 2],
                                _permeability_gradients.at(i).first,
                                _permeability_gradients.at(i).second,
                                _porosity_gradients.at(i).first,
                                _porosity_gradients.at(i).second
                                );

        code_interface.setWaterSaturation(i,_saturations[i]);
        
    }

    user_input_ok = true;

}


void FlowVisualizationController::getPropertyArea( int& np, std::vector< double >& values , std::vector< double >& perm, std::vector< double >& poros, std::vector< double >& visc )
{
    np = code_interface.getNumberofRegions();

    values.resize( 3*np );
    perm.resize( np );
    poros.resize( np );
    visc.resize( np );
    std::vector< double > porevolume;
    porevolume.resize( np );

    for( int i = 0; i < np; ++i )
    {
        code_interface.getRegion( i, values[ 3*i ], values[ 3*i + 1 ], values[ 3*i +2 ], perm[ i ], poros[ i ], visc[ i ], porevolume[ i ] );
    }

}



/// @FIXME September
void FlowVisualizationController::setFluidProperty(double _viscosity, double _bo, const double& _oildensity, const std::pair<int, int>& _phase_method)
{
    // Tells the FlowDiagnosticsInterface whether we want
    // single-phase  := (_phase_method.first == 1), or
    // multi-phase   := (_phase_method.first == 2)
    // flow diagnostics
    this->code_interface.setNumberOfPhases( _phase_method.first );

    this->code_interface.setViscosity(_viscosity);
    this->code_interface.setBo(_bo);

    // Set water saturation by region  := (_phase_method.second == 1)
    // Set water saturation by gravity := (_phase_method.second == 2)
    // This should only be set if "multi-phase" diagnostics was selected
    this->code_interface.setSaturationMethod( _phase_method.second );

    // This should only be set if "multi-phase" diagnostics was selected
    this->code_interface.setOilGravity(_oildensity);


	std::cout << "------" << std::endl;

	std::cout << " viscosity " << _viscosity << std::endl;
	std::cout << " bo     " << _bo << std::endl;
	std::cout << " oildensity   " << _oildensity << std::endl;
	std::cout << " phase  " << code_interface.getNumberOfPhases() << std::endl;
    
}

/* /1* Suggestion *1/ */

using WellType = FlowDiagnosticsInterface::WellType;

/* struct WellDescriptor { */
/*     double qt_x; */
/*     double qt_y; */
/*     double well_depth; */
/* }; */

/* void FlowVisualizationController::setWellsValues( const int nw,  const std::vector<std::string>& type, const std::vector< double >& pressure_value, const std::vector<WellDescriptor>&descriptors  ) */

void FlowVisualizationController::setWellsValues(const int nw, const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign, const std::vector<Eigen::Vector4d>& positions, std::vector< Eigen::Vector2d>& range)
{
    code_interface.setNumberofWells(nw);
    for( int i = 0; i < nw; ++i )
    {
        std::cout << "setWellsValues " << positions[i];
        std::cout << " ------ " << positions[i];
        std::cout << "Top " << range[i].y();
        std::cout << "Bottom " << range[i].x();


        /* Old */
        /* code_interface.setWell( i, type[ i ], value[ i ], sign[ i ] ); */

        /* New (suggestion) */
        /* code_interface.setWell( i, type[ i ], value[ i ],
        *       descriptors[i].qt_x, descriptors[ i ].qt_y, descriptors[ i ].well_depth ); */

        /* Before the complete integration */
        if ( sign[ i ] == 1 ) // means this well is an injector
        {
            //code_interface.setWell(i, WellType::INJECTOR, value[i], positions[i].x(), positions[i].y(), positions[i].z(), positions[i].w());
            code_interface.setVerticalWell(i, WellType::INJECTOR, value[i], positions[i].x(), positions[i].y(), range[i].y(), range[i].x());
        }
        else if ( sign[ i ]== -1 )
        {
            //code_interface.setWell(i, WellType::PRODUCER, value[i], positions[i].x(), positions[i].y(), positions[i].z(), positions[i].w());
            code_interface.setVerticalWell(i, WellType::PRODUCER, value[i], positions[i].x(), positions[i].y(), range[i].y(), range[i].x());
        }
        else
        {
            std::cerr << "You shall not reach this point!\n\n";
        }

    }

}

void FlowVisualizationController::getWellsValues( int& nw,  std::vector< unsigned int >& type, std::vector< double >& value, std::vector< int >& sign )
{

    nw = code_interface.getNumberofWells();
    type.resize( nw );
    value.resize( nw );
    sign.resize( nw );

    for( int i = 0; i < nw; ++i )
    {
        code_interface.getWell( i, type[ i ], value[ i ], sign[ i ] );
    }

}
/// Exporters

/// @TODO June 2017

void FlowVisualizationController::clearComputedQuantities()
{
    code_interface.clearComputedQuantities();
};

void FlowVisualizationController::getUpscalledPermeability( std::string& _result)
{
    //std::string up;
    code_interface.getUpscalledPermeability(_result);

};

void FlowVisualizationController::getOilInPlace( std::string& _result)
{
    //std::string up;
    double value  = 0.0;
	int integer_value = 0;
    code_interface.getOilInPlace( value );
	/// FIXME January 2018
	integer_value = static_cast<int>(value);
    _result.clear();
	_result = std::to_string(integer_value);

};

void FlowVisualizationController::exportDerivedQuantities()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName(this, tr("Export File"), "./exported/Derived Quantities/",
        ".txt files (*.txt)", &selected_format);
    if (filename.isEmpty() == true) return;

    code_interface.exportDerivedQuantities(filename.toStdString());
}

void FlowVisualizationController::exportSurfacetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/surfaces/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    code_interface.exportSurfacetoVTK( filename.toStdString() );
}

void FlowVisualizationController::exportVolumetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/unstructured/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportSurfacetoVTK( filename.toStdString() );
}

void FlowVisualizationController::exportCornerPointtoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/corner-point/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportCornerPointtoVTK( filename.toStdString() );
}

void FlowVisualizationController::exportCornerPointtoGRDECL()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/corner-point/",
                                                     ".GRDECL files (*.grdecl)", &selected_format );
    if( filename.isEmpty() == true ) return;

     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}

void FlowVisualizationController::exportResultstoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/results/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportResultstoVTK( filename.toStdString() );
}

void FlowVisualizationController::exportMeshtoMSH()
{

	QString selected_format = "";
	QString filename = QFileDialog::getSaveFileName(this, tr("Export File"), "./exported/results/",
		".Mesh files (*.msh)", &selected_format);
	if (filename.isEmpty() == true) return;


	code_interface.exportMeshtoMSH(filename.toStdString());
}

void FlowVisualizationController::clear()
{
    //is_surface_loaded = false;
    //user_input_ok = false;
    //is_volumetric_built = false;
    //are_properties_computed = false;
    code_interface.clear();
}

void FlowVisualizationController::setCurrentColormap(ColorMap::COLORMAP cm)
{
    current_colormap_ = cm;
}

ColorMap::COLORMAP FlowVisualizationController::getCurrentColormap() const
{
    return current_colormap_;
}

/// OpenVolumeMesh Interface	 ----- ---------------------------------------->>
/// Here, doesnt matter which type of mesh we are dealing with. The Topology of a cells is irrelevant
void FlowVisualizationController::loadPropertiesTetrahedron()
{

    std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();

    if (ptr_mesh)
    {

        // Vertex Properties ------------------------------------------------->
        std::vector<double> values;
        bool status = false;
        // Pressure
        values.clear();
        int i = 0;
        code_interface.getPressure(values);

        OpenVolumeMesh::VertexPropertyT<double> ph = ptr_mesh->request_vertex_property<double>("Pressure (bar)");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        // Foward TOF
        values.clear();
        i = 0;
        code_interface.getForwardTOF(values);

        ph = ptr_mesh->request_vertex_property<double>("TOF from Injectors (day)");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        // Foward TOF Log 10
        values.clear();
        i = 0;
        code_interface.getFowardTOF_log10(values);

        ph = ptr_mesh->request_vertex_property<double>("Log10 TOF from Injectors (Log10(day)) ");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        // BackWard TOF
        values.clear();
        i = 0;
        code_interface.getBackwardTOF(values);

        ph = ptr_mesh->request_vertex_property<double>("TOF to Producers (day)");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        // BackWard TOF Log 10
        values.clear();
        i = 0;
        code_interface.getBackwardTOF_log10(values);

        ph = ptr_mesh->request_vertex_property<double>("Log10 TOF to Producers (Log10(day))");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }


        // Total TOF
        values.clear();
        i = 0;
        code_interface.getTotalTOF(values);

        ph = ptr_mesh->request_vertex_property<double>("Total TOF (day)");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        // Total TOF Log 10
        values.clear();
        i = 0;
        code_interface.getTotalTOF_log10(values);

        ph = ptr_mesh->request_vertex_property<double>("Log10 Total TOF (Log10(day))");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }


        //Maximum Tracer
        values.clear();
        i = 0;
        code_interface.getMaxForwardTracer(values);

        ph = ptr_mesh->request_vertex_property<double>("Injectors Tracer Partitioning");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }

        //Max BackTracer
        values.clear();
        i = 0;
        code_interface.getMaxBackwardTracer(values);

        ph = ptr_mesh->request_vertex_property<double>("Producers Tracer Partitioning");
        ptr_mesh->set_persistent(ph);

        for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        {
            // Set property value
            ph[*v_it] = values[i++];
        }


        //Capillary Pressure
        values.clear();
        i = 0;
        status = code_interface.getCapillaryPressure(values);

        if ( status)
        {
            ph = ptr_mesh->request_vertex_property<double>("Capillary Pressure");
            ptr_mesh->set_persistent(ph);

            for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
            {
                // Set property value
                ph[*v_it] = values[i++];
            }
        }

        // Cell Properties --------------------------------------------------->

        //Permeability
        values.clear();
        i = 0;
        code_interface.getPermeabilitybyCells(values);

        OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Permeability (mD)");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
        //	// Set property value
            ch[*c_it] = values[i++];

			
        }
		/// Stilll getting the same values
		std::cout << "getPermeabilitybyCells :" << values[i-2] << std::endl;

        //Porosity
        values.clear();
        i = 0;
        code_interface.getPorosity(values);

        ch = ptr_mesh->request_cell_property<double>("Porosity");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

        //Water Saturation
        values.clear();
        i = 0;
        status = code_interface.getWaterSaturationByCells(values);

        if (status)
        {
            ch = ptr_mesh->request_cell_property<double>("Water Saturation");
            ptr_mesh->set_persistent(ch);

            for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
            {
                // Set property value
                ch[*c_it] = values[i++];
            }
        }

        //Velocity
        values.clear();
        i = 0;
        code_interface.getVelocitybyCells(values);

        OpenVolumeMesh::CellPropertyT<double> chv = ptr_mesh->request_cell_property<double>("Velocity (m/s)");
        ptr_mesh->set_persistent(chv);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            OpenVolumeMesh::Vector3D v (values[3 * i + 0], values[3 * i + 1], values[3 * i + 2]);

            // Set property value
            chv[*c_it] = v.norm();

            i++;
        }


        //Velocity Log 10
        values.clear();
        i = 0;
        code_interface.getVelocityMagnitudebyCells_log10(values);

        chv = ptr_mesh->request_cell_property<double>("Velocity Log10 (log10(m/s))");
        ptr_mesh->set_persistent(chv);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {

            // Set property value
            chv[*c_it] = values[i]; //here debug error
            i++;
        }


        ////@TODO September
        //Water Saturation
        values.clear();
        i = 0;
        status = code_interface.getWaterSaturationByCells(values);

        if (status)
        {
            chv = ptr_mesh->request_cell_property<double>("Water Saturation");
            ptr_mesh->set_persistent(chv);

            for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
            {

                // Set property value
                chv[*c_it] = values[i];
                i++;
            }
        }

        //Oil Relative permeability
        values.clear();
        i = 0;
        status = code_interface.getKroByCells(values);

        if (status)
        {
            chv = ptr_mesh->request_cell_property<double>("Oil Relative Permeability");
            ptr_mesh->set_persistent(chv);

            for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
            {

                // Set property value
                chv[*c_it] = values[i];
                i++;
            }
        }

        //Water Relative Permeability
        values.clear();
        i = 0;
        status = code_interface.getKrwByCells(values);

        if (status)
        {
            chv = ptr_mesh->request_cell_property<double>("Water Relative Permeability");
            ptr_mesh->set_persistent(chv);

            for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
            {

                // Set property value
                chv[*c_it] = values[i];
                i++;
            }
        }

    }

}

/// FIXEME June 2017
void FlowVisualizationController::getPoreVolume()
{

    std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();

    if (ptr_mesh)
    {
        //Pore Volume
        std::vector<double> values;
        values.clear();

        code_interface.getPoreVolumebyRegion(values);
        std::vector< int > idregion_by_cell;
        code_interface.getRegionId(idregion_by_cell);

        OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Pore Volume");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            //	// Set property value
            ch[*c_it] = values[idregion_by_cell[*c_it]];
        }
    }

}


/// Here, doesnt matter which type of mesh we are dealing with. The Topology of a cells is irrelevant
void FlowVisualizationController::loadPropertiesHexahedron()
{

    std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> ptr_mesh = flow_model_.getPtrHexahedralMesh();

    if (ptr_mesh)
    {
        // Vertex Properties ------------------------------------------------->
        std::vector<double> values;
        int i = 0;

        //// Pressure
        //values.clear();
        //int i = 0;
        //code_interface.getCPGPressure(values);

        //OpenVolumeMesh::VertexPropertyT<double> ph = ptr_mesh->request_vertex_property<double>("Pressure");
        //ptr_mesh->set_persistent(ph);

        //for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        //{
        //	// Set property value
        //	ph[*v_it] = values[i++];
        //}


        ////Fake Tracer, testing another dimension types
        //values.clear();
        //i = 0;
        //code_interface.getMaxForwardTracer(values);

        //OpenVolumeMesh::VertexPropertyT<OpenVolumeMesh::Matrix3D> pe = ptr_mesh->request_vertex_property<OpenVolumeMesh::Matrix3D>("Fake Tracer");
        //ptr_mesh->set_persistent(pe);

        //for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
        //{

        //	// Set property value
        //	pe[*v_it] = OpenVolumeMesh::Matrix3D();
        //}

        // Cell Properties --------------------------------------------------->

        //Pressure
        values.clear();
        i = 0;
        code_interface.getCPGPressure(values);

        OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Pressure");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            //	// Set property value
            ch[*c_it] = values[i++];
        }

        //Forward TOF
        values.clear();
        i = 0;
        code_interface.getCPGTOF(values);

        ch = ptr_mesh->request_cell_property<double>("Forward TOF");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

        //Backward TOF
        values.clear();
        i = 0;
        code_interface.getCPGBackwardTOF(values);

        ch = ptr_mesh->request_cell_property<double>("Backward Tracer");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

        //Total TOF
        values.clear();
        i = 0;
        code_interface.getCPGTotalTOF(values);

        ch = ptr_mesh->request_cell_property<double>("Total TOF");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

        //Total Permeability
        values.clear();
        i = 0;
        code_interface.getCPGPermeability(values);

        ch = ptr_mesh->request_cell_property<double>("Permeability");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

        //Total Porosity
        values.clear();
        i = 0;
        code_interface.getCPGPorosity(values);

        ch = ptr_mesh->request_cell_property<double>("Porosity");
        ptr_mesh->set_persistent(ch);

        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = values[i++];
        }

    }

}

void FlowVisualizationController::updateTetrahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector<float>& _colors, double& _min, double& _max)
{

    std::vector<double> values;

    flow_model_.updateTetrahedronColors(_property_name, _entity_name, _dimension, values);

    // Get the property by the interator
    auto min_max = std::minmax_element(values.begin(), values.end());

    std::cout << "min element at: " << (min_max.first - values.begin()) << " " << *min_max.first << std::endl;
    std::cout << "max element at: " << (min_max.second - values.begin()) << " " << *min_max.second << std::endl;

    _colors.clear();
    _colors.resize(values.size() * 3);

    _min = *min_max.first;
    _max = *min_max.second;

    for (unsigned int i = 0; i < values.size(); ++i)
    {
        QVector3D c = colormap.getColor(current_colormap_, values[i], *min_max.first, *min_max.second);
        _colors[i * 3 + 0] = static_cast<float>(c[0]);
        _colors[i * 3 + 1] = static_cast<float>(c[1]);
        _colors[i * 3 + 2] = static_cast<float>(c[2]);
    }


}

void FlowVisualizationController::updateHexahedronColors(const std::string& _property_name, const std::string& _entity_name, const std::string& _dimension, std::vector<float>& _colors, double& _min, double& _max)
{

    std::vector<double> values;

    flow_model_.updateHexahedronColors(_property_name, _entity_name, _dimension, values);

    // Get the property by the interator
    auto min_max = std::minmax_element(values.begin(), values.end());

    std::cout << "min element at: " << (min_max.first - values.begin()) << " " << *min_max.first << std::endl;
    std::cout << "max element at: " << (min_max.second - values.begin()) << " " << *min_max.second << std::endl;

    _colors.clear();
    _colors.resize(values.size() * 3);

    _min = *min_max.first;
    _max = *min_max.second;

    for (unsigned int i = 0; i < values.size(); ++i)
    {
        QVector3D c = colormap.getColor(current_colormap_, values[i], *min_max.first, *min_max.second);
        _colors[i * 3 + 0] = static_cast<float>(c[0]);
        _colors[i * 3 + 1] = static_cast<float>(c[1]);
        _colors[i * 3 + 2] = static_cast<float>(c[2]);
    }


}

std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> FlowVisualizationController::getPtrHexahedralMesh()
{
    return flow_model_.getPtrHexahedralMesh();
}

std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> FlowVisualizationController::getPtrTetrahedralMesh()
{
    return flow_model_.getPtrTetrahedralMesh();
}


/// @TODO Later, move this fucntions to FlowvizualizationController
Eigen::Affine3d FlowVisualizationController::getModelMatrix() const
{
    return this->flow_model_.getModelMatrix();
}


void FlowVisualizationController::setSaturationMethod( const SaturationMethod& option )
{

	code_interface.setNumberOfPhases(2);

    if( option == SaturationMethod::PERREGION )
    {
        code_interface.setSaturationMethod( 1 );
    }
    else if( option == SaturationMethod::APIGRAVITY )
    {
        code_interface.setSaturationMethod( 2 );
    }
    else
    {
        std::cout << "Not implemented yet" << std::flush << std::endl;
    }
}


void FlowVisualizationController::setMultiPhase(SaturationMethod _method)
{
	code_interface.setNumberOfPhases(2);

	if (_method == SaturationMethod::PERREGION)
	{
		code_interface.setSaturationMethod(1);
	}
	else if (_method == SaturationMethod::APIGRAVITY)
	{
		code_interface.setSaturationMethod(2);
	}

}

void FlowVisualizationController::setSinglePhase( )
{
        code_interface.setNumberOfPhases( 1 );
}


void FlowVisualizationController::updateTetrahedonRegions( const std::vector< int >& regions_, std::vector<double> &values_for_visualization_ )
{
    code_interface.setTetrahedralMeshRegions( regions_ );


    std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();
    if (ptr_mesh)
    {
        // Vertex Properties ------------------------------------------------->

        OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Regions");
        ptr_mesh->set_persistent(ch);

        int i = 0;
        for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
        {
            // Set property value
            ch[*c_it] = regions_[ i ];//values[i++];
            ++i;
        }

        flow_model_.updateTetrahedronCellScalarProperty("Regions", values_for_visualization_);
    }
}
