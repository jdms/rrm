#include "sketchwindow.h"

SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
    createWindow();
}


void SketchWindow::createWindow()
{
    cs = new CanvasStack();
    setCentralWidget( cs );

}

void SketchWindow::addCanvas( std::size_t id_ )
{
    cs->addElement( id_, new QGraphicsView() );
}
