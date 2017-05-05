#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QSvgGenerator>
#include <QFileDialog>


#include "PathScene.h"

PathScene::PathScene( QObject* parent )
{
    sketch = new InputSketch( QColor( 0, 255, 0 ) );
    csection_image = new QGraphicsPixmapItem();
    current_interaction = UserInteraction::SKETCHING;
    draw_cross_sections = true;
    object = nullptr;
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
    if ( sketch->isEmpty() == true ) return;

    Curve2D curve_;
    if( acceptSketch( curve_ ) == true  )
        emit curveAccepted( curve_ );

    clearSketch();
    updateScene();
}


bool PathScene::acceptSketch( Curve2D &curve_ )
{
    curve_ = PolyQtUtils::qPolyginFToCurve2D( sketch->getSketch() );
    return true;
}

void PathScene::addCrossSection( double depth_ )
{
    csections[ depth_ ] = new CrossSectionItemWrapper( volume.getWidth(), depth_ );
    csections[ depth_ ]->setVisible( draw_cross_sections );
    addItem( csections[ depth_ ] );
    update();
}


void PathScene::resetData()
{
    csections.clear();

    removeItem( &volume );
    removeItem( sketch );
    removeItem( csection_image );

    clear();

    addItem( csection_image );
    addItem( &volume );
    addItem( sketch );

    setSceneRect( volume.boundingRect() );
    update();
}

void PathScene::clearSketch()
{
    sketch->clear();
    update();
}


void PathScene::updateScene()
{
    volume.update();
    setSceneRect( volume.boundingRect() );

    if( object->hasPathCurve() == false ) return;

    QPainterPath path;
    path.addPolygon( PolyQtUtils::curve2DToQPolyginF( object->getPathCurve() ) );


    QPen pen_color = QColor( 0, 0, 255 );
    pen_color.setCapStyle( Qt::RoundCap );
    pen_color.setJoinStyle( Qt::RoundJoin );
    pen_color.setWidth( 3 );

    addPath( path,  pen_color );

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

    QImage image( sceneRect().size().toSize(), QImage::Format_ARGB32 );
    image.fill( Qt::transparent );

    QPainter painter( &image );
    render( &painter );

    image = image.mirrored( false, true );
    image.save( filename );

}

void PathScene::savetoVectorImage( const QString& filename )
{

    QSvgGenerator svgGen;

    svgGen.setFileName( filename );
    svgGen.setSize( QSize( width(), height() ) );

    svgGen.setViewBox( sceneRect() );
    svgGen.setTitle( tr( "Rapid Reservoir Modelling - SVG generated by Qt5" ) );
    svgGen.setDescription( tr( "SVG output of Rapid Reservoir Modelling Software" ) );

    QPainter painter( &svgGen );
    painter.scale( 1.0, -1.0 );
    painter.translate( QPointF( 0.0, -height() ) );

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


    if( object != nullptr )
    {
        object->clear();
        object = nullptr;
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

    sketch = new InputSketch( QColor( 0, 255, 0 ) );
    csection_image = new QGraphicsPixmapItem();

    addItem( sketch );
    addItem( csection_image );

}


void PathScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        if( event->buttons() & Qt::LeftButton )
        {
            sketch->create( event->scenePos() );
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

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketch->add( event->scenePos() );
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        int w_ = event->scenePos().x() - boundary_anchor.x() ;
        int d_ = event->scenePos().y() - boundary_anchor.y() ;

        volume.editGeometry( boundary_anchor.x(), boundary_anchor.y(), std::abs( w_),
                             std::abs( d_) ) ;
    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}

void PathScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketch->process( event->scenePos() );
        setModeSketching();
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        double w_ = ( double )std::abs( event->scenePos().x() - boundary_anchor.x() );
        double d_ = ( double )std::abs( event->scenePos().y() - boundary_anchor.y() );

        emit updateVolumeWidthDepth( w_, d_ );
    }

    QGraphicsScene::mouseReleaseEvent( event );
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


