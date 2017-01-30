#include "SketchingCanvas.h"

SketchingCanvas::SketchingCanvas( QWidget* parent ): QGraphicsView( parent )
{
    setup();
}


void SketchingCanvas::setup()
{


    setRenderHint( QPainter::Antialiasing, true );
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );


    setBackGround();
    scale( 1, -1 );

    lb_coordinates = new QLabel();

}






void SketchingCanvas::setBackGround()
{

    int step = 20;

    QRadialGradient gradient( (qreal)width()*0.5f, (qreal)height()*0.5f, width() );

    for ( qreal radius = 0.0f; radius <= 1.0f; radius += 0.1f )
    {
        gradient.setColorAt( radius, QColor::fromRgb( 154.28f + ( radius*step ), 176.20f + ( radius*step ), 199.16f + ( radius*step ) ) );
    }

    setBackgroundBrush( QBrush( gradient ) );

}


void SketchingCanvas::updateCoordinates( float posx, float posy )
{
    lb_coordinates->setText( QString( "%1, %2" ).arg( posx ).arg( posy ) );

}


void SketchingCanvas::screenshot()
{

    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( this, tr( "Save Image" ), "./screenshots/", tr( "PNG (*.png);;SVG (*.svg)" ), &selectedFilter );


    if( selectedFilter == "PNG (*.png)" )
    {
        scene->savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        scene->savetoVectorImage( name_of_file );
    }

}



void SketchingCanvas::wheelEvent( QWheelEvent *event )
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
