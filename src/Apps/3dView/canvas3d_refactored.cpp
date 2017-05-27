#include <iostream>
#include <stdlib.h>

#include "canvas3d_refactored.h"

Canvas3d_Refactored::Canvas3d_Refactored()
{
    createScene();
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

    makeCurrent();
    glClearColor( 1.0f , 1.0 , 1.0 , 1.0f );

    shareOpenGLContext();
}


void Canvas3d_Refactored::resizeGL( int width, int height )
{
    glViewport( 0 , 0 , width , height );
}


void Canvas3d_Refactored::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void Canvas3d_Refactored::createScene()
{
    scene3d = new Scene3d_refactored();
    connect( scene3d, SIGNAL( updateCanvas() ), this, SLOT( update() ) );
}


Scene3d_refactored* Canvas3d_Refactored::getScene() const
{
    return scene3d;
}


void Canvas3d_Refactored::shareOpenGLContext()
{
    scene3d->setOpenGLContext( context() );
}
