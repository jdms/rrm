
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
    if( isValidSketch() == true ) return;

    sketch = new InputSketch();
    sketch->create( p );
    addItem( sketch );
}


void SketchScene_Refactored::clearSketch()
{
    if( isValidSketch() == true ) return;

    removeItem( sketch );
    delete sketch;
    sketch = nullptr;
}


void SketchScene_Refactored::finishSketch()
{

    if( isValidSketch() == false ) return;
    if( sketch->isEmpty() == true ) return;

    Curve2D curve;
    if( acceptSketch( curve ) == true  )
    {
        Curve2D cpy_curve;
        curve.douglasPeuckerSimplify( cpy_curve, 1.0 );
        emit curveAccepted( cpy_curve );
    }

    clearSketch();

}

void SketchScene_Refactored::acceptSketch( Curve2D& curve )
{

}





bool SketchScene_Refactored::isValidSketch()
{
    if ( sketch == nullptr ) return false;
    return true;
}
