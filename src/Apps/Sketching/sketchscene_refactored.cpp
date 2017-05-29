#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDir>
#include <QUrl>

#include "sketchscene_refactored.h"


SketchScene_Refactored::SketchScene_Refactored()
{
    sketch = nullptr;
    current_interaction = UserInteraction::SKETCHING;

    createCrossSectionImageItem();
}


void SketchScene_Refactored::addVolume( Volume* const& vol )
{
    volume.setRawVolume( vol );
    setSceneRect( volume.boundingRect() );
    addItem( &volume );
}




void SketchScene_Refactored::startSketch( const QPointF& p )
{
    if( isValidSketch() == true ) return;

    sketch = new InputSketch( QColor( 255, 0, 0 ) );
    sketch->create( p );
    addItem( sketch );
}


void SketchScene_Refactored::clearSketch()
{
    if( isValidSketch() == false ) return;

    removeItem( sketch );
    delete sketch;
    sketch = nullptr;
}


void SketchScene_Refactored::finishSketch()
{

    if( isValidSketch() == false ) return;


    Curve2D curve;
    if( acceptSketch( curve ) == true  )
    {
        emit addCurveToObject( curve );
    }

    clearSketch();

}


bool SketchScene_Refactored::acceptSketch( Curve2D& curve )
{
    if( sketch->isEmpty() == true ) return false;

    QPolygonF c = sketch->getSketchFunctionGraph();
    curve = PolyQtUtils::qPolyginFToCurve2D( c );
    processCurve( curve );

    return true;

}


bool SketchScene_Refactored::isValidSketch()
{
    if ( sketch == nullptr ) return false;
    return true;
}




void SketchScene_Refactored::processCurve( Curve2D& curve )
{
    Curve2D cpy = curve;
    cpy.lineFilter();
    cpy.meanFilter();
    cpy.meanFilter();
    curve = cpy;
}



void SketchScene_Refactored::createCrossSectionImageItem()
{
    csection_image = new QGraphicsPixmapItem();
    csection_image->setFlag( QGraphicsItem::ItemStacksBehindParent, true );
    addItem( csection_image );
}


void SketchScene_Refactored::setImageToCrossSection( const QString& file )
{
    QPixmap image = QPixmap( file );
    csection_image->setPixmap( image );

    //TODO: add to map of images and cross_sections, saving the position too

}


void SketchScene_Refactored::resetImageToCrossSection()
{
    //TODO: create ana ction to this method
    removeItem( csection_image );
    delete csection_image;

    createCrossSectionImageItem();
}


void SketchScene_Refactored::mousePressEvent( QGraphicsSceneMouseEvent *event )
{

    if(  ( event->modifiers() & Qt::ControlModifier ) &&
         ( current_interaction == UserInteraction::MOVING_IMAGE ) )
    {
        csection_image->setFlag( QGraphicsItem::ItemIsMovable, true );
    }

    else if ( ( event->buttons() & Qt::LeftButton ) &&
              ( current_interaction == UserInteraction::SKETCHING ) )
    {
        startSketch( event->scenePos() );
    }

    else if ( ( event->buttons() & Qt::RightButton ) &&
              ( current_interaction == UserInteraction::SKETCHING ) )
    {
        finishSketch();
    }

    QGraphicsScene::mousePressEvent( event );
}


void SketchScene_Refactored::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if ( ( event->buttons() & Qt::LeftButton ) &&
         ( current_interaction == UserInteraction::SKETCHING ) &&
         ( isValidSketch() == true ) )
    {
            sketch->add( event->scenePos() );
    }


    QGraphicsScene::mouseMoveEvent( event );
}


void SketchScene_Refactored::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( ( current_interaction == UserInteraction::SKETCHING ) &&
            ( isValidSketch() == true ) )
    {
        sketch->process( event->scenePos() );
    }

    else if( ( event->modifiers() & Qt::ControlModifier ) &&
             ( current_interaction == UserInteraction::MOVING_IMAGE ) )
    {
        csection_image->setFlag( QGraphicsItem::ItemIsMovable, false );
        csection_image->setFlag( QGraphicsItem::ItemIsSelectable, false );
        current_interaction = UserInteraction::SKETCHING;
    }

    QGraphicsScene::mouseReleaseEvent( event );
}




void SketchScene_Refactored::dragEnterEvent( QGraphicsSceneDragDropEvent* event )
{
    if( ( event->mimeData()->hasUrls() == true ) && ( event->mimeData()->hasImage() ) )
    {
            event->acceptProposedAction();
    }

}


void SketchScene_Refactored::dropEvent( QGraphicsSceneDragDropEvent* event )
{
    const QMimeData *mime_data = event->mimeData();
    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    setImageToCrossSection( url_file );

}


void SketchScene_Refactored::dragMoveEvent( QGraphicsSceneDragDropEvent* event )
{
    event->accept();
}

