#include "BoundingBox3D.h"

#include "Tucano/BoundingBox3.hpp"



BoundingBox3D::BoundingBox3D( float min_x, float min_y, float min_z, float w, float h, float d ): minx( min_x ), miny( min_y ), minz( min_z ), width( w ), height( h ), depth( d )
{
    is_initialized = false;
}




void BoundingBox3D::initData()
{

    shader_boundingbox = nullptr;

    va_boundingbox = 0;
    vb_vertices = 0;
    slot_vertices = 0;
    number_of_lines = 0;

}


void BoundingBox3D::init()
{

    initData();
    initShaders();
    initBuffers();

    is_initialized = true;

}


void BoundingBox3D::create()
{

    Eigen::Vector3f A( minx, miny,  minz + depth );
    Eigen::Vector3f B(  minx + width, miny,  minz + depth );
    Eigen::Vector3f C(  minx + width,  miny + height,  minz + depth );
    Eigen::Vector3f D( minx,  miny + height,  minz + depth );
    Eigen::Vector3f E( minx, miny, minz );
    Eigen::Vector3f F(  minx + width, miny, minz );
    Eigen::Vector3f G(  minx + width,  miny + height, minz );
    Eigen::Vector3f H( minx,  miny + height, minz );


    std::vector< Eigen::Vector3f > wireframe_eigen;



    // TOP

    wireframe_eigen.push_back( C );
    wireframe_eigen.push_back( D );
    wireframe_eigen.push_back( G );
    wireframe_eigen.push_back( H );


    // BOTTOM

    wireframe_eigen.push_back( B );
    wireframe_eigen.push_back( A );
    wireframe_eigen.push_back( F );
    wireframe_eigen.push_back( E );


    // FRONT

    wireframe_eigen.push_back( C );
    wireframe_eigen.push_back( D );
    wireframe_eigen.push_back( B );
    wireframe_eigen.push_back( A );


    // BACK

    wireframe_eigen.push_back( G );
    wireframe_eigen.push_back( F );
    wireframe_eigen.push_back( H );
    wireframe_eigen.push_back( E );


    // RIGHT

    wireframe_eigen.push_back( G );
    wireframe_eigen.push_back( C );
    wireframe_eigen.push_back( F );
    wireframe_eigen.push_back( B );


    // LEFT

    wireframe_eigen.push_back( D );
    wireframe_eigen.push_back( H );
    wireframe_eigen.push_back( A );
    wireframe_eigen.push_back( E );

    int number_of_vertices = (int) wireframe_eigen.size();



    std::vector< float > wireframe;
    for( unsigned int it = 0; it < number_of_vertices; ++it )
    {
        wireframe.push_back( wireframe_eigen[ it ].x() );
        wireframe.push_back( wireframe_eigen[ it ].y() );
        wireframe.push_back( wireframe_eigen[ it ].z() );
    }

    loadBuffers( wireframe );

}



void BoundingBox3D::resetBuffers()
{

    if( va_boundingbox != 0 )
    {
        glDeleteVertexArrays( 1, &va_boundingbox );

        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
    }

    va_boundingbox = 0;
    vb_vertices = 0;

}


void BoundingBox3D::initBuffers()
{

    glGenVertexArrays( 1 , &va_boundingbox );
    glBindVertexArray( va_boundingbox );

    glGenBuffers( 1 , &vb_vertices );
    glBindBuffer( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
    glEnableVertexAttribArray( slot_vertices );
    glVertexAttribPointer( slot_vertices , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );

}




void BoundingBox3D::loadBuffers( const std::vector< float >& wireframe )
{

    number_of_lines = (GLuint) wireframe.size()/3;

    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , wireframe.size() * sizeof ( GLfloat ) , wireframe.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

}


void BoundingBox3D::resetShaders()
{

    if ( shader_boundingbox != nullptr )
    {
        delete( shader_boundingbox );
        shader_boundingbox = nullptr;
    }

}


void BoundingBox3D::initShaders()
{

    shader_boundingbox = new Tucano::Shader( "BoundingBox3D", ( shader_directory + "Shaders/BlankScreenCube.vert" ).toStdString(), ( shader_directory + "Shaders/BlankScreenCube.frag").toStdString(),
                                             ( shader_directory + "Shaders/BlankScreenCube.geom" ).toStdString(), "", "" );
    shader_boundingbox->initialize();

}


void BoundingBox3D::reloadShaders()
{
    shader_boundingbox->reloadShaders();
}




void BoundingBox3D::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h )
{

    Eigen::Affine3f M;
    M.setIdentity();


//    float scale = (float)1.5*width/(float)height;

    shader_boundingbox->bind();
    
    shader_boundingbox->setUniform( "ModelMatrix", M );
    shader_boundingbox->setUniform( "ViewMatrix",  V );
    shader_boundingbox->setUniform( "ProjectionMatrix", P );
    shader_boundingbox->setUniform( "WIN_SCALE", (float) w, (float) h );

//    shader_boundingbox->setUniform( "scale", scale );

    glBindVertexArray( va_boundingbox );
        glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_lines );
    glBindVertexArray( 0 );

    shader_boundingbox->unbind();

}



void BoundingBox3D::update()
{

    minx = bd->getX();
    miny = bd->getY();
    minz = bd->getZ();


    width  = bd->getWidth();
    height = bd->getHeight();
    depth  = bd->getDepth();

    create();

}
