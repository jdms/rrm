
#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    user_input_ok = false;

    current_method = MESHING_METHOD::UNSTRUCTURED;

    code_interface.loadDefaultValues(1);

}




void FlowVisualizationController::readSkeletonFiles( const std::string& filename )
{

    if( mesh_ok == true ) return;


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

    mesh_ok = true;

}




void FlowVisualizationController::readInputParameters( const std::string& input_file )
{
    code_interface.readUserInput( input_file );
    user_input_ok = true;
}




void FlowVisualizationController::generateCornerPoint()
{

    std::cout << " Going to Build Volumetric "  << mesh_ok << user_input_ok << std::endl;

    if( mesh_ok == false || user_input_ok == false ) return;

    std::cout << " Going to Build Volumetric " << std::endl;
    code_interface.buildCPGVolumetricMesh();
    volumetric_ok = true;

}


void FlowVisualizationController::generateUnstructured()
{

    if( mesh_ok == false || user_input_ok == false ) return;


    code_interface.buildVolumetricMesh();
    volumetric_ok = true;


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
     volumetric_ok = true;
}


void FlowVisualizationController::updateVolumetricMesh( std::vector< float >& vertices, std::vector< unsigned int >& edges, std::vector< unsigned int >& faces )
{


    code_interface.getVolumeVertices( vertices );
    code_interface.getVolumeEdges( edges );
    code_interface.getVolumeCells( faces );
    volumetric_ok = true;

}


void FlowVisualizationController::setSkeletonData( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{

    std::vector< unsigned int> nu_uint( nu.begin(), nu.end() );
    std::vector< unsigned int> nv_uint( nv.begin(), nv.end() );

    unsigned int surfaces_number = ( unsigned int) nu.size();
//    unsigned int extrusion_size = ( unsigned int) num_extrusion_steps;

	std::vector<double> scaled_points;
	scaled_points.resize(points.size());
	Eigen::Vector3f v; Eigen::Vector3f sv;

	for (auto i = 0; i <= (points.size() - 3); i+=3)
	{
		v[0] = static_cast<float>(points[i + 0]);
		v[1] = static_cast<float>(points[i + 2]);
		v[2] = static_cast<float>(points[i + 1]);

		sv = this->scene3Dto2D(v);
		std::cout << sv << std::endl;

		scaled_points[i + 0] = static_cast<double>(sv[0]);
		scaled_points[i + 1] = static_cast<double>(sv[2]);
		scaled_points[i + 2] = static_cast<double>(sv[1]);
	}

    //code_interface.setSkeletonData( surfaces_number, nu_uint, nv_uint, points );
	code_interface.setSkeletonData(surfaces_number, nu_uint, nv_uint, scaled_points);

}


void FlowVisualizationController::computeFlowProperties()
{

    if( volumetric_ok == false || mesh_ok == false || properties_computed == true ) return;


    code_interface.init();
    code_interface.computeProperties();

    emit clearPropertiesMenu();


    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {
        emit propertybyVertexComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyVertexComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Forward TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Total TOF", "SCALAR" ) ;
        emit propertybyVertexComputed( "Max Backward Tracer", "SCALAR" ) ;
        emit propertybyVertexComputed( "Max Forward Tracer", "SCALAR" ) ;

        emit propertybyFaceComputed( "Velocity", "VECTOR" );
        emit propertybyFaceComputed( "Permeability", "SCALAR" );

    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh
    {
        emit propertybyFaceComputed( "Corrected Pressure", "SCALAR" ) ;
        emit propertybyFaceComputed( "Backward TOF", "SCALAR" ) ;
        emit propertybyFaceComputed( "Total TOF", "SCALAR" ) ;

        emit propertybyFaceComputed( "Max Backward Tracer", "SCALAR" ) ;
        emit propertybyFaceComputed( "Max Forward Tracer", "SCALAR" ) ;
    }



    emit setColorMap();



    properties_computed = true;




}


void FlowVisualizationController::getRegionsColor( std::vector< QColor >& color_by_cells )
{


    std::vector< int > idregion_by_cell;
    code_interface.getRegionId( idregion_by_cell );


    std::vector< int > regions_id = idregion_by_cell;
    auto last = std::unique( regions_id.begin(), regions_id.end() );
    regions_id.erase( last, regions_id.end() );


    std::random_device rd;
    std::mt19937 seed(rd());
    std::uniform_int_distribution< unsigned int > distr( 0, 255 );


    std::map< int, QColor > color_by_region;
    for( unsigned int i = 0; i < regions_id.size(); ++i )
    {
        int id = regions_id[ i ];

        int r = distr( seed );
        int g = distr( seed );
        int b = distr( seed );

        color_by_region[ id ] = QColor( r, g, b );
    }

    for( unsigned int i = 0; i < idregion_by_cell.size(); ++i )
    {
        int id = idregion_by_cell[ i ];
        color_by_cells.push_back( color_by_region[ id ] );
    }

}





std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;


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
    else if( name_of_property.compare( "Max Backward Tracer" ) == 0 )
    {
        code_interface.getMaxBackwardTracer( values );
    }
    else if( name_of_property.compare( "Max Forward Tracer" ) == 0 )
    {
        code_interface.getMaxForwardTracer( values );
    }




    if ( values.empty() == false )
    {

        std::vector< double >::iterator itmin = std::min_element( values.begin(), values.end() );
        std::vector< double >::iterator itmax = std::max_element( values.begin(), values.end() );

        int idmin = std::distance( values.begin(), itmin );
        int idmax = std::distance( values.begin(), itmax );

        min = values[ idmin ];
        max = values[ idmax ];

    }

    return values;

}


std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( current_method == MESHING_METHOD::UNSTRUCTURED ) // Unstructure Volumetric Mesh
    {
        if ( name_of_property.compare( "Velocity" ) == 0 )
        {
            type = "VECTOR";
            code_interface.getVelocitybyCells( values );
        }
         else if( name_of_property.compare( "Permeability" ) == 0 )
        {
            code_interface.getPermeability( values );
        }

    }

    else if( current_method == MESHING_METHOD::CORNERPOINT ) // Corner Point Mesh  // by cell
    {
        if ( name_of_property.compare( "Corrected Pressure" ) == 0 )
        {
            code_interface.getCPGPressure( values );
        }
        else if( name_of_property.compare( "Backward TOF" ) == 0 )
        {
            code_interface.getCPGBackwardTOF( values );
        }
        else if( name_of_property.compare( "Total TOF" ) == 0 )
        {
            code_interface.getCPGTotalTOF( values );
        }
        else if( name_of_property.compare( "TOF" ) == 0 )
        {
            code_interface.getCPGTOF( values );
        }
        else if( name_of_property.compare( "Backward Tracer" ) == 0 )
        {
            code_interface.getCPGMaxBackwardTracer( values );
        }
        else if( name_of_property.compare( "Tracer" ) == 0 )
        {
            code_interface.getCPGMaxForwardTracer( values );
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
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportVolumetoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportSurfacetoVTK( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}


void FlowVisualizationController::exportCornerPointtoGRDECL()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

     code_interface.exportCornerPointtoGRDECL( filename.toStdString() );
}


void FlowVisualizationController::exportResultstoVTK()
{

    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;


     code_interface.exportResultstoVTK( filename.toStdString() );
}




void FlowVisualizationController::clear()
{
    mesh_ok = false;
    user_input_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    code_interface.clear();
}

void FlowVisualizationController::setScene2Dto3D(const Eigen::Affine3f& m)
{
	this->m_2dto3d = m;
}
void FlowVisualizationController::setScene3Dto2D(const Eigen::Affine3f& m)
{
	this->m_3dto2d = m;

}

Eigen::Vector3f FlowVisualizationController::scene2Dto3D(const Eigen::Vector2f& p)
{
	return Eigen::Vector3f();
}
Eigen::Vector3f FlowVisualizationController::scene2Dto3D(const Eigen::Vector3f& p)
{
	return Eigen::Vector3f();
}
Eigen::Vector3f FlowVisualizationController::scene3Dto2D(const Eigen::Vector3f& p)
{
	Eigen::Vector4f p_cpy(p.x(), p.y(), p.z(), 1.0f);

	p_cpy = m_3dto2d.matrix()*p_cpy;
	return Eigen::Vector3f(p_cpy.x(), p_cpy.y(), p_cpy.z());
}