#include "Mesh.h"


#include "Tucano/BoundingBox3.hpp"

Mesh::Mesh()
{
    show_vertices = true;
    show_edges = true;
    show_faces = true;
    show_bbox = true;

    apply_crosssection_clipping = false;
}


void Mesh::setVertices( const std::vector< float >& v )
{
    vertices.clear();
    std::copy( v.begin(), v.end(), std::back_inserter( vertices ) );
}


void Mesh::setVertice( int id, float x, float y, float z ){

    if ( isValidVertice( id ) == false ) return;

    vertices[ 3*id ] = x ;
    vertices[ 3*id + 1 ] = y ;
    vertices[ 3*id + 2 ] = z ;

}


void Mesh::getVertice( int id, float& x, float& y, float& z )
{

    if ( isValidVertice( id ) == false ) return;

    x = vertices[ 3*id ];
    y = vertices[ 3*id + 1 ];
    z = vertices[ 3*id + 2 ];
}


void Mesh::setEdge( int id, const std::vector< unsigned int >& e )
{

    if( isValidEdge( id ) == false ) return;

    for( int i = 0; i < 2; ++i )
    {
        wireframe[ 2*i + i ] = e[ i ];
    }

}


void Mesh::setWireframe( const std::vector< unsigned int >& w )
{
    wireframe.clear();
    std::copy( w.begin(), w.end(), std::back_inserter( wireframe ) );
}


void Mesh::setFace( int id, const std::vector< unsigned int >& f )
{

    if( isValidFace( id ) == false ) return;

    int vertices_number = getNumberofVerticesbyFace();
    for( int i = 0; i < vertices_number; ++i )
    {
        faces[ vertices_number*id + i ] = f[ i ];
    }

}


void Mesh::setFaces( const std::vector< unsigned int >& f ){

    faces.clear();
    std::copy( f.begin(), f.end(), std::back_inserter( faces ) );

}


std::vector< unsigned int > Mesh::getFace( int id )
{


    std::vector< unsigned int > f;

    if( isValidFace( id ) == false ) return f;

    int vertices_number = getNumberofVerticesbyFace();
    f.resize( vertices_number );

    for( int i = 0; i < vertices_number; ++i )
    {
        f[ i ] = faces[ vertices_number*id + i ];
    }

    return f;

}


void Mesh::setConstantColor( const float r, const float g, const float b )
{
    int number_of_vertices = getNumberofVertices();

//    std::vector <float> color;
    colors.clear();
    colors.resize( 3*number_of_vertices );

    for( int i = 0; i < number_of_vertices; ++i )
    {
        colors[ 3*i ] = r;
        colors[ 3*i + 1 ] = g;
        colors[ 3*i + 2 ] = b;
    }



    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
    glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );

}


void Mesh::setColor( const std::vector< float >& vcolors  )
{

    colors = vcolors;
    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
    glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );

}


void Mesh::setCrossSectionClippingEquation( const float& a, const float& b, const float& c, const float& d )
{

    apply_crosssection_clipping = true;

    coefACrossSectionEquation = a;
    coefBCrossSectionEquation = b;
    coefCCrossSectionEquation = c;
    coefDCrossSectionEquation = d;
}


void Mesh::disableCrossSection()
{
    apply_crosssection_clipping = false;

}


bool Mesh::isValidVertice( const int id )
{
    int vertices_number = getNumberofVertices();

    if( id < 0 || id >= vertices_number ) return false;

    return true;

}


bool Mesh::isValidEdge( const int id )
{
    int edges_number = getNumberofEdges();

    if( id < 0 || id >= edges_number ) return false;

    return true;

}


bool Mesh::isValidFace( const int id )
{
    int faces_number = getNumberofFaces();

    if( id < 0 || id >= faces_number ) return false;

    return true;

}


int Mesh::getNumberofVerticesbyFace()
{

    int vertices_number = 3;

    if ( mesh_type == TYPE::QUADRILATERAL )
    {
        vertices_number = 4;
    }
    else if ( mesh_type == TYPE::TETRAHEDRAL )
    {
        vertices_number = 4;
    }
    else if ( mesh_type == TYPE::HEXAHEDRAL )
    {
        vertices_number = 8;
    }

    return vertices_number;

}



void Mesh::buildTrianglesFacesList( vector< unsigned int >& triangles )
{

    if( mesh_type == TYPE::QUADRILATERAL )
        getTrianglesfromRectangles( triangles  );
    else if( mesh_type == TYPE::TETRAHEDRAL )
        getTrianglesfromTetrahedros( triangles  );

}


void Mesh::buildBoundingBox()
{

    if( vertices.empty() == true ) return;

    number_of_vertices = getNumberofVertices();

    vector< Eigen::Vector3f > normalized_vertices;
    for( unsigned int it = 0; it < number_of_vertices; ++it )
        normalized_vertices.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

    Celer::BoundingBox3< float > bbox_mesh;
    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


	std::cout << "Min - Max " << std::endl;
	std::cout << bbox_mesh.Min() << std::endl;
	std::cout << bbox_mesh.Max() << std::endl;


    for( unsigned int it = 0; it < number_of_vertices; ++it )
        normalized_vertices[ it ] = ( normalized_vertices[ it ] - bbox_mesh.center() )/bbox_mesh.diagonal();


    max[ 0 ] = normalized_vertices[ 0 ]( 0 );
    min[ 0 ] = normalized_vertices[ 0 ]( 0 );

    max[ 1 ] = normalized_vertices[ 0 ]( 1 );
    min[ 1 ] = normalized_vertices[ 0 ]( 1 );

    max[ 2 ] = normalized_vertices[ 0 ]( 2 );
    min[ 2 ] = normalized_vertices[ 0 ]( 2 );

    for( unsigned int i = 1; i < number_of_vertices; i++ )
    {
        if( normalized_vertices[ i ]( 0 ) > max[ 0 ] ) max[ 0 ] = normalized_vertices[ i ]( 0 );
        if( normalized_vertices[ i ]( 0 ) < min[ 0 ] ) min[ 0 ] = normalized_vertices[ i ]( 0 );

        if( normalized_vertices[ i ]( 1 ) > max[ 1 ] ) max[ 1 ] = normalized_vertices[ i ]( 1 );
        if( normalized_vertices[ i ]( 1 ) < min[ 1 ] ) min[ 1 ] = normalized_vertices[ i ]( 1 );

        if( normalized_vertices[ i ]( 2 ) > max[ 2 ] ) max[ 2 ] = normalized_vertices[ i ]( 2 );
        if( normalized_vertices[ i ]( 2 ) < min[ 2 ] ) min[ 2 ] = normalized_vertices[ i ]( 2 );

    }

    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );

    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );

    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );


    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
//fim de 3)


    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );


    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
//fim de 5)

    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );


    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
//fim de 7)


    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );



    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( max[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( max[ 2 ] );


    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( max[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );


    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( min[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
    bounding_box.push_back( min[ 0 ] );
    bounding_box.push_back( max[ 1 ] );
    bounding_box.push_back( min[ 2 ] );
}


void Mesh::getBoundingBox( float &xm, float &xM, float &ym, float &yM, float &zm, float &zM )
{
    xm = min[ 0 ];
    xM = max[ 0 ];
    ym = min[ 1 ];
    yM = max[ 1 ];
    zm = min[ 2 ];
    zM = max[ 2 ];
}


void Mesh::getTrianglesfromRectangles( vector< unsigned int >& triangles  )
{

    std::size_t nquads = faces.size()/4;

    triangles.clear();

    for( int i = 0; i < (int) nquads; ++i )
    {
        triangles.push_back( ( unsigned int )faces[ 4*i ] );
        triangles.push_back( ( unsigned int )faces[ 4*i + 1 ] );
        triangles.push_back( ( unsigned int )faces[ 4*i + 2 ] );

        triangles.push_back( ( unsigned int )faces[ 4*i + 2 ] );
        triangles.push_back( ( unsigned int )faces[ 4*i + 3 ] );
        triangles.push_back( ( unsigned int )faces[ 4*i ] );
    }

}


void Mesh::getTrianglesfromTetrahedros( vector< unsigned int >& triangles  )
{

    std::size_t nquads = faces.size()/4;

    triangles.clear();

    for( int i = 0; i < (int)nquads; ++i )
    {

        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );

    }

}


void Mesh::buildWireframeList()
{

    if( mesh_type == TYPE::TRIANGLES )
        getWireframeofTriangles();
    else if( mesh_type == TYPE::QUADRILATERAL )
        getWireframeofQuadrilaterals();

}


void Mesh::getWireframeofTriangles()
{
    if( faces.size() < 3 ) return;

    int nfaces = faces.size()/3;

    for( int i = 0; i < nfaces; i++ )
    {

        wireframe.push_back( faces[ 3*i ] );
        wireframe.push_back( faces[ 3*i + 1 ] );

        wireframe.push_back( faces[ 3*i + 1 ] );
        wireframe.push_back( faces[ 3*i + 2 ] );

        wireframe.push_back( faces[ 3*i + 2 ] );
        wireframe.push_back( faces[ 3*i ] );

    }

}


void Mesh::getWireframeofQuadrilaterals()
{
    if( faces.size() < 4 ) return;

    int nfaces = faces.size()/4;

    for( int i = 0; i < nfaces; ++i )
    {

        wireframe.push_back( faces[ 4*i ] );
        wireframe.push_back( faces[ 4*i + 1 ] );

        wireframe.push_back( faces[ 4*i + 1 ] );
        wireframe.push_back( faces[ 4*i + 2 ] );

        wireframe.push_back( faces[ 4*i + 2 ] );
        wireframe.push_back( faces[ 4*i + 3 ] );

        wireframe.push_back( faces[ 4*i + 3 ] );
        wireframe.push_back( faces[ 4*i ] );

    }
}



void Mesh::showVertices( bool status )
{
    show_vertices = status;
}


void Mesh::showEdges( bool status )
{
    show_edges = status;
}


void Mesh::showFaces( bool status )
{
    show_faces = status;
}



bool Mesh::showVertices() const
{
    return show_vertices;
}


bool Mesh::showEdges() const
{
    return show_edges;
}


bool Mesh::showFaces() const
{
    return show_faces;
}


void Mesh::showBoundingBox( bool status )
{
    show_bbox = status;
}


bool Mesh::showBoundingBox() const
{
    return show_bbox;
}

void Mesh::reloadShader()
{
    shader_mesh->reloadShaders();
}

void Mesh::initializeShader( std::string directory )
{


    //shader_mesh = new Tucano::Shader( "shader_mesh", ( directory + "shaders/vertex_mesh_shader.vert" ),
    //                                                 ( directory + "shaders/fragment_mesh_shader.frag" ),
    //                                                  "", "", "" ) ;
    //shader_mesh->initialize();

	shader_mesh = new Tucano::Shader( "shader_mesh", ( directory + "shaders/FlowDefault.vert" ),
													 (directory + "shaders/FlowDefault.frag"),
													 (directory + "shaders/FlowDefault.geom"),
	                                                  "", "") ;
	shader_mesh->initialize();

        //shader_mesh = new Tucano::Shader( "shader_mesh", ( "D:\\Workspace\\RRM\\files\\shaders\\FlowDefault.vert" ),
								//						 ("D:\\Workspace\\RRM\\files\\shaders\\FlowDefault.frag"),
        //                                                 ( "D:\\Workspace\\RRM\\files\\shaders\\FlowDefault.geom" ),
        //                                                  "", "") ;
        //shader_mesh->initialize();

    glGenVertexArrays( 1, &va_mesh );
    glBindVertexArray( va_mesh );


    glGenBuffers( 1, &bf_vertices_mesh );
    glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_mesh );
    glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 0 );


    glGenBuffers( 1, &bf_faces_mesh );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );

    glGenBuffers( 1, &bf_wireframe_mesh );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );

    glGenBuffers( 1, &bf_colors_mesh );
    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
    glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 2 );



    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glBindVertexArray( 0 );


    shader_bbox = new Tucano::Shader(  "shader_bbox", ( directory + "shaders/vertex_bbox_shader.vert" ),
                                                      ( directory + "shaders/fragment_bbox_shader.frag" ), "", "", "" );
    shader_bbox->initialize();

    glGenVertexArrays( 1, &va_bbox );
    glBindVertexArray( va_bbox );

        glGenBuffers( 1, &bf_bbox_mesh );
        glBindBuffer( GL_ARRAY_BUFFER, bf_bbox_mesh );
        glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
        glEnableVertexAttribArray( 0 );


    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

}


void Mesh::load()
{


    if( vertices.empty() == true ) return;

    number_of_vertices = getNumberofVertices();
    int vector_vertices_size = number_of_vertices * 3;


    vector< Eigen::Vector3f > normalized_vertices;
    for( unsigned int it = 0; it < number_of_vertices; ++it )
        normalized_vertices.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

    Celer::BoundingBox3< float > bbox_mesh;
    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


    for( unsigned int it = 0; it < number_of_vertices; ++it )
        normalized_vertices[ it ] = ( normalized_vertices[ it ] - bbox_mesh.center() )/bbox_mesh.diagonal();

    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


    glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_mesh );
    glBufferData( GL_ARRAY_BUFFER, vector_vertices_size*sizeof( GLfloat ), normalized_vertices.data(), GL_STATIC_DRAW );



    vector< unsigned int > triangles;

    if( mesh_type != TYPE::TRIANGLES )
        buildTrianglesFacesList( triangles );
    else
        triangles = faces;


    if( triangles.empty() == false )
    {
        vector_triangles_size = (int) triangles.size();

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vector_triangles_size*sizeof( GLuint ), triangles.data(), GL_STATIC_DRAW );
    }


    if( wireframe.empty() == true )
        buildWireframeList();


    if( wireframe.empty() == false )
    {
        vector_wireframe_size = (int) wireframe.size();

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vector_wireframe_size*sizeof( GLuint ), wireframe.data(), GL_STATIC_DRAW );
    }

    if( bounding_box.empty() == false )
    {
        number_lines_bbox = bounding_box.size();

        glBindBuffer( GL_ARRAY_BUFFER, bf_bbox_mesh );
        glBufferData( GL_ARRAY_BUFFER, bounding_box.size()*sizeof( GLfloat ), bounding_box.data(), GL_STATIC_DRAW );
    }


    if( colors.empty() == false )
    {
        glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
        glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );
    }

}


void Mesh::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const float& scale , const float width, const float height )
{

    if( vertices.empty() == true ) return;

    glPointSize( 5.0f );

    Eigen::Affine3f M;
    M.setIdentity();

    shader_mesh->bind();
    //shader_mesh->setUniform( "mmatrix", M );
    //shader_mesh->setUniform( "vmatrix", V );
    //shader_mesh->setUniform( "pmatrix", P );
    //shader_mesh->setUniform( "scale", scale );

	shader_mesh->setUniform("WIN_SCALE", width, height);

	shader_mesh->setUniform("ModelMatrix", Eigen::Affine3f::Identity().data(), 4, GL_FALSE, 1);
	shader_mesh->setUniform("ViewMatrix", V.data(), 4, GL_FALSE, 1);
	shader_mesh->setUniform("ProjectionMatrix", P.data(), 4, GL_FALSE, 1);

    glBindVertexArray( va_mesh );


    if( apply_crosssection_clipping == true )
    {
        glEnable(GL_CLIP_DISTANCE0);
        shader_mesh->setUniform( "ClipPlane", coefACrossSectionEquation, coefBCrossSectionEquation, coefCCrossSectionEquation, coefDCrossSectionEquation );

    }
  //  else
  //  {
  //      shader_mesh->setUniform( "ClipPlane", 0.0f, 0.0f, 0.0f, 0.0f );
  //      glDisable(GL_CLIP_DISTANCE0);
  //  }

  //  if( show_vertices == true )
  //  {
  //      glDrawArrays( GL_POINTS, 0, number_of_vertices );
  //  }
  //  if( show_edges == true )
  //  {
  //      glLineWidth( 2.0f );
  //      shader_mesh->setUniform( "edge", GL_TRUE );
		//shader_mesh->setUniform("index", 6);


  //      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
  //      glDrawElements( GL_LINES, vector_wireframe_size, GL_UNSIGNED_INT, 0 );

  //      shader_mesh->setUniform( "edge", GL_FALSE );
		//shader_mesh->setUniform("index", 1);

  //  }
    if( show_faces == true )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
        glDrawElements( GL_TRIANGLES, vector_triangles_size, GL_UNSIGNED_INT, 0 );
    }

    glBindVertexArray( 0 );



    shader_mesh->unbind();



    if( show_bbox == true )
    {

        glLineWidth( 4.0f );


        shader_bbox->bind();
        shader_bbox->setUniform( "mmatrix", M );
        shader_bbox->setUniform( "vmatrix", V );
        shader_bbox->setUniform( "pmatrix", P );
        shader_bbox->setUniform( "scale", 1.0 );

        glBindVertexArray( va_bbox );

            glDrawArrays( GL_LINES, 0, number_lines_bbox );

        glBindVertexArray( 0 );

    }

    shader_bbox->unbind();
}


void Mesh::clear()
{
    mesh_type = TYPE::TRIANGLES;

    vertices.clear();
    faces.clear();
    wireframe.clear();
    normals_by_vertices.clear();
    bounding_box.clear();


    max[ 0 ] = 0.0f;
    max[ 1 ] = 0.0f;
    max[ 2 ] = 0.0f;

    min[ 0 ] = 0.0f;
    min[ 1 ] = 0.0f;
    min[ 2 ] = 0.0f;


    show_vertices = true;
    show_edges = false;
    show_faces = true;
    show_bbox = true;

    apply_crosssection_clipping = false;

    coefACrossSectionEquation = 0.0f;
    coefBCrossSectionEquation = 0.0f;
    coefCCrossSectionEquation = 0.0f;
    coefDCrossSectionEquation = 0.0f;
}

void Mesh::resetBuffers()
{
    deleteShaders();

    if( va_mesh )
    {
        glDeleteVertexArrays( 1, &va_mesh );
        if( bf_vertices_mesh )
            glDeleteBuffers(1, &bf_vertices_mesh);
        if( bf_faces_mesh )
            glDeleteBuffers(1, &bf_faces_mesh);
        if( bf_wireframe_mesh )
            glDeleteBuffers(1, &bf_wireframe_mesh);
        if( bf_colors_mesh )
            glDeleteBuffers(1, &bf_colors_mesh);

    }

    if( va_bbox )
    {
        glDeleteVertexArrays( 1, &va_bbox );
        if( bf_bbox_mesh )
            glDeleteBuffers(1, &bf_bbox_mesh);
    }


    va_mesh = 0;
    bf_vertices_mesh = 0;
    bf_faces_mesh = 0;
    bf_wireframe_mesh = 0;
    bf_colors_mesh = 0;

    number_of_vertices = 0;
    vector_triangles_size = 0;
    vector_wireframe_size = 0;

    va_bbox = 0;
    bf_bbox_mesh = 0;
    number_lines_bbox = 0;

}

void Mesh::deleteShaders()
{

    if (shader_mesh)
    {
        delete (shader_mesh);
        shader_mesh = nullptr;
    }

    if (shader_bbox)
    {
        delete(shader_bbox);
        shader_bbox = nullptr;
    }

}




//#include "Mesh.h"


//#include "Tucano/BoundingBox3.hpp"

//Mesh::Mesh()
//{
//    show_vertices = true;
//    show_edges = true;
//    show_faces = true;
//    show_bbox = true;

//    apply_crosssection_clipping = false;
//}


//void Mesh::setVertices( const std::vector< float >& v )
//{
//    vertices.clear();
//    std::copy( v.begin(), v.end(), std::back_inserter( vertices ) );
//}


//void Mesh::setVertice( int id, float x, float y, float z ){

//    if ( isValidVertice( id ) == false ) return;

//    vertices[ 3*id ] = x ;
//    vertices[ 3*id + 1 ] = y ;
//    vertices[ 3*id + 2 ] = z ;

//}


//void Mesh::getVertice( int id, float& x, float& y, float& z )
//{

//    if ( isValidVertice( id ) == false ) return;

//    x = vertices[ 3*id ];
//    y = vertices[ 3*id + 1 ];
//    z = vertices[ 3*id + 2 ];
//}


//void Mesh::setEdge( int id, const std::vector< unsigned int >& e )
//{

//    if( isValidEdge( id ) == false ) return;

//    for( int i = 0; i < 2; ++i )
//    {
//        wireframe[ 2*i + i ] = e[ i ];
//    }

//}


//void Mesh::setWireframe( const std::vector< unsigned int >& w )
//{
//    wireframe.clear();
//    std::copy( w.begin(), w.end(), std::back_inserter( wireframe ) );
//}


//void Mesh::setFace( int id, const std::vector< unsigned int >& f )
//{

//    if( isValidFace( id ) == false ) return;

//    int vertices_number = getNumberofVerticesbyFace();
//    for( int i = 0; i < vertices_number; ++i )
//    {
//        faces[ vertices_number*id + i ] = f[ i ];
//    }

//}


//void Mesh::setFaces( const std::vector< unsigned int >& f ){

//    faces.clear();
//    std::copy( f.begin(), f.end(), std::back_inserter( faces ) );

//}


//std::vector< unsigned int > Mesh::getFace( int id )
//{


//    std::vector< unsigned int > f;

//    if( isValidFace( id ) == false ) return f;

//    int vertices_number = getNumberofVerticesbyFace();
//    f.resize( vertices_number );

//    for( int i = 0; i < vertices_number; ++i )
//    {
//        f[ i ] = faces[ vertices_number*id + i ];
//    }

//    return f;

//}


//void Mesh::setConstantColor( const float r, const float g, const float b )
//{
//    int number_of_vertices = getNumberofVertices();

////    std::vector <float> color;
//    colors.clear();
//    colors.resize( 3*number_of_vertices );

//    for( int i = 0; i < number_of_vertices; ++i )
//    {
//        colors[ 3*i ] = r;
//        colors[ 3*i + 1 ] = g;
//        colors[ 3*i + 2 ] = b;
//    }



//    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
//    glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );

//}


//void Mesh::setColor( const std::vector< float >& vcolors  )
//{

//    colors = vcolors;
//    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
//    glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );

//}


//void Mesh::setCrossSectionClippingEquation( const float& a, const float& b, const float& c, const float& d )
//{

//    apply_crosssection_clipping = true;

//    coefACrossSectionEquation = a;
//    coefBCrossSectionEquation = b;
//    coefCCrossSectionEquation = c;
//    coefDCrossSectionEquation = d;
//}


//void Mesh::disableCrossSection()
//{
//    apply_crosssection_clipping = false;

//}


//bool Mesh::isValidVertice( const int id )
//{
//    int vertices_number = getNumberofVertices();

//    if( id < 0 || id >= vertices_number ) return false;

//    return true;

//}


//bool Mesh::isValidEdge( const int id )
//{
//    int edges_number = getNumberofEdges();

//    if( id < 0 || id >= edges_number ) return false;

//    return true;

//}


//bool Mesh::isValidFace( const int id )
//{
//    int faces_number = getNumberofFaces();

//    if( id < 0 || id >= faces_number ) return false;

//    return true;

//}


//int Mesh::getNumberofVerticesbyFace()
//{

//    int vertices_number = 3;

//    if ( mesh_type == TYPE::QUADRILATERAL )
//    {
//        vertices_number = 4;
//    }
//    else if ( mesh_type == TYPE::TETRAHEDRAL )
//    {
//        vertices_number = 4;
//    }
//    else if ( mesh_type == TYPE::HEXAHEDRAL )
//    {
//        vertices_number = 8;
//    }

//    return vertices_number;

//}



//void Mesh::buildTrianglesFacesList( vector< unsigned int >& triangles )
//{

//    if( mesh_type == TYPE::QUADRILATERAL )
//        getTrianglesfromRectangles( triangles  );
//    else if( mesh_type == TYPE::TETRAHEDRAL )
//        getTrianglesfromTetrahedros( triangles  );

//}


//void Mesh::buildBoundingBox()
//{

//    if( vertices.empty() == true ) return;

//    number_of_vertices = getNumberofVertices();

//    vector< Eigen::Vector3f > normalized_vertices;
//    for( unsigned int it = 0; it < number_of_vertices; ++it )
//        normalized_vertices.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

//    Celer::BoundingBox3< float > bbox_mesh;
//    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


//    for( unsigned int it = 0; it < number_of_vertices; ++it )
//        normalized_vertices[ it ] = ( normalized_vertices[ it ] - bbox_mesh.center() )/bbox_mesh.diagonal();


//    max[ 0 ] = normalized_vertices[ 0 ]( 0 );
//    min[ 0 ] = normalized_vertices[ 0 ]( 0 );

//    max[ 1 ] = normalized_vertices[ 0 ]( 1 );
//    min[ 1 ] = normalized_vertices[ 0 ]( 1 );

//    max[ 2 ] = normalized_vertices[ 0 ]( 2 );
//    min[ 2 ] = normalized_vertices[ 0 ]( 2 );

//    for( unsigned int i = 1; i < number_of_vertices; i++ )
//    {
//        if( normalized_vertices[ i ]( 0 ) > max[ 0 ] ) max[ 0 ] = normalized_vertices[ i ]( 0 );
//        if( normalized_vertices[ i ]( 0 ) < min[ 0 ] ) min[ 0 ] = normalized_vertices[ i ]( 0 );

//        if( normalized_vertices[ i ]( 1 ) > max[ 1 ] ) max[ 1 ] = normalized_vertices[ i ]( 1 );
//        if( normalized_vertices[ i ]( 1 ) < min[ 1 ] ) min[ 1 ] = normalized_vertices[ i ]( 1 );

//        if( normalized_vertices[ i ]( 2 ) > max[ 2 ] ) max[ 2 ] = normalized_vertices[ i ]( 2 );
//        if( normalized_vertices[ i ]( 2 ) < min[ 2 ] ) min[ 2 ] = normalized_vertices[ i ]( 2 );

//    }

//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );

//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );

//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );


//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
////fim de 3)


//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );


//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
////fim de 5)

//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );


//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
////fim de 7)


//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );



//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( max[ 2 ] );


//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( max[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );


//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( min[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//    bounding_box.push_back( min[ 0 ] );
//    bounding_box.push_back( max[ 1 ] );
//    bounding_box.push_back( min[ 2 ] );
//}


//void Mesh::getBoundingBox( float &xm, float &xM, float &ym, float &yM, float &zm, float &zM )
//{
//    xm = min[ 0 ];
//    xM = max[ 0 ];
//    ym = min[ 1 ];
//    yM = max[ 1 ];
//    zm = min[ 2 ];
//    zM = max[ 2 ];
//}


//void Mesh::getTrianglesfromRectangles( vector< unsigned int >& triangles  )
//{

//    std::size_t nquads = faces.size()/4;

//    triangles.clear();

//    for( int i = 0; i < (int) nquads; ++i )
//    {
//        triangles.push_back( ( unsigned int )faces[ 4*i ] );
//        triangles.push_back( ( unsigned int )faces[ 4*i + 1 ] );
//        triangles.push_back( ( unsigned int )faces[ 4*i + 2 ] );

//        triangles.push_back( ( unsigned int )faces[ 4*i + 2 ] );
//        triangles.push_back( ( unsigned int )faces[ 4*i + 3 ] );
//        triangles.push_back( ( unsigned int )faces[ 4*i ] );
//    }

//}


//void Mesh::getTrianglesfromTetrahedros( vector< unsigned int >& triangles  )
//{

//    std::size_t nquads = faces.size()/4;

//    triangles.clear();

//    for( int i = 0; i < (int)nquads; ++i )
//    {

//        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

//        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

//        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 3 ] );

//        triangles.push_back( ( unsigned int ) faces[ 4*i ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 1 ] );
//        triangles.push_back( ( unsigned int ) faces[ 4*i + 2 ] );

//    }

//}


//void Mesh::buildWireframeList()
//{

//    if( mesh_type == TYPE::TRIANGLES )
//        getWireframeofTriangles();
//    else if( mesh_type == TYPE::QUADRILATERAL )
//        getWireframeofQuadrilaterals();

//}


//void Mesh::getWireframeofTriangles()
//{
//    if( faces.size() < 3 ) return;

//    int nfaces = faces.size()/3;

//    for( int i = 0; i < nfaces; i++ )
//    {

//        wireframe.push_back( faces[ 3*i ] );
//        wireframe.push_back( faces[ 3*i + 1 ] );

//        wireframe.push_back( faces[ 3*i + 1 ] );
//        wireframe.push_back( faces[ 3*i + 2 ] );

//        wireframe.push_back( faces[ 3*i + 2 ] );
//        wireframe.push_back( faces[ 3*i ] );

//    }

//}


//void Mesh::getWireframeofQuadrilaterals()
//{
//    if( faces.size() < 4 ) return;

//    int nfaces = faces.size()/4;

//    for( int i = 0; i < nfaces; ++i )
//    {

//        wireframe.push_back( faces[ 4*i ] );
//        wireframe.push_back( faces[ 4*i + 1 ] );

//        wireframe.push_back( faces[ 4*i + 1 ] );
//        wireframe.push_back( faces[ 4*i + 2 ] );

//        wireframe.push_back( faces[ 4*i + 2 ] );
//        wireframe.push_back( faces[ 4*i + 3 ] );

//        wireframe.push_back( faces[ 4*i + 3 ] );
//        wireframe.push_back( faces[ 4*i ] );

//    }
//}



//void Mesh::showVertices( bool status )
//{
//    show_vertices = status;
//}


//void Mesh::showEdges( bool status )
//{
//    show_edges = status;
//}


//void Mesh::showFaces( bool status )
//{
//    show_faces = status;
//}



//bool Mesh::showVertices() const
//{
//    return show_vertices;
//}


//bool Mesh::showEdges() const
//{
//    return show_edges;
//}


//bool Mesh::showFaces() const
//{
//    return show_faces;
//}


//void Mesh::showBoundingBox( bool status )
//{
//    show_bbox = status;
//}


//bool Mesh::showBoundingBox() const
//{
//    return show_bbox;
//}


//void Mesh::initializeShader( std::string directory )
//{


//    shader_mesh = new Tucano::Shader( "shader_mesh", ( directory + "shaders/vertex_mesh_shader.vert" ),
//                                                     ( directory + "shaders/fragment_mesh_shader.frag" ),
//                                                      "", "", "" ) ;
//    shader_mesh->initialize();


//    glGenVertexArrays( 1, &va_mesh );
//    glBindVertexArray( va_mesh );


//    glGenBuffers( 1, &bf_vertices_mesh );
//    glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_mesh );
//    glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
//    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//    glEnableVertexAttribArray( 0 );


//    glGenBuffers( 1, &bf_faces_mesh );
//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
//    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );

//    glGenBuffers( 1, &bf_wireframe_mesh );
//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
//    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );

//    glGenBuffers( 1, &bf_colors_mesh );
//    glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
//    glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
//    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//    glEnableVertexAttribArray( 1 );



//    glBindBuffer( GL_ARRAY_BUFFER, 0 );
//    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

//    glBindVertexArray( 0 );


//    shader_bbox = new Tucano::Shader(  "shader_bbox", ( directory + "shaders/vertex_bbox_shader.vert" ),
//                                                      ( directory + "shaders/fragment_bbox_shader.frag" ), "", "", "" );
//    shader_bbox->initialize();

//    glGenVertexArrays( 1, &va_bbox );
//    glBindVertexArray( va_bbox );

//        glGenBuffers( 1, &bf_bbox_mesh );
//        glBindBuffer( GL_ARRAY_BUFFER, bf_bbox_mesh );
//        glBufferData( GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );
//        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
//        glEnableVertexAttribArray( 0 );


//    glBindBuffer( GL_ARRAY_BUFFER, 0 );
//    glBindVertexArray( 0 );

//}


//void Mesh::load()
//{


//    if( vertices.empty() == true ) return;

//    number_of_vertices = getNumberofVertices();
//    int vector_vertices_size = number_of_vertices * 3;


//    vector< Eigen::Vector3f > normalized_vertices;
//    for( unsigned int it = 0; it < number_of_vertices; ++it )
//        normalized_vertices.push_back( Eigen::Vector3f( vertices[ 3*it ], vertices[ 3*it + 1 ], vertices[ 3*it + 2 ] ) );

//    Celer::BoundingBox3< float > bbox_mesh;
//    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


//    for( unsigned int it = 0; it < number_of_vertices; ++it )
//        normalized_vertices[ it ] = ( normalized_vertices[ it ] - bbox_mesh.center() )/bbox_mesh.diagonal();

//    bbox_mesh.fromPointCloud( normalized_vertices.begin(), normalized_vertices.end() );


//    glBindBuffer( GL_ARRAY_BUFFER, bf_vertices_mesh );
//    glBufferData( GL_ARRAY_BUFFER, vector_vertices_size*sizeof( GLfloat ), normalized_vertices.data(), GL_STATIC_DRAW );



//    vector< unsigned int > triangles;

//    if( mesh_type != TYPE::TRIANGLES )
//        buildTrianglesFacesList( triangles );
//    else
//        triangles = faces;


//    if( triangles.empty() == false )
//    {
//        vector_triangles_size = (int) triangles.size();

//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
//        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vector_triangles_size*sizeof( GLuint ), triangles.data(), GL_STATIC_DRAW );
//    }


//    if( wireframe.empty() == true )
//        buildWireframeList();


//    if( wireframe.empty() == false )
//    {
//        vector_wireframe_size = (int) wireframe.size();

//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
//        glBufferData( GL_ELEMENT_ARRAY_BUFFER, vector_wireframe_size*sizeof( GLuint ), wireframe.data(), GL_STATIC_DRAW );
//    }

//    if( bounding_box.empty() == false )
//    {
//        number_lines_bbox = bounding_box.size();

//        glBindBuffer( GL_ARRAY_BUFFER, bf_bbox_mesh );
//        glBufferData( GL_ARRAY_BUFFER, bounding_box.size()*sizeof( GLfloat ), bounding_box.data(), GL_STATIC_DRAW );
//    }


//    if( colors.empty() == false )
//    {
//        glBindBuffer( GL_ARRAY_BUFFER, bf_colors_mesh );
//        glBufferData( GL_ARRAY_BUFFER, colors.size()*sizeof( GLfloat ), colors.data(), GL_STATIC_DRAW );
//    }

//}


//void Mesh::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const float& scale  )
//{

//    if( vertices.empty() == true ) return;

//    glPointSize( 5.0f );

//    Eigen::Affine3f M;
//    M.setIdentity();

//    shader_mesh->bind();
//    shader_mesh->setUniform( "mmatrix", M );
//    shader_mesh->setUniform( "vmatrix", V );
//    shader_mesh->setUniform( "pmatrix", P );
//    shader_mesh->setUniform( "scale", scale );

//    glBindVertexArray( va_mesh );


//    if( apply_crosssection_clipping == true )
//    {
//        glEnable(GL_CLIP_DISTANCE0);
//        shader_mesh->setUniform( "ClipPlane", coefACrossSectionEquation, coefBCrossSectionEquation, coefCCrossSectionEquation, coefDCrossSectionEquation );

//    }
//    else
//    {
//        shader_mesh->setUniform( "ClipPlane", 0.0f, 0.0f, 0.0f, 0.0f );
//        glDisable(GL_CLIP_DISTANCE0);
//    }

//    if( show_vertices == true )
//    {
//        glDrawArrays( GL_POINTS, 0, number_of_vertices );
//    }
//    if( show_edges == true )
//    {
//        glLineWidth( 2.0f );
//        shader_mesh->setUniform( "edge", GL_TRUE );


//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_wireframe_mesh );
//        glDrawElements( GL_LINES, vector_wireframe_size, GL_UNSIGNED_INT, 0 );

//        shader_mesh->setUniform( "edge", GL_FALSE );

//    }
//    if( show_faces == true )
//    {
//        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bf_faces_mesh );
//        glDrawElements( GL_TRIANGLES, vector_triangles_size, GL_UNSIGNED_INT, 0 );
//    }

//    glBindVertexArray( 0 );



//    shader_mesh->unbind();



//    if( show_bbox == true )
//    {

//        glLineWidth( 4.0f );


//        shader_bbox->bind();
//        shader_bbox->setUniform( "mmatrix", M );
//        shader_bbox->setUniform( "vmatrix", V );
//        shader_bbox->setUniform( "pmatrix", P );
//        shader_bbox->setUniform( "scale", scale );

//        glBindVertexArray( va_bbox );

//            glDrawArrays( GL_LINES, 0, number_lines_bbox );

//        glBindVertexArray( 0 );

//    }

//    shader_bbox->unbind();
//}


//void Mesh::clear()
//{
//    mesh_type = TYPE::TRIANGLES;

//    vertices.clear();
//    faces.clear();
//    wireframe.clear();
//    normals_by_vertices.clear();
//    bounding_box.clear();


//    max[ 0 ] = 0.0f;
//    max[ 1 ] = 0.0f;
//    max[ 2 ] = 0.0f;

//    min[ 0 ] = 0.0f;
//    min[ 1 ] = 0.0f;
//    min[ 2 ] = 0.0f;


//    show_vertices = true;
//    show_edges = false;
//    show_faces = true;
//    show_bbox = true;

//    apply_crosssection_clipping = false;

//    coefACrossSectionEquation = 0.0f;
//    coefBCrossSectionEquation = 0.0f;
//    coefCCrossSectionEquation = 0.0f;
//    coefDCrossSectionEquation = 0.0f;
//}

//void Mesh::resetBuffers()
//{
//    deleteShaders();

//    if( va_mesh )
//    {
//        glDeleteVertexArrays( 1, &va_mesh );
//        if( bf_vertices_mesh )
//            glDeleteBuffers(1, &bf_vertices_mesh);
//        if( bf_faces_mesh )
//            glDeleteBuffers(1, &bf_faces_mesh);
//        if( bf_wireframe_mesh )
//            glDeleteBuffers(1, &bf_wireframe_mesh);
//        if( bf_colors_mesh )
//            glDeleteBuffers(1, &bf_colors_mesh);

//    }

//    if( va_bbox )
//    {
//        glDeleteVertexArrays( 1, &va_bbox );
//        if( bf_bbox_mesh )
//            glDeleteBuffers(1, &bf_bbox_mesh);
//    }


//    va_mesh = 0;
//    bf_vertices_mesh = 0;
//    bf_faces_mesh = 0;
//    bf_wireframe_mesh = 0;
//    bf_colors_mesh = 0;

//    number_of_vertices = 0;
//    vector_triangles_size = 0;
//    vector_wireframe_size = 0;



//    va_bbox = 0;
//    bf_bbox_mesh = 0;
//    number_lines_bbox = 0;



//}

//void Mesh::deleteShaders()
//{

//    if (shader_mesh)
//    {
//        delete (shader_mesh);
//        shader_mesh = nullptr;
//    }

//    if (shader_bbox)
//    {
//        delete(shader_bbox);
//        shader_bbox = nullptr;
//    }

//}


