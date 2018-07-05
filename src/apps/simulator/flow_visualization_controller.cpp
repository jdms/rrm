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

     is_volumetric_built = true;
}

void FlowVisualizationController::getTetrahedeonMeshGeometry(
        std::vector< float >& raw_vertices_, 
        std::vector< unsigned int >& raw_cells_,
        std::vector< float >& vertices_, 
        std::vector< unsigned int >& faces_ 
        )
{


	/* std::vector < float  > raw_vertices_; */
	/* std::vector < unsigned int  > raw_cells_; */

	code_interface.getVolumeVertices(raw_vertices_);
	code_interface.getVolumeCells(raw_cells_);
    	   

	//flow_model_.createTetrahedonMesh(raw_vertices_, raw_cells_);
	//flow_model_.uploadTetrahedron(vertices_, faces_);
		

	flow_model_.uploadTetrahedron(raw_vertices_, raw_cells_, vertices_, faces_);

	std::cout << "updateVolumetricMesh " << vertices_.size() << " Cells " << faces_.size();

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


void FlowVisualizationController::setPropertyArea(const int _number_of_regions,
	const std::map<int, std::pair<double, double> >& _x_permeability_values,
	const std::map<int, std::pair<double, double> >& _y_permeability_values,
	const std::map<int, std::pair<double, double> >& _z_permeability_values,
	const std::map<int, std::pair<double, double> >& _porosity_values_,
	const std::vector<double>& _saturations_values, /// Water Saturation
	const std::vector<double>& _pct_values,		    /// Threshold Pressure
	const std::vector<double>& _sort_factor_values, /// Lambda
	const std::vector<double>& _siw_values		    /// Connate Water Saturation 
	
	)
{
	code_interface.setNumberofRegions(_number_of_regions);
	for (int i = 0; i < _number_of_regions; ++i)
    {
        /// @FIXME January 2018
       

		code_interface.setRegionMultiphase(i,
			_x_permeability_values.at(i).first,   /// KxLow
			_x_permeability_values.at(i).second,  /// KxHigh
			_y_permeability_values.at(i).first,   /// Kylow
			_y_permeability_values.at(i).second,  /// KyHigh
			_z_permeability_values.at(i).first,   /// KzLow
			_z_permeability_values.at(i).second,  /// KzHigh
			_porosity_values_.at(i).first,  /// PoroLow
			_porosity_values_.at(i).second, /// PoroHigh
			_pct_values[i],  /// PcT
			_siw_values[i],  /// Si
			_sort_factor_values[i],   /// Sort Factor Lambda
			_saturations_values[i]    /// Water Saturation
			);
        
    }
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
void FlowVisualizationController::setFluidProperty(
	double _oil_viscosity,
	double _oil_density,
	double _bo,
	double _water_viscosity,
	double _water_density,
	double _bw,
	double _fwl,
	const std::pair<int, int>& _phase_method)
{
    // Tells the FlowDiagnosticsInterface whether we want
    // single-phase  := (_phase_method.first == 1), or
    // multi-phase   := (_phase_method.first == 2)
    // flow diagnostics
    this->code_interface.setNumberOfPhases( _phase_method.first );
    // Set water saturation by region  := (_phase_method.second == 1)
    // Set water saturation by gravity := (_phase_method.second == 2)
    // This should only be set if "multi-phase" diagnostics was selected
    this->code_interface.setSaturationMethod( _phase_method.second );

	code_interface.setOilViscosity(_oil_viscosity);
	code_interface.setOilDensity(_oil_density);
	code_interface.setBo(_bo);
	code_interface.setWaterViscosity(_water_viscosity);
	code_interface.setWaterDensity(_water_density);
	code_interface.setBw( _bw);
	code_interface.setFreeWaterLevel(_fwl);  
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

	/// Vertex Properties --->

	std::vector<RRM::PropertyProfile> vertex_properties;

	
	// vertex_properties.push_back(RRM::PropertyProfile("Tetrahedron", (*vp_it)->name(), (*vp_it)->entityType(), "", (*vp_it)->typeNameWrapper()));

	vertex_properties.push_back(RRM::PropertyProfile());

  //  std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();

  //  if (ptr_mesh)
  //  {

  //      // Vertex Properties ------------------------------------------------->
  //      std::vector<double> values;
  //      bool status = false;
  //      // Pressure
  //      values.clear();
  //      int i = 0;
  //      code_interface.getPressure(values);

  //      OpenVolumeMesh::VertexPropertyT<double> ph = ptr_mesh->request_vertex_property<double>("Pressure (bar)");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      // Foward TOF
  //      values.clear();
  //      i = 0;
  //      code_interface.getForwardTOF(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Time-of-Flight from Injectors (day)");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      // Foward TOF Log 10
  //      values.clear();
  //      i = 0;
  //      code_interface.getFowardTOF_log10(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Time-of-Flight from Injectors (Log10(day)) ");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      // BackWard TOF
  //      values.clear();
  //      i = 0;
  //      code_interface.getBackwardTOF(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Time-of-Flight to Producers (day)");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      // BackWard TOF Log 10
  //      values.clear();
  //      i = 0;
  //      code_interface.getBackwardTOF_log10(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Time-of-Flight to Producers (Log10(day))");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }


  //      // Total TOF
  //      values.clear();
  //      i = 0;
  //      code_interface.getTotalTOF(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Total Time-of-Flight (day)");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      // Total TOF Log 10
  //      values.clear();
  //      i = 0;
  //      code_interface.getTotalTOF_log10(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Total Time-of-Flight (Log10(day))");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }


  //      //Maximum Tracer
  //      values.clear();
  //      i = 0;
  //      code_interface.getMaxForwardTracer(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Injectors Tracer Partitioning");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }

  //      //Max BackTracer
  //      values.clear();
  //      i = 0;
  //      code_interface.getMaxBackwardTracer(values);

  //      ph = ptr_mesh->request_vertex_property<double>("Producers Tracer Partitioning");
  //      ptr_mesh->set_persistent(ph);

  //      for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //      {
  //          // Set property value
  //          ph[*v_it] = values[i++];
  //      }


  //      //Capillary Pressure
  //      values.clear();
  //      i = 0;
  //      status = code_interface.getCapillaryPressure(values);

  //      if ( status)
  //      {
  //          ph = ptr_mesh->request_vertex_property<double>("Capillary Pressure");
  //          ptr_mesh->set_persistent(ph);

  //          for (OpenVolumeMesh::VertexIter v_it = ptr_mesh->vertices_begin(); v_it != ptr_mesh->vertices_end(); ++v_it)
  //          {
  //              // Set property value
  //              ph[*v_it] = values[i++];
  //          }
  //      }

  //      // Cell Properties --------------------------------------------------->

  //      //Permeability X
  //      values.clear();
  //      i = 0;
  //      code_interface.getPermeabilityXbyCells(values);

  //      OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Permeability X (mD)");
  //      ptr_mesh->set_persistent(ch);

  //      for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //      {
  //      //	// Set property value
  //          ch[*c_it] = values[i++];

		//	
  //      }


		////Permeability Y
		//values.clear();
		//i = 0;
		//code_interface.getPermeabilityYbyCells(values);

		//ch = ptr_mesh->request_cell_property<double>("Permeability Y (mD)");
		//ptr_mesh->set_persistent(ch);

		//for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
		//{
		//	//	// Set property value
		//	ch[*c_it] = values[i++];


		//}


		////Permeability Z
		//values.clear();
		//i = 0;
		//code_interface.getPermeabilityZbyCells(values);

		//ch = ptr_mesh->request_cell_property<double>("Permeability Z (mD)");
		//ptr_mesh->set_persistent(ch);

		//for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
		//{
		//	//	// Set property value
		//	ch[*c_it] = values[i++];


		//}

  //      //Porosity
  //      values.clear();
  //      i = 0;
  //      code_interface.getPorosity(values);

  //      ch = ptr_mesh->request_cell_property<double>("Porosity");
  //      ptr_mesh->set_persistent(ch);

  //      for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //      {
  //          // Set property value
  //          ch[*c_it] = values[i++];
  //      }

  //      //Water Saturation
  //      values.clear();
  //      i = 0;
  //      status = code_interface.getWaterSaturationByCells(values);

  //      if (status)
  //      {
  //          ch = ptr_mesh->request_cell_property<double>("Water Saturation");
  //          ptr_mesh->set_persistent(ch);

  //          for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //          {
  //              // Set property value
  //              ch[*c_it] = values[i++];
  //          }
  //      }

  //      //Velocity
  //      values.clear();
  //      i = 0;
  //      code_interface.getVelocitybyCells(values);

  //      OpenVolumeMesh::CellPropertyT<double> chv = ptr_mesh->request_cell_property<double>("Velocity Magnitude (m/s)");
  //      ptr_mesh->set_persistent(chv);

  //      for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //      {
  //          OpenVolumeMesh::Vector3D v (values[3 * i + 0], values[3 * i + 1], values[3 * i + 2]);

  //          // Set property value
  //          chv[*c_it] = v.norm();

  //          i++;
  //      }


  //      //Velocity Log 10
  //      values.clear();
  //      i = 0;
  //      code_interface.getVelocityMagnitudebyCells_log10(values);

  //      chv = ptr_mesh->request_cell_property<double>("Velocity Magnitude (log10(m/s))");
  //      ptr_mesh->set_persistent(chv);

  //      for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //      {

  //          // Set property value
  //          chv[*c_it] = values[i]; //here debug error
  //          i++;
  //      }


  //      ////@TODO September
  //      //Water Saturation
  //      values.clear();
  //      i = 0;
  //      status = code_interface.getWaterSaturationByCells(values);

  //      if (status)
  //      {
  //          chv = ptr_mesh->request_cell_property<double>("Water Saturation");
  //          ptr_mesh->set_persistent(chv);

  //          for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //          {

  //              // Set property value
  //              chv[*c_it] = values[i];
  //              i++;
  //          }
  //      }

  //      //Oil Relative permeability
  //      values.clear();
  //      i = 0;
  //      status = code_interface.getKroByCells(values);

  //      if (status)
  //      {
  //          chv = ptr_mesh->request_cell_property<double>("Oil Relative Permeability");
  //          ptr_mesh->set_persistent(chv);

  //          for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //          {

  //              // Set property value
  //              chv[*c_it] = values[i];
  //              i++;
  //          }
  //      }

  //      //Water Relative Permeability
  //      values.clear();
  //      i = 0;
  //      status = code_interface.getKrwByCells(values);

  //      if (status)
  //      {
  //          chv = ptr_mesh->request_cell_property<double>("Water Relative Permeability");
  //          ptr_mesh->set_persistent(chv);

  //          for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
  //          {

  //              // Set property value
  //              chv[*c_it] = values[i];
  //              i++;
  //          }
  //      }

  //  }

}

/// FIXEME June 2017
void FlowVisualizationController::getPoreVolume()
{

    //std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();

    //if (ptr_mesh)
    //{
    //    //Pore Volume
    //    std::vector<double> values;
    //    values.clear();

    //    code_interface.getPoreVolumebyRegion(values);
    //    std::vector< int > idregion_by_cell;
    //    code_interface.getRegionId(idregion_by_cell);

    //    OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Pore Volume");
    //    ptr_mesh->set_persistent(ch);

    //    for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
    //    {
    //        //	// Set property value
    //        ch[*c_it] = values[idregion_by_cell[*c_it]];
    //    }
    //}

}



void FlowVisualizationController::updateTetrahedronColors(const RRM::PropertyProfile & _property, std::vector<float>& _colors, double& _min, double& _max)
{

    std::vector<double> values;
	std::vector<unsigned int> cells;
	code_interface.getVolumeCells(cells);
	

	values = flow_model_.updateTetrahedronColors(_property, cells, this->getComputedProperitesValues(_property));
	

    // Get the property by the interator
    auto min_max = std::minmax_element(values.begin(), values.end());

    std::cout << "min element at: " << (min_max.first - values.begin()) << " " << *min_max.first << std::endl;
    std::cout << "max element at: " << (min_max.second - values.begin()) << " " << *min_max.second << std::endl;

    _colors.clear();
    _colors.resize(values.size() * 3);

    _min = *min_max.first;
    _max = *min_max.second;

    for (std::size_t i = 0; i < values.size(); ++i)
    {
        QVector3D c = colormap.getColor(current_colormap_, values[i], *min_max.first, *min_max.second);
        _colors[i * 3 + 0] = static_cast<float>(c[0]);
        _colors[i * 3 + 1] = static_cast<float>(c[1]);
        _colors[i * 3 + 2] = static_cast<float>(c[2]);
    }
		
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


    //std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> ptr_mesh = flow_model_.getPtrTetrahedralMesh();
    //if (ptr_mesh)
    //{
    //    // Vertex Properties ------------------------------------------------->

    //    OpenVolumeMesh::CellPropertyT<double> ch = ptr_mesh->request_cell_property<double>("Regions");
    //    ptr_mesh->set_persistent(ch);

    //    int i = 0;
    //    for (OpenVolumeMesh::CellIter c_it = ptr_mesh->cells_begin(); c_it != ptr_mesh->cells_end(); ++c_it)
    //    {
    //        // Set property value
    //        ch[*c_it] = regions_[ i ];//values[i++];
    //        ++i;
    //    }

    //    flow_model_.updateTetrahedronCellScalarProperty("Regions", values_for_visualization_);
    //}
}

/// @FIXME June 2018
/// New Interface -- Retriving data from Flow Diagnostic Module


/// Mesh Geometry
std::vector<float>		  FlowVisualizationController::getVertices() 
{
	std::vector<float> vertices;
	code_interface.getVolumeVertices(vertices);
	return vertices;

}
std::vector<unsigned int> FlowVisualizationController::getCells() 
{
	std::vector<unsigned int> cells;
	code_interface.getVolumeCells(cells);
	return cells;
}

std::vector<RRM::PropertyProfile>  FlowVisualizationController::getListOfComputedProperites( )
{
        std::vector<RRM::PropertyProfile> properties;
		std::vector<double> values;
		bool status = false;

		/// Vertex Properties
		properties.emplace_back(RRM::PropertyProfile("Pressure (bar)", "Vertex", "Bar", "Scalar"));											/// code_interface.getPressure(values);
		properties.emplace_back(RRM::PropertyProfile("Time - of - Flight from Injectors (day)", "Vertex", "Day", "Scalar"));				/// code_interface.getForwardTOF(values)
		properties.emplace_back(RRM::PropertyProfile("Time - of - Flight from Injectors (Log10(day))", "Vertex,", "Log10(day)", "Scalar")); /// code_interface.getFowardTOF_log10(values);
		properties.emplace_back(RRM::PropertyProfile("Time - of - Flight to Producers (day)", "Vertex", "Day", "Scalar"));					/// code_interface.getBackwardTOF(values);
		properties.emplace_back(RRM::PropertyProfile("Time - of - Flight to Producers (Log10(day))", "Vertex", "(Log10(day))", "Scalar"));	/// code_interface.getBackwardTOF_log10(values);
		properties.emplace_back(RRM::PropertyProfile("Total Time-of-Flight (day)", "Vertex", "Day", "Scalar"));								/// code_interface.getTotalTOF(values);
		properties.emplace_back(RRM::PropertyProfile("Total Time-of-Flight (Log10(day))", "Vertex", "(Log10(day))", "Scalar"));				/// code_interface.getTotalTOF_log10(values);
		properties.emplace_back(RRM::PropertyProfile("Injectors Tracer Partitioning", "Vertex", "", "Scalar"));								/// code_interface.getMaxForwardTracer(values);
		properties.emplace_back(RRM::PropertyProfile("Producers Tracer Partitioning", "Vertex", "", "Scalar"));								/// code_interface.getMaxBackwardTracer(values);

		//Capillary Pressure
		values.clear();
		status = code_interface.getCapillaryPressure(values);
		if ( status)
	    {
			properties.emplace_back(RRM::PropertyProfile("Capillary Pressure", "Vertex", "", "Scalar"));								    /// code_interface.getCapillaryPressure(values);
		}

		//// Cell Properties
		properties.emplace_back(RRM::PropertyProfile("Permeability", "Cell", "", "Scalar"));								               /// code_interface.getPermeabilitybyCells(values);
		properties.emplace_back(RRM::PropertyProfile("Porosity", "Cell", "", "Scalar"));								                   /// code_interface.getPorosity(values);
		properties.emplace_back(RRM::PropertyProfile("Velocity", "Cell", "", "Vector"));								                   /// code_interface.getVelocitybyCells(values);
		properties.emplace_back(RRM::PropertyProfile("Velocity Log10", "Cell", "", "Vector"));								               /// code_interface.getVelocityMagnitudebyCells_log10(values);
		
		values.clear();
		status = code_interface.getWaterSaturationByCells(values);
		if (status)
		{
			properties.emplace_back(RRM::PropertyProfile("Water Saturation", "Cell", "", "Scalar"));									   /// code_interface.getWaterSaturationByCells(values);
		}

		values.clear();
		status = code_interface.getKroByCells(values);
		if (status)
		{
			properties.emplace_back(RRM::PropertyProfile("Oil Relative Permeability", "Cell", "", "Scalar"));							   /// code_interface.getKroByCells(values);
		}

		values.clear();
		status = code_interface.getKrwByCells(values);
		if (status)
		{
			properties.emplace_back(RRM::PropertyProfile("Water Relative Permeability", "Cell", "", "Scalar"));							  /// code_interface.getKrwByCells(values);
		}

		return properties;
}


std::vector<double> FlowVisualizationController::getComputedProperitesValues(const RRM::PropertyProfile& _property)
{
	std::vector<double> values;
	
	/// Vertex Properties
	if (_property.name() == "Pressure (bar)")
	{
		code_interface.getPressure(values);
	}
	else if (_property.name() == "Time - of - Flight from Injectors (day)")
	{		
		code_interface.getForwardTOF(values);
	}
	else if (_property.name() == "Time - of - Flight from Injectors (Log10(day))")
	{
		code_interface.getFowardTOF_log10(values);
	}
	else if (_property.name() == "Time - of - Flight to Producers (day)")
	{
		code_interface.getBackwardTOF(values);
	}
	else if (_property.name() == "Time - of - Flight to Producers (Log10(day)))")
	{
		code_interface.getBackwardTOF_log10(values);
	}
	else if (_property.name() == "Total Time-of-Flight (day)")
	{
		code_interface.getTotalTOF(values);
	}
	else if (_property.name() == "Total Time-of-Flight (Log10(day))")
	{
		code_interface.getTotalTOF_log10(values);
	}
	else if (_property.name() == "Injectors Tracer Partitioning")
	{
		code_interface.getMaxForwardTracer(values);
	}
	else if (_property.name() == "Producers Tracer Partitioning")
	{
		code_interface.getMaxBackwardTracer(values);
	}
	else if (_property.name() == "Capillary Pressure")
	{
		code_interface.getCapillaryPressure(values);
	}
	/// Cells Properties
	else if (_property.name() == "Permeability")
	{
		code_interface.getPermeabilitybyCells(values);
	}
	else if (_property.name() == "Porosity")
	{
		code_interface.getPorosity(values);
	}

	/// All velocity is read as its magnitute ?
	else if (_property.name() == "Velocity")
	{
		code_interface.getVelocitybyCells(values);
	}
	else if (_property.name() == "Velocity Log10")
	{
		code_interface.getVelocityMagnitudebyCells_log10(values); 
	}
	else if (_property.name() == "Water Saturation")
	{
		code_interface.getWaterSaturationByCells(values);
	}
	else if (_property.name() == "Oil Relative Permeability")
	{
		code_interface.getKroByCells(values);
	}
	else if (_property.name() == "Water Relative Permeability")
	{
		code_interface.getKrwByCells(values);
	}

	return values;
}