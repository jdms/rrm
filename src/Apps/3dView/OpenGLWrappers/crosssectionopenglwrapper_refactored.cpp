#include <vector>
#include <cmath>


#include "crosssectionopenglwrapper_refactored.h"



CrossSectionOpenGLWrapper_Refactored::CrossSectionOpenGLWrapper_Refactored()
{
    is_visible = true;
    depth = 0.0;
}


void CrossSectionOpenGLWrapper_Refactored::setShaderDirectory( const std::string& dir )
{
    shader_directory = dir;
}

void CrossSectionOpenGLWrapper_Refactored::setDepth( double d )
{
    depth = d;
    createPlane();
}

double CrossSectionOpenGLWrapper_Refactored::getDepth() const
{
    return depth;
}

bool CrossSectionOpenGLWrapper_Refactored::isVisible() const
{
    return is_visible;
}

void CrossSectionOpenGLWrapper_Refactored::setVisible( bool status )
{
    is_visible = status;
}


void CrossSectionOpenGLWrapper_Refactored::setMinimum( float mx, float my, float mz )
{
    minimum = Eigen::Vector3f( mx, my, mz );
}

void CrossSectionOpenGLWrapper_Refactored::setMaximum( float mx, float my, float mz )
{
    maximum = Eigen::Vector3f( mx, my, mz );
}


void CrossSectionOpenGLWrapper_Refactored::createPlane()
{

    float M = (float)( minimum.z() + depth );
    float m = (float)( minimum.z() );


    float z = ( depth - m )*( maximum.z() - minimum.z() )/( M - m ) + minimum.z();


    Eigen::Vector3f A( minimum.x(), minimum.y(), z );
    Eigen::Vector3f B( maximum.x(), minimum.y(), z );
    Eigen::Vector3f C( maximum.x(), maximum.y(), z );
    Eigen::Vector3f D( minimum.x(), maximum.y(), z );


    std::vector< float > plane =
    {
        A.x(), A.y(), A.z(), 1.0f,
        B.x(), B.y(), B.z(), 1.0f,
        D.x(), D.y(), D.z(), 1.0f,
        C.x(), C.y(), C.z(), 1.0f
    };


    number_of_vertices = static_cast< GLuint >(plane.size()/3);
    reloadBuffers( plane );

    defineNormals();

}


void CrossSectionOpenGLWrapper_Refactored::defineNormals()
{
    Eigen::Vector3f normal_A( -1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_B(  1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_C(  1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_D( -1.0f,  1.0f,  1.0f );

    normal_A /= 3;
    normal_B /= 3;
    normal_C /= 3;
    normal_D /= 3;

    normal_A.normalize();
    normal_B.normalize();
    normal_C.normalize();
    normal_D.normalize();

    std::vector< float > normals =
    {
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z()
    };

    glBindBuffer( GL_ARRAY_BUFFER , vb_csection_normals );
    glBufferData( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() ,
                  GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER , 0 );

}


void CrossSectionOpenGLWrapper_Refactored::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.frag" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );

    shader->initialize();
}


void CrossSectionOpenGLWrapper_Refactored::initBuffers()
{
    glGenVertexArrays( 1 , &va_csection );
    glBindVertexArray( va_csection );

        glGenBuffers( 1 , &vb_csection_lines );
        glBindBuffer( GL_ARRAY_BUFFER , vb_csection_lines );
        glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_csection_normals );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_csection_normals );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );
}

void CrossSectionOpenGLWrapper_Refactored::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}

void CrossSectionOpenGLWrapper_Refactored::resetBuffers()
{
    if( va_csection != 0 )
    {
        glDeleteVertexArrays( 1, &va_csection );

        if ( vb_csection_lines != 0 )
        {
            glDeleteBuffers( 1, &vb_csection_lines );
        }
        if ( vb_csection_normals != 0 )
        {
            glDeleteBuffers( 1, &vb_csection_normals );
        }
    }
}

void CrossSectionOpenGLWrapper_Refactored::reloadShaders()
{
    shader->reloadShaders();
}

void CrossSectionOpenGLWrapper_Refactored::reloadBuffers( const std::vector< float >& wireframe )
{


    glBindBuffer ( GL_ARRAY_BUFFER, vb_csection_lines );
    glBufferData ( GL_ARRAY_BUFFER, wireframe.size() * sizeof ( float ), wireframe.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}


void CrossSectionOpenGLWrapper_Refactored::init()
{
    initShaders();
    initBuffers();
}


void CrossSectionOpenGLWrapper_Refactored::reset()
{
    resetShaders();
    resetBuffers();

    init();
    createPlane();
}


void CrossSectionOpenGLWrapper_Refactored::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
           const int& h )
{
    if( isVisible() == false ) return;

    Eigen::Affine3f M;
    M.setIdentity();

    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_DEPTH_TEST );

    shader->bind();

        shader->setUniform( "ModelMatrix", M );
        shader->setUniform( "ViewMatrix",  V );
        shader->setUniform( "ProjectionMatrix", P );
        shader->setUniform( "WIN_SCALE", (float) w, (float) h );
        shader->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );

        glBindVertexArray( va_csection );
            glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_vertices );
        glBindVertexArray( 0 );

    shader->unbind();

    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
}


void CrossSectionOpenGLWrapper_Refactored::clear()
{
    resetShaders();
    resetBuffers();

    depth = 0.0f;
    number_of_vertices = 0;
    is_visible = true;

    minimum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
    maximum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
}
