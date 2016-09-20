#include "GradientBackgroundShader.hpp"



GradientBackgroundShader::GradientBackgroundShader()
{

}



void GradientBackgroundShader::init()
{
	initBuffers();
	initShaders();
}


void GradientBackgroundShader::resetBuffers()
{
    if( va_background != 0 )
    {
        glDeleteVertexArrays( 1, &va_background );

        if ( vb_background != 0 )
        {
            glDeleteBuffers( 1, &vb_background );
        }
    }

    va_background = 0;
    vb_background = 0;
}


void GradientBackgroundShader::initBuffers()
{
    glGenVertexArrays ( 1 , &va_background );
    glBindVertexArray ( va_background );

    glGenBuffers ( 1 , &vb_background );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_background );
    glBufferData ( GL_ARRAY_BUFFER , 0, 0 , GL_STATIC_DRAW );
    glEnableVertexAttribArray ( 0 );
    glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

}	





void GradientBackgroundShader::resetShaders()
{

    if ( shader_background != nullptr )
    {
        delete( shader_background );
        shader_background = nullptr;
    }

}

void GradientBackgroundShader::initShaders()
{

    shader_background = new Tucano::Shader( "Background", ( shader_directory + "Shaders/DummyQuad.vert" ).toStdString(), ( shader_directory + "Shaders/DummyQuad.frag").toStdString(),
                                            ( shader_directory + "Shaders/DummyQuad.geom" ).toStdString(), "", "" );


    shader_background->initialize ( );


    glGenVertexArrays ( 1 , &va_background );
    glBindVertexArray ( va_background );
    /// Requesting Vertex Buffers to the GPU
    glGenBuffers ( 1 , &vb_background );
    glBindBuffer ( GL_ARRAY_BUFFER , vb_background );
    glBufferData ( GL_ARRAY_BUFFER , 0 , 0 , GL_STATIC_DRAW );

    glEnableVertexAttribArray ( 0 );
    glVertexAttribPointer ( 0 , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindVertexArray ( 0 );

}

void GradientBackgroundShader::reloadShaders()
{
        shader_background->reloadShaders();
}


void GradientBackgroundShader::draw( const int& width, const int& height )
{

    glDisable( GL_DEPTH_TEST );
    glDepthMask( GL_FALSE );

    shader_background->bind();

    shader_background->setUniform( "viewportSize", width, height );

    glBindVertexArray ( va_background );
    glDrawArrays ( GL_POINTS , 0 , 1 );
    glBindVertexArray ( 0 );

    shader_background->unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


}

