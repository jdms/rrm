#include <iostream>
#include <stdlib.h>

#include "canvas3d.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>


Canvas3d::Canvas3d()
{
    createScene();
}


void Canvas3d::initializeGL()
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


void Canvas3d::resizeGL( int width, int height )
{
    glViewport( 0 , 0 , width , height );
    camera.setViewport( Eigen::Vector2f( (float) width, (float) height ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width/(float)height, 0.1f , 100.0f );
}


void Canvas3d::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( scene3d != nullptr )
        scene3d->draw( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );

}


void Canvas3d::createScene()
{
    scene3d = new Scene3d();
    connect( scene3d, SIGNAL( updateCanvas() ), this, SLOT( update() ) );
}


Scene3d* Canvas3d::getScene() const
{
    return scene3d;
}





void Canvas3d::screenshot()
{
    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );

    if( selectedFilter == "PNG (*.png)" )
    {
        savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        savetoVectorImage( name_of_file );
    }
}



void Canvas3d::savetoRasterImage( const QString& filename )
{
    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;

    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_model." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;


    QImage image = grabFramebuffer();
    image.save( filename_csection );
}

void Canvas3d::savetoVectorImage( const QString& filename )
{
    std::cout << "Not implemented yet to 3dview\n" << std::flush;
}



void Canvas3d::shareOpenGLContext()
{
    scene3d->setOpenGLContext( context() );
}




void Canvas3d::mousePressEvent( QMouseEvent *event )
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


void Canvas3d::mouseMoveEvent( QMouseEvent *event )
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


void Canvas3d::mouseReleaseEvent( QMouseEvent *event )
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


void Canvas3d::wheelEvent( QWheelEvent *event )
{

    float pos = event->delta()/float( WHEEL_STEP );


    if( event->modifiers() & Qt::ControlModifier )
    {
        if ( pos > 0 )
        {
            emit decreaseSlider();
        }

        else if ( pos < 0 )
        {
            emit increaseSlider();
        }
    }
    else
    {
        if ( pos > 0 )
        {
            camera.increaseZoom ( 1.05f );
        }

        else if( pos < 0 )
        {
            camera.increaseZoom ( 1.0f/1.05f );
        }
    }

    QOpenGLWidget::wheelEvent( event );
    update();
}
