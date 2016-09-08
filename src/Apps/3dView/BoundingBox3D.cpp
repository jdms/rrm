#include "BoundingBox3D.h"

#include "Tucano/BoundingBox3.hpp"



BoundingBox3D::BoundingBox3D( int w, int h, int d ): width( w ), height( h ), depth( d )
{
    initData();
    init();

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

    initShaders();
    initBuffers();

}


void BoundingBox3D::create()
{

    QVector3D A( -0.5f*width, -0.5f*height,  0.5f*depth );
    QVector3D B(  0.5f*width, -0.5f*height,  0.5f*depth );
    QVector3D C(  0.5f*width,  0.5f*height,  0.5f*depth );
    QVector3D D( -0.5f*width,  0.5f*height,  0.5f*depth );
    QVector3D E( -0.5f*width, -0.5f*height, -0.5f*depth );
    QVector3D F(  0.5f*width, -0.5f*height, -0.5f*depth );
    QVector3D G(  0.5f*width,  0.5f*height, -0.5f*depth );
    QVector3D H( -0.5f*width,  0.5f*height, -0.5f*depth );


    std::vector< Eigen::Vector3f > wireframe_eigen;



    // TOP

    wireframe_eigen.push_back( Eigen::Vector3f( C.x(), C.y(), C.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( D.x(), D.y(), D.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( G.x(), G.y(), G.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( H.x(), H.y(), H.z() ) );


    // BOTTOM

    wireframe_eigen.push_back( Eigen::Vector3f( B.x(), B.y(), B.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( A.x(), A.y(), A.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( F.x(), F.y(), F.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( E.x(), E.y(), E.z() ) );


    // FRONT

    wireframe_eigen.push_back( Eigen::Vector3f( C.x(), C.y(), C.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( D.x(), D.y(), D.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( B.x(), B.y(), B.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( A.x(), A.y(), A.z() ) );


    // BACK

    wireframe_eigen.push_back( Eigen::Vector3f( G.x(), G.y(), G.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( F.x(), F.y(), F.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( H.x(), H.y(), H.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( E.x(), E.y(), E.z() ) );


    // RIGHT

    wireframe_eigen.push_back( Eigen::Vector3f( G.x(), G.y(), G.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( C.x(), C.y(), C.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( F.x(), F.y(), F.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( B.x(), B.y(), B.z() ) );


    // LEFT

    wireframe_eigen.push_back( Eigen::Vector3f( D.x(), D.y(), D.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( H.x(), H.y(), H.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( A.x(), A.y(), A.z() ) );
    wireframe_eigen.push_back( Eigen::Vector3f( E.x(), E.y(), E.z() ) );




    Celer::BoundingBox3< float > bbox_mesh;
    bbox_mesh.fromPointCloud( wireframe_eigen.begin(), wireframe_eigen.end() );

    int number_of_vertices = (int) wireframe_eigen.size();
    for( int it = 0; it < number_of_vertices; ++it )
        wireframe_eigen[ it ] = ( wireframe_eigen[ it ] - bbox_mesh.center() )/bbox_mesh.diagonal();



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

    glGenVertexArrays ( 1 , &va_boundingbox );
    glBindVertexArray ( va_boundingbox );

    glGenBuffers ( 1 , &vb_vertices );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
    glEnableVertexAttribArray ( slot_vertices );
    glVertexAttribPointer ( slot_vertices , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

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




void BoundingBox3D::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{

    Eigen::Affine3f M;
    M.setIdentity();

    shader_boundingbox->bind ( );
    
    shader_boundingbox->setUniform ( "ModelMatrix" , M );
    shader_boundingbox->setUniform ( "ViewMatrix" , V );
    shader_boundingbox->setUniform ( "ProjectionMatrix" , P );
    shader_boundingbox->setUniform ( "WIN_SCALE" , (float) width, (float) height );

    glBindVertexArray ( va_boundingbox );
    glDrawArrays ( GL_LINES_ADJACENCY , 0 , number_of_lines );
    glBindVertexArray ( 0 );

    shader_boundingbox->unbind ( );

}



void BoundingBox3D::update()
{
    width = bd->getWidth();
    height = bd->getHeight();
    depth = bd->getDepth();

    create();

}
