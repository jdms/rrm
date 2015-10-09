#include "Canvas3D.h"

Canvas3D::Canvas3D( QGLFormat format, QWidget* parent ) : QGLWidget( format, parent )
{
    this->makeCurrent();
    initializeOpenGLFunctions();
}

void Canvas3D::initializeGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );
}

void Canvas3D::resizeGL( int width, int height )
{
    height = height? height:1;
    glViewport( 0, 0, (GLint) width, (GLint) height );
}

void Canvas3D::paintGL()
{
    this->makeCurrent();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}
