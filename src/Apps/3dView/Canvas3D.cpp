#include "Canvas3D.h"

Canvas3D::Canvas3D()
{
    scene = nullptr;

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
    glEnable( GL_BLEND );
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    camera.setPerspectiveMatrix ( 60.0 , (float) width()/(float)height(), 0.1f , 100.0f );

    shareContext();

}

void Canvas3D::resizeGL( int width , int height )
{
    makeCurrent();
    glViewport ( 0 , 0 , width , height );

    camera.setViewport( Eigen::Vector2f ( (float) width , (float) height ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width/(float)height , 0.1f , 100.0f );
}

void Canvas3D::paintGL()
{
    makeCurrent();

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( scene != nullptr )
        scene->draw( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );


}


void Canvas3D::keyPressEvent( QKeyEvent * event )
{

    makeCurrent();

    switch ( event->key ( ) )
    {
        case Qt::Key_F5:
        {
//            reloadShaders();
        }
        break;

        case Qt::Key_R:
        {
            camera.reset();
//            axes.resetBuffers();
        }
        break;



        default:
            break;


    }

    update();

}


void Canvas3D::mousePressEvent( QMouseEvent *event )
{

    makeCurrent();


    Eigen::Vector2f screen_pos ( event->x(), event->y() );

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



    update();

}


void Canvas3D::mouseMoveEvent ( QMouseEvent *event )
{

    makeCurrent();

    Eigen::Vector2f screen_pos ( event->x() , event->y() );


    if ( ( event->modifiers() & Qt::ShiftModifier )  )
    {

        if ( event->buttons() & Qt::LeftButton )
        {
            camera.translateCamera ( screen_pos );
        }

    }
    else
    {
        if ( event->buttons() & Qt::LeftButton )
        {
            camera.rotateCamera ( screen_pos );
        }

    }

    update( );
}


void Canvas3D::mouseReleaseEvent ( QMouseEvent *event )
{

    makeCurrent();

    if ( event->button ( ) == Qt::LeftButton )
    {
        camera.endTranslation ( );
        camera.endRotation ( );
    }

    update ( );

}


void Canvas3D::wheelEvent ( QWheelEvent *event )
{

    const int WHEEL_STEP = 120;
    float pos = event->delta()/float( WHEEL_STEP );


    if ( event->modifiers() & Qt::ShiftModifier )
    {
//        camera.incrementFov ( pos );
    }

    else if( event->modifiers() & Qt::ControlModifier )
    {
        if ( pos > 0 )
        {
            camera.increaseZoom ( 1.05f );
        }

        else if ( pos < 0 )
        {
            camera.increaseZoom ( 1.0f/1.05f );
        }
    }
    else
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


    QOpenGLWidget::wheelEvent( event );

    update();
}


