#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>

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

    statusBar()->showMessage( "Status" );

}

void MainWindow::createMenuBar()
{

    QMenu *mn_file = menuBar()->addMenu( tr( "&File" ) );
    mn_file->addMenu( "&New" );
    mn_file->addMenu( "&Open" );
    mn_file->addMenu( "&Save" );
    mn_file->addMenu( "&Export" );
    mn_file->addMenu( "E&xit" );

    QMenu *mn_tools = menuBar()->addMenu( tr( "&Tools" ) );
    mn_tools->addMenu( "&Compute" );

    QMenu *mn_help = menuBar()->addMenu( tr( "&Help" ) );
    mn_help->addMenu( "Contents" );
    mn_help->addMenu( "&About" );


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
