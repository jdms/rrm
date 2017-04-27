#include "CanvasPath.h"

CanvasPath::CanvasPath( QWidget* parent )
{
    draw_cross_sections = true;
    scene = nullptr;
}

void CanvasPath::wheelEvent( QWheelEvent *event )
{

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


void CanvasPath::keyPressEvent( QKeyEvent *event )
{
    switch ( event->key() )
    {

        case Qt::Key_C:
        {
            draw_cross_sections = !draw_cross_sections;
            scene->drawCrossSections( draw_cross_sections );
        }
            break;

        default:
            break;

    };

}
