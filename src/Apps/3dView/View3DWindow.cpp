#include "View3DWindow.hpp"

View3DWindow::View3DWindow ( QWidget* parent ) : QMainWindow(parent)
{

    createWindow();

    connect( canvas3d, &View3DCanvas::openglInitialized, [=](){ emit initializeScene(); } );

}


void View3DWindow::createWindow()
{


    canvas3d = new View3DCanvas ( this );


    QVBoxLayout* hb_canvas3d = new QVBoxLayout();
    hb_canvas3d->addWidget( canvas3d );



    QFrame *fr = new QFrame ( this );
    fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );
    fr->setLayout ( hb_canvas3d );


    setCentralWidget( fr );


}


