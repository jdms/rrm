#include "SketchingScene.h"

SketchingScene::SketchingScene( QObject *parent ): QGraphicsScene( parent )
{

}

void SketchingScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if( event->buttons() == Qt::MouseButton::LeftButton )
    {
        sketch = new InputSketch();
        sketch->create( event->scenePos() );
        this->addItem( sketch );

    }


    update();
}

void SketchingScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    if( event->buttons() == Qt::MouseButton::LeftButton )
    {
        sketch->add( event->scenePos() );
    }

    update();
}

void SketchingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{


    sketch->setDone( true );
    horizonc = new HorizonController();
    horizonc->setSketching( &sketch->getSketch() );

    this->removeItem( sketch );
    delete sketch;

    bool valid = horizonc->isValid( this );

    if( valid == true )
        this->addItem( horizonc );


    update();
}
