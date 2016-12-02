#include "Scene.h"

#include <random>

#include <QUrl>
#include <QDir>
#include <QDebug>


Scene::Scene( QObject* parent ): QGraphicsScene( parent )
{
    initData();

}




void Scene::init()
{


    if( views().empty() == true ) return;


    QGraphicsView* view = views()[0];

    int max_value =  std::max( (int)view->width()*0.8f, (int)view->height()*0.8f );
    int min_value =  std::min( (int)view->width()*0.8f, (int)view->height()*0.8f );

    int depth = 200*(max_value/min_value);
    defineVolumeQtCoordinates( 0, 0, 0, (int)view->width()*0.8f, (int)view->height()*0.8f, /*400*/ depth );


    background_image = new QGraphicsPixmapItem();
    addItem( background_image );


    defining_above = false;
    defining_below = false;


    createVolume3D();
    createSketchingBoundary();
    newSketch();

    update();

}




void Scene::initData()
{


    current_mode = InteractionMode::OVERSKETCHING;

    current_color  = QColor( 255, 75, 75 );
    random_color = true;

    qtscene_origin_x = 0;
    qtscene_origin_y = 0;
    qtscene_origin_z = 0;
    qtscene_width  = 0;
    qtscene_height = 0;
    qtscene_depth = 0;


    m_2dto3d = Eigen::Affine3f::Identity();
    m_3dto2d = Eigen::Affine3f::Identity();

    boundary_anchor = QPointF( 0.0f, 0.0f );


    defining_above = false;
    defining_below = false;


    allowed_above_surfaces.clear();
    allowed_below_surfaces.clear();

    selected_above_surfaces.clear();
    selected_below_surfaces.clear();


    background_image = NULL;
    temp_sketch = NULL;
    sketching_boundary = NULL;
    boundary3D = NULL;

}



void Scene::setController( Controller* const& c )
{
    controller = c;
    connect( controller, SIGNAL( updateScene() ), this, SLOT( updateScene() ) );
}



void Scene::defineVolumeQtCoordinates( int origin_x, int origin_y, int origin_z, int width, int height, int depth )
{


    qtscene_origin_x = origin_x;
    qtscene_origin_y = origin_y;
    qtscene_origin_z = origin_z;

    qtscene_width  = width;
    qtscene_height = height;
    qtscene_depth  = depth;


    updateTransformationsMatrices();

}


void Scene::updateTransformationsMatrices()
{


    m_2dto3d = Model3DUtils::normalizePointCloud( qtscene_origin_x, qtscene_origin_x + qtscene_width,
                                                  qtscene_origin_y, qtscene_origin_y + qtscene_height,
                                                  qtscene_origin_z, qtscene_origin_z + qtscene_depth );

    m_3dto2d = m_2dto3d.inverse();




    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );
    QPointF center ( (  2*qtscene_origin_x + qtscene_width )*0.5/L, ( 2*qtscene_origin_y + qtscene_height )* 0.5/L );


    mA = QTransform();
    mA.translate( -center.x(), -center.y() );
    mA.scale( 1/L, 1/L );
    mA = mA.inverted();


}



void Scene::createVolume3D()
{

    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );


    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );
    Eigen::Vector3f dim = max - min;


    boundary3D = new BoundingBox3D( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );
    boundary3D->setCurrentDirectory( shader_directory.toStdString() );

    controller->initRulesProcessor( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z());


    emit initContext();



}




void Scene::createCrossSection( const float& d )
{

    controller->addCrossSection( d );
    addCrossSectionToScene();

}


void Scene::addCrossSectionToScene()
{

    // criar um crosssection desenhavl: ira desenhar um plano e as curvas dentro

    crosssections3d_list.push_back( 0 );

}





void Scene::createSketchingBoundary()
{

    if( controller->hasCrossSection() == false )
        createCrossSection( 0.0f );


    Eigen::Vector3f origin;
    Eigen::Vector3f dimension;

    boundary3D->getMinimum(   origin.x(),    origin.y(),    origin.z() );
    boundary3D->getDimension( dimension.x(), dimension.y(), dimension.z() );

    bool ok = controller->addBoundary( origin.x(), origin.y(), origin.z(), dimension.x(), dimension.y(), dimension.z() );

    if( ok == false  ) return;



    addBoundaryToScene();

}


void Scene::addBoundaryToScene()
{

    if( sketching_boundary != NULL )
        delete sketching_boundary;


    Boundary* b = controller->getCurrentBoundary();
    boundary3D->setGeoData( b );

    sketching_boundary = new BoundaryItem2D();
    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );
    addItem( sketching_boundary );
    setSceneRect( sketching_boundary->boundingRect() );



    arrangement.setBoundary( sketching_boundary->getOriginX(), sketching_boundary->getOriginY(),
                             sketching_boundary->getWidth(), sketching_boundary->getHeight() );




}


void Scene::editBoundary( const int &x, const int &y, const int &w, const int &h )
{


    defineVolumeQtCoordinates( x, y, qtscene_origin_z, abs( w ), abs( h ), qtscene_depth );


    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );


    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );
    Eigen::Vector3f dim = max - min;


    controller->editBoundary( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );


    Boundary* b = controller->getCurrentBoundary();
    boundary3D->setGeoData( b );


    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );
    setSceneRect( sketching_boundary->boundingRect() );


    controller->editRulesProcessor( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );

    arrangement.setBoundary( sketching_boundary->getOriginX(), sketching_boundary->getOriginY(),
                             sketching_boundary->getWidth(), sketching_boundary->getHeight() );

}




bool Scene::addCurve()
{

    current_mode = InteractionMode::INSERTING;


    Curve2D c_qt = PolyQtUtils::qPolyginFToCurve2D( temp_sketch->getSketch() );
    Curve2D c = Scene::scene2Dto3D( c_qt );

    bool add_ok = controller->addCurve( c );
    if( add_ok == false )
    {
        removeItem( temp_sketch );
        return false;
    }


    Stratigraphy* strat = controller->getCurrentStratigraphy();
    size_t id = strat->getId();

    add_ok = arrangement.insert( c_qt, id );
    if( add_ok == false )
    {
        removeItem( temp_sketch );
        newSketch();
        return false;
    }

    addStratigraphyToScene();


}


void Scene::addStratigraphyToScene()
{


    Stratigraphy* strat = controller->getCurrentStratigraphy();
    size_t id = strat->getId();


    Surface* strat3D = new Surface();
    strat3D->setGeoData( strat );
    strat3D->setCurrentDirectory( shader_directory.toStdString() );
    strat3D->setCurrentColor( current_color.redF(), current_color.greenF(), current_color.blueF() );



    stratigraphics_list[ id ] = new StratigraphicItem();
    stratigraphics_list[ id ]->setColor( current_color );
    stratigraphics_list[ id ]->setGeoData( strat );

    addItem( stratigraphics_list[ id ] );

    surfaces_list[ id ] = strat3D;


    emit initContext();

}



void Scene::newSketch()
{

    if( random_color == true )
    {

        std::random_device rd;
        std::mt19937 eng( rd() );
        std::uniform_int_distribution< size_t > distr( 0, 255 );

        int r = distr( eng );
        int g = distr( eng );
        int b = distr( eng );

        current_color = QColor( r, g, b );

    }


    if( temp_sketch != NULL )
    {
        removeItem( temp_sketch );
        delete temp_sketch;
    }


    temp_sketch = NULL;
    temp_sketch = new InputSketch( current_color );
    addItem( temp_sketch );


    current_mode = InteractionMode::OVERSKETCHING;



}


void Scene::undoLastSketch()
{

    if( temp_sketch != NULL )
        temp_sketch->clear();

}



void Scene::clearScene()
{

    stratigraphics_list.clear();
    crosssections3d_list.clear();
    surfaces_list.clear();

    if( controller != NULL )
        controller->clear();

    if( temp_sketch == NULL )
    {
        temp_sketch->clear();
        delete temp_sketch;
    }

    if( sketching_boundary != NULL )
    {
        sketching_boundary->clear();
        delete sketching_boundary;
    }

    if( boundary3D != NULL )
    {
        boundary3D->clear();
        delete boundary3D;
    }

    arrangement.clear();


    clear();
    initData();
    init();

    emit updatedScene();

}


void Scene::updateScene()
{

    float d = controller->getCurrentCrossSection();

    std::map< size_t, std::vector< Curve2D > > curve_map;
    std::map< size_t, StratigraphicItem* >::iterator it;

    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->update( mA/*m_3dto2d*/, d );

        curve_map[ strat->getId() ] = strat->getSubCurves2D();
    }

    arrangement.updateSubcurves( curve_map );

    emit updateContext();
    emit updatedScene();

    update();

}


void Scene::drawScene3D( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{


    int number_of_surfaces = surfaces_list.size();
    std::map< size_t, Surface* >::iterator it;

    for( it = surfaces_list.begin(); it != surfaces_list.end(); ++it )
    {
        Surface* s = it->second;
        s->draw( V, P, width, height );
    }


    if( boundary3D != 0 )
        boundary3D->draw( V, P, width, height );

}






void Scene::updateGLContext()
{


    boundary3D->update();

    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    Eigen::Vector3f center( 0.5f*qtscene_width/L, 0.5f*qtscene_height/L, 0.5f*qtscene_depth/L ) ;

    std::map< size_t, Surface* >::iterator it;
    for( it = surfaces_list.begin(); it != surfaces_list.end(); ++it )
    {
        Surface* s = it->second;
        s->update( center );
    }


}


void Scene::initGLContext()
{

    if( boundary3D->initialized() == false )
    {
        boundary3D->init();
        boundary3D->create();
    }


    std::map< size_t, Surface* >::iterator it;

    for( it = surfaces_list.begin(); it != surfaces_list.end(); ++it )
    {
        Surface* s = it->second;
        if( s->initialized() == false )
            s->init();

    }


}


void Scene::updateColor( const QColor& color )
{

    current_color = color;


    if( temp_sketch != NULL )
        temp_sketch->setColor( current_color );


    random_color = false;

    update();

}






void Scene::enableSketchingAboveRegion( bool option )
{

    std::cout << "\n-- Enabling define above = " << option << "\n"  << std::flush;
    std::cout << "\t-- Define above = " << option << ", define below = "  << defining_below << "\n" << std::flush;


    if( option == false )
    {
        stopSketchingAboveRegion();
        return;
    }

    bool define_above = controller->defineSketchingAbove( allowed_above_surfaces );
    if( define_above == false )
    {
        return;
    }

    defineSketchingAboveRegion();


}


/// if it is ok to sketching above it highlited the allowed sketches and wait the user
/// selected on of the highlited sketches
void Scene::defineSketchingAboveRegion()
{

    setSelectionMode( true );

    size_t number_allowed_surfaces = allowed_above_surfaces.size();
    std::cout << "\t-- Surfaces (above) are allowed = " << number_allowed_surfaces << std::flush;
    std::cout << "\t\t-- Allowed (above) surfaces = " << std::flush;

    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_above_surfaces[ i ];
        std::cout << id << ", " << std::flush;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );
    }

    std::cout << "\n" << std::flush;

    current_mode = InteractionMode::SELECTING_ABOVE;
    defining_above = true;

    update();


}




void Scene::stopSketchingAboveRegion()
{


    if( defining_below == false )
    {
        std::cout << "\t-- Above and Below are false... \n" << std::flush;
        setSelectionMode( false );
    }
    else{


        size_t number_allowed_surfaces = allowed_above_surfaces.size();


        std::cout << "\t-- Stop only define above, below is still on\n" << std::flush;
        std::cout << "\t-- Allowed (above) surfaces to be stopped = " << number_allowed_surfaces << "\n" << std::flush;
        std::cout << "\t\t-- Stopped surfaces = " << std::flush;

        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_above_surfaces[ i ];

            std::cout << id << ", " << std::flush;

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
        }



        size_t number_selected_surfaces = selected_below_surfaces.size();

        std::cout << "\n\t-- Reselect (below) surfaces = " << number_selected_surfaces << "\n"  << std::flush;
        std::cout << "\t\t-- Below surfaces = " << std::flush;


        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_below_surfaces[ i ];
            std::cout << id << ", " << std::flush;

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setSelection( true );

        }

        std::cout << "\n" << std::flush;
    }

    defining_above = false;

    allowed_above_surfaces.clear();
    selected_above_surfaces.clear();

    controller->stopSketchingAbove();
    current_mode = InteractionMode::OVERSKETCHING;


    update();


}






void Scene::enableSketchingBelowRegion( bool option )
{

    std::cout << "\n-- Enabling define below = " << option << "\n"  << std::flush;
    std::cout << "\t-- Define above = " << defining_above << ", define below = "  << option << "\n" << std::flush;

    if( option == false )
    {
        stopSketchingBelowRegion();
        return;
    }

    bool define_below = controller->defineSketchingBelow( allowed_below_surfaces );
    if( define_below == false )
    {
        return;
    }

    defineSketchingBelowRegion();

}


/// if it is ok to sketching below it highlited the allowed sketches and wait the user
/// selected on of the highlited sketches
void Scene::defineSketchingBelowRegion()
{


    setSelectionMode( true );
    size_t number_allowed_surfaces = allowed_below_surfaces.size();

    std::cout << "\t-- Surfaces (below) are allowed = " << number_allowed_surfaces << std::flush;
    std::cout << "\t\t-- Allowed (below) surfaces = " << std::flush;


    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_below_surfaces[ i ];
        std::cout << id << ", " << std::flush;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );
    }

    std::cout << "\n" << std::flush;

    current_mode = InteractionMode::SELECTING_BELOW;
    defining_below = true;

    update();


}



void Scene::stopSketchingBelowRegion()
{


    if( defining_above == false )
    {
        std::cout << "\t-- Above and Below are false... \n" << std::flush;
        setSelectionMode( false );
    }
    else
    {

        size_t number_allowed_surfaces = allowed_below_surfaces.size();

        std::cout << "\t-- Stop only define below, above is still on\n" << std::flush;
        std::cout << "\t-- Allowed (below) surfaces to be stopped = " << number_allowed_surfaces << "\n" << std::flush;
        std::cout << "\t\t-- Stopped surfaces = " << std::flush;

        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_below_surfaces[ i ];
            std::cout << id << ", " << std::flush;

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
        }

        size_t number_selected_surfaces = selected_above_surfaces.size();
        std::cout << "\n\t-- Reselect (above) surfaces = " << number_selected_surfaces << "\n"  << std::flush;
        std::cout << "\t\t-- Above surfaces = " << std::flush;

        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_above_surfaces[ i ];
            std::cout << id << ", " << std::flush;

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setAllowed( true );
            strat->setSelection( true );
        }

        std::cout << "\n" << std::flush;


    }

    defining_below = false;

    allowed_below_surfaces.clear();
    selected_below_surfaces.clear();

    controller->stopSketchingBelow();
    current_mode = InteractionMode::OVERSKETCHING;

    update();

}




void Scene::disallowCurves( const std::vector< size_t >& curves_id )
{

    size_t number_allowed = curves_id.size();

    std::cout << "\n\t-- Not allow surfaces anymore = " << std::flush;


    for ( size_t i = 0; i < number_allowed; ++i )
    {
        size_t id = curves_id[ i ];


        StratigraphicItem* strat = stratigraphics_list[ id ];
        if( strat->getSelection() ) continue;

        std::cout << id << ", " << std::flush;

        strat->setAllowed( false );
        strat->setSelection( false );
        strat->setUnderOperation( false );

    }
    std::cout << "\n" << std::flush;

}


/// Allow to select sketches
void Scene::setSelectionMode( const bool status )
{

    std::cout << "\t-- Set selection mode to all ... " << status << "\n" << std::flush;


    std::map< size_t, StratigraphicItem* >::iterator it;
    for ( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->setUnderOperation( status );
    }

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
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );

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
    size_t number_of_points = c.size();

    for( size_t i = 0; i < number_of_points; ++i )
    {
        Eigen::Vector3f p = scene2Dto3D( c.at( i ) );
        c3d.add( Point2D( p.x(), p.y() ) );
    }

    return c3d;

}


Curve2D Scene::scene3Dto2D( const Curve2D &c )
{

    Curve2D c2d;
    size_t number_of_points = c.size();

    for( size_t i = 0; i < number_of_points; ++i )
    {
        Point2D p = c.at( i );
        c2d.add( scene3Dto2D( Eigen::Vector3f( p.x(), p.y(), 0.0f ) ) );
    }

    return c2d;

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






std::vector< size_t > Scene::getAllSelectedItems()
{

    std::vector< size_t > id_items;

    QList< QGraphicsItem* > items = selectedItems();
    QList< QGraphicsItem* >::iterator it;

    for ( it = items.begin(); it != items.end(); ++it ){
        StratigraphicItem* s = (StratigraphicItem*) (*it);
        id_items.push_back( (size_t) s->getId() );
        s->setSelection( true );
    }
    return id_items;

}




void Scene::setBackGroundImage( const QString& url )
{
    QPixmap image = QPixmap( url );

    QTransform myTransform;
    myTransform.scale( 1, -1 );
    image = image.transformed( myTransform );

    editBoundary( image.rect().x(), image.rect().y(), image.rect().width(), image.rect().height() );

    background_image->setPixmap( image );

    update();

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
            temp_sketch->create( event->scenePos() );
        }


    }

    else if ( event->buttons() & Qt::RightButton )
    {

        if( temp_sketch->isEmpty() ) return;


        bool add_ok = addCurve();
        if( add_ok == false )
            return;

        // should be fixed
        std::vector< size_t > upper_bound = arrangement.getLastCurveLowerBound();
        std::vector< size_t > lower_bound = arrangement.getLastCurveUpperBound();


        controller->interpolateStratigraphy( lower_bound, upper_bound );

        newSketch();
        controller->addStratigraphy();

    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

	Eigen::Vector3f p = scene2Dto3D(Point2D(event->scenePos().x(), event->scenePos().y()));
	Point2D pb = scene3Dto2D(p);
	emit sendCoordinates(pb.x(), pb.y());

    //emit sendCoordinates( event->scenePos().x(), event->scenePos().y() );


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

            sketching_boundary->edit( boundary_anchor.x(), boundary_anchor.y(), w,  h );

        }

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();

}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if ( current_mode == InteractionMode::OVERSKETCHING  )
    {
        temp_sketch->process( event->scenePos() );
    }


    else if( current_mode == InteractionMode::BOUNDARY )
    {

        int minx = std::min( boundary_anchor.x(), event->scenePos().x() );
        int miny = std::min( boundary_anchor.y(), event->scenePos().y() );

        int maxx = std::max( boundary_anchor.x(), event->scenePos().x() );
        int maxy = std::max( boundary_anchor.y(), event->scenePos().y() );


        int w = std::abs( maxx - minx );
        int h = std::abs( maxy - miny );


        editBoundary( minx, miny, w, h );

        current_mode = InteractionMode::OVERSKETCHING;

    }


    else if( current_mode == InteractionMode::SELECTING_ABOVE )
    {

        selected_above_surfaces.clear();
        selected_above_surfaces = getAllSelectedItems();
        if( selected_above_surfaces.empty() == true ) return;

        controller->defineRegionAbove( selected_above_surfaces );
        disallowCurves( allowed_above_surfaces );

/*        testing

        QPainterPath bound;
        bound.addRect( sketching_boundary->boundingRect() );


        int id = selected_above_surfaces[ 0 ];
        StratigraphicItem* strat0 = stratigraphics_list[ id ];
        QPainterPath path0 = strat0->getPath();

        QPointF p0 = path0.pointAtPercent( 0 );
        QPointF pn = path0.pointAtPercent( 1 );

        QPointF pB = QPointF( pn.x(), qtscene_height );
        QPointF pA = QPointF( p0.x(), qtscene_height );


        path0.lineTo( pB );
        path0.lineTo( pA );
        path0.lineTo( p0 );

//        addPath( path0, QPen( QColor( Qt::blue ) ), QBrush( QColor( Qt::gray ) ) );



        QPainterPath path1;

        if( selected_below_surfaces.empty() == true ){
            path1 = QPainterPath();
            path1.addPolygon( QPolygonF( sketching_boundary->boundingRect() ) );
        }
        else
        {
            int id1 = selected_below_surfaces[ 0 ];
            StratigraphicItem* strat1 = stratigraphics_list[ id1 ];
            path1 = strat1->getPath();
        }


        QPointF p10 = path1.pointAtPercent( 0 );
        QPointF p1n = path1.pointAtPercent( 1 );

        QPointF p1B = QPointF( p1n.x(), qtscene_origin_y );
        QPointF p1A = QPointF( p10.x(), qtscene_origin_y );


        path1.lineTo( p1B );
        path1.lineTo( p1A );
        path1.lineTo( p10 );

//        addPath( path1, QPen( QColor( Qt::darkCyan ) ), QBrush( QColor( Qt::cyan ) ) );

        QPainterPath A = path0.intersected( path1 );
        QPainterPath B = path1.intersected( path0 );

        QPainterPath punion = path0.intersected( path1 ) + ( path0 - path1 ) + ( path1 - path0 );
        //punion = punion.intersected( bound );
        addPath( punion, QPen( QColor( Qt::darkCyan ) ), QBrush( QColor( Qt::cyan ) ) );

*/



        current_mode = InteractionMode::OVERSKETCHING;
        emit enableSketching( true );


    }


    else if( current_mode == InteractionMode::SELECTING_BELOW )
    {

        selected_below_surfaces.clear();
        selected_below_surfaces = getAllSelectedItems();
        if( selected_below_surfaces.empty() == true ) return;

        controller->defineRegionBelow( selected_below_surfaces );
        disallowCurves( allowed_below_surfaces );





        current_mode = InteractionMode::OVERSKETCHING;
        emit enableSketching( true );

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

    setBackGroundImage( url_file );


}



void Scene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}


void Scene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}


void Scene::send2Dto3DMatrix(Eigen::Affine3f& p_2d_to_3d)
{
	p_2d_to_3d = this->m_2dto3d;

	std::cout << "-- Scene --" << std::endl;
	std::cout << this->m_2dto3d.matrix() << std::endl;

}
void Scene::send3Dto2DMatrix(Eigen::Affine3f& p_3d_to_2d)
{
	p_3d_to_2d = this->m_3dto2d;
	std::cout << "-- Scene --" << std::endl;
	std::cout << this->m_3dto2d.matrix() << std::endl;
	

}