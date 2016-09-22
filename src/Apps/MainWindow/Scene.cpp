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


    testingMatrices();


    update();



    /*




    updateSpace3D( 0.8f*view->width(), 0.8f*view->height(), int_depth );

    createVolume3D();
    createBoundary();

    newSketch();

*/

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
    m_2dto3d = Model3DUtils::normalizePointCloud( qtscene_origin_x, qtscene_width, qtscene_origin_y, qtscene_height, qtscene_origin_z, qtscene_depth );
    m_3dto2d = m_2dto3d.inverse();
}


void Scene::testingMatrices()
{


    ///
    /// \brief update
    /// creating the 3d volume
    ///


    Eigen::Vector3f A( 0.0f,          0.0f,           qtscene_depth );
    Eigen::Vector3f B( qtscene_width, 0.0f,           qtscene_depth );
    Eigen::Vector3f C( qtscene_width, qtscene_height, qtscene_depth );
    Eigen::Vector3f D( 0.0f,          qtscene_height, qtscene_depth );
    Eigen::Vector3f E( 0.0f,          0.0f,           0.0f );
    Eigen::Vector3f F( qtscene_width, 0.0f,           0.0f );
    Eigen::Vector3f G( qtscene_width, qtscene_height, 0.0f );
    Eigen::Vector3f H( 0.0f         , qtscene_height, 0.0f );



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


    Eigen::Vector3f P1( 0.0f,          0.0f,           0.5f*qtscene_depth );
    Eigen::Vector3f P2( qtscene_width, 0.0f,           0.5f*qtscene_depth );
    Eigen::Vector3f P3( qtscene_width, qtscene_height, 0.5f*qtscene_depth );
    Eigen::Vector3f P4( 0.0f,          qtscene_height, 0.5f*qtscene_depth );


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

    if( boundary3D != 0 || views().empty() == true ) return;

    Eigen::Vector3f min( qtscene_origin_x, qtscene_origin_y, qtscene_origin_z );
    Eigen::Vector3f max( qtscene_width,    qtscene_height,   qtscene_depth );

    min = Scene::scene2Dto3D( min );
    max = Scene::scene2Dto3D( max );

    Eigen::Vector3f dim = max - min;


    boundary3D = new BoundingBox3D( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );
    boundary3D->setCurrentDirectory( shader_directory.toStdString() );
    boundary3D->init();
    boundary3D->create();


    /*

    controller->initRulesProcessor( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );
*/

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



//    if( controller->hasCrossSection() == false )
//        createCrossSection( 0.0f );



//    bool ok = controller->addBoundary( boundary3D->getWidth(), boundary3D->getHeight(), boundary3D->getDepth() );
//    if( ok == false  ) return;


    addBoundaryToScene();


}


void Scene::addBoundaryToScene()
{

    if( sketching_boundary != 0 )
        delete sketching_boundary;


    sketching_boundary = new BoundaryItem2D();
    sketching_boundary->setOriginX( qtscene_origin_x );
    sketching_boundary->setOriginY( qtscene_origin_y );

    sketching_boundary->setWidth( qtscene_width );
    sketching_boundary->setHeight( qtscene_height );

    sketching_boundary->load();

    addItem( sketching_boundary );
    setSceneRect( sketching_boundary->boundingRect() );





    /*
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
*/

}


void Scene::editBoundary( const int &x, const int &y, const int &w, const int &h )
{


    /*

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


//    controller->updateRulesProcessor( min.x(), min.y(), min.z(), dim.x(), dim.y(), dim.z() );

    */

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

//    unsigned int number_of_stratigraphies = stratigraphics_list.size();
//    for( int i = 0; i < number_of_stratigraphies; ++i )
//    {
//        StratigraphicItem* strat = stratigraphics_list[ i ];
//        strat->update( m_3dto2d, d );
//    }


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

    unsigned int number_of_surfaces = surfaces_list.size();
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        Surface* surface = surfaces_list[ i ];
        surface->update();
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

