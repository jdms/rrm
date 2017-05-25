#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QSvgGenerator>
#include <QFileDialog>


#include "PathScene.h"



PathScene::PathScene( QObject* parent )
{
    sketch = nullptr;
    csection_image = new QGraphicsPixmapItem();
    current_interaction = UserInteraction::SKETCHING;
    draw_cross_sections = true;
    object = nullptr;

    pen_color = QColor( 0, 0, 255 );
    pen_color.setCapStyle( Qt::RoundCap );
    pen_color.setJoinStyle( Qt::RoundJoin );
    pen_color.setWidth( 3 );

    path = new QGraphicsPathItem();
    path->setPen( pen_color );
    addItem( path );


}

void PathScene::setImagetoCrossSection( const QString &url_ )
{
    QPixmap image_ = QPixmap( url_ );
    csection_image->setPixmap( image_ );

    emit updateVolumeWidthDepth( image_.rect().width(), image_.rect().height() );

    update();
}


void PathScene::finishSketch()
{
    if ( sketch == nullptr ) return;
    if ( sketch->isEmpty() == true ) return;

    Curve2D curve_;
    if( acceptSketch( curve_ ) == true  )
    {
        Curve2D cpy_curve_;
        curve_.douglasPeuckerSimplify( cpy_curve_, 1.0 );
        emit curveAccepted( cpy_curve_ );
    }

    clearSketch();
    updateScene();
}


bool PathScene::acceptSketch( Curve2D &curve_ )
{
    if ( sketch == nullptr ) return false;

    curve_ = PolyQtUtils::qPolyginFToCurve2D( sketch->getSketch() );
    return true;
}


void PathScene::addCrossSection( double depth_ )
{

    auto search = csections.find( depth_ );
    if( search != csections.end() ) return;

    csections[ depth_ ] = new CrossSectionItemWrapper( volume.getWidth(), depth_ );
    csections[ depth_ ]->setCurrent( false );
    csections[ depth_ ]->setVisible( draw_cross_sections );
    addItem( csections[ depth_ ] );
    update();
}


void PathScene::setCrossSection( double depth_ )
{
    current_csection->setDepth( depth_ );
    current_csection->setCurrent( true );
    update();
}


void PathScene::resetData()
{


    for( auto &it: items() )
    {
        removeItem( it );
    }

    for( auto &it: csections )
    {
        if( (it.second) != nullptr )
        {
//            it.second->clear();
            delete it.second;
        }
    }
    csections.clear();

    if( sketch != nullptr )
    {
        sketch->clear();
        delete sketch;
        sketch = nullptr;
    }

    if( current_csection != nullptr )
    {
//        sketch->clear();
        delete current_csection;

        current_csection = nullptr;
        current_csection = new CrossSectionItemWrapper( volume.getWidth(), volume.getHeight() );
        current_csection->setPen( pen_color );
        addItem( current_csection );

    }

    if( path != nullptr )
    {
        delete path;
        path = nullptr;
        path = new QGraphicsPathItem();
        path->setPen( pen_color );
        addItem( path );
    }


    addItem( csection_image );
    addItem( &volume );
    setSceneRect( volume.boundingRect());

}

void PathScene::clearSketch()
{
    removeItem( sketch );
    delete sketch;

    sketch = nullptr;
}


void PathScene::updateScene()
{
    volume.update();
    setSceneRect( volume.boundingRect() );

    if( object->hasPathCurve() == false ) return;


    for( auto &it: csections )
    {
        CrossSectionItemWrapper* csection = it.second;
        csection->setDimensions( volume.getWidth(), it.first );
    }

    int r_, g_, b_;
    object->getColor( r_, g_, b_ );
    pen_color.setColor( QColor( r_, g_, b_ ) );

    QPainterPath curve_ = QPainterPath();
    curve_.addPolygon( PolyQtUtils::curve2DToQPolyginF( object->getPathCurve() ) );
    path->setPath( curve_ );
    path->setPen( pen_color );

    update();

}


void PathScene::screenshot()
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



void PathScene::savetoRasterImage( const QString& filename )
{


    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_trajectory." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;

    QImage image( sceneRect().size().toSize(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    render( &painter );

    image.save( filename_csection );

}

void PathScene::savetoVectorImage( const QString& filename )
{

    QStringList name_and_extension = filename.split('.', QString::SkipEmptyParts );

    QString filename_csection;
    if( name_and_extension.size() > 1 ){
        filename_csection = name_and_extension[ 0 ].append( "_trajectory." );
        filename_csection.append( name_and_extension[1] );
    }
    else
        filename_csection = filename;


    QSvgGenerator svgGen;

    svgGen.setFileName( filename_csection );
    svgGen.setSize( QSize( width(), height() ) );

    svgGen.setViewBox( sceneRect() );
    svgGen.setTitle( tr( "Rapid Reservoir Modelling - SVG generated by Qt5" ) );
    svgGen.setDescription( tr( "SVG output of Rapid Reservoir Modelling Software" ) );

    QPainter painter( &svgGen );
    render( &painter );
}



void PathScene::clearScene()
{

    for( auto &it: items() )
        removeItem( it );


    if( sketch != nullptr )
    {
        sketch->clear();
        delete sketch;
        sketch = nullptr;
    }

    if( path != nullptr )
    {
        delete path;
        path = nullptr;
        path = new QGraphicsPathItem();
        path->setPen( pen_color );
        addItem( path );
    }


    if( object != nullptr )
    {
        object->clear();
        object = nullptr;
    }

    if( current_csection != nullptr )
    {
        delete current_csection;
        current_csection = nullptr;
    }

    if( csection_image != nullptr )
    {
        delete csection_image;
        csection_image = nullptr;
    }


    for( auto &it: csections )
    {
        if( (it.second) != nullptr )
        {
//            it.second->clear();
            delete it.second;
        }
    }
    csections.clear();
    volume.clear();

    boundary_anchor = QPointF( 0.0f, 0.0f );
    draw_cross_sections = true;
    current_interaction = UserInteraction::SKETCHING;


    csection_image = new QGraphicsPixmapItem();
    addItem( csection_image );

}


void PathScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        if( event->buttons() & Qt::LeftButton )
        {
            if( sketch != nullptr ) return;

            sketch = new InputSketch( QColor( 0, 255, 0 ) );
            sketch->create( event->scenePos() );
            addItem( sketch );
        }
        else if( event->buttons() & Qt::RightButton )
        {
            finishSketch();
        }
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        boundary_anchor = event->buttonDownScenePos( Qt::LeftButton );
    }


    QGraphicsScene::mousePressEvent( event );
    update();
}


void PathScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_interaction == UserInteraction::SKETCHING )
        {
            if ( sketch == nullptr ) return;
            sketch->add( event->scenePos() );
        }
        else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
        {
            int w_ = event->scenePos().x() - boundary_anchor.x() ;
            int h_ = event->scenePos().y() - boundary_anchor.y() ;

            volume.editGeometry( boundary_anchor.x(), boundary_anchor.y(), std::abs( w_),
                                 std::abs( h_) ) ;
        }

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();


    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void PathScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        if ( sketch == nullptr ) return;
        sketch->process( event->scenePos() );
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        double w_ = ( double )std::abs( event->scenePos().x() - boundary_anchor.x() );
        double d_ = ( double )std::abs( event->scenePos().y() - boundary_anchor.y() );

        emit updateVolumeWidthDepth( w_, d_ );
        setModeSketching();
    }


    QGraphicsScene::mouseReleaseEvent( event );
    update();
}


void PathScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    if( current_interaction == UserInteraction::SKETCHING )
    {
         emit interpolateObject();
    }

    QGraphicsScene::mouseDoubleClickEvent( event );
    update();
}



void PathScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void PathScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void PathScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}

void PathScene::dropEvent( QGraphicsSceneDragDropEvent* event )
{

    const QMimeData *mime_data = event->mimeData();

    if ( mime_data->hasUrls() == false )
        return;

    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    setImagetoCrossSection( url_file );
}



