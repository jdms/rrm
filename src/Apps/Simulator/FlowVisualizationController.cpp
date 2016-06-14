#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
}

void FlowVisualizationController::readCornerPoint( bool read_file, const std::string& mesh_file )
{
    if( read_file == true )
        region.readskeleton( (char *) mesh_file.c_str() );
    else
        getSurfaceFromCrossSection();

    region.cornerpointgrid();

    emit updateMesh();

    volumetric_ok = false;


}

void FlowVisualizationController::readUnstructured( bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  )
{
    region.userinput( (char *) input_file.c_str() );

    if( read_file == true && type_of_file.compare( "poly" ) == 0 )
        region.readsurfacemeshPOLY( (char *) mesh_file.c_str() );
    else if( read_file == true && type_of_file.compare( "txt" ) == 0 ){
        std::string all_filename = mesh_file + ".txt";
        region.readskeleton( (char *) all_filename.c_str() );
        region.unstructuredsurfacemesh();
    }
    else if( read_file == false ){
        getSurfaceFromCrossSection();
        region.unstructuredsurfacemesh();
    }

    emit updateMesh();

    volumetric_ok = false;


}

void FlowVisualizationController::reloadMesh( const int& type_of_mesh, bool read_file,  const std::string& mesh_file, const std::string& type_of_file, const std::string& input_file  )
{

    if( type_of_mesh == 2 )
    {
        readCornerPoint( read_file, mesh_file );
    }
    else if ( type_of_mesh == 1 )
    {
        readUnstructured( read_file, mesh_file, type_of_file, input_file );
    }

}



//void FlowVisualizationController::readfromFiles( const std::string& input_file, const std::string& mesh_file, std::string type, int option )
//{

//    region.userinput( (char *) input_file.c_str() );

//    if( true && type.compare( "POLY" ) == 0 )
//        region.readsurfacemeshPOLY( (char *) mesh_file.c_str() );
//    else if( true && type.compare( "txt" ) == 0 )
//        region.readskeleton( (char *) mesh_file.c_str() );

//    if( option == 1 )
//        region.unstructuredsurfacemesh();
//    else if( option == 2 )
//        region.cornerpointgrid();

//    volumetric_ok = false;

//    emit updateMesh();
//}


//void FlowVisualizationController::readMeshFile( const std::string& mesh_file, std::string type, int option )
//{

//    if( true && type.compare( "POLY" ) == 0 )
//        region.readsurfacemeshPOLY( (char *) mesh_file.c_str() );
//    else if( true && type.compare( "txt" ) == 0 )
//        region.readskeleton( (char *) mesh_file.c_str() );
//    else if( false )
//        getSurfaceFromCrossSection();

//    if( option == 1 )
//        region.unstructuredsurfacemesh();
//    else if( option == 2 )
//        region.cornerpointgrid();

//    emit updateMesh();

//    volumetric_ok = false;
//}


//void FlowVisualizationController::readParametersFile( const std::string& input_file, int option )
//{

//    region.userinput( (char *) input_file.c_str() );
//    getSurfaceFromCrossSection();


//    if( option == 1 )
//        region.unstructuredsurfacemesh();
//    else if( option == 2 )
//        region.cornerpointgrid();

//    emit updateMesh();

//    volumetric_ok = false;



//}



void FlowVisualizationController::updateMeshFromFile( Mesh* mesh )
{
   obj = region.getTetgenioObject();

   std::vector< float > vertices = getMeshVerticesfromFile( obj );
   std::vector< unsigned int > edges = getMeshWireFramefromFile( obj );
   std::vector< unsigned int > faces = getMeshFacesfromFile( obj );

   mesh->setMeshType( Mesh::TYPE::QUADRILATERAL );
   mesh->setVertices( vertices );
   mesh->setWireframe( edges );
   mesh->setFaces( faces );
   mesh->buildBoundingBox();

   mesh_ok = true;

}


void FlowVisualizationController::setCounterProgressinData()
{
    region.setProgressCounter( &counter );

    connect( &counter, SIGNAL( startProcess( const unsigned int&, const unsigned int& ) ), controllerParent, SLOT( startProgressBar( const unsigned int&, const unsigned int& ) ) );
    connect( &counter, SIGNAL( updateProcess( const unsigned int& ) ), controllerParent, SLOT( updateProgressBar( const unsigned int& ) ) );
}



void FlowVisualizationController::computeVolumetricMesh()
{

    counter.start( 0, 2 );

    region.unstructuredvolumemesh();

    counter.update( 1 );

    region.modelpreparation();

    counter.update( 2 );

    emit updateVolumetricMesh();


/*
    counter.start( 0, 2 );

    region.buildtetrahedralmesh();

    counter.update( 1 );

    region.flowpreparation();

    counter.update( 2 );

    emit updateVolumetricMesh();

    volumetric_ok = true;

*/

}


void FlowVisualizationController::updateVolumetricMesh( Mesh* mesh )
{

    std::vector< NODE > nodes = region.getnodelist();
    std::vector< TETRAHEDRON > elements = region.getelementlist();

    std::vector< float > vertices = getVolumeVerticesfromNodeStructure( nodes );
    std::vector< unsigned int > edges = getVolumeEdgesfromTetrahedronStructure( elements );
    std::vector< unsigned int > faces = getVolumeCellsfromTetrahedronStructure( elements );

    mesh->setMeshType( Mesh::TYPE::TETRAHEDRAL );
    mesh->setVertices( vertices );
    mesh->setWireframe( edges );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();


    volumetric_ok = true;

}


std::vector< float > FlowVisualizationController::getMeshVerticesfromFile( const tetgenio& obj )
{

    int number_of_points = obj.numberofpoints;
    double *points_list = obj.pointlist;

    std::vector< float > vertices;

    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( (float)points_list[ 3*i ] );
        vertices.push_back( (float)points_list[ 3*i + 1 ] );
        vertices.push_back( (float)points_list[ 3*i + 2 ] );
    }

    return vertices;

}


 std::vector< unsigned int > FlowVisualizationController::getMeshWireFramefromFile( const tetgenio& obj )
{

    std::vector< unsigned int > wireframe;

    int number_of_faces = obj.numberoffacets;
    tetgenio::facet* faces_list = obj.facetlist;

    for( int i = 0; i < number_of_faces; ++i )
    {
        tetgenio::facet f = faces_list[ i ];

        int number_of_polygons = f.numberofpolygons;
        tetgenio::polygon *polygons_list = f.polygonlist;

        for( int j = 0; j < number_of_polygons; ++j )
        {
            tetgenio::polygon p = polygons_list[ j ];
            int number_of_vertices = p.numberofvertices;
            int *vertices_list = p.vertexlist;

            if( number_of_vertices != 4 ) continue;

            wireframe.push_back( vertices_list[ 0 ] );
            wireframe.push_back( vertices_list[ 1 ] );

            wireframe.push_back( vertices_list[ 1 ] );
            wireframe.push_back( vertices_list[ 2 ] );

            wireframe.push_back( vertices_list[ 2 ] );
            wireframe.push_back( vertices_list[ 3 ] );

            wireframe.push_back( vertices_list[ 3 ] );
            wireframe.push_back( vertices_list[ 0 ] );

        }

    }

    return wireframe;

}


std::vector< unsigned int > FlowVisualizationController::getMeshFacesfromFile( const tetgenio& obj )
{

    std::vector< unsigned int > faces;

    int number_of_faces = obj.numberoffacets;
    tetgenio::facet* faces_list = obj.facetlist;

    for( int i = 0; i < number_of_faces; ++i )
    {
        tetgenio::facet f = faces_list[ i ];

        int number_of_polygons = f.numberofpolygons;
        tetgenio::polygon *polygons_list = f.polygonlist;

        for( int j = 0; j < number_of_polygons; ++j )
        {
            tetgenio::polygon p = polygons_list[ j ];

            int number_of_vertices = p.numberofvertices;
            int *vertices_list = p.vertexlist;

            if( number_of_vertices != 4 ) continue;

            for( int l = 0; l < number_of_vertices; ++l )
                faces.push_back( vertices_list[ l ] );
        }

    }


    return faces;

}


std::vector< float > FlowVisualizationController::getVolumeVerticesfromNodeStructure( const std::vector< NODE >& nodes )
{

    int number_of_points = (int)nodes.size();

    std::vector< float > vertices;
    for( int i = 0; i < number_of_points; ++i )
    {
        vertices.push_back( nodes[ i ].x() );
        vertices.push_back( nodes[ i ].y() );
        vertices.push_back( nodes[ i ].z() );
    }

    return vertices;

}


 std::vector< unsigned int > FlowVisualizationController::getVolumeEdgesfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements )
{

     int number_of_cells = (int)elements.size();

     std::vector< unsigned int > wireframe;

     for( int i = 0; i < number_of_cells; ++i )
     {
         TETRAHEDRON tet = elements[ i ];

         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 1 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 0 ) );

         wireframe.push_back( ( unsigned int ) tet.node( 3 ) );
         wireframe.push_back( ( unsigned int ) tet.node( 2 ) );
     }


    return wireframe;

}


std::vector< unsigned int > FlowVisualizationController::getVolumeCellsfromTetrahedronStructure( const std::vector< TETRAHEDRON >& elements )
{

    int number_of_cells = (int)elements.size();

    std::vector< unsigned int > faces;

    for( int i = 0; i < number_of_cells; ++i )
    {
        TETRAHEDRON tet = elements[ i ];

        faces.push_back( ( unsigned int ) tet.node( 0 ) );
        faces.push_back( ( unsigned int ) tet.node( 1 ) );
        faces.push_back( ( unsigned int ) tet.node( 2 ) );
        faces.push_back( ( unsigned int ) tet.node( 3 ) );
    }


    return faces;

}



void FlowVisualizationController::getSurfaceFromCrossSection( /*RRM::CrossSection<double>& _cross_section*/ )
{
//        int i, ny, j;
//        ny = 6; //extrude 6 layers of nodes; depth = 1

//        vector< int > nu, nv;
//        vector< double > positions;


//        int number_of_surfaces = _cross_section.numberofedges();


//        for (auto& curve_iterator : _cross_section.edges_){//now is number of surfaces

//            if (curve_iterator.second.is_boudary_ == false){ //only internal sketched

//                int nu_ = curve_iterator.second.segment.curve.size();

//                nu.push_back( nu_ );
//                nv.push_back( ny );


//                for (j = 0; j < ny; j++){//output is firstly x direction then y direction for each surface

//                    for (std::size_t it = 0; it < nu_; it++){

//                        double x = curve_iterator.second.segment.curve[it].x();
//                        double y = j;
//                        double z = curve_iterator.second.segment.curve[it].y();


//                        positions.push_back( x );
//                        positions.push_back( y );
//                        positions.push_back( z );

//                    }

//                }
//            }
//        }


//        region.readskeleton( number_of_surfaces, nu, nv, positions, 0 /* neighbor information missing*/ );
}


void FlowVisualizationController::computeFlowProperties()
{


    counter.start( 0, 3 );

    region.steadystateflowsolver();
    region.flowdiagnostics();

    counter.update( 3 );

    properties_computed = true;

    emit propertybyVertexComputed( "PRESSURE", "SCALAR" ) ;

    emit propertybyVertexComputed( "VELOCITY", "VECTOR" ) ;
    emit propertybyFaceComputed( "VELOCITY", "VECTOR" ) ;
    emit propertybyVertexComputed( "TOF", "SCALAR" ) ;
    emit propertybyFaceComputed( "TOF", "SCALAR" ) ;


/*
    counter.start( 0, 3 );

    computePressure();

    counter.update( 1 );

    computeVelocity();

    counter.update( 2 );

    computeTOF();

    counter.update( 3 );

    properties_computed = true;


*/
}


void FlowVisualizationController::computePressure()
{
//    region.computepressure();
}


void FlowVisualizationController::computeVelocity()
{
//    region.computevelocity();
}


void FlowVisualizationController::computeTOF()
{
//    region.flowdiagnostics();



}



std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;
    
    if( name_of_property.compare( "VELOCITY" ) == 0 )
    {
        type = "VECTOR";
        getVelocityValuesbyVertex( values );
    }
    else if ( name_of_property.compare( "PRESSURE" ) == 0 )
    {
        getPressureValuesbyVertex( values );
    }
    else if( name_of_property.compare( "TOF" ) == 0 )
        getTOFValuesbyVertex( values );
    
    
    std::vector< double > scalar_values;
    
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



std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( name_of_property.compare( "VELOCITY" ) == 0 )
    {
        type = "VECTOR";
        getVelocityValuesbyCell( values );
    }

    else if( name_of_property.compare( "TOF" ) == 0 )
        getTOFValuesbyCell( values );


    std::vector< double > scalar_values;

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
    std::vector< double > scalar_values;
    
    int number_of_vertices = (int)values.size()/3;

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


void FlowVisualizationController::setTetgenCommand( std::string& cmd )
{
    region.setTetgenCommand( cmd.c_str() );
}


void FlowVisualizationController::setViscosityValue( double value )
{
    region.setViscosityValue( value );
}


void FlowVisualizationController::setPermeabilityValues( int n, vector<double> &values )
{
//    std::size_t number_values = values.size();

//    for( std::size_t it = 0; it < number_values; ++it )
//        cout << values.at( it ) << endl;

    region.setPermeabilityValue( n, values );
}


void FlowVisualizationController::setBoundariesValues( int nb, vector< double > &values )
{
//    std::size_t number_values = values.size();

//    for( std::size_t it = 0; it < number_values; ++it )
//        cout << "d: " << values.at( it ) << endl;

    region.setBoundariesSurface( nb, values );
}

void FlowVisualizationController::setWellsValues( int nw,  vector< double > &values )
{
//    std::size_t number_values = values.size();

//    for( std::size_t it = 0; it < number_values; ++it )
//        cout << "d: " << values.at( it ) << endl;

    region.setWells( nw, values );
}


void FlowVisualizationController::setTofBoundaryValues( int n, vector< double > &values )
{
//    std::size_t number_values = n;//values.size()/2;

    vector< int > values_;
    int ntfsignal = values[ 0 ];


    for( std::size_t it = 0; it < n; ++it )
    {
        values_.push_back( (int)values.at( 2*it + 1 ) );
        values_.push_back( (int)values.at( 2*it + 2 ) );
    }

    region.setTofBoundaries( n, ntfsignal, values_ );
}


void FlowVisualizationController::setTrBoundaryValues(int n, std::vector< double > &values )
{
//    std::size_t number_values = n;//values.size()/3;

    int ntrsignal = values[ 0 ];

     vector< int > values_;

    for( std::size_t it = 0; it < n; ++it )
    {
        cout << "trb: " << values.at( 2*it + 1 ) << ", " << values.at( 2*it + 2 ) << endl;
        values_.push_back( (int)values.at( 2*it + 1) );
        values_.push_back( (int)values.at( 2*it + 2 ) );
    }

    region.setTracerBoundaries( n, ntrsignal, values_ );

}



void FlowVisualizationController::getVelocityValuesbyVertex( std::vector< double >& values )
{
    region.getNodesVelocity( values );
}


void FlowVisualizationController::getVelocityValuesbyCell( std::vector< double >& values )
{
    region.getElementalsVelocity( values );
}


void FlowVisualizationController::getPressureValuesbyVertex( std::vector< double >& values )
{
    region.getPressureValues( values );
}


void FlowVisualizationController::getTOFValuesbyVertex( std::vector< double >& values )
{
    region.getNodesTOF( values );
}


void FlowVisualizationController::getTOFValuesbyCell( std::vector< double >& values )
{
    region.getElementalsTOF( values );
}


void FlowVisualizationController::exportSurfacetoVTK( const std::string& filename )
{
    region.writeresult( (char *)filename.c_str() );
}


void FlowVisualizationController::exportVolumetoVTK( const std::string& filename )
{
    region.writevolumemesh( (char *)filename.c_str() );
}


void FlowVisualizationController::exportCornerPointtoVTK( const std::string& filename )
{
    region.writecornerpointgridVTK( (char *)filename.c_str() );
}


void FlowVisualizationController::clear()
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    region.clearregion();
}
