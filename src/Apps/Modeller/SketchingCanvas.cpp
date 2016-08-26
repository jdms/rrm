#include "SketchingCanvas.h"

SketchingCanvas::SketchingCanvas( QWidget* parent ): QGraphicsView( parent )
{
    setup();
    createConnections();
}


void SketchingCanvas::setup()
{


    setRenderHint( QPainter::Antialiasing, true );
//    setOptimizationFlags( QGraphicsView::DontSavePainterState );
//    setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
//    setFocusPolicy( Qt::StrongFocus );
//    setInteractive( true );
//    setBackgroundRole( QPalette::Base );
//    setAutoFillBackground( true );

    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
//    viewport()->grabGesture( Qt::PinchGesture );
//    viewport()->grabGesture( Qt::SwipeGesture );
//    viewport()->grabGesture( Qt::PanGesture );


    scene = new SketchingScene( this );
    setScene( scene );
    scale( 1, -1 );

    scene->init();


    lb_coordinates = new QLabel();


}


void SketchingCanvas::createConnections()
{
}


void SketchingCanvas::updateCoordinates( float posx, float posy )
{
    lb_coordinates->setText( QString( "%1, %2" ).arg( posx ).arg( posy ) );

}


void SketchingCanvas::screenshot()
{

    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( this, tr( "Save Image" ), "", tr( "PNG (*.png);;SVG (*.svg)" ), &selectedFilter );


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
