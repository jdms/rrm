#include "Controller/MainWindow.h"

/// Skecthing
void MainWindow::create3DModule()
{

    dc_3DModule = new QDockWidget( this );
    dc_3DModule->setAllowedAreas( Qt::RightDockWidgetArea );
    dc_3DModule->setWindowTitle( "3D View" );

    glWidget = new GLWidget(this);

    QFrame *fr = new QFrame( this );
    fr->setFrameStyle( QFrame::Box | QFrame::Sunken );

    QHBoxLayout *hb_canvas3d = new QHBoxLayout( this );

    hb_canvas3d->addWidget(glWidget);

    fr->setLayout( hb_canvas3d );
    dc_3DModule->setWidget( fr );
    addDockWidget( Qt::RightDockWidgetArea, dc_3DModule );

}
