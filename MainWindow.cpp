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
    createToolbar();
    create2DModule();
    create3DModule();

    statusBar()->showMessage( "Status" );

}

void MainWindow::createActions()
{
    ac_new = new QAction( tr( "&New" ), this );
    ac_open = new QAction( tr( "&Open" ), this );
    ac_save = new QAction( tr( "&Save" ), this );
    ac_export = new QAction( tr( "&Export" ), this );
    ac_exit = new QAction( tr( "E&xit" ), this );

    ac_compute = new QAction( tr( "&Compute" ), this );

    ac_contents = new QAction( tr( "Contents" ), this );
    ac_about = new QAction( tr( "&About" ), this );


}

void MainWindow::createMenuBar()
{

    QMenu *mn_file = menuBar()->addMenu( tr( "&File" ) );
    mn_file->addAction( "&New" );
    mn_file->addAction( "&Open" );
    mn_file->addAction( "&Save" );
    mn_file->addAction( "&Export" );
    mn_file->addAction( "E&xit" );

    QMenu *mn_tools = menuBar()->addMenu( tr( "&Tools" ) );
    mn_tools->addAction( "&Compute" );

    QMenu *mn_help = menuBar()->addMenu( tr( "&Help" ) );
    mn_help->addAction( "Contents" );
    mn_help->addAction( "&About" );


}

void MainWindow::createToolbar()
{
    tlb_section = addToolBar( tr( "Section" ) );
    tlb_section->addAction( "&New" );
    tlb_section->addAction( "&Open" );
    tlb_section->addAction( "&Save" );
    tlb_section->addAction( "&Export" );

}

void MainWindow::create2DModule()
{

    dc_2DModule = new QDockWidget( this );
    dc_2DModule->setAllowedAreas( Qt::LeftDockWidgetArea );

    Canvas2D *canvas2D = new Canvas2D( this );

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
