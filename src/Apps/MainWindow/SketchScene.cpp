#include <QMimeData>
#include <QUrl>
#include <QDir>

#include "SketchScene.h"

SketchScene::SketchScene( QObject* parent )
{
    sketch = new InputSketch( QColor( 255, 0, 0 ) );
    csection_image = new QGraphicsPixmapItem();

}


void SketchScene::resetData()
{
    object_list.clear();

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

void SketchScene::finishSketch()
{
    if ( sketch->isEmpty() == true ) return;

    Curve2D curve_;
    if( acceptSketch( curve_ ) == true  )
        emit curveAccepted( curve_ );

    clearSketch();
}


bool SketchScene::acceptSketch( Curve2D& curve_ )
{
    curve_ = PolyQtUtils::qPolyginFToCurve2D( sketch->getSketch() );

    //arrangement.insert( curve_, id );
//    std::vector< size_t > upper_bound = arrangement.getLastCurveLowerBound();
//    std::vector< size_t > lower_bound = arrangement.getLastCurveUpperBound();
//    controller->interpolateStratigraphy(lower_bound, upper_bound);

    return true;
}


void SketchScene::clearSketch()
{
    sketch->clear();
    update();
}



void SketchScene::addObject( Object* obj_ )
{
    ObjectItemWrap* obj_wrapper_;

    auto search = object_list.find( obj_->getId() );
    if( search == object_list.end() )
    {
        obj_wrapper_ = new ObjectItemWrap();
        addItem( obj_wrapper_ );

        object_list[ obj_->getId() ] = obj_wrapper_;
    }
    else
        obj_wrapper_ = object_list[ obj_->getId() ];


    obj_wrapper_->setObjectRaw( obj_, csection.getZCoordinate() );




    update();
}


void SketchScene::setImagetoCrossSection( const QString& url_ )
{
//    QTransform matrix_;
//    matrix_.scale( 1, -1 );

    QPixmap image_ = QPixmap( url_ );
    csection_image->setPixmap( image_ );

    emit updateVolumeRawGeometry( image_.rect().width(), image_.rect().height() );

    update();
}


void SketchScene::updateScene()
{
    volume.update();
    setSceneRect( volume.boundingRect() );

    for( auto &it_: object_list )
    {
        ObjectItemWrap* obj_= it_.second;
        obj_->updateCrossSection( csection.getZCoordinate() );
    }

    update();
}




void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        boundary_anchor = event->buttonDownScenePos( Qt::LeftButton );
    }


    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        int w_ = event->scenePos().x() - boundary_anchor.x() ;
        int h_ = event->scenePos().y() - boundary_anchor.y() ;

        volume.editGeometry( boundary_anchor.x(), boundary_anchor.y(), std::abs( w_),
                             std::abs( h_) ) ;
    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
    }
    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        double w_ = ( double )std::abs( event->scenePos().x() - boundary_anchor.x() );
        double h_ = ( double )std::abs( event->scenePos().y() - boundary_anchor.y() );

        emit updateVolumeRawGeometry( w_, h_ );
    }

    QGraphicsScene::mouseReleaseEvent( event );
    update();
}


void SketchScene::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
{
    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
    }

    QGraphicsScene::mouseDoubleClickEvent( event );
    update();
}


void SketchScene::sketchingInteractions( QGraphicsSceneMouseEvent* event )
{

    if( event->type() == QEvent::GraphicsSceneMousePress )
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

    else if( event->type() == QEvent::GraphicsSceneMouseMove )
    {
        sketch->add( event->scenePos() );
    }

    else if( event->type() == QEvent::GraphicsSceneMouseRelease )
    {
        sketch->process( event->scenePos() );
        setModeSketching();
    }

    else if( event->type () == QEvent::GraphicsSceneMouseDoubleClick )
    {
        emit interpolateObject();
    }
}




void SketchScene::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dragLeaveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}


void SketchScene::dropEvent( QGraphicsSceneDragDropEvent* event )
{

    const QMimeData *mime_data = event->mimeData();

    if ( mime_data->hasUrls() == false )
        return;

    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    setImagetoCrossSection( url_file );
}
