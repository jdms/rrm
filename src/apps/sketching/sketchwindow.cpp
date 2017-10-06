#include "sketchwindow.h"

SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
    createWindow();
}


void SketchWindow::createWindow()
{
    cs = new CanvasStack();
    setCentralWidget( cs );

    cs->addElement( 0, new QGraphicsView() );
    cs->addElement( 1, new QGraphicsView() );
    cs->addElement( 2, new QGraphicsView() );
}
