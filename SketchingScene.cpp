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

    QGraphicsScene::mousePressEvent( event );
    update();
}

void SketchingScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    if( event->buttons() == Qt::MouseButton::LeftButton )
    {
        sketch->add( event->scenePos() );

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}

void SketchingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{


    sketch->setDone( true );
    horizonc = new HorizonController();
    horizonc->setSketching( &sketch->getSketch() );

    this->removeItem( sketch );
    bool valid = horizonc->isValid( this );

    if( valid == false ){
        delete sketch;

        QGraphicsScene::mouseReleaseEvent( event );
        update();
        return;
    }


    this->addItem( horizonc );

    QGraphicsScene::mouseReleaseEvent( event );
    update();


}
