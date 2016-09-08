#include "View3DCanvas.hpp"

View3DCanvas::View3DCanvas ( QWidget* parent ) : QOpenGLWidget ( parent )
{
    scene = nullptr;

}


View3DCanvas::~View3DCanvas()
{
    resetBuffers();
}



void View3DCanvas::clear()
{


}


void View3DCanvas::initializeGL ( )
{

    connect( context(), &QOpenGLContext::aboutToBeDestroyed, this, &View3DCanvas::resetBuffers );

    setFocus();
    setMouseTracking ( true );
    setFocusPolicy ( Qt::StrongFocus );


    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();

    if ( GLEW_OK != glewInitResult )
    {
        std::cerr << "Error: " << glewGetErrorString ( glewInitResult ) << endl;
        exit ( EXIT_FAILURE );
    }



    glClearColor ( 1.0f , 1.0 , 1.0 , 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_CULL_FACE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_MULTISAMPLE );
    glMinSampleShading( 1.0f );



    camera.setPerspectiveMatrix ( 60.0 , (float) width()/(float)height(), 0.1f , 100.0f );

    background = new GradientBackgroundShader();


    emit openglInitialized();


}


void View3DCanvas::resizeGL ( int width , int height )
{

    glViewport ( 0 , 0 , width , height );

    camera.setViewport( Eigen::Vector2f ( (float) width , (float) height ) );
    camera.setPerspectiveMatrix( camera.getFovy(), (float) width/(float)height , 0.1f , 100.0f );

}


void View3DCanvas::paintGL ( )
{

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    background->draw( width(), height() );

    if( scene != nullptr )
        scene->drawScene3D( camera.getViewMatrix(), camera.getProjectionMatrix(), width(), height() );

}




void View3DCanvas::resetBuffers()
{

    if( background )
        background->resetBuffers();


}


void View3DCanvas::reloadShaders ( )
{


    if( background )
        background->reloadShaders();

}




void View3DCanvas::keyPressEvent ( QKeyEvent * event )
{
    switch ( event->key ( ) )
    {
        case Qt::Key_F5:
        {
            reloadShaders();
        }
        break;

        case Qt::Key_R:
        {
            camera.reset();
        }
        break;



        default:
            break;


    }

    update();

}


void View3DCanvas::mousePressEvent ( QMouseEvent *event )
{

    setFocus();


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


void View3DCanvas::mouseMoveEvent ( QMouseEvent *event )
{

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


void View3DCanvas::mouseReleaseEvent ( QMouseEvent *event )
{

    if ( event->button ( ) == Qt::LeftButton )
    {
        camera.endTranslation ( );
        camera.endRotation ( );
    }

    update ( );

}


void View3DCanvas::wheelEvent ( QWheelEvent *event )
{

    const int WHEEL_STEP = 120;
    float pos = event->delta()/float( WHEEL_STEP );


    if ( event->modifiers() & Qt::ShiftModifier )
    {
        camera.incrementFov ( pos );
    }

    else
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

    update();
}


