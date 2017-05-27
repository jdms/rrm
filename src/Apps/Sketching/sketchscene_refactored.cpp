
#include "sketchscene_refactored.h"

#include "Sketching/InputSketch.h"


SketchScene_Refactored::SketchScene_Refactored()
{
    sketch = nullptr;
}

void SketchScene_Refactored::addVolume( Volume* const& vol )
{
    volume.setRawVolume( vol );
    setSceneRect( volume.boundingRect() );
    addItem( &volume );
}

void SketchScene_Refactored::startSketch( const QPointF& p )
{
    if( sketch != nullptr ) return;

    sketch = new InputSketch();
    sketch->create( p );
    addItem( sketch );
}


void SketchScene_Refactored::clearSketch()
{
    if( sketch == nullptr ) return;

    removeItem( sketch );
    delete sketch;
    sketch = nullptr;
}
