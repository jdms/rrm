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

    std::cout << "zmax: " << zmax_ << ", zmin: " << zmin_ << std::endl << std::flush;
    std::cout << "height_: " << height_ << std::endl << std::flush;

    makeCurrent();
    std::string path_ = "./tmp/mapview.png";
    int width1 = static_cast< int >( width_ );
    int height1 = static_cast< int >( height_ );

    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    QOpenGLFramebufferObject* m_fbo = new QOpenGLFramebufferObject( width(), height(), format);

    m_fbo->bind();


    Eigen::Matrix3f mat;
    mat = Eigen::AngleAxisf(Math::Constants::HalfPi, Eigen::Vector3f::UnitX() )
            * Eigen::AngleAxisf( 0.0, Eigen::Vector3f::UnitY() )
            * Eigen::AngleAxisf(0.0, Eigen::Vector3f::UnitZ() );
    Eigen::Quaternionf q(mat);

    glViewport( 0 , 0 , width(), height() );


    camera.reset();
    camera.setViewport( Eigen::Vector2f( width(), height() ) );
    camera.rotate( q );


    double V = static_cast< double >( width_/height_ );
    double V_ = static_cast< double >( width()/height() );

    double W = static_cast< double >( width_/width() );
    double H = static_cast< double >( height_/height() );
    double V1 = static_cast< double >( V/V_ );


    if( V >= 1)
        camera.setOrthographicMatrix( -0.5, 0.5, -0.5/V, 0.5/V, 0.1f, 100.f );
    else
    {

        if( zmax_ >= height1 )
        {
            camera.setOrthographicMatrix( -0.5*(width_/width()), 0.5*(width_/width()), -0.5/(zmax_/height1),0.5/(zmax_/height1), 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5/(zmax_/height1), 0.5/(zmax_/height1), -0.5*V,0.5*V, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5/(zmax_/height1), 0.5/(zmax_/height1), -0.5,0.5, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5*(zmax_/height1), 0.5*(zmax_/height1), -0.5,0.5, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5/V, 0.5/V, -0.5*V,0.5*V, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5, 0.5, -0.5,0.5, 0.1f/V, 100.f/V );
//            camera.setOrthographicMatrix( -0.5, 0.5, -0.5,0.5, 0.1f*V, 100.f*V );
//            camera.setOrthographicMatrix( -0.5, 0.5, -0.5/V,0.5/V, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5, 0.5, -0.5*V,0.5*V, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5/V, 0.5/V, -0.5,0.5, 0.1f, 100.f );
//            camera.setOrthographicMatrix( -0.5*V, 0.5*V, -0.5,0.5, 0.1f, 100.f );
            std::cout << "A: height greater than depth" << std::flush << std::endl;
        }
        else
        {
            camera.setOrthographicMatrix( -0.5*V, 0.5*V, -0.5,0.5, 0.1f, 100.f );
            std::cout << "B: height less than depth" << std::flush << std::endl;
        }

    }




    scene3d->setHeightMap( zmin_, zmax_ );



    paintGL();


    QOpenGLFramebufferObject* m_dest = new QOpenGLFramebufferObject( width1, height1, format );
    QOpenGLFramebufferObject::blitFramebuffer( m_dest, m_fbo );

    QImage image2 = m_dest->toImage();
    image2.save( QString( path_.c_str() ) );

    m_fbo->release();



    delete m_fbo;
    delete m_dest;




    canvas_width = width();
    canvas_height = height();

    camera.reset();
    glViewport( 0 , 0 , (float) width() , (float)height() );
    camera.setViewport( Eigen::Vector2f( width(), (float)height() ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width()/(float)height(), 0.1f , 100.0f );

    scene3d->updateObjects();
    update();


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
