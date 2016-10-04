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

    Eigen::Vector3f min( minx, miny, minz );
    Eigen::Vector3f max( minx + width, miny + height, minz + depth );


    std::vector< float > wireframe =
            {
                //  Top Face
                max.x(), max.y(), max.z(), 1.0f,
                min.x(), max.y(), max.z(), 1.0f,
                max.x(), max.y(), min.z(), 1.0f,
                min.x(), max.y(), min.z(), 1.0f,
                // Bottom Face
                max.x(), min.y(), max.z(), 1.0f,
                min.x(), min.y(), max.z(), 1.0f,
                max.x(), min.y(), min.z(), 1.0f,
                min.x(), min.y(), min.z(), 1.0f,
                // Front Face
                max.x(), max.y(), max.z(), 1.0f,
                min.x(), max.y(), max.z(), 1.0f,
                max.x(), min.y(), max.z(), 1.0f,
                min.x(), min.y(), max.z(), 1.0f,
                // Back Face
                max.x(), max.y(), min.z(), 1.0f,
                min.x(), max.y(), min.z(), 1.0f,
                max.x(), min.y(), min.z(), 1.0f,
                min.x(), min.y(), min.z(), 1.0f,
                // Left Face
                max.x(), max.y(), min.z(), 1.0f,
                max.x(), max.y(), max.z(), 1.0f,
                max.x(), min.y(), min.z(), 1.0f,
                max.x(), min.y(), max.z(), 1.0f,
                // Right Face
                min.x(), max.y(), max.z(), 1.0f,
                min.x(), max.y(), min.z(), 1.0f,
                min.x(), min.y(), max.z(), 1.0f,
                min.x(), min.y(), min.z(), 1.0f
            };



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
    glVertexAttribPointer( slot_vertices , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

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

    shader_boundingbox = new Tucano::Shader( "BoundingBox3D", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ).toStdString(), ( shader_directory + "shaders/CubeSinglePassWireframe.frag").toStdString(),
                                             ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ).toStdString(), "", "" );
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


    float scale = (float)1.5*width/(float)height;

    shader_boundingbox->bind();
    
    shader_boundingbox->setUniform( "ModelMatrix", M );
    shader_boundingbox->setUniform( "ViewMatrix",  V );
    shader_boundingbox->setUniform( "ProjectionMatrix", P );
    shader_boundingbox->setUniform( "WIN_SCALE", (float) w, (float) h );
    shader_boundingbox->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );
//    shader_boundingbox->setUniform( "scale", scale );

    glBindVertexArray( va_boundingbox );
        glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_lines );
    glBindVertexArray( 0 );

    shader_boundingbox->unbind();

}


void BoundingBox3D::clear()
{
    width = 0.0f;
    height = 0.0f;
    depth = 0.0f;

    resetBuffers();
    resetShaders();

    va_boundingbox = 0;
    vb_vertices = 0;
    number_of_lines = 0;

    is_initialized = false;


    minx = 0.0f;
    miny = 0.0f;
    minz = 0.0f;

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
