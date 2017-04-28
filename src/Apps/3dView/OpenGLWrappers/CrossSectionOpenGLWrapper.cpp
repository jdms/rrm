#include "CrossSectionOpenGLWrapper.h"

CrossSectionOpenGLWrapper::CrossSectionOpenGLWrapper()
{
//    object = nullptr;
    shader = nullptr;

    va_csection = 0;
    vb_vertices = 0;
    nvertices = 0;
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
    glGenVertexArrays ( 1 , &va_csection );
    glBindVertexArray ( va_csection );

        glGenBuffers ( 1 , &vb_vertices );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 0 );
        glVertexAttribPointer ( 0 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

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

        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }

    }

    va_csection = 0;
    vb_vertices = 0;
    nvertices = 0;
}



void CrossSectionOpenGLWrapper::draw( const Eigen::Affine3f& V_, const Eigen::Matrix4f& P_, const int& w_,
                                                                                   const int& h_ )
{

    Eigen::Affine3f M_;
    M_.setIdentity();

    shader->bind();

    shader->setUniform( "ModelMatrix" , M_ );
    shader->setUniform( "ViewMatrix" , V_ );
    shader->setUniform( "ProjectionMatrix" , P_ );
    shader->setUniform( "WIN_SCALE" , (float) w_ , (float) h_ );

        glBindVertexArray( va_csection );
        glDrawArrays ( GL_LINES_ADJACENCY , 0 , nvertices );
        glBindVertexArray ( 0 );

    shader->unbind();



//    seismic_plane_shader_->bind ( );
//    /// 3rd attribute buffer : vertices
//    seismic_plane_shader_->setUniform ( "ModelMatrix" , camera.getViewMatrix ( ) );
//    seismic_plane_shader_->setUniform ( "ViewMatrix" , camera.getViewMatrix ( ) );
//    seismic_plane_shader_->setUniform ( "ProjectionMatrix" , camera.getProjectionMatrix ( ) );
//    seismic_plane_shader_->setUniform ( "WIN_SCALE" , (float) width ( ) , (float) height ( ) );
//    seismic_plane_shader_->setUniform ( "color_plane" , 0.0f,1.0f,0.0f,1.0f );
//    seismic_plane_shader_->setUniform ( "z" , this->seismic_slice_plane_position );
//            glBindVertexArray ( vertexArray_Seismic_plane_ );
//            /// Draw the triangle !
//            glDrawArrays ( GL_LINES_ADJACENCY , 0 , seismic_plane_.size() );

//            glBindVertexArray ( 0 );
//    seismic_plane_shader_->unbind ( );
}

