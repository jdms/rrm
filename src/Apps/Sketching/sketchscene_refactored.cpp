#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QDir>
#include <QUrl>
#include <QKeyEvent>

#include "Core/Geology/Models/object_refactored.h"
#include "sketchscene_refactored.h"


SketchScene_Refactored::SketchScene_Refactored()
{
    sketch = nullptr;
    current_interaction = UserInteraction::SKETCHING;
    current_color = Qt::red;
    current_csection = 0;

    createCrossSectionImageItem();
}


void SketchScene_Refactored::addVolume( Volume* const& vol )
{
    volume.setRawVolume( vol );
    setSceneRect( volume.boundingRect() );
    addItem( &volume );
}


void SketchScene_Refactored::resizingVolume( const QPointF& point, bool done )
{
    int w = static_cast< int > ( point.x() - boundary_anchor.x() );
    int h = static_cast< int > ( point.y() - boundary_anchor.y() );

    if( done == false )
        volume.resize(boundary_anchor.x(), boundary_anchor.y(), w, h ) ;
    else
    {
        emit updateVolumeDimensions( static_cast< double > ( abs( w ) ),
                                static_cast< double > ( abs( h ) ) );
        setModeSketching();
    }

}



void SketchScene_Refactored::updateVolume()
{
    volume.updateItem();
    setSceneRect( volume.boundingRect() );
    update();
}


bool SketchScene_Refactored::isAddedObject( std::size_t id ) const
{
    auto search = objects.find( id );
    if( search == objects.end() )
        return false;
    else
        return true;
}



void SketchScene_Refactored::addObject( Object_Refactored* const& obj )
{
    if( isAddedObject( obj->getId() ) == true ) return;

    ObjectItemWrapper_Refactored* wrapper = new ObjectItemWrapper_Refactored();
    wrapper->setRawObject( obj, current_csection );

    objects[ obj->getId() ] = wrapper;

    addItem( wrapper );
    update();
}


void SketchScene_Refactored::updateObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectItemWrapper_Refactored* const& wrapper = objects[ id ];
    wrapper->updateObject();
    update();
}


void SketchScene_Refactored::reActiveObject( std::size_t id )
{
    if( isAddedObject( id ) == false ) return;

    ObjectItemWrapper_Refactored* const& wrapper = objects[ id ];
    wrapper->updateDepth( current_csection );

    addItem( wrapper );
    update();
}



void SketchScene_Refactored::removeObjectsFromScene()
{
    for( auto &it: items() )
        removeItem( it );

    if( isValidSketch() == true )
        clearSketch();

    addItem( csection_image );
    addItem( &volume );
    setSceneRect( volume.boundingRect() );

}


void SketchScene_Refactored::setModeEditable( bool status )
{
    if( status == false )
    {
        setModeSketching();
        return;
    }

    for( auto it: objects )
    {
        ObjectItemWrapper_Refactored* const& wrapper = objects[ it.first ];
        wrapper->enableEditing();
    }

    current_interaction = UserInteraction::EDITING_SCENE;
}


void SketchScene_Refactored::setModeSketching()
{
    current_interaction = UserInteraction::SKETCHING;
}


void SketchScene_Refactored::setModeSelecting()
{
    current_interaction = UserInteraction::SELECTING;
}


void SketchScene_Refactored::setModeEditingBoundary()
{
    current_interaction = UserInteraction::EDITING_BOUNDARY;
}


void SketchScene_Refactored::setModeMovingImage()
{
    current_interaction = UserInteraction::MOVING_IMAGE;
    csection_image->setFlag( QGraphicsItem::ItemIsMovable, true );
}


void SketchScene_Refactored::disableMovingImage()
{
    csection_image->setFlag( QGraphicsItem::ItemIsMovable, false );
    csection_image->setFlag( QGraphicsItem::ItemIsSelectable, false );
    setModeSketching();
}


void SketchScene_Refactored::setCurrentColor( const QColor& color )
{
    current_color = color;

    if( isValidSketch() == true )
        sketch->setColor( color );

    update();

}


void SketchScene_Refactored::setCurrentColor( int r, int g, int b )
{
    setCurrentColor( QColor( r, g, b ) );
}


void SketchScene_Refactored::startSketch( const QPointF& p )
{
    if( isValidSketch() == true ) return;

    sketch = new InputSketch( current_color );
    sketch->create( p );
    addItem( sketch );

    update();
}


void SketchScene_Refactored::clearSketch()
{
    if( isValidSketch() == false ) return;

    removeItem( sketch );
    delete sketch;
    sketch = nullptr;

    update();
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

    update();
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



void SketchScene_Refactored::removeCurve()
{

    if( current_interaction != UserInteraction::EDITING_SCENE ) return;

    QList < QGraphicsItem* > items = selectedItems();
    if( items.empty() == true ) return;

    ObjectItemWrapper_Refactored* obj = ( ObjectItemWrapper_Refactored* )items[ 0 ];
    std::size_t id = obj->getId();
    obj->clear();

    removeItem( obj );

    delete objects[ id ];
    objects.erase( id );

    emit removeCurveFromObject( current_csection );
    update();
}


void SketchScene_Refactored::processCurve( Curve2D& curve )
{
    Curve2D cpy = curve;
    cpy.lineFilter();
    cpy.meanFilter();
    cpy.meanFilter();
    curve = cpy;
}


void SketchScene_Refactored::setCurrentCrossSection( double depth )
{
    current_csection = depth;
    removeObjectsFromScene();

    if( hasImageInCrossSection() == false ) return;
    setImageToCrossSection( backgrounds[ current_csection ] );

}



void SketchScene_Refactored::createCrossSectionImageItem()
{
    csection_image = new QGraphicsPixmapItem();
    csection_image->setFlag( QGraphicsItem::ItemStacksBehindParent, true );
    addItem( csection_image );


    update();
}


void SketchScene_Refactored::setImageToCrossSection( const QString& file )
{
    QPixmap image = QPixmap( file );
    csection_image->setPixmap( image );

    //TODO: save the position
    backgrounds[ current_csection ] = file;



    update();
}


void SketchScene_Refactored::removeImageFromCrossSection()
{
    removeItem( csection_image );
    delete csection_image;

    createCrossSectionImageItem();

    update();
}


bool SketchScene_Refactored::hasImageInCrossSection()
{
    auto search = backgrounds.find( current_csection );
    if( search == backgrounds.end() )
        return false;
    else
        return true;
}


void SketchScene_Refactored::mousePressEvent( QGraphicsSceneMouseEvent *event )
{


    if(  event->modifiers() & Qt::ControlModifier )
    {
        if( csection_image->isUnderMouse() == true )
            setModeMovingImage();
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


    else if( ( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        boundary_anchor = event->scenePos();
    }
    
    QGraphicsScene::mousePressEvent( event );
    update();
}


void SketchScene_Refactored::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    if ( ( event->buttons() & Qt::LeftButton ) &&
         ( current_interaction == UserInteraction::SKETCHING ) &&
         ( isValidSketch() == true ) )
    {
            sketch->add( event->scenePos() );
    }
    else if( ( event->buttons() & Qt::LeftButton ) &&
             ( current_interaction == UserInteraction::EDITING_BOUNDARY ) )
    {
        resizingVolume( event->scenePos() );
    }

    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchScene_Refactored::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if( ( current_interaction == UserInteraction::SKETCHING ) &&
            ( isValidSketch() == true ) )
    {
        sketch->process( event->scenePos() );
    }

    else if( current_interaction == UserInteraction::MOVING_IMAGE )
    {
        disableMovingImage();
    }

    else if( current_interaction == UserInteraction::EDITING_BOUNDARY )
    {
        bool done = true;
        resizingVolume( event->scenePos(), done );
    }


    QGraphicsScene::mouseReleaseEvent( event );
    update();
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



void SketchScene_Refactored::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
        case Qt::Key_Delete:
            removeCurve();
            break;
        default:
            break;
    };
}
