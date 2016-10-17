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
    temp_sketch = 0;

    current_color = QColor( 255, 75, 75 );
    random_color = true;
}




void Scene::init()
{


    if( views().empty() == true ) return;
    QGraphicsView* view = views()[0];

    defineVolumeQtCoordinates( 0, 0, 0, (int)view->width()*0.8f, (int)view->height()*0.8f, 400 );



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

    /*

    m_2dto3d = Model3DUtils::normalizePointCloud( qtscene_origin_x, qtscene_origin_x + qtscene_width,
                                                  qtscene_origin_y, qtscene_origin_y + qtscene_height,
                                                  qtscene_origin_z, qtscene_origin_z + qtscene_depth );

    m_3dto2d = m_2dto3d.inverse();

    float scale = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth );


    m_planinto2d = Eigen::Affine3f::Identity();
    m_planinto2d.scale( Eigen::Vector3f( scale, scale, scale ) );

    m_2dtoplanin = Eigen::Affine3f::Identity();
    m_2dtoplanin.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );
*/


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

    //    Eigen::Vector4f orig_pl = m_2dtoplanin.matrix()*orig_qt;


    //    m_planinto2d = Eigen::Affine3f::Identity();
    //    m_planinto2d.translation() = L*Eigen::Vector3f( orig_pl.x(), orig_pl.y(), orig_pl.z() ) ;
    //    m_planinto2d.scale( Eigen::Vector3f( L, L, L ) );






    /*
       m_planinto2d = Eigen::Affine3f::Identity();
       m_planinto2d.translate( Eigen::Vector3f( qtscene_origin_x, qtscene_origin_y, qtscene_origin_z ) );
       m_planinto2d.scale( Eigen::Vector3f( scale, scale, scale ) );


       m_2dtoplanin = Eigen::Affine3f::Identity();
       m_2dtoplanin.translate( Eigen::Vector3f( -qtscene_origin_x, -qtscene_origin_y, -qtscene_origin_z ) );
       m_2dtoplanin.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );

   */

}



void Scene::createVolume3D()
{


    //    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    //    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );

    //    min = Scene::scene2Dto3D( min );
    //    max = Scene::scene2Dto3D( max );

    //    Eigen::Vector3f dim = max - min;


    //    boundary3D = new BoundingBox3D( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );
    //    boundary3D->setCurrentDirectory( shader_directory.toStdString() );
    //    boundary3D->init();
    //    boundary3D->create();



    //    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );

    //    controller->initRulesProcessor( 0, 0, 0, qtscene_width/L, qtscene_height/L, qtscene_depth/L );



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
    boundary3D->init();
    boundary3D->create();



    controller->initRulesProcessor( min_pl.x(), min_pl.y(), min_pl.z(), dim_pl.x(), dim_pl.y(), dim_pl.z() );

    //    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );
    //    controller->initRulesProcessor( 0, 0, 0, qtscene_width/L, qtscene_height/L, qtscene_depth/L );


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

    sketching_boundary = new BoundaryItem2D();
    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );



    addItem( sketching_boundary );
    setSceneRect( sketching_boundary->boundingRect() );


}


void Scene::editBoundary( const int &x, const int &y, const int &w, const int &h )
{

    /*
    int origin_x = qtscene_origin_x - x;
    int origin_y = qtscene_origin_y - y;


    defineVolumeQtCoordinates( origin_x, origin_y, qtscene_origin_z, w, h, qtscene_depth );


    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );

    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );

    Eigen::Vector3f dim = max - min;


    controller->editBoundary( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );


    Boundary* b = controller->getCurrentBoundary();
    boundary3D->setGeoData( b );
    boundary3D->update();


    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );
    setSceneRect( sketching_boundary->boundingRect() );


    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );

    controller->editRulesProcessor( 0, 0, 0, qtscene_width/L, qtscene_height/L, qtscene_depth/L );


*/


    defineVolumeQtCoordinates( x, /*-1**/y, qtscene_origin_z, abs( w ), abs( h ), qtscene_depth );


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
    boundary3D->update();


    sketching_boundary->setGeoData( b );
    sketching_boundary->update( m_3dto2d );
    setSceneRect( sketching_boundary->boundingRect() );


    controller->editRulesProcessor( min_pl.x(), min_pl.y(), min_pl.z(), dim_pl.x(), dim_pl.y(), dim_pl.z() );
    //min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z()
    //    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );
    //    controller->editRulesProcessor( min.x(), min.y(), min.z(), qtscene_width/L, qtscene_height/L, qtscene_depth/L );




}





void Scene::addCurve()
{


    current_mode = InteractionMode::INSERTING;


    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( sketch->getCurve() );


    unsigned int number_of_points = c.size();
    c = Scene::scene2DtoPlanin( c );

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
    strat3D->setCurrentDirectory( shader_directory.toStdString() );
    strat3D->setCurrentColor( current_color.redF(), current_color.greenF(), current_color.blueF() );


    unsigned int id = strat->getId();

    stratigraphics_list[ id ] = sketch;
    surfaces_list[ id ] = strat3D;


    emit initContext();

}



void Scene::removeStratigraphyFromScene( unsigned int id )
{

    stratigraphics_list.erase( id );
    surfaces_list.erase( id );
    //    emit updatedScene();

}




void Scene::newSketch()
{


    if( random_color == true )
    {

        std::random_device rd;
        std::mt19937 eng( rd() );
        std::uniform_int_distribution< unsigned int > distr( 0, 255 );

        int r = distr( eng );
        int g = distr( eng );
        int b = distr( eng );

        current_color = QColor( r, g, b );

    }


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

    if( controller != 0 )
        controller->clear();

    if( temp_sketch = 0 )
    {
        temp_sketch->clear();
        delete temp_sketch;
    }

    if( sketch != 0 )
    {
        sketch->clear();
        delete sketch;
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


    clear();

    boundary3D = 0;
    sketching_boundary = 0;
    sketch = 0;
    temp_sketch = 0;


    init();

    emit updatedScene();

}


void Scene::updateScene()
{

    //    boundary->update();
    boundary3D->update();

    float d = controller->getCurrentCrossSection();

    std::map< unsigned int, StratigraphicItem* >::iterator it;
    for( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->update( m_planinto2d, d );
    }


    emit updateContext();
    emit updatedScene();

    update();

}


void Scene::drawScene3D( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& width, const int& height )
{


    int number_of_surfaces = surfaces_list.size();
    std::map< unsigned int, Surface* >::iterator it;

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

    /*
    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    Eigen::Vector3f center( 0.5f*qtscene_width/L, 0.5f*qtscene_height/L, 0.5f*qtscene_depth/L ) ;

    unsigned int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* surface = surfaces_list[ i ];
        surface->update( center );
    }

*/


    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    Eigen::Vector3f center( 0.5f*qtscene_width, 0.5f*qtscene_height, 0.5f*qtscene_depth ) ;

    center = Scene::scene2DtoPlanin( center );

    std::map< unsigned int, Surface* >::iterator it;
    for( it = surfaces_list.begin(); it != surfaces_list.end(); ++it )
    {
        Surface* s = it->second;
        s->update( center );
    }


}


void Scene::initGLContext()
{


    int number_of_surfaces = surfaces_list.size();

    std::map< unsigned int, Surface* >::iterator it;

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

    if( sketch != NULL )
        sketch->setColor( current_color );

    random_color = false;

    update();

}




void Scene::enableSketchingAboveRegion( bool option )
{

    bool region_ok = true;

    if( option == true )
    {
        region_ok = defineSketchingAboveRegion();
    }

    if( option == false || region_ok == false )
    {
        stopSketchingAboveRegion();
    }

}


void Scene::enableSketchingBelowRegion( bool option )
{

    bool region_ok = true;

    if( option == true )
    {
        region_ok = defineSketchingBelowRegion();
    }

    if( option == false || region_ok == false )
    {
        stopSketchingBelowRegion();
    }

}




bool Scene::defineSketchingAboveRegion()
{


    allowed_above_surfaces.clear();
    bool sketchingabove_ok = controller->defineSketchingAbove( allowed_above_surfaces );
    if( ( sketchingabove_ok == false ) || ( allowed_above_surfaces.empty() == true  ) ) return false;


    startOperations();


    size_t number_allowed_surfaces = allowed_above_surfaces.size();
    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_above_surfaces[ i ];
        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setFlag( QGraphicsItem::ItemIsSelectable, true );
        strat->setAllowed( true );
    }


    current_mode = InteractionMode::SELECTION;
    defining_above = true;
    update();

    return true;
}


void Scene::stopSketchingAboveRegion()
{

    size_t number_allowed_surfaces = allowed_above_surfaces.size();
    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_above_surfaces[ i ];

        if ( id == id_below )
            continue;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setFlag( QGraphicsItem::ItemIsSelectable, false );
        strat->setAllowed( false );
        strat->setSelection( false );
    }


    if( defining_below == false )
        stopOperations();

    current_mode = InteractionMode::OVERSKETCHING;
    defining_above = false;
    id_above = 100000;


    update();

}


bool Scene::defineSketchingBelowRegion()
{

    allowed_below_surfaces.clear();
    bool sketchingbelow_ok = controller->defineSketchingBelow( allowed_below_surfaces );
    if( ( sketchingbelow_ok == false )  || ( allowed_below_surfaces.empty() == true  ) ) return false;


     startOperations();


    size_t number_allowed_surfaces = allowed_below_surfaces.size();
    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_below_surfaces[ i ];
        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setFlag( QGraphicsItem::ItemIsSelectable, true );
        strat->setAllowed( true );
    }


    current_mode = InteractionMode::SELECTION;
    defining_below = true;
    update();

    return true;

}


void Scene::stopSketchingBelowRegion()
{

    size_t number_allowed_surfaces = allowed_below_surfaces.size();
    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_below_surfaces[ i ];

        if ( id == id_above )
            continue;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setFlag( QGraphicsItem::ItemIsSelectable, false );
        strat->setAllowed( false );
        strat->setSelection( false );
    }


    if( defining_above == false )
    {
        stopOperations();
    }



    current_mode = InteractionMode::OVERSKETCHING;
    defining_below = false;
    id_below = 100000;
    update();

}





void Scene::setUnallowed()
{

    if( defining_above == true ){

        size_t number_allowed_surfaces = allowed_above_surfaces.size();
        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_above_surfaces[ i ];
            StratigraphicItem* strat = stratigraphics_list[ id ];

            if( strat->isSelected() ) continue;

            strat->setFlag( QGraphicsItem::ItemIsSelectable, false );
            strat->setAllowed( false );
        }

    }
    if( defining_below == true ){

        size_t number_allowed_surfaces = allowed_below_surfaces.size();
        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_below_surfaces[ i ];
            StratigraphicItem* strat = stratigraphics_list[ id ];

            if( strat->isSelected() ) continue;

            strat->setFlag( QGraphicsItem::ItemIsSelectable, false );
            strat->setAllowed( false );
        }

    }

}


void Scene::startOperations()
{

    std::map< unsigned int, StratigraphicItem* >::iterator it;
    for ( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->setUnderOperation( true );
    }

}


void Scene::stopOperations()
{

    std::map< unsigned int, StratigraphicItem* >::iterator it;
    for ( it = stratigraphics_list.begin(); it != stratigraphics_list.end(); ++it )
    {
        StratigraphicItem* strat = it->second;
        strat->setUnderOperation( false );
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

    /*
    Eigen::Vector4f p_cpy( p.x(), p.y(), 0.0f, 1.0f );

    //p_cpy = m_2dtoplanin.matrix()*p_cpy;
    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    p_cpy = p_cpy/L;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );
*/

    Eigen::Vector4f p_cpy( p.x(), p.y(), 0.0f, 1.0f );

    p_cpy = m_2dtoplanin.matrix()*p_cpy;
    //    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    //    p_cpy = p_cpy/L;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );
}


Eigen::Vector3f Scene::scene2DtoPlanin( const Eigen::Vector3f& p )
{

    /*
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );



    //p_cpy = m_2dtoplanin.matrix()*p_cpy;
    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    p_cpy = p_cpy/L;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );
*/


    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );



    p_cpy = m_2dtoplanin.matrix()*p_cpy;
    //    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    //    p_cpy = p_cpy/L;
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

            sketching_boundary->edit( boundary_anchor.x(), boundary_anchor.y(), w,  h );

        }

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();

}


void Scene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    /*
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
*/


    if ( current_mode == InteractionMode::OVERSKETCHING  )
    {

        if( temp_sketch == NULL ) return;

        if( temp_sketch->isValid() == false ) return;


        sketch->addSegment( *temp_sketch );
        removeItem( temp_sketch );

        delete temp_sketch;
        temp_sketch = NULL;

    }


    else if( current_mode == InteractionMode::BOUNDARY )
    {

        int w = event->scenePos().x() - boundary_anchor.x();
        int h = -event->scenePos().y() + boundary_anchor.y();


        editBoundary( boundary_anchor.x(), height() - boundary_anchor.y(), w, h );

        current_mode = InteractionMode::OVERSKETCHING;

    }


    else if( current_mode == InteractionMode::SELECTION )
    {
        std::cout << "Selection mode" << std::endl;

        std::vector< size_t > id_items = getAllSelectedItems();
        if( id_items.empty() == true ) return;

        controller->defineRegion( id_items );

        setUnallowed();

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

    sketching_boundary->setBackGroundImage( url_file );


}


void Scene::dragMoveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}


void Scene::dragLeaveEvent( QGraphicsSceneDragDropEvent * event )
{
    event->accept();
}

