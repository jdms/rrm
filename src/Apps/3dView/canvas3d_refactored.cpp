#include <iostream>
#include <stdlib.h>

#include "canvas3d_refactored.h"

Canvas3d_Refactored::Canvas3d_Refactored()
{

}

void Canvas3d_Refactored::initializeGL()
{
    glewExperimental = TRUE;
    GLenum err = glewInit();

    if( err != GLEW_OK )
    {
        std::cout << "glewInit failed: " << glewGetErrorString( err ) << std::endl;
        exit( EXIT_FAILURE );
    }

    glClearColor( 1.0f , 1.0 , 1.0 , 1.0f );

}


void Canvas3d_Refactored::resizeGL( int width, int height )
{
    glViewport( 0 , 0 , width , height );
}


void Canvas3d_Refactored::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
