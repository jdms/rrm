#include <iostream>
#include <stdlib.h>

#include "canvas3d_refactored.h"

#include <QMouseEvent>
#include <QWheelEvent>



Canvas3d_Refactored::Canvas3d_Refactored()
{
    createScene();
}

void Canvas3d_Refactored::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if( err != GLEW_OK )
    {
        std::cout << "glewInit failed: " << glewGetErrorString( err ) << std::endl;
        exit( EXIT_FAILURE );
    }

    makeCurrent();
    glClearColor( 1.0f , 1.0 , 1.0 , 1.0f );
    glEnable( GL_MULTISAMPLE );

    camera.setPerspectiveMatrix ( 60.0 , (float) width()/(float)height(), 0.1f , 100.0f );

    shareOpenGLContext();
}


void Canvas3d_Refactored::resizeGL( int width, int height )
{
    glViewport( 0 , 0 , width , height );
    camera.setViewport( Eigen::Vector2f( (float) width, (float) height ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width/(float)height, 0.1f , 100.0f );
}


void Canvas3d_Refactored::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( scene3d != nullptr )
        scene3d->draw( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );

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




void Canvas3d_Refactored::mousePressEvent( QMouseEvent *event )
{
    makeCurrent();

    Eigen::Vector2f screen_pos( event->x(), event->y() );

    if ( event->modifiers() & Qt::ShiftModifier )
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.translateCamera( screen_pos );
        }
    }
    else
    {
        if ( event->button() == Qt::LeftButton )
        {
            camera.rotateCamera( screen_pos );
        }
    }

    QOpenGLWidget::mousePressEvent( event );
    update();

}


void Canvas3d_Refactored::mouseMoveEvent( QMouseEvent *event )
{

    makeCurrent();

    Eigen::Vector2f screen_pos( event->x() , event->y() );


    if ( ( event->modifiers() & Qt::ShiftModifier )  )
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.translateCamera( screen_pos );
        }

    }
    else
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.rotateCamera( screen_pos );
        }

    }

    QOpenGLWidget::mouseMoveEvent( event );
    update();
}


void Canvas3d_Refactored::mouseReleaseEvent( QMouseEvent *event )
{

    makeCurrent();

    if ( event->button ( ) == Qt::LeftButton )
    {
        camera.endTranslation();
        camera.endRotation();
    }

    QOpenGLWidget::mouseReleaseEvent( event );
    update();

}


void Canvas3d_Refactored::wheelEvent( QWheelEvent *event )
{

    float pos = event->delta()/float( WHEEL_STEP );

    if ( pos > 0 )
    {
        camera.increaseZoom ( 1.05f );
    }

    else if( pos < 0 )
    {
        camera.increaseZoom ( 1.0f/1.05f );
    }

    QOpenGLWidget::wheelEvent( event );
    update();
}
