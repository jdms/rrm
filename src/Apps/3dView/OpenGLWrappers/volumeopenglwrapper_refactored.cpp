#include <vector>
#include <cmath>

#include "volumeopenglwrapper_refactored.h"
#include "./Core/Geology/Models/Volume.h"



VolumeOpenGLWrapper_Refactored::VolumeOpenGLWrapper_Refactored()
{
    raw = nullptr;
    setDefaultValues();
}


void VolumeOpenGLWrapper_Refactored::setShaderDirectory( const std::string& dir )
{
    shader_directory = dir;
}




void VolumeOpenGLWrapper_Refactored::setRawVolume( Volume* const& vol )
{
    raw = vol;
    createVolumeBox();
}


Volume* VolumeOpenGLWrapper_Refactored::getRawVolume() const
{
    return raw;
}


void VolumeOpenGLWrapper_Refactored::getOrigin( double& w, double& h, double& d ) const
{
    raw->getOrigin( w, h, d );
}


double VolumeOpenGLWrapper_Refactored::getWidth() const
{
    return raw->getWidth();
}


double VolumeOpenGLWrapper_Refactored::getHeight() const
{
    return raw->getHeight();
}


double VolumeOpenGLWrapper_Refactored::getDepth() const
{
    return raw->getDepth();
}




void VolumeOpenGLWrapper_Refactored::createVolumeBox()
{
    double ox = 0.0f, oy = 0.0f, oz = 0.0f;
    raw->getOrigin( ox, oy, oz );

    float minx = static_cast< GLfloat > ( ox );
    float miny = static_cast< GLfloat > ( oy );
    float minz = static_cast< GLfloat > ( oz );

    float maxx = static_cast< GLfloat > ( ox + raw->getWidth() );
    float maxy = static_cast< GLfloat > ( oy + raw->getHeight() );
    float maxz = static_cast< GLfloat > ( oz + raw->getDepth() );


    Eigen::Vector3f min_real( minx, miny, minz );
    Eigen::Vector3f max_real( maxx, maxy, maxz );


    minimum = Model3DUtils::normalizePointCloud( min_real, max_real, min_real );
    maximum = Model3DUtils::normalizePointCloud( max_real, max_real, min_real );

    Eigen::Vector3f A( minimum.x(), minimum.y(), maximum.z() );
    Eigen::Vector3f B( maximum.x(), minimum.y(), maximum.z() );
    Eigen::Vector3f C( maximum.x(), maximum.y(), maximum.z() );
    Eigen::Vector3f D( minimum.x(), maximum.y(), maximum.z() );
    Eigen::Vector3f E( maximum.x(), minimum.y(), minimum.z() );
    Eigen::Vector3f F( minimum.x(), minimum.y(), minimum.z() );
    Eigen::Vector3f G( minimum.x(), maximum.y(), minimum.z() );
    Eigen::Vector3f H( maximum.x(), maximum.y(), minimum.z() );


    /*

*/


    std::vector< float > wireframe =
    {
        maximum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        // Bottom Face
        maximum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0,
        // Front Face
        maximum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        // Back Face
        maximum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0,
        // Left Face
        maximum.x(), maximum.y(), minimum.z(),1.0,
        maximum.x(), maximum.y(), maximum.z(),1.0,
        maximum.x(), minimum.y(), minimum.z(),1.0,
        maximum.x(), minimum.y(), maximum.z(),1.0,
        // Right Face
        minimum.x(), maximum.y(), maximum.z(),1.0,
        minimum.x(), maximum.y(), minimum.z(),1.0,
        minimum.x(), minimum.y(), maximum.z(),1.0,
        minimum.x(), minimum.y(), minimum.z(),1.0
    };

//    std::vector< float > wireframe =
//    {
//        A.x(), A.y(), A.z(), 1.0f,
//        B.x(), B.y(), B.z(), 1.0f,
//        D.x(), D.y(), D.z(), 1.0f,
//        C.x(), C.y(), C.z(), 1.0f,

//        B.x(), B.y(), B.z(), 1.0f,
//        E.x(), E.y(), E.z(), 1.0f,
//        C.x(), C.y(), C.z(), 1.0f,
//        H.x(), H.y(), H.z(), 1.0f,

//        E.x(), E.y(), E.z(), 1.0f,
//        F.x(), F.y(), F.z(), 1.0f,
//        H.x(), H.y(), H.z(), 1.0f,
//        G.x(), G.y(), G.z(), 1.0f,

//        F.x(), F.y(), F.z(), 1.0f,
//        A.x(), A.y(), A.z(), 1.0f,
//        G.x(), G.y(), G.z(), 1.0f,
//        D.x(), D.y(), D.z(), 1.0f,

//        D.x(), D.y(), D.z(), 1.0f,
//        C.x(), C.y(), C.z(), 1.0f,
//        G.x(), G.y(), G.z(), 1.0f,
//        H.x(), H.y(), H.z(), 1.0f,

//        F.x(), F.y(), F.z(), 1.0f,
//        E.x(), E.y(), E.z(), 1.0f,
//        A.x(), A.y(), A.z(), 1.0f,
//        B.x(), B.y(), B.z(), 1.0f,
//    };


    defineNormals();
    reloadBuffers( wireframe );

}


void VolumeOpenGLWrapper_Refactored::defineNormals()
{
    Eigen::Vector3f normal_A( -1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_B(  1.0f, -1.0f,  1.0f );
    Eigen::Vector3f normal_C(  1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_D( -1.0f,  1.0f,  1.0f );
    Eigen::Vector3f normal_E(  1.0f, -1.0f, -1.0f );
    Eigen::Vector3f normal_F( -1.0f, -1.0f, -1.0f );
    Eigen::Vector3f normal_G( -1.0f,  1.0f, -1.0f );
    Eigen::Vector3f normal_H(  1.0f,  1.0f, -1.0f );

    normal_A /= 3;
    normal_B /= 3;
    normal_C /= 3;
    normal_D /= 3;
    normal_E /= 3;
    normal_F /= 3;
    normal_G /= 3;
    normal_H /= 3;

    normal_A.normalize();
    normal_B.normalize();
    normal_C.normalize();
    normal_D.normalize();
    normal_E.normalize();
    normal_F.normalize();
    normal_G.normalize();
    normal_H.normalize();


    std::vector< float > normals =
    {
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),

        normal_B.x(), normal_B.y(), normal_B.z(),
        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),

        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),

        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),
        normal_D.x(), normal_D.y(), normal_D.z(),

        normal_D.x(), normal_D.y(), normal_D.z(),
        normal_C.x(), normal_C.y(), normal_C.z(),
        normal_G.x(), normal_G.y(), normal_G.z(),
        normal_H.x(), normal_H.y(), normal_H.z(),

        normal_F.x(), normal_F.y(), normal_F.z(),
        normal_E.x(), normal_E.y(), normal_E.z(),
        normal_A.x(), normal_A.y(), normal_A.z(),
        normal_B.x(), normal_B.y(), normal_B.z()
    };


    glBindBuffer( GL_ARRAY_BUFFER , vb_volume_normals );
    glBufferData( GL_ARRAY_BUFFER , normals.size() * sizeof ( GLfloat )  , normals.data() ,
                  GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER , 0 );

}


void VolumeOpenGLWrapper_Refactored::getMinimum( float& mx, float& my, float &mz ) const
{
    mx = minimum.x();
    my = minimum.y();
    mz = minimum.z();
}


void VolumeOpenGLWrapper_Refactored::getMaximum( float& Mx, float& My, float &Mz ) const
{
    Mx = maximum.x();
    My = maximum.y();
    Mz = maximum.z();
}



void VolumeOpenGLWrapper_Refactored::initShaders()
{
    shader = new Tucano::Shader( "VolumeOpenGLWrap", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ), ( shader_directory + "shaders/CubeSinglePassWireframe.frag"),
                                             ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );
    shader->initialize();
}


void VolumeOpenGLWrapper_Refactored::initBuffers()
{
    glGenVertexArrays( 1 , &va_volume );
    glBindVertexArray( va_volume );

        glGenBuffers( 1 , &vb_volume_vertices );
        glBindBuffer( GL_ARRAY_BUFFER , vb_volume_vertices );
        glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_volume_normals );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_volume_normals );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );

}



void VolumeOpenGLWrapper_Refactored::resetShaders()
{
    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }
}


void VolumeOpenGLWrapper_Refactored::resetBuffers()
{
    if( va_volume != 0 )
    {
        glDeleteVertexArrays( 1, &va_volume );

        if ( vb_volume_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_volume_vertices );
        }
        if ( vb_volume_normals != 0 )
        {
            glDeleteBuffers( 1, &vb_volume_normals );
        }
    }
}




void VolumeOpenGLWrapper_Refactored::reloadShaders()
{
    shader->reloadShaders();
}


void VolumeOpenGLWrapper_Refactored::reloadBuffers( const std::vector< float >& wireframe )
{
    number_of_vertices =  static_cast< GLuint > (wireframe.size()/4);

    glBindBuffer ( GL_ARRAY_BUFFER , vb_volume_vertices );
    glBufferData ( GL_ARRAY_BUFFER , wireframe.size() * sizeof ( GLfloat ) ,
                                     wireframe.data() , GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}




void VolumeOpenGLWrapper_Refactored::updateGeometry()
{
    createVolumeBox();
}


void VolumeOpenGLWrapper_Refactored::init()
{
    initShaders();
    initBuffers();
}


void VolumeOpenGLWrapper_Refactored::reset()
{
    resetShaders();
    resetBuffers();

    init();
    createVolumeBox();
}


bool VolumeOpenGLWrapper_Refactored::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->getVisibility();
}


void VolumeOpenGLWrapper_Refactored::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
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

        glBindVertexArray( va_volume );
            glDrawArrays( GL_LINES_ADJACENCY , 0 , number_of_vertices );
        glBindVertexArray( 0 );

    shader->unbind();


    glDisable( GL_BLEND );

}


void VolumeOpenGLWrapper_Refactored::clear()
{
    clearData();
    setDefaultValues();
}


void VolumeOpenGLWrapper_Refactored::clearData()
{
    resetShaders();
    resetBuffers();

//    if( raw != nullptr )
//        raw->clear();
    raw = nullptr;

}


void VolumeOpenGLWrapper_Refactored::setDefaultValues()
{
    va_volume = 0;
    vb_volume_vertices = 0;
    vb_volume_normals = 0;
    number_of_vertices = 0;

    minimum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
    maximum = Eigen::Vector3f( 0.0f, 0.0f, 0.0f );
}
