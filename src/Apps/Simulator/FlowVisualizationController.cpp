#include "FlowVisualizationController.h"


FlowVisualizationController::FlowVisualizationController( QWidget *parent )
{
    mesh_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    user_input_ok = false;

    current_method = MESHING_METHOD::UNSTRUCTURED;

}




void FlowVisualizationController::readPolyFiles( const std::string& mesh_file )
{

    if( mesh_ok == true ) return;

    region.readsurfacemeshPOLY( (char *) mesh_file.c_str() );
    emit updatePolyMesh();
    mesh_ok = true;
}


void FlowVisualizationController::readSkeletonFiles( const std::string& all_filename/*, std::vector< double >& positions, std::vector< unsigned int >& faces */ )
{

    if( mesh_ok == true ) return;

    std::vector< int > nu;
    std::vector< int > nv;
    std::vector< double > positions;

//    region.readskeleton( (char *) all_filename.c_str(), nu, nv, positions );

    region.readskeleton( (char *) all_filename.c_str() );
    region.getSkeleton( nu, nv, positions );

    std::vector< unsigned int > faces;
    faces = getSkeletonFaces( nu, nv, positions );
    if( faces.empty() == true ) return;


    mesh_ok = true;
    emit updateMesh( Mesh::TYPE::QUADRILATERAL, positions, faces );

}


void FlowVisualizationController::readInputParameters( const std::string& input_file )
{
    region.userinput( (char *) input_file.c_str() );
    user_input_ok = true;
}


void FlowVisualizationController::generateCornerPoint()
{


    if( mesh_ok == false || user_input_ok == false ) return;

    counter.start( 0, 2 );
    region.cornerpointgrid();
    counter.update( 1 );
    emit updateCornerPoint();
    counter.update( 2 );

    volumetric_ok = true;

}


void FlowVisualizationController::generateUnstructured()
{

    if( mesh_ok == false || user_input_ok == false ) return;


    counter.start( 0, 4 );
    region.unstructuredsurfacemesh();
    counter.update( 1 );
    region.unstructuredvolumemesh();
    counter.update( 2 );
    region.modelpreparation();
    counter.update( 3 );
    emit updateVolumetricMesh();
    counter.update( 4 );

    volumetric_ok = true;


}


void FlowVisualizationController::getMeshVisualizationParameters(std::string& trianglecmd, int& resolutiontype, int& npartitionedge, double& lenghtedge )
{
    region.getVisualizationParameters( trianglecmd, resolutiontype, npartitionedge, lenghtedge);
}


void FlowVisualizationController::setMeshVisualizationParameters( const std::string& trianglecmd, const int& resolutiontype, const int& npartitionedge, const double& lenghtedge )
{
    region.setVisualizationParameters( trianglecmd, resolutiontype, npartitionedge, lenghtedge);
}



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



}


void FlowVisualizationController::updateMeshFromSurface( Mesh* mesh )
{

    std::vector< float > vertices;
    std::vector< unsigned int > faces;

    region.getSurface( vertices, faces );

    mesh->setMeshType( Mesh::TYPE::TRIANGLES );
    mesh->setVertices( vertices );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();


}


void FlowVisualizationController::updateCornerPointFromSurface( Mesh* mesh )
{

    std::vector< float > vertices;
    std::vector< unsigned int > faces;
    std::vector< unsigned int > edges;

    region.getCornerPoint( vertices, edges, faces );

    mesh->setMeshType( Mesh::TYPE::QUADRILATERAL );
    mesh->setVertices( vertices );
    mesh->setWireframe( edges );
    mesh->setFaces( faces );
    mesh->buildBoundingBox();


}



void FlowVisualizationController::setCounterProgressinData()
{
    region.setProgressCounter( &counter );

    connect( &counter, SIGNAL( startProcess( const unsigned int&, const unsigned int& ) ), controllerParent, SLOT( startProgressBar( const unsigned int&, const unsigned int& ) ) );
    connect( &counter, SIGNAL( updateProcess( const unsigned int& ) ), controllerParent, SLOT( updateProgressBar( const unsigned int& ) ) );
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



void FlowVisualizationController::getSurfacesFromCrossSection( const CrossSection &_cross_section )
{

    CrossSection cross_section = _cross_section;


    std::vector< double > positions;
    vector< int > nu, nv;

    int extrusion_size = 3;
    int number_points = 0; /// Number of point per sketch
	int total_number_points = 0; 

    std::vector<unsigned int> number_edges_; // Number of Sketch Segments
    std::vector< float > colors;

    // Number of Internal Surface
    //int number_of_surfaces = cross_section.curves_history_.size();
	int number_of_surfaces = 0;

    float diagonal = std::pow((cross_section.viewPort_.first.x() - cross_section.viewPort_.second.x()), 2) + std::pow((cross_section.viewPort_.first.y() - cross_section.viewPort_.second.y()), 2);
    diagonal = std::sqrt(diagonal);



    // for (auto& history_iterator : cross_section.curves_history_)
    {

        number_points = 0;
        number_edges_.clear(); //number of surfaces

        for ( auto& edge_iterator : cross_section.edges_ )
        {
            //now is number of surfaces

            if ((edge_iterator.second.is_boundary_ == false) && (edge_iterator.second.is_visible_ == true))
				// && (edge_iterator.second.segment.curve_index == history_iterator.second.curve_index))
            {
                //only internal sketched
                number_edges_.push_back(edge_iterator.second.id_);
                edge_iterator.second.segment.curve.chaikinFilter(3);
                number_points += edge_iterator.second.segment.curve.size();

				nu.push_back(number_points);
				nv.push_back(extrusion_size);
				total_number_points += number_points*extrusion_size;
				number_points = 0;
				++number_of_surfaces;
            }
        }

//        number_points -= (number_edges_.size() - 1);


		float extrusion_step = (diagonal * 0.5);

/*
		int index = j*nu[k] + i + offset;

		double x = position[3 * index];
		double y = position[3 * index + 1];
		double z = position[3 * index + 2];
*/
		int index = 0; 
		int offset = 0; 
		
		positions.resize(3*total_number_points); 
		colors.resize(3*total_number_points);

            for (unsigned int id = 0; id < number_edges_.size(); id++)
            {
                //now is number of surfaces

                unsigned int edge_id = number_edges_[id];


                for (std::size_t it = 0; it < cross_section.edges_[edge_id].segment.curve.size(); it++)

				{

					for (int j = 0; j < extrusion_size; j++) //output is firstly x direction then y direction for each surface
					
					{

//                    his->curves_colors[edges_iterator.second.segment.curve_index] = Eigen::Vector3f(edges_iterator.second.r, edges_iterator.second.g, edges_iterator.second.b);

                    //positions.push_back( cross_section.edges_[edge_id].segment.curve[it].x() );
                    //positions.push_back( j + extrusion_step );
                    //positions.push_back( cross_section.edges_[edge_id].segment.curve[it].y() );

                    //colors.push_back( cross_section.edges_[edge_id].r );
                    //colors.push_back( cross_section.edges_[edge_id].g );
                    //colors.push_back( cross_section.edges_[edge_id].b );

					index = j*cross_section.edges_[edge_id].segment.curve.size() + it + offset; 

					positions[3*index +0] = (cross_section.edges_[edge_id].segment.curve[it].x());
					positions[3*index +1] = (j * extrusion_step);
					positions[3*index +2] = (cross_section.edges_[edge_id].segment.curve[it].y());

					colors[3*index +0] = (cross_section.edges_[edge_id].r);
					colors[3*index +1] = (cross_section.edges_[edge_id].g);
					colors[3*index +2] = (cross_section.edges_[edge_id].b);

					
                }
            }

			offset += cross_section.edges_[edge_id].segment.curve.size() * extrusion_size;

            //for (std::size_t it = 0; it < cross_section.edges_[number_edges_.back()].segment.curve.size(); it++)
            //{


            //    positions.push_back( (float) cross_section.edges_[number_edges_.back()].segment.curve[it].x() );
            //    positions.push_back( (float) j + extrusion_step );
            //    positions.push_back( (float) cross_section.edges_[number_edges_.back()].segment.curve[it].y() );

            //    colors.push_back( cross_section.edges_[number_edges_.back()].r );
            //    colors.push_back( cross_section.edges_[number_edges_.back()].g );
            //    colors.push_back( cross_section.edges_[number_edges_.back()].b );


            //}

            //extrusion_step += ( diagonal * 0.5);
        }

    }

    std::vector< unsigned int > faces;
    region.setSkeleton( number_of_surfaces, nu, nv, positions );
    faces = getSkeletonFaces( nu, nv, positions );

    if( faces.empty() == true ) return;

    mesh_ok = true;
    emit updateMesh( Mesh::TYPE::QUADRILATERAL, positions, faces );
    emit updateColors( colors );


}


std::vector< unsigned int > FlowVisualizationController::getSkeletonFaces( const std::vector< int >& nu_list, const std::vector< int >&nv_list, const std::vector< double >& positions )
{


    std::vector< unsigned int > faces;

    unsigned int number_lines_ = nu_list.size();
    int offset = 0;

    for( unsigned int k = 0; k < number_lines_; ++k  )
    {

        int nu_number = nu_list[ k ];
        int nv_number = nv_list[ k ];


        for(  unsigned int j = 0; j < nv_number - 1; ++j )
        {
            for( unsigned int i = 0; i < nu_number - 1 ; ++i )
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

    return faces;
}


void FlowVisualizationController::computeFlowProperties()
{

    if( volumetric_ok == false || mesh_ok == false ) return;


    counter.start( 0, 3 );
    region.steadystateflowsolver();
    counter.update( 1 );
    region.flowdiagnostics();
    counter.update( 2 );

    emit propertybyVertexComputed( "PRESSURE", "SCALAR" ) ;
    emit propertybyVertexComputed( "TOF", "SCALAR" ) ;
    emit propertybyVertexComputed( "TRACER", "SCALAR" ) ;

    emit propertybyFaceComputed( "PERMEABILITY", "SCALAR" ) ;
    emit propertybyFaceComputed( "VELOCITY", "VECTOR" ) ;


    counter.update( 3 );

    properties_computed = true;



    //    emit propertybyVertexComputed( "VELOCITY", "VECTOR" ) ;
    //    emit propertybyFaceComputed( "TOF", "SCALAR" ) ;
    //    emit propertybyFaceComputed( "TRACER", "SCALAR" ) ;


}



std::vector< double > FlowVisualizationController::getVerticesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;
    
    if( name_of_property.compare( "VELOCITY" ) == 0 )
    {
        type = "VECTOR";
        region.getNodesVelocity( values );
    }
    else if ( name_of_property.compare( "PRESSURE" ) == 0 )
    {
        region.getPressureValues( values );
    }

    else if( name_of_property.compare( "TOF" ) == 0 )
    {
        region.getNodesTOF( values );
    }
    else if( name_of_property.compare( "TRACER" ) == 0 )
    {
        region.getNodesTracer( values );
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



std::vector< double > FlowVisualizationController::getFacesPropertyValues( std::string name_of_property, std::string method, double& min, double& max )
{

    std::string type = "SCALAR";
    std::vector< double > values;

    if( name_of_property.compare( "VELOCITY" ) == 0 )
    {
        type = "VECTOR";
        region.getElementalsVelocity( values );
    }
    else if ( name_of_property.compare( "PERMEABILITY" ) == 0 )
    {
        region.getPermeabilityValues( values );
    }


    else if( name_of_property.compare( "TOF" ) == 0 )
    {
        region.getElementalsTOF( values );
    }

    else if( name_of_property.compare( "TRACER" ) == 0 ){
        region.getElementalsTracer( values );
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


void FlowVisualizationController::setTetgenCommand( std::string& cmd )
{
    region.setTetgenCommand( cmd.c_str() );
}


void FlowVisualizationController::setTriangleCommand( std::string& cmd )
{
    region.setTriangleCommand( cmd.c_str() );
}


void FlowVisualizationController::setBoundariesValues( int nb, vector< double > &values )
{

    region.setBoundariesSurface( nb, values );
}

void FlowVisualizationController::setWellsValues( int nw,  vector< double > &values )
{

    region.setWells( nw, values );
}


void FlowVisualizationController::setTofBoundaryValues( int n, vector< double > &values )
{

    vector< int > values_;
    int ntfsignal = values[ 0 ];


    for( std::size_t it = 0; it < n; ++it )
    {
        values_.push_back( (int)values.at( /*2**/it + 1 ) );
    }

    region.setTofBoundaries( n, ntfsignal, values_ );
}


void FlowVisualizationController::setTrBoundaryValues(int n, std::vector< double > &values )
{

    int ntrsignal = values[ 0 ];

     vector< int > values_;

    for( std::size_t it = 0; it < n; ++it )
    {
        values_.push_back( (int)values.at( /*2**/it + 1) );
    }

    region.setTracerBoundaries( n, ntrsignal, values_ );

}



void FlowVisualizationController::exportSurfacetoVTK( const std::string& filename )
{
    region.writesurfacemeshVTK( (char *)filename.c_str() );
}


void FlowVisualizationController::exportVolumetoVTK( const std::string& filename )
{
    region.writevolumemesh( (char *)filename.c_str() );
}


void FlowVisualizationController::exportCornerPointtoVTK( const std::string& filename )
{
    region.writecornerpointgridVTK( (char *)filename.c_str() );
}


void FlowVisualizationController::exportResultstoVTK( const std::string& filename )
{
    region.writeresult( (char*)filename.c_str() );
}


void FlowVisualizationController::increaseNumberofEdges()
{
    region.increaseNumberofEdges();
}


void FlowVisualizationController::decreaseNumberofEdges()
{
    region.decreaseNumberofEdges();
}


void FlowVisualizationController::increaseEdgeLength()
{
    region.increaseEdgeLength();
}


void FlowVisualizationController::decreaseEdgeLength()
{
    region.decreaseEdgeLength();
}


void FlowVisualizationController::clear()
{
    mesh_ok = false;
    user_input_ok = false;
    volumetric_ok = false;
    properties_computed = false;
    region.clearregion();
}
