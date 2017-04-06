#include "SketchScene.h"

SketchScene::SketchScene( QObject* parent )
{
    sketch = new InputSketch( QColor( 255, 0, 0 ) );
    addItem( sketch );
}


void SketchScene::resetData()
{
    object_list.clear();

    removeItem( sketch );
    removeItem( &volume );

    clear();

    addItem( sketch );
    addItem( &volume );

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
    return true;//arrangement.insert( curve_, id );
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




void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
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


    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::SKETCHING )
    {
        sketchingInteractions( event );
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
    }

    else if( event->type () == QEvent::GraphicsSceneMouseDoubleClick )
    {
        emit createNewObject();
    }
}
