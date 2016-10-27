#include "Scene.h"

#include <random>

#include <QUrl>
#include <QDir>
#include <QDebug>


Scene::Scene( QObject* parent ): QGraphicsScene( parent )
{
    boundary3D = 0;
    sketching_boundary = 0;

    sketch = 0;
    temp_sketch = NULL;

    current_color = QColor( 255, 75, 75 );
    random_color = true;

}




void Scene::init()
{


    if( views().empty() == true ) return;
    QGraphicsView* view = views()[0];


    int max_value =  std::max( (int)view->width()*0.8f, (int)view->height()*0.8f );
    int min_value =  std::min( (int)view->width()*0.8f, (int)view->height()*0.8f );

    int teste = 200*(max_value/min_value);
    defineVolumeQtCoordinates( 0, 0, 0, (int)view->width()*0.8f, (int)view->height()*0.8f, /*400*/ teste );



    defining_above = false;
    defining_below = false;


    createVolume3D();
    createSketchingBoundary();
    newSketch();

    update();

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



    Eigen::Vector4f orig_qt = Eigen::Vector4f( qtscene_origin_x, qtscene_origin_y, qtscene_origin_z, 1.0f );

    m_2dtoplanin = Eigen::Affine3f::Identity();
    m_2dtoplanin.translation() = -Eigen::Vector3f( orig_qt.x(), orig_qt.y(), orig_qt.z() )/L ;
    m_2dtoplanin.scale( Eigen::Vector3f( 1/L, 1/L, 1/L ) );


    m_planinto2d = m_2dtoplanin.inverse();


}



void Scene::createVolume3D()
{

    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );


    Eigen::Vector3f min_pl = Scene::scene2DtoPlanin( min );
    Eigen::Vector3f max_pl = Scene::scene2DtoPlanin( max );
    Eigen::Vector3f dim_pl = max_pl - min_pl;



    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );
    Eigen::Vector3f dim = max - min;



    boundary3D = new BoundingBox3D( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );
    boundary3D->setCurrentDirectory( shader_directory.toStdString() );

    controller->initRulesProcessor( min_pl.x(), min_pl.y(), min_pl.z(), dim_pl.x(), dim_pl.y(), dim_pl.z() );

    emit initContext();



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

    if( sketching_boundary != 0 )
        delete sketching_boundary;


    Boundary* b = controller->getCurrentBoundary();
    boundary3D->setGeoData( b );
    boundary3D->update();

    sketching_boundary = new BoundaryItem2D();
    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );


    arrangement.setBoundary( sketching_boundary->getOriginX(), sketching_boundary->getOriginY(),
                             sketching_boundary->getWidth(), sketching_boundary->getHeight() );


    addItem( sketching_boundary );
    setSceneRect( sketching_boundary->boundingRect() );


}


void Scene::editBoundary( const int &x, const int &y, const int &w, const int &h )
{


    defineVolumeQtCoordinates( x, y, qtscene_origin_z, abs( w ), abs( h ), qtscene_depth );


    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );


    Eigen::Vector3f min_pl = Scene::scene2DtoPlanin( min );
    Eigen::Vector3f max_pl = Scene::scene2DtoPlanin( max );
    Eigen::Vector3f dim_pl = max_pl - min_pl;



    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );
    Eigen::Vector3f dim = max - min;


    controller->editBoundary( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );


    Boundary* b = controller->getCurrentBoundary();
    boundary3D->setGeoData( b );


    sketching_boundary->setGeoData( b );
    setSceneRect( sketching_boundary->boundingRect() );


    controller->editRulesProcessor( min_pl.x(), min_pl.y(), min_pl.z(), dim_pl.x(), dim_pl.y(), dim_pl.z() );


}




void Scene::addCurve()
{

    current_mode = InteractionMode::INSERTING;


    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( temp_sketch->getSketch() );


    size_t number_of_points = c.size();
    c = Scene::scene2DtoPlanin( c );

    bool add_ok = controller->addCurve( c );


    if( add_ok == false )
    {
        removeItem( temp_sketch );
        return;
    }


    addStratigraphyToScene();


}


void Scene::addStratigraphyToScene()
{


    Stratigraphy* strat = controller->getCurrentStratigraphy();
    size_t id = strat->getId();


    float depth_current = controller->getCurrentCrossSection();
    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( temp_sketch->getSketch() );
    arrangement.insert( c, id );


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


    current_mode = InteractionMode::OVERSKETCHING;


    current_color  = QColor( 255, 75, 75 );
    random_color = true;


    qtscene_origin_x = 0;
    qtscene_origin_y = 0;
    qtscene_origin_z  = 0;
    qtscene_width  = 0;
    qtscene_height = 0;
    qtscene_depth = 0;



    m_2dto3d = Eigen::Affine3f::Identity();;
    m_3dto2d = Eigen::Affine3f::Identity();;
    m_2dtoplanin = Eigen::Affine3f::Identity();;
    m_planinto2d = Eigen::Affine3f::Identity();;


    boundary_anchor = QPointF( 0.0f, 0.0f );


    stratigraphics_list.clear();
    crosssections3d_list.clear();
    surfaces_list.clear();

    allowed_above_surfaces.clear();
    allowed_below_surfaces.clear();

    selected_above_surfaces.clear();
    selected_below_surfaces.clear();


    if( controller != 0 )
        controller->clear();

    if( temp_sketch == NULL )
    {
        temp_sketch->clear();
        delete temp_sketch;
    }

    if( sketching_boundary != 0 )
    {
        sketching_boundary->clear();
        delete sketching_boundary;
    }

    if( boundary3D != 0 )
    {
        boundary3D->clear();
        delete boundary3D;
    }


    boundary3D = 0;
    sketching_boundary = 0;
    temp_sketch = NULL;


    arrangement.clear();

    clear();
    init();

    emit updatedScene();

}


void Scene::updateScene()
{


    sketching_boundary->update( m_3dto2d );

    float d = controller->getCurrentCrossSection();

    std::map< size_t, std::vector< Curve2D > > curve_map;
    std::map< size_t, StratigraphicItem* >::iterator it;

    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->update( m_planinto2d, d );

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

//    if( sketch != NULL )
//        sketch->setColor( current_color );

    random_color = false;

    update();

}






void Scene::enableSketchingAboveRegion( bool option )
{

    if( option == false )
    {
        stopSketchingAboveRegion();
        return;
    }

    bool define_above = controller->defineSketchingAbove( allowed_above_surfaces );
    if( define_above == false )
    {
        stopSketchingAboveRegion();
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

    std::cout << "-- Define sketch above... " << std::endl;
    std::cout << "\t * " << number_allowed_surfaces << " allowed Curves: ";

    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_above_surfaces[ i ];
        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );

        std::cout << id << ", ";
    }

    std::cout << "\n";

    current_mode = InteractionMode::SELECTING_ABOVE;
    defining_above = true;

    update();


}




void Scene::stopSketchingAboveRegion()
{


    if( defining_below == false )
    {
        setSelectionMode( false );
    }
    else{

        std::cout << "-- Stop only sketch above... " << std::endl;

        size_t number_allowed_surfaces = allowed_above_surfaces.size();

        std::cout << "\t * Turning off " << number_allowed_surfaces << "  Curves: ";

        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_above_surfaces[ i ];
            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
            std::cout << id << ", ";
        }

        std::cout << "\n";



        size_t number_selected_surfaces = selected_below_surfaces.size();

        std::cout << "\t Still on * " << number_selected_surfaces << " selected curves from sketch below: ";


        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_below_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setSelection( true );

            std::cout << id << ", ";
        }

        std::cout << "\n";
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


    if( option == false )
    {
        stopSketchingBelowRegion();
        return;
    }

    bool define_below = controller->defineSketchingBelow( allowed_below_surfaces );
    if( define_below == false )
    {
//        stopSketchingBelowRegion();
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

    std::cout << "-- Define sketch below... " << std::endl;
    std::cout << "\t * " << number_allowed_surfaces << " allowed Curves: ";


    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_below_surfaces[ i ];
        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );
        std::cout << id << ", ";
    }

    std::cout << "\n";


    current_mode = InteractionMode::SELECTING_BELOW;
    defining_below = true;

    update();


}



void Scene::stopSketchingBelowRegion()
{


    if( defining_above == false )
    {
        setSelectionMode( false );
    }
    else
    {

        std::cout << "-- Stop only sketch below... " << std::endl;

        size_t number_allowed_surfaces = allowed_below_surfaces.size();

        std::cout << "\t * Turning off " << number_allowed_surfaces << "  Curves: ";



        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_below_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
            std::cout << id << ", ";
        }

        std::cout << "\n";

        size_t number_selected_surfaces = selected_above_surfaces.size();


        std::cout << "\t Still on * " << number_selected_surfaces << " selected curves from sketch above: ";


        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_above_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setSelection( true );
            std::cout << id << ", ";
        }

        std::cout << "\n";

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
    for ( size_t i = 0; i < number_allowed; ++i )
    {
        size_t id = curves_id[ i ];

        StratigraphicItem* strat = stratigraphics_list[ id ];
        if( strat->getSelection() ) continue;

        strat->setUnderOperation( false );

    }

}


/// Allow to select sketches
void Scene::setSelectionMode( const bool status )
{

    std::cout << "-- Set selection mode to all ... " << status <<  std::endl;


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



Eigen::Vector3f Scene::scene2DtoPlanin( const Point2D &p )
{

    Eigen::Vector4f p_cpy( p.x(), p.y(), 0.0f, 1.0f );

    p_cpy = m_2dtoplanin.matrix()*p_cpy;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );
}


Eigen::Vector3f Scene::scene2DtoPlanin( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );

    p_cpy = m_2dtoplanin.matrix()*p_cpy;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );
}



Curve2D Scene::scene2DtoPlanin( const Curve2D& c )
{

    Curve2D c3d;
    size_t number_of_points = c.size();

    for( size_t i = 0; i < number_of_points; ++i )
    {
        Eigen::Vector3f p = scene2DtoPlanin( c.at( i ) );
        c3d.add( Point2D( p.x(), p.y() ) );
    }

    return c3d;

}



Point2D Scene::scenePlaninto2D( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );

    p_cpy = m_planinto2d.matrix()*p_cpy;
    return Point2D( p_cpy.x(), p_cpy.y() );

}



Curve2D Scene::scenePlaninto2D( const Curve2D &c )
{

    Curve2D c2d;
    size_t number_of_points = c.size();

    for( size_t i = 0; i < number_of_points; ++i )
    {
        Point2D p = c.at( i );
        c2d.add( scenePlaninto2D( Eigen::Vector3f( p.x(), p.y(), 0.0f ) ) );
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

        addCurve();

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

        int w = event->scenePos().x() - boundary_anchor.x();
        int h = -event->scenePos().y() + boundary_anchor.y();


        editBoundary( boundary_anchor.x(), /*height() - */boundary_anchor.y(), w - boundary_anchor.x(), h - boundary_anchor.y() );

        current_mode = InteractionMode::OVERSKETCHING;

    }


    else if( current_mode == InteractionMode::SELECTING_ABOVE )
    {

        selected_above_surfaces.clear();
        selected_above_surfaces = getAllSelectedItems();
        if( selected_above_surfaces.empty() == true ) return;

        controller->defineRegionAbove( selected_above_surfaces );
        disallowCurves( allowed_above_surfaces );


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

