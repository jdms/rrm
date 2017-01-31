/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Scene.h"
#include "Exporter/CPS3Exporter.hpp"
#include "Exporter/IrapGridExporter.hpp"


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
    defineVolumeQtCoordinates( 0, 0, 0, (int)view->width()*0.8f, (int)view->height()*0.8f, depth );


    background_image = new QGraphicsPixmapItem();
    addItem( background_image );


    defining_above = false;
    defining_below = false;


    createVolume3D();
    createSketchingBoundary();
    newSketch();

	/// Calls createRegions
	emit requestNumberOfRegion();
    initRegions();
	

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

    number_of_flow_regions_ = 0;
    is_region_visible = false;

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

    emit updateBoundGeometry( width, height , depth );

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


	/// @FIXME Everytime the setSceneRect is invoked, the regions neeed to be restarted. 
	/// Clear region. It deletes the scene regionItems
    clearRegions();
	/// request the number of regions from flow interface
	emit requestNumberOfRegion();
	/// create/set the regions based on the new boundary and current number of regions
    initRegions();


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

	clearRegions();

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
        strat->update( mA, d );

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


void Scene::getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
{

    controller->getLegacyMeshes( points, nu, nv, num_extrusion_steps );


    std::size_t number_of_points = ( std::size_t ) points.size()/3;


    double xmax = 0.0, xmin = 0.0;
    double ymax = 0.0, ymin = 0.0;
    double zmax = 0.0, zmin = 0.0;

    Eigen::Vector3f v; Eigen::Vector3f sv;
    for ( auto i = 0; i < number_of_points; ++i )
    {
        v[ 0 ] = static_cast<float>( points[ 3*i + 0 ] );
        v[ 1 ] = static_cast<float>( points[ 3*i + 2 ] );
        v[ 2 ] = static_cast<float>( points[ 3*i + 1 ] );

        sv = scene3DtoPlane( v );

        double x = points[ 3*i + 0 ] = static_cast<double>( sv[ 0 ] );
        double y = points[ 3*i + 1 ] = static_cast<double>( sv[ 2 ] );
        double z = points[ 3*i + 2 ] = static_cast<double>( sv[ 1 ] );


        if( x > xmax ) xmax = x;
        if( x < xmin ) xmin = x;

        if( y > ymax ) ymax = y;
        if( y < ymin ) ymin = y;


        if( z > zmax ) zmax = z;
        if( z < zmin ) zmin = z;


    }

    emit volumeDimensions( xmax, ymax, zmax );


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

void Scene::resetBuffers()
{

    if( boundary3D->initialized() == true )
    {
        boundary3D->resetBuffers();
    }


    std::map< size_t, Surface* >::iterator it;

    for( it = surfaces_list.begin(); it != surfaces_list.end(); ++it )
    {
        Surface* s = it->second;
        if( s->initialized() == true )
        {
            s->resetBuffers();
        }
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

    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_above_surfaces[ i ];
        std::cout << id << ", " << std::flush;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );
    }


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


        size_t number_allowed_surfaces = allowed_above_surfaces.size();


        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_above_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
        }



        size_t number_selected_surfaces = selected_below_surfaces.size();

        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_below_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setSelection( true );

        }

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


    for ( size_t i = 0; i < number_allowed_surfaces; ++i )
    {
        size_t id = allowed_below_surfaces[ i ];
        std::cout << id << ", " << std::flush;

        StratigraphicItem* strat = stratigraphics_list[ id ];
        strat->setAllowed( true );
    }


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

        size_t number_allowed_surfaces = allowed_below_surfaces.size();

        for ( size_t i = 0; i < number_allowed_surfaces; ++i )
        {
            size_t id = allowed_below_surfaces[ i ];
            std::cout << id << ", " << std::flush;

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( false );
        }

        size_t number_selected_surfaces = selected_above_surfaces.size();

        for ( size_t i = 0; i < number_selected_surfaces; ++i )
        {
            size_t id = selected_above_surfaces[ i ];

            StratigraphicItem* strat = stratigraphics_list[ id ];
            strat->setUnderOperation( true );
            strat->setAllowed( true );
            strat->setSelection( true );
        }


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

        strat->setAllowed( false );
        strat->setSelection( false );
        strat->setUnderOperation( false );

    }

}


/// Allow to select sketches
void Scene::setSelectionMode( const bool status )
{

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


Eigen::Vector3f Scene::scene3DtoPlane( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );

    p_cpy = m_3dto2d.matrix()*p_cpy;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );

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


void Scene::exportToCPS3( const std::string& filename )
{


    std::vector<double> points_list;
    std::vector<size_t> nu_list;
    std::vector<size_t> nv_list; // surfaces number: or height
    size_t num_extrusion_steps = 5;

    getLegacyMeshes( points_list, nu_list, nv_list, num_extrusion_steps );


    CPS3Exporter exporter;

    size_t number_surfaces = nu_list.size();
    std::vector<float> points;

    size_t id = 0;

    QString surface_filename;

    for( auto k = 0; k < number_surfaces; ++k )
    {
        size_t nu = nu_list[ k ];
        size_t nv = nv_list[ k ];
        size_t number_elements = nu*nv;

        double xmax = points_list[ 3*id + 0 ], xmin = points_list[ 3*id + 0 ];
        double ymax = points_list[ 3*id + 1 ], ymin = points_list[ 3*id + 1 ];
        double zmax = points_list[ 3*id + 2 ], zmin = points_list[ 3*id + 2 ];


        for( auto i = 0; i < number_elements; ++i )
        {

            double x = points_list[ 3*( id + i ) + 0 ];
            double y = points_list[ 3*( id + i ) + 1 ];
            double z = points_list[ 3*( id + i ) + 2 ];

            if( x > xmax ) xmax = x;
            if( x < xmin ) xmin = x;

            if( y > ymax ) ymax = y;
            if( y < ymin ) ymin = y;


            if( z > zmax ) zmax = z;
            if( z < zmin ) zmin = z;

            points.push_back( (float) z );

        }

        id += number_elements;


        float dx = (float)( xmax - xmin )/nu;
        float dy = (float)( ymax - ymin )/nv;

        exporter.setBoundingBox( (float )xmin, (float)xmax, (float)ymin, (float) ymax, (float) zmin, (float) zmax );
        exporter.setVectorValues( points );
        exporter.setSize( (int) nu,  (int) nv );
        exporter.setSpacing( dx, dy );

        surface_filename = QString( filename.c_str() );

        if( number_surfaces > 1 )
            surface_filename.replace( QString(".CPS3"), QString("%1.CPS3").arg( k ) );

        exporter.writeGridData( surface_filename.toStdString() );
        exporter.clearData();

        points.clear();
        surface_filename.clear();


    }

}


void Scene::exportToIrapGrid( const std::string& filename )
{


    std::vector<double> points_list;
    std::vector<size_t> nu_list;
    std::vector<size_t> nv_list; // surfaces number: or height
    size_t num_extrusion_steps = 5;

    getLegacyMeshes( points_list, nu_list, nv_list, num_extrusion_steps );


    IrapGridExporter exporter;

    size_t number_surfaces = nu_list.size();
    std::vector<float> points;

    size_t id = 0;

    QString surface_filename;

    for( auto k = 0; k < number_surfaces; ++k )
    {
        size_t nu = nu_list[ k ];
        size_t nv = nv_list[ k ];
        size_t number_elements = nu*nv;

        double xmax = points_list[ 3*id + 0 ], xmin = points_list[ 3*id + 0 ];
        double ymax = points_list[ 3*id + 1 ], ymin = points_list[ 3*id + 1 ];
        double zmax = points_list[ 3*id + 2 ], zmin = points_list[ 3*id + 2 ];


        for( auto i = 0; i < number_elements; ++i )
        {

            double x = points_list[ 3*( id + i ) + 0 ];
            double y = points_list[ 3*( id + i ) + 1 ];
            double z = points_list[ 3*( id + i ) + 2 ];

            if( x > xmax ) xmax = x;
            if( x < xmin ) xmin = x;

            if( y > ymax ) ymax = y;
            if( y < ymin ) ymin = y;


            if( z > zmax ) zmax = z;
            if( z < zmin ) zmin = z;

            points.push_back( (float) z );

        }

        id += number_elements;


        float dx = (float)( xmax - xmin )/nu;
        float dy = (float)( ymax - ymin )/nv;

        exporter.setBoundingBox( (float )xmin, (float)xmax, (float)ymin, (float) ymax, (float) zmin, (float) zmax );
        exporter.setVectorValues( points );
        exporter.setSize( (int) nu,  (int) nv );
        exporter.setSpacing( dx, dy );

        surface_filename = QString( filename.c_str() );

        if( number_surfaces > 1 )
            surface_filename.replace( QString(".IRAPG"), QString("%1.IRAPG").arg( k ) );

        exporter.writeGridData( surface_filename.toStdString() );
        exporter.clearData();

        points.clear();
        surface_filename.clear();


    }

}


void Scene::finishCurve()
{

    if (temp_sketch->isEmpty()) return;


    bool add_ok = addCurve();
    if (add_ok == false)
        return;

    // should be fixed
    std::vector< size_t > upper_bound = arrangement.getLastCurveLowerBound();
    std::vector< size_t > lower_bound = arrangement.getLastCurveUpperBound();


    controller->interpolateStratigraphy(lower_bound, upper_bound);

    newSketch();
    controller->addStratigraphy();



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
		else if (current_mode == InteractionMode::SELECTING_REGION)
		{
			event->ignore();
		} 

    }
    else if ( event->buttons() & Qt::RightButton )
    {

        if (current_mode == InteractionMode::OVERSKETCHING )
		{

            finishCurve();

		}
		else if (current_mode == InteractionMode::SELECTING_REGION)
		{
			event->ignore();
		}

    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

	Eigen::Vector3f p = scene2Dto3D(Point2D(event->scenePos().x(), event->scenePos().y()));
	Point2D pb = scene3Dto2D(p);
	//emit sendCoordinates(pb.x(), pb.y());

    emit sendCoordinates( event->scenePos().x(), event->scenePos().y() );



    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_mode == InteractionMode::OVERSKETCHING )
        {
            temp_sketch->add( event->scenePos() );
        }

        else if( current_mode == InteractionMode::BOUNDARY )
        {


            int w = event->scenePos().x() - boundary_anchor.x() ;
            int h = event->scenePos().y() - boundary_anchor.y() ;

            sketching_boundary->edit( boundary_anchor.x(), boundary_anchor.y(), w,  h );
            emit updateBoundGeometry( std::abs( w ), std::abs( h ) , qtscene_depth );

		}
		else if (current_mode == InteractionMode::SELECTING_REGION)
		{
			event->ignore();
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


        editBoundary( 0, 0, w, h );


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
//        emit enableSketching( true );


    }


    else if( current_mode == InteractionMode::SELECTING_BELOW )
    {

        selected_below_surfaces.clear();
        selected_below_surfaces = getAllSelectedItems();
        if( selected_below_surfaces.empty() == true ) return;

        controller->defineRegionBelow( selected_below_surfaces );
        disallowCurves( allowed_below_surfaces );





        current_mode = InteractionMode::OVERSKETCHING;
//        emit enableSketching( true );

    }


    else if( current_mode == InteractionMode::SELECTING_REGION )
    {
		std::map<int, Eigen::Vector3f> region_points;

		for (auto index : flow_regions_)
		{
			region_points[index.first] = Eigen::Vector3f(index.second->pos().x(), index.second->pos().y(),350.0);
		}

		emit sendRegionPoints(region_points);
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


/// Flow regions releated functions
void Scene::createRegions(int number_of_regions)
{	

	regionVisibility(false);

	this->number_of_flow_regions_ = number_of_regions;

	for (auto index = 0; index < number_of_regions; index++)
	{
		if (!flow_regions_.count(index))
		{
			flow_regions_[index] = new RRM::RegionItem(index + 1);
			this->addItem(flow_regions_[index]);
			flow_regions_[index]->setPos((flow_regions_[index]->boundingRect().width() * index), sketching_boundary->boundingRect().height());
		}
		else
		{
			flow_regions_[index]->setVisible(true);
		}
	}

	regionVisibility(is_region_visible);
}

/// Flow regions releated functions
void Scene::regionVisibility(bool _is_visible)
{
	for (auto index = 0; index < number_of_flow_regions_; index++)
	{
		if (flow_regions_.count(index))
		{
			flow_regions_[index]->setVisible(_is_visible);
		}		
	}
}

/// Flow regions releated functions
void Scene::initRegions( )
{
	if (this->number_of_flow_regions_ != 0)
	{
		for (auto index = 0; index < this->number_of_flow_regions_; index++)
		{
			if (this->flow_regions_.count(index))
			{
				flow_regions_[index]->setPos((flow_regions_[index]->boundingRect().width() * index), sketching_boundary->boundingRect().height());
			}
		}
	}		
}

void Scene::clearRegions()
{
	for (auto region : flow_regions_)
	{
		delete region.second;
	}

	flow_regions_.clear();
}
