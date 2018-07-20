#include "mainwindow1.h"

MainWindow1::MainWindow1( QWidget* parent_ ): QMainWindow( parent_ )
{

    setWindowProperties();
    createWindow();


}


void MainWindow1::setWindowProperties()
{

    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setDockNestingEnabled( true );

    setWindowTitle( "Rapid Reservoir Modelling" );

}


void MainWindow1::createWindow()
{

    createActions();
    createMenuBar();
    createToolbar();
    createController();

    plug3dInterface();
    plugSketchInterface();

}


void MainWindow1::createActions()
{
    ac_save = new QAction( "Save", this );
    ac_save->setIcon(QIcon(":/images/icons/save.png"));

    ac_load = new QAction( "Load", this );
    ac_load->setIcon(QIcon(":/images/icons/load.png"));

    ac_clear = new QAction( "New", this );
    ac_clear->setIcon(QIcon(":/images/icons/new.png"));

    ac_export = new QAction( "Export", this );
    ac_exit = new QAction( tr ( "E&xit" ) , this );

    ac_manual = new QAction( tr ( "RRM Manual" ), this );
    ac_about = new QAction( tr ( "&About" ) , this );


    ac_undo = new QAction( "Undo", this );
    ac_undo->setIcon(QIcon(":/images/icons/undo.png"));

    ac_redo = new QAction( "Redo", this );
    ac_redo->setIcon(QIcon(":/images/icons/redo.png"));

    ac_sketch_above = new QAction( "PA", this ); // create above!
    ac_sketch_above->setCheckable( true );

    ac_sketch_below = new QAction( "PB", this ); // create below!
    ac_sketch_below->setCheckable( true );


    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );

    ac_remove_above_int = new QAction( "RAI", this );
    ac_remove_above_int->setCheckable( true );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );

    ac_remove_below_int = new QAction( "RBI", this );
    ac_remove_below_int->setCheckable( true );

    ac_screenshot = new QAction( "Screenshot", this );
    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));


}


void MainWindow1::createMenuBar()
{

    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_clear );
    mn_file->addAction ( ac_save );
    mn_file->addAction ( ac_load );
    mn_file->addSeparator();
    mn_file->addAction( ac_export );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar()->addMenu ( tr ( "&Windows" ) );


    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
    mn_help->addAction( ac_manual );
    mn_help->addAction( ac_about );


}


void MainWindow1::createToolbar()
{


    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->setExclusive( true );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_remove_above_int );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_remove_below_int );

    tb_mainwindow = addToolBar( "");
    tb_mainwindow->addAction( ac_clear );
    tb_mainwindow->addAction( ac_save );
    tb_mainwindow->addAction( ac_load );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_undo );
    tb_mainwindow->addAction( ac_redo );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_sketch_above );
    tb_mainwindow->addAction( ac_sketch_below );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_rules->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_screenshot );


}


void MainWindow1::createController()
{
    controller = new Controller();
    app = new RRMApplication();
    app->setController( controller );
    app->init();
}


void MainWindow1::plugSketchInterface()
{
    sketchapp = std::make_shared< SketchInterface >( this );
    sketchapp->createInterface();
    sketchapp->init();
}


void MainWindow1::plug3dInterface()
{
    view3dapp = std::make_shared< View3dInterface >( this );
    view3dapp->createInterface();
    view3dapp->init();
}
