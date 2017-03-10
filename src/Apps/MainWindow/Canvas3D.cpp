#include "Canvas3D.h"

Canvas3D::Canvas3D()
{
    scene = new Scene3D();
}

void Canvas3D::initializeGL()
{

    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();

    if ( GLEW_OK != glewInitResult )
    {
        std::cerr << "Error: " << glewGetErrorString ( glewInitResult ) << std::endl;
        exit ( EXIT_FAILURE );
    }


    makeCurrent();

    glClearColor ( 1.0f , 1.0 , 1.0 , 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_MULTISAMPLE );
    glMinSampleShading( 1.0f );

}

void Canvas3D::resizeGL( int width , int height )
{
    glViewport ( 0 , 0 , width , height );
}

void Canvas3D::paintGL()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}
