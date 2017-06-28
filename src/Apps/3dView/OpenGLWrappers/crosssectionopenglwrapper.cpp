#include <vector>
#include <cmath>


#include "crosssectionopenglwrapper.h"



CrossSectionOpenGLWrapper::CrossSectionOpenGLWrapper()
{
    setDefaultValues();
}


void CrossSectionOpenGLWrapper::setShaderDirectory( const std::string& dir )
{
    shader_directory = dir;
}

void CrossSectionOpenGLWrapper::setDepth( double d )
{
    depth = d;
    createPlane();
}

double CrossSectionOpenGLWrapper::getDepth() const
{
    return depth;
}

bool CrossSectionOpenGLWrapper::isVisible() const
{
    return is_visible;
}

void CrossSectionOpenGLWrapper::setVisible( bool status )
{
    is_visible = status;
}


void CrossSectionOpenGLWrapper::setMinimum( float mx, float my, float mz )
{
    minimum = Eigen::Vector3f( mx, my, mz );
}

void CrossSectionOpenGLWrapper::setMaximum( float mx, float my, float mz )
{
    maximum = Eigen::Vector3f( mx, my, mz );
}


void CrossSectionOpenGLWrapper::createPlane()
{

    if( depth > maximum.z() )
        depth = maximum.z();
    else if( depth < minimum.z() )
        depth = minimum.z();

    float z = static_cast<float> (depth);

    Eigen::Vector3f A( minimum.x(), minimum.y(), z );
    Eigen::Vector3f B( maximum.x(), minimum.y(), z );
    Eigen::Vector3f C( maximum.x(), maximum.y(), z );
    Eigen::Vector3f D( minimum.x(), maximum.y(), z );

    Eigen::Affine3f matrix = Model3DUtils::normalizePointCloud( minimum.x(), maximum.x(), minimum.y(),
                                                                maximum.y(), minimum.z(), maximum.z() );

    A = matrix * A;
    B = matrix * B;
    C = matrix * C;
    D = matrix * D;

    std::vector< float > plane =
    {
        C.x(), C.y(), C.z(), 1.0f,
        D.x(), D.y(), D.z(), 1.0f,
        B.x(), B.y(), B.z(), 1.0f,
        A.x(), A.y(), A.z(), 1.0f
    };


//    std::vector< float > plane =
//    {
//        A.x(), A.y(), A.z(), 1.0f,
//        B.x(), B.y(), B.z(), 1.0f,
//        D.x(), D.y(), D.z(), 1.0f,
//        C.x(), C.y(), C.z(), 1.0f
//    };

    /*
    std::vector< float > wireframe =
    {
        maximum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,

*/

    number_of_vertices = static_cast< GLuint > (plane.size() );
    reloadBuffers( plane );

    defineNormals();

}


void CrossSectionOpenGLWrapper::defineNormals()
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


void CrossSectionOpenGLWrapper::updateGeometry()
{
    createPlane();
}

void CrossSectionOpenGLWrapper::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.frag" ),
                                            ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );

    shader->initialize();
}


void CrossSectionOpenGLWrapper::initBuffers()
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


void CrossSectionOpenGLWrapper::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}

void CrossSectionOpenGLWrapper::resetBuffers()
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

void CrossSectionOpenGLWrapper::reloadShaders()
{
    shader->reloadShaders();
}

void CrossSectionOpenGLWrapper::reloadBuffers( const std::vector< float >& wireframe )
{


    glBindBuffer ( GL_ARRAY_BUFFER, vb_csection_lines );
    glBufferData ( GL_ARRAY_BUFFER, wireframe.size() * sizeof ( float ), wireframe.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );
}


void CrossSectionOpenGLWrapper::init()
{
    initShaders();
    initBuffers();
}


void CrossSectionOpenGLWrapper::reset()
{
    resetShaders();
    resetBuffers();

    init();
    createPlane();
}


void CrossSectionOpenGLWrapper::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
           const int& h )
{
    if( isVisible() == false ) return;

    Eigen::Affine3f M;
    M.setIdentity();

    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
}


void CrossSectionOpenGLWrapper::clear()
{
    clearData();
    setDefaultValues();
}


void CrossSectionOpenGLWrapper::clearData()
{
    resetShaders();
    resetBuffers();

}

void CrossSectionOpenGLWrapper::setDefaultValues()
{
    depth = 0.0f;
    number_of_vertices = 0;
    is_visible = true;

    minimum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
    maximum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
}
