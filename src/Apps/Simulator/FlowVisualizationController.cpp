
#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    is_surface_loaded = false;
    is_volumetric_built = false;
    are_properties_computed = false;
    user_input_ok = false;

	current_method = MESHING_METHOD::UNSTRUCTURED;

    code_interface.loadDefaultValues(1);

}




void FlowVisualizationController::readSkeletonFiles( const std::string& filename )
{

    if( is_surface_loaded == true ) return;


    code_interface.readSkeletonFile( filename );
    // chmar funcao para desenhar a superficie


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
    code_interface.readUserInput( input_file );
    user_input_ok = true;
}




void FlowVisualizationController::generateCornerPoint()
{

    std::cout << " Going to Build Volumetric "  << is_surface_loaded << user_input_ok << std::endl;

    if( is_surface_loaded == false || user_input_ok == false ) return;

    std::cout << " Going to Build Volumetric " << std::endl;
    code_interface.buildCPGVolumetricMesh();
    is_volumetric_built = true;

}


void FlowVisualizationController::generateUnstructured()
{
    std::cout << " Going to Build Unstructured "  << is_surface_loaded << user_input_ok << std::endl;

    if( is_surface_loaded == false || user_input_ok == false ) return;


    code_interface.buildVolumetricMesh();
    is_volumetric_built = true;
std::cout << " Finish to Build Unstructured "  << is_surface_loaded << user_input_ok << std::endl;

}




void FlowVisualizationController::updateSurfaces( std::vector< float >& vertices, std::vector< unsigned int >& faces )
{

   code_interface.getSurfaceVertices( vertices );
   code_interface.getSurfaceFaces( faces );

}


void FlowVisualizationController::updateCornerPoint( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces )
{

     code_interface.getCPGVolumeVertices( vertices );
     code_interface.getCPGVolumeEdges( edges );
     code_interface.getCPGVolumeCells( faces );
     is_volumetric_built = true;
}


void FlowVisualizationController::updateVolumetricMesh( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces )
{


    code_interface.getVolumeVertices( vertices );
    code_interface.getVolumeEdges( edges );
    code_interface.getVolumeCells( faces );
    is_volumetric_built = true;

}


void FlowVisualizationController::setSkeletonData( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{

    std::vector< unsigned int> nu_uint( nu.begin(), nu.end() );
    std::vector< unsigned int> nv_uint( nv.begin(), nv.end() );

    unsigned int surfaces_number = ( unsigned int) nu.size();
////    unsigned int extrusion_size = ( unsigned int) num_extrusion_steps;

//	std::vector<double> scaled_points;
//	scaled_points.resize(points.size());
//	Eigen::Vector3f v; Eigen::Vector3f sv;

//	for (auto i = 0; i <= (points.size() - 3); i+=3)
//	{
//		v[0] = static_cast<float>(points[i + 0]);
//		v[1] = static_cast<float>(points[i + 2]);
//		v[2] = static_cast<float>(points[i + 1]);

//		sv = this->scene3Dto2D(v);
//		std::cout << sv << std::endl;

//		scaled_points[i + 0] = static_cast<double>(sv[0]);
//		scaled_points[i + 1] = static_cast<double>(sv[2]);
//		scaled_points[i + 2] = static_cast<double>(sv[1]);
//	}

    code_interface.setSkeletonData( surfaces_number, nu_uint, nv_uint, points );
//	code_interface.setSkeletonData(surfaces_number, nu_uint, nv_uint, scaled_points);

}


void FlowVisualizationController::computeFlowProperties()
{

    if( is_volumetric_built == false || is_surface_loaded == false ) return;

	std::cout << "Computing Proterties" << std::endl; 
	if (current_method == MESHING_METHOD::UNSTRUCTURED)
	{
		std::cout << "MESHING_METHOD::UNSTRUCTURED" << std::endl; 
	}
	else if (current_method == MESHING_METHOD::CORNERPOINT)
	{
		std::cout << "MESHING_METHOD::CORNERPOINT" << std::endl;
	}else
	{
		std::cout << "Should not Reach This Point" << std::endl;
		return;
	}
	

    code_interface.init();
    code_interface.computeProperties();

    emit clearPropertiesMenu();


    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {
        emit propertybyVertexComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyVertexComputed( "Forward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Total TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Backward Tracer", "SCALAR" ) ;
        emit propertybyVertexComputed( "Forward Tracer", "SCALAR" ) ;
        emit propertybyVertexComputed( "Maximum Tracer", "SCALAR" ) ;
        emit propertybyVertexComputed( "Velocity", "VECTOR" );
        emit propertybyVertexComputed( "Permeability", "SCALAR" );

        emit propertybyFaceComputed( "Permeability", "SCALAR" );
//        emit propertybyFaceComputed( "Porosity", "SCALAR" );
        emit propertybyFaceComputed( "Velocity", "VECTOR" );


    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh
    {

        emit propertybyFaceComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyFaceComputed( "Forward TOF", "SCALAR" ) ;
        emit propertybyFaceComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyFaceComputed( "Total TOF", "SCALAR" ) ;
        emit propertybyFaceComputed( "Backward Tracer", "SCALAR" ) ;
        emit propertybyFaceComputed( "Forward Tracer", "SCALAR" ) ;
        emit propertybyFaceComputed( "Maximum Tracer", "SCALAR" ) ;


        emit propertybyFaceComputed( "Permeability", "SCALAR" );
//        emit propertybyFaceComputed( "Porosity", "SCALAR" );
//        emit propertybyFaceComputed( "Velocity", "VECTOR" );

    }



    emit setColorMap();

    are_properties_computed = true;

}


void FlowVisualizationController::getRegionsColor(std::vector< QColor >& color_by_cells, ColorMap::COLORMAP current_colormap, std::vector<int>& _ids)
{
    std::vector< int > idregion_by_cell;
    code_interface.getRegionId( idregion_by_cell );

	_ids = idregion_by_cell;

	auto min_max = std::minmax_element(idregion_by_cell.begin(), idregion_by_cell.end());

	QVector3D c;

    for( unsigned int i = 0; i < idregion_by_cell.size(); ++i )
    {
		c = colormap.getColor(current_colormap, idregion_by_cell[i], *min_max.first, *min_max.second);

		color_by_cells.push_back(QColor::fromRgbF(c.x(),c.y(),c.z(),1.0));
    }

}





std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;


    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {

        if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
        {
            code_interface.getPressure( values );
        }
        else if( name_of_property.compare( "Backward TOF" ) == 0 )
        {
            code_interface.getBackwardTOF( values );
        }
        else if( name_of_property.compare( "Forward TOF" ) == 0 )
        {
            code_interface.getForwardTOF( values );
        }
        else if( name_of_property.compare( "Total TOF" ) == 0 )
        {
            code_interface.getTotalTOF( values );
        }
        else if( name_of_property.compare( "Backward Tracer" ) == 0 )
        {
            code_interface.getMaxBackwardTracer( values );
        }
        else if( name_of_property.compare( "Forward Tracer" ) == 0 )
        {
            code_interface.getMaxForwardTracer( values );
        }
        else if( name_of_property.compare( "Maximum Tracer" ) == 0 )
        {
            code_interface.getMaxForwardTracer( values );
        }
        else if( name_of_property.compare( "Permeability" ) == 0 )
        {
            code_interface.getPermeabilitybyVertices( values );
        }

        else if( name_of_property.compare( "Velocity" ) == 0 )
        {
            type = "VECTOR";
            code_interface.getVelocitybyVertices( values );
        }

    }


    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh  // by cell
    {
        // not developed yet
    }


    std::vector< double > scalar_values;
    if ( values.empty() == true ) return scalar_values;


    if( type.compare( "VECTOR" ) == 0 )
    {
        scalar_values = vectorToScalarProperties( values, method, min, max );
    }
    else
    {


//    if ( values.empty() == false )
//    {
        scalar_values = values;

        std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
        std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

        int idmin = std::distance( scalar_values.begin(), itmin );
        int idmax = std::distance( scalar_values.begin(), itmax );

        min = scalar_values[ idmin ];
        std::cout << "Min : " << min << std::endl;
        max = scalar_values[ idmax ];
        std::cout << "Max : " << max << std::endl;


    }

    return scalar_values;

}


std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;
	std::vector< double > replicate_values;


    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {

        if ( name_of_property.compare( "Permeability" ) == 0 )
        {
            code_interface.getPermeabilitybyCells( values );
        }
        else if ( name_of_property.compare( "Porosity" ) == 0 ) // <= missing
        {

        }
        else if ( name_of_property.compare( "Velocity" ) == 0 )
        {
            type = "VECTOR";
            code_interface.getVelocitybyCells( values );
        }

    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh  // by cell
    {

        if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
        {
            code_interface.getCPGPressure(replicate_values);
        }
        else if( name_of_property.compare( "Backward TOF" ) == 0 )
        {
            code_interface.getCPGBackwardTOF(replicate_values);
        }
        else if( name_of_property.compare( "Forward TOF" ) == 0 )
        {
            code_interface.getCPGTOF(replicate_values);
        }
        else if( name_of_property.compare( "Total TOF" ) == 0 )
        {
            code_interface.getCPGTotalTOF(replicate_values);
        }
        else if( name_of_property.compare( "Backward Tracer" ) == 0 )
        {
            code_interface.getCPGMaxBackwardTracer(replicate_values);
        }
        else if( name_of_property.compare( "Forward Tracer" ) == 0 )
        {
            code_interface.getCPGMaxForwardTracer(replicate_values);
        }

        else if( name_of_property.compare( "Permeability" ) == 0 )
        {
            code_interface.getCPGPermeability(replicate_values);
        }

//        else if( name_of_property.compare( "Porosity" ) == 0 )
//        {
//            code_interface.getCPGMaxForwardTracer(replicate_values);
//        }
//        else if ( name_of_property.compare( "Velocity" ) == 0 )
//        {
//            type = "VECTOR";
//            code_interface.getVelocitybyCells( values );
//        }


        values.resize(replicate_values.size() * 6);
        for (auto i = 0; i < replicate_values.size(); i++)
        {
            values[i * 6 + 0] = replicate_values[i];
            values[i * 6 + 1] = replicate_values[i];
            values[i * 6 + 2] = replicate_values[i];
            values[i * 6 + 3] = replicate_values[i];
            values[i * 6 + 4] = replicate_values[i];
            values[i * 6 + 5] = replicate_values[i];
        }

    }


    std::vector< double > scalar_values;
    if ( values.empty() == true ) return scalar_values;


    if( type.compare( "VECTOR" ) == 0 )
    {
        scalar_values = vectorToScalarProperties( values, method, min, max );
    }
    else
    {
        scalar_values = values;

        std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
        std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

        int idmin = std::distance( scalar_values.begin(), itmin );
        int idmax = std::distance( scalar_values.begin(), itmax );

        min = scalar_values[ idmin ];
        max = scalar_values[ idmax ];
    }


    return scalar_values;
}


std::vector< double > FlowVisualizationController::vectorToScalarProperties(const std::vector<double> &values, std::string type, double& min , double& max  )
{

    int number_of_vertices = (int)values.size()/3;
    std::vector< double > scalar_values;


    if( type.compare( "COORDX" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i ] );
        }

    }
    else if( type.compare( "COORDY" ) == 0 )
    {
        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i + 1 ] );
        }

    }
    else if( type.compare( "COORDZ" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            scalar_values.push_back( values[ 3*i + 2 ] );
        }
    }
    else if( type.compare( "LENGTH" ) == 0 )
    {

        for( int i = 0; i < number_of_vertices; ++i )
        {
            double x = values[ 3*i ];
            double y = values[ 3*i + 1 ];
            double z = values[ 3*i + 2 ];

            double length = x*x + y*y + z*z;

            scalar_values.push_back( length );

        }
    }

    std::vector< double >::iterator itmin = std::min_element( scalar_values.begin(), scalar_values.end() );
    std::vector< double >::iterator itmax = std::max_element( scalar_values.begin(), scalar_values.end() );

    int idmin  = std::distance( scalar_values.begin(), itmin );
    int idmax = std::distance( scalar_values.begin(), itmax );

    min = scalar_values[ idmin ];
    max = scalar_values[ idmax ];

    return scalar_values;

}






void FlowVisualizationController::setPropertyArea( const int np, const std::vector< double >& values , const std::vector< double >& perm, const std::vector< double >& poros, const std::vector< double >& visc )
{
    code_interface.setNumberofRegions(np);
    for( int i = 0; i < np; ++i )
    {
        code_interface.setRegion( i, values[ 3*i ], values[ 3*i + 1 ], values[ 3*i +2 ], perm[ i ], poros[ i ], visc[ i ] );
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

    for( int i = 0; i < np; ++i )
    {
        code_interface.getRegion( i, values[ 3*i ], values[ 3*i + 1 ], values[ 3*i +2 ], perm[ i ], poros[ i ], visc[ i ] );
    }

}



void FlowVisualizationController::setWellsValues( const int nw,  const std::vector< unsigned int >& type, const std::vector< double >& value, const std::vector< int >& sign )
{
    code_interface.setNumberofWells(nw);
    for( int i = 0; i < nw; ++i )
    {
        code_interface.setWell( i, type[ i ], value[ i ], sign[ i ] );
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





void FlowVisualizationController::exportSurfacetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportVolumetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportCornerPointtoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoGRDECL()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".GRDECL files (*.grdecl)", &selected_format );
    if( filename.isEmpty() == true ) return;

     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}


void FlowVisualizationController::exportResultstoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".VTK files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportResultstoVTK( filename.toStdString() );
}




void FlowVisualizationController::clear()
{
    is_surface_loaded = false;
    user_input_ok = false;
    is_volumetric_built = false;
    are_properties_computed = false;
    code_interface.clear();
}

//void FlowVisualizationController::setScene2Dto3D(const Eigen::Affine3f& m)
//{
//	this->m_2dto3d = m;
//}
//void FlowVisualizationController::setScene3Dto2D(const Eigen::Affine3f& m)
//{
//	this->m_3dto2d = m;

//}

//Eigen::Vector3f FlowVisualizationController::scene2Dto3D(const Eigen::Vector2f& p)
//{
//	return Eigen::Vector3f();
//}
//Eigen::Vector3f FlowVisualizationController::scene2Dto3D(const Eigen::Vector3f& p)
//{
//	return Eigen::Vector3f();
//}
//Eigen::Vector3f FlowVisualizationController::scene3Dto2D(const Eigen::Vector3f& p)
//{
//	Eigen::Vector4f p_cpy(p.x(), p.y(), p.z(), 1.0f);

//	p_cpy = m_3dto2d.matrix()*p_cpy;
//	return Eigen::Vector3f(p_cpy.x(), p_cpy.y(), p_cpy.z());
//}
