#include "mainwindow_refactored.h"
#include "3dView/canvas3d_refactored.h"
#include "controller_refactored.h"
#include "Sketching/sketchwindow.h"
#include "Widgets/pagesstack.h"
#include "Object_Tree/ObjectTree.h"

#include <QtWidgets>
#include <QActionGroup>
#include <QSlider>


MainWindow_Refactored::MainWindow_Refactored( QWidget *parent ) : QMainWindow( parent )
{
    controller = nullptr;

    setupWindowProperties();
    createWindow();
    setDefaultValues();

    show();

    createController();
    setupController();
    setupInterface();

}


void MainWindow_Refactored::setupWindowProperties()
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


void MainWindow_Refactored::createWindow()
{
    createMainInterface();
    createSidebar();
    createCrossSectionInterface();
    createTopViewInterface();
    createToolbarActions();
}


void MainWindow_Refactored::createMainInterface()
{

    canvas3d = new Canvas3d_Refactored();

    sl_depth_csection = new QSlider( Qt::Vertical, this );
    sl_depth_csection->setTickPosition( QSlider::TicksRight );
    sl_depth_csection->setInvertedAppearance( true );


    connect( this, &MainWindow_Refactored::resizedVolume, [=]( double w, double h, double d )
                                    { sl_depth_csection->setRange( 0, static_cast< int > (d) );
                                      sl_depth_csection->setValue( static_cast< int > (d) );    } );


    QHBoxLayout* hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );

    QWidget* central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );
}


void MainWindow_Refactored::createToolbarActions()
{

    ac_show_sidebar = new QAction( "Show Sidebar", this );
    ac_show_sidebar->setCheckable( true );

    connect( ac_show_sidebar, &QAction::toggled, dw_object_tree, &QDockWidget::setVisible );
    connect( ac_show_sidebar, &QAction::toggled, dw_info_objects, &QDockWidget::setVisible );



    QAction* ac_new = new QAction( "Clear", this );

    QAction* ac_undo = new QAction( "Undo", this );

    QAction* ac_redo = new QAction( "Redo", this );


    QToolBar* tb_general = new QToolBar( this );
    tb_general->addAction( ac_show_sidebar );
    tb_general->addSeparator();
    tb_general->addAction( ac_new );
    tb_general->addAction( ac_undo );
    tb_general->addAction( ac_redo );
    addToolBar( tb_general );



    ac_show_topview = new QAction( "Show Top-View", this );
    ac_show_topview->setCheckable( true );
    connect( ac_show_topview, &QAction::toggled, dw_topview, &QDockWidget::setVisible );


    QAction* ac_sketch_above = new QAction( "SA", this );
    QAction* ac_sketch_below = new QAction( "SB", this );

    QActionGroup* ag_region_sketching = new QActionGroup( this );
    ag_region_sketching->addAction( ac_sketch_above );
    ag_region_sketching->addAction( ac_sketch_below );
    ag_region_sketching->setExclusive( false );


    QAction* ac_remove_above = new QAction( "RA", this );
    QAction* ac_ra_intersection = new QAction( "RAI", this );

    QAction* ac_remove_below = new QAction( "RB", this );
    QAction* ac_rb_intersection = new QAction( "RBI", this );


    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_ra_intersection );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_rb_intersection );
    ag_rules->setExclusive( true );


    QToolBar* tb_app_rrm = new QToolBar( this );
    tb_app_rrm->addAction( ac_show_topview );
    tb_app_rrm->addSeparator();
    tb_app_rrm->addActions( ag_region_sketching->actions() );
    tb_app_rrm->addSeparator();
    tb_app_rrm->addActions( ag_rules->actions() );
    addToolBar( tb_app_rrm );


    QAction* ac_screenshot = new QAction( "Screenshot", this );

    QToolBar* tb_misc = new QToolBar( this );
    tb_misc->addAction( ac_screenshot );
    addToolBar( tb_misc );

}


void MainWindow_Refactored::createCrossSectionInterface()
{
    sketch_window = new SketchWindow( this );

    dw_csection = new QDockWidget( "Cross-Section" );
    dw_csection->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_csection->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_csection );
}


void MainWindow_Refactored::createTopViewInterface()
{
    topview_window = new SketchWindow( this, false );

    dw_topview = new QDockWidget( "Top View" );
    dw_topview->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_topview->setWidget( topview_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_topview );
}


void MainWindow_Refactored::createSidebar()
{
    object_tree = new ObjectTree( this );
    object_tree->setMaximumWidth( 0.2*app_width );

    dw_object_tree = new QDockWidget( "" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_tree );


    pages_sidebar = new PagesStack();
    dw_info_objects = new QDockWidget( "Properties" );
    dw_info_objects->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_info_objects->setWidget( pages_sidebar );
    addDockWidget( Qt::LeftDockWidgetArea, dw_info_objects );

    connect( this, &MainWindow_Refactored::resizedVolume, pages_sidebar,
             &PagesStack::changeVolumeSize );

}




void MainWindow_Refactored::createController()
{
    if( controller != nullptr )
        delete controller;

    controller = new Controller_Refactored();
}


void MainWindow_Refactored::setupController()
{
    CSectionScene* csection_scene = ( CSectionScene* ) ( sketch_window->getScene() );
    TopViewScene* topview_scene = ( TopViewScene* ) ( topview_window->getScene() );
    Scene3d_refactored* scene3d = canvas3d->getScene();

    controller->setCSectionScene( csection_scene );
    controller->setTopViewScene( topview_scene );
    controller->setScene3d( scene3d );
    controller->setObjectTree( object_tree );

    controller->init();




}

void MainWindow_Refactored::setupInterface()
{
    double width = controller->getVolumeWidth();
    double height = controller->getVolumeHeight();
    double depth = controller->getVolumeDepth();

    pages_sidebar->changeRangeSize( 2*width, 2*height, 2*depth );
    emit resizedVolume( width, height, depth );
}







void MainWindow_Refactored::setDefaultValues()
{
    ac_show_sidebar->setChecked( SIDEBAR_VISIBLE );
    ac_show_topview->setChecked( TOPVIEW_VISIBLE );

    dw_topview->setVisible( TOPVIEW_VISIBLE );
    dw_csection->setVisible( CSECTION_VISIBLE );

//    pages_sidebar->setDefaultValues();
//    sketch_window->setDefaultValues();
//    topview_window->setDefaultValues();

}