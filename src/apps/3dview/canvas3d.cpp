#include <iostream>
#include <stdlib.h>

#include "canvas3d.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>
#include <QOffscreenSurface>
#include <QScreen>
#include <QPixmap>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFramebufferObject>


Canvas3d::Canvas3d()
{
    createScene();
    canvas_width = width();
    canvas_height = height();
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

    glViewport( 0 , 0 , (float) width , (float)height );
    camera.setViewport( Eigen::Vector2f( width, (float)height ) );
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


std::string Canvas3d::sendImage( double zmin_, double zmax_, double width_, double height_  )
{

    makeCurrent();


    int V = static_cast< int >( width()/height() );

    int delta_w = width() - static_cast<int>( width_ );
    int delta_h = height() - static_cast<int>( height_ );

    int x_ = static_cast<int>( std::ceil( delta_w*0.5f ) );
    int y_ = static_cast<int>( std::ceil( delta_h*0.5 ) );


    std::cout << "window width: " << width() << ", height: " << height() << std::endl;
    std::cout << "box width: " << width_ << ", height: " << height_ << std::endl;



    Eigen::Matrix3f mat;
    mat = Eigen::AngleAxisf(Math::Constants::HalfPi, Eigen::Vector3f::UnitX() )
            * Eigen::AngleAxisf( 0.0, Eigen::Vector3f::UnitY() )
            * Eigen::AngleAxisf(0.0, Eigen::Vector3f::UnitZ() );
    Eigen::Quaternionf q(mat);


    camera.reset();
    camera.rotate( q );

//    glViewport( 0 , 0 , static_cast<int>( width_ ) , static_cast<int>( height_ ) );

    if( V >= 1 )
    {
        std::cout << "ortographic values: " << V  << ", 0.5" << std::endl << std::flush;
        camera.setOrthographicMatrix( -V *0.5, V *0.5, - 0.5, 0.5, -0.1f, 100.f );
    }
    else
    {
        camera.setOrthographicMatrix( -0.5, 0.5, -V*0.5,V*0.5, 0.1f, 100.f );

    }

    scene3d->setHeightMap( zmin_, zmax_ );
    update();


    QImage image = grabFramebuffer();
    std::string path_ = "./tmp/mapview.png";

    std::cout << "Resolution image from framebuffer: ( " << image.width() << ", "
              << image.height() << ") " << std::endl << std::flush;

//    QImage image1 = image.copy(  x_, y_, static_cast<int>(width_), static_cast<int>(height_) );

    double x = 297;
    double y = 0;

    QImage image1 = image.copy(  static_cast<int>(x), static_cast<int>(y), image.width() - 2*static_cast< int >( x ), image.height() - 2*static_cast< int >( y ) );
    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image1 = image1.transformed( myTransform );
    image1.save( QString( path_.c_str() ) );

    std::cout << "Resolution image from framebuffer: ( " << image1.width() << ", "
              << image1.height() << ") " << std::endl << std::flush;

    canvas_width = width();
    canvas_height = height();

    camera.reset();
    glViewport( 0 , 0 , (float) width() , (float)height() );
    camera.setViewport( Eigen::Vector2f( width(), (float)height() ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width()/(float)height(), 0.1f , 100.0f );

    scene3d->updateObjects();
    update();


    std::cout << "Image origin: " << x_ << ", " << y_ << std::endl << std::flush;

    return path_;
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
