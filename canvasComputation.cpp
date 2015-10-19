#include "canvasComputation.h"

CanvasComputation::CanvasComputation( QGLFormat format, QWidget* parent ) : QGLWidget( format, parent )
{
    this->makeCurrent();
    initializeOpenGLFunctions();
}

void CanvasComputation::initializeGL()
{
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glEnable( GL_DEPTH_TEST );
}

void CanvasComputation::resizeGL( int width, int height )
{
    height = height? height:1;
    glViewport( 0, 0, (GLint) width, (GLint) height );
}

void CanvasComputation::paintGL()
{
    this->makeCurrent();
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

}
