#include "CanvasPath.h"

CanvasPath::CanvasPath( QWidget* parent )
{
    draw_cross_sections = true;
    scene = nullptr;
    setRenderHint( QPainter::Antialiasing, true );
}

void CanvasPath::wheelEvent( QWheelEvent *event )
{


    if( event->modifiers() & Qt::ControlModifier )
    {

        const int WHEEL_STEP = 120;
        float pos = event->delta()/float( WHEEL_STEP );

        if ( pos > 0 )
        {
            emit decreaseSlider();
        }

        else if ( pos < 0 )
        {
            emit increaseSlider();
        }
    }
    else
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
