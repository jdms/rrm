#include <QGraphicsSceneMouseEvent>

#include "sketchscene.h"



SketchScene::SketchScene()
{
    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ )
{
    volume = nullptr;
    readCrossSection( raw_ );
}


void SketchScene::readCrossSection( CrossSection* const& raw_ )
{

    Volume* const& vol_ = raw_->getVolume();
    addVolume( vol_ );

//    vol_->getObjects();
//    addObjects();



}



void SketchScene::addVolume( Volume* const &raw_ )
{
    clearVolume();
    volume = new VolumeItemWrapper( raw_, VolumeItemWrapper::Section::XY );

    addItem( volume );
    setSceneRect( volume->boundingRect() );
    update();
}


void SketchScene::updateVolume()
{
    volume->updateItem();
    setSceneRect( volume->boundingRect() );
    update();
}


void SketchScene::clearVolume()
{
    if( volume != nullptr )
        delete volume;
    volume = nullptr;
}






void SketchScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        volume->startPoint( event->scenePos() );
    }

    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
    if( ( event->buttons() & Qt::LeftButton ) &&
        ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        volume->resize( event->scenePos() );
    }

    QGraphicsScene::mouseMoveEvent( event );
    update();

}


void SketchScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        emit acceptVolumeDimensions( static_cast< double >( volume->boundingRect().width() ),
                                     static_cast< double >( volume->boundingRect().height() ) );
    }
}
