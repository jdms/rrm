#include "VolumeOpenGLWrap.h"

VolumeOpenGLWrap::VolumeOpenGLWrap()
{
    shader = nullptr;
    va_volume = 0;
    vb_vertices = 0;
    nlines = 0;

    volume = nullptr;

}


void VolumeOpenGLWrap::initShaders()
{
    shader = new Tucano::Shader( "VolumeOpenGLWrap", ( shader_directory + "shaders/CubeSinglePassWireframe.vert" ), ( shader_directory + "shaders/CubeSinglePassWireframe.frag"),
                                             ( shader_directory + "shaders/CubeSinglePassWireframe.geom" ), "", "" );
    shader->initialize();
}

void VolumeOpenGLWrap::initBuffers()
{
    glGenVertexArrays( 1 , &va_volume );
    glBindVertexArray( va_volume );

        glGenBuffers( 1 , &vb_vertices );
        glBindBuffer( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray( 0 );
}


void VolumeOpenGLWrap::resetShaders()
{

    if ( shader != nullptr )
    {
        delete( shader );
        shader = nullptr;
    }

}

void VolumeOpenGLWrap::resetBuffers()
{
    if( va_volume != 0 )
    {
        glDeleteVertexArrays( 1, &va_volume );

        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }
    }

    va_volume = 0;
    vb_vertices = 0;
}


void VolumeOpenGLWrap::setVolumeRaw( Volume* const& vol_ )
{
    volume = vol_;
    createVolumeBox();
}

void VolumeOpenGLWrap::createVolumeBox()
{
    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

    volume->getOrigin( ox_, oy_, oz_ );
    volume->getDimensions( w_, h_, d_ );

    float originx_ = (float)ox_, originy_ =  (float)oy_, originz_ = (float)oz_;
    float width_ = (float)w_, height_ = (float)h_, depth_ = (float)d_;

    Eigen::Vector3f min( originx_, originy_, originz_ );
    Eigen::Vector3f max( originx_ + width_, originy_ + height_, originz_ + depth_ );

    std::vector< float > wireframe_ =
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

    std::vector< float > unit_wireframe_ = Model3DUtils::normalizePointCloud( wireframe_ );
    reloadBuffers( unit_wireframe_ ) ;

}


void VolumeOpenGLWrap::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                                   const int& h_ )
{
    Eigen::Affine3f M_;
    M_.setIdentity();

    shader->bind();

        shader->setUniform( "ModelMatrix", M_ );
        shader->setUniform( "ViewMatrix",  V_ );
        shader->setUniform( "ProjectionMatrix", P_ );
        shader->setUniform( "WIN_SCALE", (float) w_, (float) h_ );
        shader->setUniform ( "color_plane" , 0.5f, 0.5f, 0.5f, 0.2f );

        glBindVertexArray( va_volume );
            glDrawArrays( GL_LINES_ADJACENCY , 0 , nlines );
        glBindVertexArray( 0 );

    shader->unbind();
}
