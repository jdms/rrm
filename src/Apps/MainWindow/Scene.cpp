#include "Scene.h"

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
}




void Scene::init()
{


    if( views().empty() == true ) return;
    QGraphicsView* view = views()[0];

    defineVolumeQtCoordinates( 0, 0, 0, (int)view->width()*0.8f, (int)view->height()*0.8f, 400 );


    createVolume3D();
    createSketchingBoundary();
    newSketch();


//    testingMatrices();


    update();



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

    Eigen::Vector4f origin = Eigen::Vector4f( qtscene_origin_x, qtscene_origin_y, qtscene_origin_z, 1.0f );
    origin = m_2dto3d.matrix()*origin;


    float scale = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth );

    m_planinto2d = Eigen::Affine3f::Identity();
    m_planinto2d.scale( Eigen::Vector3f( scale, scale, scale ) );

    m_2dtoplanin = Eigen::Affine3f::Identity();
    m_2dtoplanin.scale( Eigen::Vector3f( 1.0f/scale, 1.0f/scale, 1.0f/scale ) );


}


void Scene::testingMatrices()
{


    ///
    /// \brief update
    /// creating the 3d volume
    ///
    ///

    surfaces_list.clear();


    Eigen::Vector3f A( qtscene_origin_x,                 qtscene_origin_y,                  qtscene_origin_z + qtscene_depth );
    Eigen::Vector3f B( qtscene_origin_x + qtscene_width, qtscene_origin_y,                  qtscene_origin_z + qtscene_depth );
    Eigen::Vector3f C( qtscene_origin_x + qtscene_width, qtscene_origin_y + qtscene_height, qtscene_origin_z + qtscene_depth );
    Eigen::Vector3f D( qtscene_origin_x,                 qtscene_origin_y + qtscene_height, qtscene_origin_z + qtscene_depth );
    Eigen::Vector3f E( qtscene_origin_x,                 qtscene_origin_y,                  qtscene_origin_z );
    Eigen::Vector3f F( qtscene_origin_x + qtscene_width, qtscene_origin_y,                  qtscene_origin_z );
    Eigen::Vector3f G( qtscene_origin_x + qtscene_width, qtscene_origin_y + qtscene_height, qtscene_origin_z );
    Eigen::Vector3f H( qtscene_origin_x,                 qtscene_origin_y + qtscene_height, qtscene_origin_z );



    A = scene2Dto3D( A );
    B = scene2Dto3D( B );
    C = scene2Dto3D( C );
    D = scene2Dto3D( D );
    E = scene2Dto3D( E );
    F = scene2Dto3D( F );
    G = scene2Dto3D( G );
    H = scene2Dto3D( H );


    std::vector < float > vA;
    std::vector < float > vB;
    std::vector < float > vC;
    std::vector < float > vD;
    std::vector < float > vE;
    std::vector < float > vF;
    std::vector < float > vG;
    std::vector < float > vH;

    vA.push_back( A.x() );
    vA.push_back( A.y() );
    vA.push_back( A.z() );

    vB.push_back( B.x() );
    vB.push_back( B.y() );
    vB.push_back( B.z() );

    vC.push_back( C.x() );
    vC.push_back( C.y() );
    vC.push_back( C.z() );

    vD.push_back( D.x() );
    vD.push_back( D.y() );
    vD.push_back( D.z() );

    vE.push_back( E.x() );
    vE.push_back( E.y() );
    vE.push_back( E.z() );

    vF.push_back( F.x() );
    vF.push_back( F.y() );
    vF.push_back( F.z() );

    vG.push_back( G.x() );
    vG.push_back( G.y() );
    vG.push_back( G.z() );

    vH.push_back( H.x() );
    vH.push_back( H.y() );
    vH.push_back( H.z() );



    Surface* sA = new Surface( 0 );
    sA->init();
    sA->loadBuffers( vA );

    Surface* sB = new Surface( 1 );
    sB->init();
    sB->loadBuffers( vB );

    Surface* sC = new Surface( 2 );
    sC->init();
    sC->loadBuffers( vC );

    Surface* sD = new Surface( 3 );
    sD->init();
    sD->loadBuffers( vD );

    Surface* sE = new Surface( 4 );
    sE->init();
    sE->loadBuffers( vE );


    Surface* sF = new Surface( 5 );
    sF->init();
    sF->loadBuffers( vF );

    Surface* sG = new Surface( 6 );
    sG->init();
    sG->loadBuffers( vG );

    Surface* sH = new Surface( 7 );
    sH->init();
    sH->loadBuffers( vH );



    surfaces_list.push_back( sA );
    surfaces_list.push_back( sB );
    surfaces_list.push_back( sC );
    surfaces_list.push_back( sD );
    surfaces_list.push_back( sE );
    surfaces_list.push_back( sF );
    surfaces_list.push_back( sG );
    surfaces_list.push_back( sH );


    ///
    /// \brief update
    /// creating a cross section
    ///


    Eigen::Vector3f P1( qtscene_origin_x,                   qtscene_origin_y,                     qtscene_origin_z + 0.5f*qtscene_depth );
    Eigen::Vector3f P2( qtscene_origin_x + qtscene_width,   qtscene_origin_y,                     qtscene_origin_z + 0.5f*qtscene_depth );
    Eigen::Vector3f P3( qtscene_origin_x + qtscene_width,   qtscene_origin_y + qtscene_height,    qtscene_origin_z + 0.5f*qtscene_depth );
    Eigen::Vector3f P4( qtscene_origin_x,                   qtscene_origin_y + qtscene_height,    qtscene_origin_z + 0.5f*qtscene_depth );


    P1 = scene2Dto3D( P1 );
    P2 = scene2Dto3D( P2 );
    P3 = scene2Dto3D( P3 );
    P4 = scene2Dto3D( P4 );



    std::vector < float > vCrossSection;

    vCrossSection.push_back( P1.x() );
    vCrossSection.push_back( P1.y() );
    vCrossSection.push_back( P1.z() );
    vCrossSection.push_back( P2.x() );
    vCrossSection.push_back( P2.y() );
    vCrossSection.push_back( P2.z() );
    vCrossSection.push_back( P3.x() );
    vCrossSection.push_back( P3.y() );
    vCrossSection.push_back( P3.z() );
    vCrossSection.push_back( P4.x() );
    vCrossSection.push_back( P4.y() );
    vCrossSection.push_back( P4.z() );


    Surface* sCrossSection = new Surface( 8 );
    sCrossSection->init();
    sCrossSection->loadBuffers( vCrossSection );



    surfaces_list.push_back( sCrossSection );

    ///
    /// \brief update
    /// mapping this cross section in qt
    ///


    Point2D PI1 = scene3Dto2D( P1 );
    Point2D PI2 = scene3Dto2D( P2 );
    Point2D PI3 = scene3Dto2D( P3 );
    Point2D PI4 = scene3Dto2D( P4 );



    addEllipse( PI1.x(),  PI1.y(), 10, 10 );
    addEllipse( PI2.x(),  PI2.y(), 10, 10 );
    addEllipse( PI3.x(),  PI3.y(), 10, 10 );
    addEllipse( PI4.x(),  PI4.y(), 10, 10 );




}






void Scene::createVolume3D()
{


    Eigen::Vector3f min( qtscene_origin_x,                  qtscene_origin_y,                   qtscene_origin_z );
    Eigen::Vector3f max( qtscene_origin_x + qtscene_width,  qtscene_origin_y + qtscene_height,  qtscene_origin_z + qtscene_depth );

    Eigen::Vector3f min1 = Scene::scene2Dto3D( min );
    Eigen::Vector3f max1 = Scene::scene2Dto3D( max );

    Eigen::Vector3f dim1 = max1 - min1;


    boundary3D = new BoundingBox3D( min1.x(), min1.y(), min1.z(), dim1.x(), dim1.y(), dim1.z() );
    boundary3D->setCurrentDirectory( shader_directory.toStdString() );
    boundary3D->init();
    boundary3D->create();



    float L = std::max( std::max( qtscene_width, qtscene_height ), qtscene_depth );



    controller->initRulesProcessor( 0, 0, 0, qtscene_width/L, qtscene_height/L, qtscene_depth/L );


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


    defineVolumeQtCoordinates( x, y, qtscene_origin_z, w, h, qtscene_depth );

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


//    testingMatrices();

//    controller->editRulesProcessor( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );



}





void Scene::addCurve()
{


    current_mode = InteractionMode::INSERTING;


    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( sketch->getCurve() );
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

    stratigraphics_list.push_back( sketch );
    surfaces_list.push_back( strat3D );

    emit initContext();

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






void Scene::updateScene()
{

//    boundary->update();
//    boundary3D->update();

    float d = controller->getCurrentCrossSection();

    unsigned int number_of_stratigraphies = stratigraphics_list.size();
    for( int i = 0; i < number_of_stratigraphies; ++i )
    {
        StratigraphicItem* strat = stratigraphics_list[ i ];
        strat->update( m_planinto2d, d );
    }


    emit updateContext();

    emit updatedScene();

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






void Scene::updateGLContext()
{

    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    Eigen::Vector3f center( 0.5f*qtscene_width/L, 0.5f*qtscene_height/L, 0.5f*qtscene_depth/L ) ;

    unsigned int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* surface = surfaces_list[ i ];
        surface->update( center );
    }



}


void Scene::initGLContext()
{


    int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* s = surfaces_list[ i ];
        if( s->initialized() == false )
            s->init();
    }


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
    unsigned int number_of_points = c.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        Eigen::Vector3f p = scene2Dto3D( c.at( i ) );
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



//===


Eigen::Vector3f Scene::scene2DtoPlanin( const Point2D &p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), 0.0f, 1.0f );

    //p_cpy = m_2dtoplanin.matrix()*p_cpy;
    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    p_cpy = p_cpy/L;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );

}


Eigen::Vector3f Scene::scene2DtoPlanin( const Eigen::Vector3f& p )
{
    Eigen::Vector4f p_cpy( p.x(), p.y(), p.z(), 1.0f );



    //p_cpy = m_2dtoplanin.matrix()*p_cpy;
    float L = std::max( std::max(qtscene_width, qtscene_height), qtscene_depth);
    p_cpy = p_cpy/L;
    return Eigen::Vector3f( p_cpy.x(), p_cpy.y(), p_cpy.z() );

}



Curve2D Scene::scene2DtoPlanin( const Curve2D& c )
{

    Curve2D c3d;
    unsigned int number_of_points = c.size();

    for( int i = 0; i < number_of_points; ++i )
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
    unsigned int number_of_points = c.size();

    for( int i = 0; i < number_of_points; ++i )
    {
        Point2D p = c.at( i );
        c2d.add( scenePlaninto2D( Eigen::Vector3f( p.x(), p.y(), 0.0f ) ) );
    }

    return c2d;

}



//===











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

