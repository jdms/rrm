#include "SketchCanvas.h"

SketchCanvas::SketchCanvas( QWidget* parent )
{
    setRenderHint( QPainter::Antialiasing, true );
    scale( 1, -1 );
}

void SketchCanvas::wheelEvent( QWheelEvent *event )
{

    // Scale the view
    double scaleFactor = 1.5;

    if( event->delta() > 0 )
    {
        scale( scaleFactor, scaleFactor );
    }

    else
    {
        scale( 1.0/scaleFactor, 1.0/scaleFactor );
    }

}