#include "surfaceshader.h"

SurfaceShader::SurfaceShader()
{
    setDefaultValues();
}


void SurfaceShader::initShaders()
{
    shader = new Tucano::Shader( "Surface", ( shader_directory + "shaders/gouraud_surface.vert" ),
                                            ( shader_directory + "shaders/gouraud_surface.frag" ),
                                            "", "", "" );

    shader->initialize();
}

void SurfaceShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_surface );
    glBindVertexArray ( va_surface );


        glGenBuffers ( 1 , &vb_vertices );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_vertices );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 0 );
        glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_normals );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 1 );
        glVertexAttribPointer ( 1 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers ( 1 , &vb_colors );
        glBindBuffer ( GL_ARRAY_BUFFER , vb_colors );
        glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
        glEnableVertexAttribArray ( 2 );
        glVertexAttribPointer ( 2 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );


        glGenBuffers( 1, &vb_faces );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW );


    glBindVertexArray ( 0 );
}



void SurfaceShader::resetShaders()
{
    if ( shader == nullptr ) return;

    delete( shader );
    shader = nullptr;
}

void SurfaceShader::resetBuffers()
{

    if( va_surface != 0 )
    {
        glDeleteVertexArrays( 1, &va_surface );

        if ( vb_vertices != 0 )
        {
            glDeleteBuffers( 1, &vb_vertices );
        }

        if ( vb_normals != 0 )
        {
            glDeleteBuffers( 1, &vb_normals );
        }

        if ( vb_colors != 0 )
        {
            glDeleteBuffers( 1, &vb_colors );
        }

        if ( vb_faces != 0 )
        {
            glDeleteBuffers( 1, &vb_faces );
        }
    }

}



void SurfaceShader::updateGeometryBuffers( const std::vector< GLfloat >& vertices_,
                                           const std::vector< GLfloat >& normals_,
                                           const std::vector< GLuint >& faces_ )
{
    glBindBuffer ( GL_ARRAY_BUFFER, vb_vertices );
    glBufferData ( GL_ARRAY_BUFFER, vertices_.size() * sizeof ( GLfloat ), vertices_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER, 0 );


    glBindBuffer ( GL_ARRAY_BUFFER , vb_normals );
    glBufferData ( GL_ARRAY_BUFFER , normals_.size() * sizeof ( GLfloat ), normals_.data() ,
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );

    number_of_faces = faces_.size();
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
    glBufferData ( GL_ELEMENT_ARRAY_BUFFER, faces_.size() * sizeof ( GLuint ), faces_.data(),
                   GL_STATIC_DRAW );
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


void SurfaceShader::updateColorBuffers( const std::vector< GLfloat >& colors_ )
{
    glBindBuffer( GL_ARRAY_BUFFER, vb_colors );
    glBufferData( GL_ARRAY_BUFFER, colors_.size() * sizeof ( GLfloat ), colors_.data(),
                  GL_STATIC_DRAW );
    glBindBuffer ( GL_ARRAY_BUFFER , 0 );
}



void SurfaceShader::draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w,
                          const int& h )
{

    Eigen::Affine3f M;
    M.setIdentity();

    shader->bind();

        shader->setUniform( "ModelMatrix" , M );
        shader->setUniform( "ViewMatrix" , V );
        shader->setUniform( "ProjectionMatrix" , P );
        shader->setUniform( "WIN_SCALE" , (float) w , (float) h );

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 2.0f, 1.0f );


            glBindVertexArray( va_surface );

                shader->setUniform( "solid" , true );
                shader->setUniform( "testing" , true );

                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

            glBindVertexArray ( 0 );


        glDisable(GL_POLYGON_OFFSET_FILL);


        glEnable( GL_DEPTH_TEST );
        glDepthFunc( GL_LEQUAL );
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


            glBindVertexArray( va_surface );

                shader->setUniform( "solid" , false );
                glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vb_faces );
                glDrawElements ( GL_TRIANGLES , number_of_faces , GL_UNSIGNED_INT , 0 );

            glBindVertexArray ( 0 );

        glDisable( GL_BLEND );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    shader->unbind();

}


void SurfaceShader::clear()
{
    reset();
    setDefaultValues();
}

void SurfaceShader::setDefaultValues()
{
    va_surface = 0;
    vb_vertices = 0;
    vb_normals = 0;
    vb_colors = 0;
    vb_faces = 0;

    number_of_faces = 0;
    number_of_vertices = 0;

}
