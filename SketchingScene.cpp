#include "SketchingScene.h"

SketchingScene::SketchingScene( QObject *parent ): QGraphicsScene( parent )
{
    sketch = NULL;
    horizonc = NULL;
    boundaryc = NULL;

    mode = InteractionMode::SKETCHING;

    createBoundary();

}

SketchingScene::~SketchingScene()
{
    if( horizonc == NULL )
        delete horizonc;
    horizonc = NULL;

    if( sketch == NULL )
        delete sketch;
    sketch = NULL;

    if( boundaryc == NULL )
        delete boundaryc;
    boundaryc = NULL;
}

void SketchingScene::createBoundary()
{

    boundaryc = new BoundaryController( this->sceneRect().width() - 900, this->sceneRect().height() - 700 );
    this->addItem( boundaryc );
}


void SketchingScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if( mode == SELECT )
        return;

    if( event->buttons() == Qt::MouseButton::LeftButton  )
    {

        sketch = new InputSketch( currentColor );
        sketch->create( event->scenePos() );
        this->addItem( sketch );

    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchingScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{

    if( mode == SELECT )
        return;


    if( event->buttons() == Qt::MouseButton::LeftButton )
    {
        sketch->add( event->scenePos() );

    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{

    if( mode == SELECT )
        return;

    sketch->setDone( true );
    horizonc = new HorizonController( currentColor );
    horizonc->setSketching( &sketch->getSketch() );

    this->removeItem( sketch );
    bool valid = horizonc->isValid( this );

    if( valid == false ){
        delete sketch;

        QGraphicsScene::mouseReleaseEvent( event );
        update();
        return;
    }


    horizonc->applyRule();
    this->addItem( horizonc );
    updateAllElements();


    QGraphicsScene::mouseReleaseEvent( event );
    update();


}




void SketchingScene::applyRemoveAbove()
{
    horizonc->setRule( HorizonController::RuleType::REMOVE_ABOVE );
}



void SketchingScene::applyRemoveBelow()
{
    horizonc->setRule( HorizonController::RuleType::REMOVE_BELOW );
}



void SketchingScene::updateAllElements()
{

    // only a test to verify if the updateGeometry is being called correctly
//    for( auto item: this->items() )
//    {
//        if( item->type() == HorizonController::ControllerType::HORIZON )
//        {
//            HorizonController *hc = (HorizonController*) item;
//            hc->updateGeometry();

//        }


//    }

}


void SketchingScene::setColor( int R, int G, int B )
{
    currentColor = QColor( R, G, B );
//    horizonc->setColor( R, G, B );
}


void SketchingScene::getColor( int R, int G, int B )
{
    R = currentColor.red();
    G = currentColor.green();
    B = currentColor.blue();

}


void SketchingScene::setModeSelect()
{
    mode = InteractionMode::SELECT;
}

void SketchingScene::setModeSketching()
{
    mode = InteractionMode::SKETCHING;
}
