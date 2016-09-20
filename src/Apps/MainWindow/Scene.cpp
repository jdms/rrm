#include "Scene.h"

#include <QUrl>
#include <QDir>
#include <QDebug>


Scene::Scene( QObject* parent ): QGraphicsScene( parent )
{
    boundary3D = 0;
    boundary = 0;
    int_width = 0;
    int_height = 0;
    int_depth = 400;

    sketch = 0;
    temp_sketch = 0;

    current_color = QColor( 255, 75, 75 );
}


void Scene::init()
{

    if( views().empty() == true ) return;
    QGraphicsView* view = views()[0];

    updateSpace3D( 0.8f*view->width(), 0.8f*view->height(), int_depth );

    createVolume3D();
    createBoundary();

//    Surface *s = new Surface( 2 );
//    s->loadBuffers();
//    surfaces_list.push_back( s );

    newSketch();

}



void Scene::initGLContext()
{
    if( boundary3D->initialized() == false )
        boundary3D->init();

    int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* s = surfaces_list[ i ];
        if( s->initialized() == false )
            s->init();
    }


}



void Scene::drawScene3D( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{

    if( boundary3D != 0 )
        boundary3D->draw( V, P, width, height );

    int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* s = surfaces_list[ i ];
        s->draw( V, P, width, height );
    }


}


void Scene::updateSpace3D( const int& width, const int& height, const int& depth )
{

    int_width = width;
    int_height = height;
    int_depth = depth;

    m_2dto3d = Model3DUtils::normalizePointCloud( -0.5f*int_width, 0.5f*int_width, -0.5f*int_height, 0.5f*int_height, -0.5f*int_depth, 0.5f*int_depth );
    m_3dto2d = m_2dto3d.inverse();

}




void Scene::createVolume3D()
{

    if( boundary3D != 0 || views().empty() == true ) return;


    Eigen::Vector3f min( -0.5f*int_width, -0.5f*int_height, -0.5f*int_depth );
    Eigen::Vector3f max(  0.5f*int_width,  0.5f*int_height,  0.5f*int_depth );

    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );


    Eigen::Vector3f dim = max - min;

    boundary3D = new BoundingBox3D( dim.x(), dim.y(), dim.z() );

//    emit initContext();
    boundary3D->init();
    boundary3D->create();



}





void Scene::createCrossSection( const float& d )
{

    controller->addCrossSection( d );
    addCrossSectionToScene();

}


void Scene::addCrossSectionToScene()
{

    // criar um crosssection desenhavel: ira desenhar um plano e as curvas dentro

    crosssections3d_list.push_back( 0 );

}





void Scene::createBoundary()
{



    if( controller->hasCrossSection() == false )
        createCrossSection( 0.0f );



    bool ok = controller->addBoundary( boundary3D->getWidth(), boundary3D->getHeight(), boundary3D->getDepth() );
    if( ok == false  ) return;


    addBoundaryToScene();


}


void Scene::addBoundaryToScene()
{

    Boundary* b = controller->getCurrentBoundary();

    if( boundary != 0 )
        delete boundary;

    boundary = new BoundaryItem2D();
    boundary->setGeoData( b );
    boundary->update( m_3dto2d );


    addItem( boundary );
    setSceneRect( boundary->boundingRect() );


    // criar boundary no 3d, e setar o dado geologico


    boundary3D->setGeoData( b );
    boundary3D->update();

//    emit initContext();



}


void Scene::editBoundary( const int &x, const int &y, const int &w, const int &h )
{


    updateSpace3D( w, h, int_depth );


    Eigen::Vector3f p( x, y, 0.0f );
    Eigen::Vector3f p1( x + w, y + h, 0.0f );


    p  = scene2Dto3D( p );
    p1 = scene2Dto3D( p1 );

    float width  = ( p1 - p ).x();
    float height = ( p1 - p ).y();
    float depth = boundary3D->getDepth();



    controller->editBoundary( p.x(), p.y(), width, height, depth );


    boundary->update( m_3dto2d );
    setSceneRect( boundary->boundingRect() );
    boundary3D->update();


    controller->setRulesProcessorBoundingBox( 0.0f, 0.0f, 0.0f, (int)boundary3D->getWidth(), (int)boundary3D->getHeight(), (int)boundary3D->getDepth() );



}



void Scene::addCurve()
{


    current_mode = InteractionMode::INSERTING;


    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( sketch->getCurve() );
    c = Scene::scene2Dto3D( c );

    bool add_ok = controller->addCurve( c );
    if( add_ok == false )
    {
        removeItem( sketch );
        return;
    }


    addStratigraphyToScene();

}


void Scene::addStratigraphyToScene()
{


    Stratigraphy* strat = controller->getCurrentStratigraphy();
    sketch->setGeoData( strat );


    Surface* strat3D = new Surface();
    strat3D->setGeoData( strat );

    emit initContext();

    stratigraphics_list.push_back( sketch );
    surfaces_list.push_back( strat3D );



}






void Scene::newSketch()
{

    sketch = new StratigraphicItem();
    sketch->setColor( current_color );
    addItem( sketch );

    temp_sketch = NULL;

    current_mode = InteractionMode::OVERSKETCHING;

}


void Scene::undoLastSketch()
{

    if( sketch != NULL )
        sketch->clear();

    if( temp_sketch != NULL )
        temp_sketch->clear();

}





void Scene::updateColor( const QColor& color )
{

    current_color = color;

    if( temp_sketch != NULL )
        temp_sketch->setColor( color );

    if( sketch != NULL )
        sketch->setColor( color );

    update();

}





Eigen::Vector3f Scene::scene2Dto3D( const Point2D &p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), 0.0f, 1.0f );

    p_cpy = m_2dto3d.matrix()*p_cpy;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );

}


Eigen::Vector3f Scene::scene2Dto3D( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );

    p_cpy = m_2dto3d.matrix()*p_cpy;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );;

}


Point2D Scene::scene3Dto2D( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );

    p_cpy = m_3dto2d.matrix()*p_cpy;
    return Point2D( p_cpy.x(), p_cpy.y() );

}


Curve2D Scene::scene2Dto3D( const Curve2D& c )
{

    Curve2D c3d;
    unsigned int number_of_points = c.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        Eigen::Vector3f p( scene2Dto3D( c.at( i ) ) );
        c3d.add( Point2D( p.x(), p.y() ) );
    }

    return c3d;

}


Curve2D Scene::scene3Dto2D( const Curve2D &c )
{

    Curve2D c2d;
    unsigned int number_of_points = c.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        Point2D p = c.at( i );
        c2d.add( scene3Dto2D( Eigen::Vector3f( p.x(), p.y(), 0.0f ) ) );
    }

    return c2d;

}






void Scene::updateScene()
{

//    boundary->update();
//    boundary3D->update();

    float d = controller->getCurrentCrossSection();

    unsigned int number_of_stratigraphies = stratigraphics_list.size();
    for( int i = 0; i < number_of_stratigraphies; ++i )
    {
        StratigraphicItem* strat = stratigraphics_list[ i ];
        strat->update( m_3dto2d, d );
    }


    unsigned int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* surface = surfaces_list[ i ];
        surface->update();
    }


    emit updatedScene();

}




void Scene::savetoRasterImage( const QString& filename )
{

    QImage image( sceneRect().size().toSize(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    render( &painter );

    image = image.mirrored( false, true );
    image.save( filename );

}


void Scene::savetoVectorImage( const QString& filename )
{

    QSvgGenerator svgGen;

    svgGen.setFileName( filename );
    svgGen.setSize( QSize( width(), height() ) );

    svgGen.setViewBox( sceneRect() );
    svgGen.setTitle( tr( "Rapid Reservoir Modelling - SVG generated by Qt5" ) );
    svgGen.setDescription( tr( "SVG output of Rapid Reservoir Modelling software" ) );

    QPainter painter( &svgGen );
    painter.scale( 1.0, -1.0 );
    painter.translate( QPointF( 0.0, -height() ) );

    render( &painter );
}





void Scene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_mode == InteractionMode::BOUNDARY )
        {
            boundary_anchor = event->buttonDownScenePos( Qt::LeftButton );
        }

        else if( current_mode == InteractionMode::OVERSKETCHING )
        {

            temp_sketch = new InputSketch( current_color );
            temp_sketch->create( event->scenePos() );
            addItem( temp_sketch );

        }

    }

    else if ( event->buttons() & Qt::RightButton )
    {
        addCurve();
        controller->interpolateStratigraphy();

        newSketch();
        controller->addStratigraphy();

    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    emit sendCoordinates( event->scenePos().x(), event->scenePos().y() );


    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_mode == InteractionMode::OVERSKETCHING )
        {
            temp_sketch->add( event->scenePos() );
        }

        else if( current_mode == InteractionMode::BOUNDARY )
        {

            int w = event->scenePos().x() - boundary_anchor.x();
            int h = event->scenePos().y() - boundary_anchor.y();

            boundary->edit( boundary_anchor.x(), boundary_anchor.y(), w,  h );

        }

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();

}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if ( current_mode == InteractionMode::OVERSKETCHING  )
    {

        if( temp_sketch == NULL ) return;


        sketch->addSegment( *temp_sketch );
        removeItem( temp_sketch );

        delete temp_sketch;
        temp_sketch = NULL;

    }

    else if( current_mode == InteractionMode::BOUNDARY )
    {

        int w = event->scenePos().x() - boundary_anchor.x();
        int h = event->scenePos().y() - boundary_anchor.y();


        editBoundary( boundary_anchor.x(), boundary_anchor.y(), w, h );

        current_mode = InteractionMode::OVERSKETCHING;

    }

    QGraphicsScene::mouseReleaseEvent( event );
    update();



}




void Scene::dragEnterEvent( QGraphicsSceneDragDropEvent *event )
{
    event->accept();
}


void Scene::dropEvent( QGraphicsSceneDragDropEvent *event )
{

    const QMimeData *mime_data = event->mimeData();


    if ( mime_data->hasUrls() == false )
        return;


    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    boundary->setBackGroundImage( url_file );


}


void Scene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}


void Scene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}

