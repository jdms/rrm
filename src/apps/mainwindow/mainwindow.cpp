
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

    show();

//    createController();
//    setupController();
    run_app();
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
    createToolbar();

    createActions();

    createSketchingWindow();
    createSketchingActions();

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


    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );

    ac_remove_above_int = new QAction( "RAI", this );
    ac_remove_above_int->setCheckable( true );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );

    ac_remove_below_int = new QAction( "RBI", this );
    ac_remove_below_int->setCheckable( true );


    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->setExclusive( true );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_remove_above_int );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_remove_below_int );


    QAction* ac_output_volume = new QAction( "Get Regions", this );

    tb_mainwindow = addToolBar( "Test ");
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




void MainWindow::createActions()
{

    createMainWindowActions();
    createSidebarActions();

}



void MainWindow::createMainWindowActions()
{

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( double d_ ){ app->setCurrentCrossSection( d_ ); } );

    connect( ac_remove_above, &QAction::triggered, [=](){ app->setStratigraphicRule( RRMApplication::StratigraphicRules::REMOVE_ABOVE ); } );

    connect( ac_remove_above_int, &QAction::triggered, [=](){ app->setStratigraphicRule( RRMApplication::StratigraphicRules::REMOVE_ABOVE_INTERSECTION ); } );

    connect( ac_remove_below, &QAction::triggered, [=](){ app->setStratigraphicRule( RRMApplication::StratigraphicRules::REMOVE_BELOW ); } );

    connect( ac_remove_below_int, &QAction::triggered, [=](){ app->setStratigraphicRule( RRMApplication::StratigraphicRules::REMOVE_BELOW_INTERSECTION ); } );

}



void MainWindow::createSidebarActions()
{

    connect( object_tree, &ObjectTree::setVolumeName, [=]( std::size_t index_, const std::string& name_ ){ app->setVolumeName( index_, name_ ); } );

    connect( object_tree, &ObjectTree::setVolumeVisible, [=]( std::size_t index_, bool status_ ){ app->setVolumeVisible( index_, status_ ); } );


    connect( object_tree, &ObjectTree::setObjectName, [=]( std::size_t index_, const std::string& name_ ){ app->setObjectName( index_, name_ ); } );

    connect( object_tree, &ObjectTree::setObjectVisible, [=]( std::size_t index_, bool status_ ){ app->setObjectVisible( index_, status_ ); } );

    connect( object_tree, &ObjectTree::setObjectColor, [=]( std::size_t index_, const QColor& color_ )
                                                       { app->setObjectColor( index_, color_.red(), color_.green(), color_.blue() ); } );

    connect( object_tree, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_, int column_ ) { app->getObjectInformation( item_ ); } );



    connect( object_properties, &PagesStack::widthVolumeChanged, [=]( double w_ )
                                                                 { app->changeVolumeDimension( RRMApplication::AxesDirection::X, w_ ); } );
    connect( object_properties, &PagesStack::heightVolumeChanged, [=]( double h_ )
                                                                 { app->changeVolumeDimension( RRMApplication::AxesDirection::Y, h_ ); } );
    connect( object_properties, &PagesStack::depthVolumeChanged, [=]( double l_ )
                                                                 { app->changeVolumeDimension( RRMApplication::AxesDirection::Z, l_ ); } );


    connect( object_properties, &PagesStack::saveText, [=]( const QString& text_ ){ app->saveObjectInformation( text_.toStdString() ); } );


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


void MainWindow::createSketchingActions()
{

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=](){ app->updateSketchingCanvas(); } );

    connect( sketch_window, &SketchWindow::updateVolume, [=]( CrossSection::Direction dir_, double w_, double h_ )
                                                         { app->changeVolumeDimensions( dir_, w_, h_ ); } );

    connect( sketch_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_ )
                                                        { app->acceptSketchingCurve( curve_ ); } );

    connect( sketch_window, &SketchWindow::commitObject, [=](){ app->createObjectSurface(); } );




    connect( object_properties, &PagesStack::widthVolumeChanged, [=]( double w_ )
                                                                 { app->updateSketchingCanvas(); } );
    connect( object_properties, &PagesStack::heightVolumeChanged, [=]( double h_ )
                                                                 {  app->updateSketchingCanvas(); } );
    connect( object_properties, &PagesStack::depthVolumeChanged, [=]( double l_ )
                                                                 {  app->updateSketchingCanvas(); } );




    connect( sketch_topview_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_ )
                                                        { app->acceptSketchingTrajectory( curve_ ); } );

    connect( sketch_topview_window, &SketchWindow::commitObject, [=](){ app->createObjectSurface(); } );

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

    app = new RRMApplication( this );
    app->init();

    app->initSketchingApp();

}


void MainWindow::checkUndoRedo()
{
//    if( controller == nullptr ) return;

//    ac_undo->setEnabled( controller->canUndo() );
//    ac_redo->setEnabled( controller->canRedo() );

//    checkSketchStatus();
}


void MainWindow::checkSketchStatus()
{
//    if( controller == nullptr ) return;

//    ac_sketch_above->setChecked( controller->isDefineAboveActive() );
//    ac_sketch_below->setChecked( controller->isDefineBelowActive() );
//    emit updateObjects();
}


void MainWindow::clear()
{

//    sketch_window->clear();
//    sketch_topview_window->clear();
//    controller->clear();
//    sl_depth_csection->clear();

//    emit resetMenus();
//    emit resetWindows();


//    run_app();

}


