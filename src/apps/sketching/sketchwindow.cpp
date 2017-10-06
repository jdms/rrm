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

void SketchWindow::addCanvas( CrossSection* const& cs_ )
{
    SketchScene* scene_ = new SketchScene( cs_ );
    cs->addElement( cs_->getIndex(), new QGraphicsView( scene_ ) );
}
