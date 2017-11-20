
#include "mainwindow.h"

#include <QtWidgets>
#include <QActionGroup>
#include <QSlider>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>


MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
    controller = nullptr;

    setupWindowProperties();
    createWindow();

//    show();

//    createController();
//    setupController();
//    run_app();
}



void MainWindow::setupWindowProperties()
{
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setDockNestingEnabled( true );

    QRect rect = QGuiApplication::primaryScreen()->geometry();

    app_width  = static_cast< int > (  rect.width() * 0.8f );
    app_height = static_cast< int > ( rect.height() * 0.8f );

    app_orig_x = static_cast< int > ( (  rect.width() - app_width  ) * 0.5f );
    app_orig_y = static_cast< int > ( ( rect.height() - app_height ) * 0.5f );

    setGeometry( app_orig_x, app_orig_y, app_width, app_height );
    setWindowTitle( "Rapid Reservoir Modelling" );
}


void MainWindow::createWindow()
{
    createMainInterface();
    createSidebar();
    createSketchingWindow();

    createToolbar();

}




void MainWindow::createMainInterface()
{

    canvas3d = new Canvas3d();

    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
    sl_depth_csection->setDiscretization( 500 );
    sl_depth_csection->setRange( 0, 500 );



    hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

}



void MainWindow::createToolbar()
{

    ac_save = new QAction( "Save", this );
    ac_load = new QAction( "Load", this );

    ac_clear = new QAction( "New", this );

    ac_undo = new QAction( "Undo", this );
    ac_redo = new QAction( "Redo", this );

    ac_sketch_above = new QAction( "SA", this );
    ac_sketch_above->setCheckable( true );

    ac_sketch_below = new QAction( "SB", this );
    ac_sketch_below->setCheckable( true );



    QAction* ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );

    QAction* ac_remove_above_int = new QAction( "RAI", this );
    ac_remove_above_int->setCheckable( true );

    QAction* ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );

    QAction* ac_remove_below_int = new QAction( "RBI", this );
    ac_remove_below_int->setCheckable( true );

    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->setExclusive( true );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_remove_above_int );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_remove_below_int );


    QAction* ac_output_volume = new QAction( "Get Regions", this );

    tb_mainwindow =  addToolBar( "Test ");
    tb_mainwindow->addAction( ac_clear );
    tb_mainwindow->addAction( ac_save );
    tb_mainwindow->addAction( ac_load );
    tb_mainwindow->addAction( ac_undo );
    tb_mainwindow->addAction( ac_redo );
    tb_mainwindow->addAction( ac_sketch_above );
    tb_mainwindow->addAction( ac_sketch_below );
    tb_mainwindow->addActions( ag_rules->actions() );
    tb_mainwindow->addAction( ac_output_volume );

//    addToolBar( tb_mainwindow );


}


void MainWindow::createSidebar()
{

    object_tree = new ObjectTree( this );
    dw_object_tree = new QDockWidget( "" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_tree );


    object_properties = new PagesStack();
    dw_object_properties = new QDockWidget( "" );
    dw_object_properties->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_properties->setWidget( object_properties );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_properties );

}



void MainWindow::createSketchingWindow()
{

    sketch_window = new SketchWindow();
    dw_sketchwindow = new QDockWidget( "Cross-Section" );
    dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketchwindow->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketchwindow );

    sketch_topview_window = new SketchWindow();
    dw_topview_window = new QDockWidget( "Top-View" );
    dw_topview_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_topview_window->setWidget( sketch_topview_window );
    dw_topview_window->setVisible( true );
    addDockWidget( Qt::BottomDockWidgetArea, dw_topview_window );


}




void MainWindow::createController()
{
    if( controller != nullptr )
        delete controller;

    controller = new Controller();
}


void MainWindow::setupController()
{
    Scene3d* scene3d = canvas3d->getScene();
    controller->setScene3d( scene3d );
    controller->setObjectTree( object_tree );
}


void MainWindow::setupSlider()
{
    double w_, h_, d_;
    controller->getVolumeDimensions( w_, h_, d_ );

    emit defineMainCrossSection( d_ );
    emit defineVolumeDimensions( w_, h_, d_ );;

    sl_depth_csection->setValue( d_ );
}



void MainWindow::run_app()
{

//    app = new RRMApplication( this );
////    app->init();

//    controller->init();
//    setupSlider();


}


void MainWindow::checkUndoRedo()
{
    if( controller == nullptr ) return;

    ac_undo->setEnabled( controller->canUndo() );
    ac_redo->setEnabled( controller->canRedo() );

    checkSketchStatus();
}


void MainWindow::checkSketchStatus()
{
    if( controller == nullptr ) return;

    ac_sketch_above->setChecked( controller->isDefineAboveActive() );
    ac_sketch_below->setChecked( controller->isDefineBelowActive() );
    emit updateObjects();
}


void MainWindow::clear()
{

    sketch_window->clear();
    sketch_topview_window->clear();
    controller->clear();
    sl_depth_csection->clear();

    emit resetMenus();
    emit resetWindows();


    run_app();

}


