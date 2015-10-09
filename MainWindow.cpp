#include <QHBoxLayout>

#include "MainWindow.h"
#include "Canvas2D.h"
#include "Canvas3D.h"


MainWindow::MainWindow( QWidget *parent) : QMainWindow( parent )
{
    createWindow();
}

void MainWindow::createWindow()
{
     this->setMinimumSize( 800, 600 );


    createMenuBar();
    create2DModule();
    create3DModule();

}

void MainWindow::createMenuBar()
{


}

void MainWindow::create2DModule()
{

    dc_2DModule = new QDockWidget( this );
    dc_2DModule->setAllowedAreas( Qt::LeftDockWidgetArea );

    Canvas2D *canvas2D = new Canvas2D();

    dc_2DModule->setWidget( canvas2D );
    addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

}

void MainWindow::create3DModule()
{

    dc_3DModule = new QDockWidget( this );
    dc_3DModule->setAllowedAreas( Qt::RightDockWidgetArea );

    QGLFormat fmt;
    fmt.setVersion( 4, 1 );
    fmt.setProfile( QGLFormat::CoreProfile);

    Canvas3D *canvas3d = new Canvas3D( fmt );

    dc_3DModule->setWidget( canvas3d );
    addDockWidget( Qt::RightDockWidgetArea, dc_3DModule );

}
