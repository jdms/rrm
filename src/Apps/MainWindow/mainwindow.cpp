#include "mainwindow.h"
#include "3dView/canvas3d.h"
#include "controller.h"
#include "Sketching/sketch_window.h"
#include "Widgets/pages_stack.h"
#include "Widgets/real_slider.h"
#include "Object_Tree/object_tree.h"
#include "Sketching/csection_scene.h"
#include "Sketching/topview_scene.h"
#include "About/about_widget.hpp"

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
    setDefaultValues();

    show();

    createController();
    setupController();
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
    createCrossSectionInterface();
    createTopViewInterface();
    createToolbarActions();
    createMenuBar();


}


void MainWindow::createMainInterface()
{

    canvas3d = new Canvas3d();

    sl_depth_csection = new RealSlider( Qt::Vertical );
    sl_depth_csection->setTickPosition( QSlider::TicksRight );
    sl_depth_csection->setInvertedAppearance( true );


    connect( sl_depth_csection, &RealSlider::sliderMoved, [=]( const double& v )
                                      { controller->setCurrentCrossSection( v ); } );


    connect( this, &MainWindow::resizedVolume, [=]( double w, double h, double d )
                                            { sl_depth_csection->setRange( 0, d );
                                              sl_depth_csection->setValue( d ); } );


    connect( canvas3d, &Canvas3d::increaseSlider, sl_depth_csection,
             &RealSlider::increaseValue );
    connect( canvas3d, &Canvas3d::decreaseSlider, sl_depth_csection,
             &RealSlider::decreaseValue );

    hb_central_widget = new QHBoxLayout( this );
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );

    central_widget = new QWidget( this );
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

    aboutRRM = new AboutWidget( this );
}


void MainWindow::createToolbarActions()
{

    ac_show_sidebar = new QAction( "Show Sidebar", this );
    ac_show_sidebar->setCheckable( true );

    connect( ac_show_sidebar, &QAction::toggled, dw_object_tree, &QDockWidget::setVisible );
    connect( ac_show_sidebar, &QAction::toggled, dw_info_objects, &QDockWidget::setVisible );


    connect( dw_object_tree, &QDockWidget::visibilityChanged, [=]( bool status )
    {
        if( ( status == false ) && ( dw_info_objects->isVisible() == false ) )
            ac_show_sidebar->setChecked( false );
    } );

    connect( dw_info_objects, &QDockWidget::visibilityChanged, [=]( bool status )
    {
        if( ( status == false ) && ( dw_object_tree->isVisible() == false ) )
            ac_show_sidebar->setChecked( false );
    } );



    ac_new = new QAction( "New", this );
    connect( ac_new, &QAction::triggered, this, &MainWindow::clear );

    ac_save_file = new QAction( "Save", this );
    connect( ac_save_file, &QAction::triggered, this, &MainWindow::saveFile );
    ac_load_file = new QAction( "Load", this );
    connect( ac_load_file, &QAction::triggered, this, &MainWindow::loadFile );


    ac_undo = new QAction( "Undo", this );
    connect( ac_undo, &QAction::triggered, this, &MainWindow::undo );

    ac_redo = new QAction( "Redo", this );
    connect( ac_redo, &QAction::triggered, this, &MainWindow::redo );


    QToolBar* tb_general = new QToolBar( this );
    tb_general->addAction( ac_show_sidebar );
    tb_general->addSeparator();
    tb_general->addAction( ac_new );
    tb_general->addAction( ac_save_file );
    tb_general->addAction( ac_load_file );
    tb_general->addSeparator();
    tb_general->addAction( ac_undo );
    tb_general->addAction( ac_redo );
    addToolBar( tb_general );



    ac_show_topview = new QAction( "Show Top-View", this );
    ac_show_topview->setCheckable( true );
    connect( ac_show_topview, &QAction::toggled, dw_topview, &QDockWidget::setVisible );
    connect( dw_topview, &QDockWidget::visibilityChanged, ac_show_topview, &QAction::setChecked );


    ac_sketch_above = new QAction( "SA", this );
    ac_sketch_above->setCheckable( true );
    connect( ac_sketch_above, &QAction::triggered, [=]( bool status ){
          controller->enableCreateAbove( status ); } );

    ac_sketch_below = new QAction( "SB", this );
    ac_sketch_below->setCheckable( true );
    connect( ac_sketch_below, &QAction::triggered, [=]( bool status ){
          controller->enableCreateBelow( status ); } );


    QActionGroup* ag_region_sketching = new QActionGroup( this );
    ag_region_sketching->addAction( ac_sketch_above );
    ag_region_sketching->addAction( ac_sketch_below );
    ag_region_sketching->setExclusive( false );


    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );
    connect( ac_remove_above, &QAction::triggered, [=](){
          controller->setCurrentRule( Controller::StratigraphicRules::REMOVE_ABOVE );} );

    ac_ra_intersection = new QAction( "RAI", this );
    ac_ra_intersection->setCheckable( true );
    connect( ac_ra_intersection, &QAction::triggered, [=](){
          controller->setCurrentRule( Controller::StratigraphicRules::REMOVE_ABOVE_INTERSECTION );} );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );
    connect( ac_remove_below, &QAction::triggered, [=](){
          controller->setCurrentRule( Controller::StratigraphicRules::REMOVE_BELOW );} );

    ac_rb_intersection = new QAction( "RBI", this );
    ac_rb_intersection->setCheckable( true );
    connect( ac_rb_intersection, &QAction::triggered, [=](){
          controller->setCurrentRule( Controller::StratigraphicRules::REMOVE_BELOW_INTERSECTION );} );



    QAction* ac_show_simulator = new QAction( "Simulator", this );
    ac_show_simulator->setCheckable( true );
    connect( ac_show_simulator, &QAction::toggled, this, &MainWindow::setSimulationMode );



    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_ra_intersection );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_rb_intersection );    
    ag_rules->setExclusive( true );


    QToolBar* tb_app_rrm = new QToolBar( this );
    tb_app_rrm->addActions( ag_region_sketching->actions() );
    tb_app_rrm->addSeparator();
    tb_app_rrm->addActions( ag_rules->actions() );
    tb_app_rrm->addSeparator();
    tb_app_rrm->addAction( ac_show_topview );
    tb_app_rrm->addAction( ac_show_simulator );
    tb_app_rrm->addSeparator();



    addToolBar( tb_app_rrm );


    QAction* ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, canvas3d, &Canvas3d::screenshot );

    QToolBar* tb_misc = new QToolBar( this );
    tb_misc->addAction( ac_screenshot );
    addToolBar( tb_misc );



    ac_show_sidebar->setIcon( QIcon( ":/images/icons/sidebar.png" ) );
    ac_new->setIcon( QIcon( ":/images/icons/new.png" ) );
    ac_save_file->setIcon( QIcon( ":/images/icons/save.png" ) );
    ac_load_file->setIcon( QIcon( ":/images/icons/load.png" ) );


    ac_undo->setIcon( QIcon( ":/images/icons/undo.png" ) );
    ac_redo->setIcon( QIcon( ":/images/icons/redo.png" ) );


    ac_sketch_above->setIcon( QIcon( ":/images/icons/sketchabove.png" ) );
    ac_sketch_below->setIcon( QIcon( ":/images/icons/sketchbelow.png" ) );

    ac_remove_above->setIcon( QIcon( ":/images/icons/removeabove.png" ) );
    ac_ra_intersection->setIcon( QIcon( ":/images/icons/removeaboveintersecion.png" ) );
    ac_remove_below->setIcon( QIcon( ":/images/icons/removebelow.png" ) );
    ac_rb_intersection->setIcon( QIcon( ":/images/icons/removebelowintersecion.png" ) );
    ac_show_topview->setIcon( QIcon( ":/images/icons/topview.png" ) );
    ac_screenshot->setIcon( QIcon( ":/images/icons/Camera.png" ) );

}


void MainWindow::createMenuBar()
{



    QAction* ac_exit = new QAction( tr ( "E&xit" ) , this );
    ac_exit->setIcon( QIcon( ":/images/icons/door_out.png" ) );
    QAction* ac_manual = new QAction( tr ( "RRM Manual" ), this );
    QAction* ac_about = new QAction( tr ( "&About" ) , this );

    connect( ac_about, &QAction::triggered, aboutRRM, &AboutWidget::show );
    connect( ac_manual, &QAction::triggered, this, &MainWindow::showHelp );
    connect( ac_exit, &QAction::triggered , this, &MainWindow::close );



    QAction* ac_csection = new QAction ( tr ( "Cross-Section Window" ) , this );
    ac_csection->setCheckable ( true );

    connect( ac_csection, &QAction::toggled, dw_csection , &QDockWidget::setVisible );
    connect( dw_csection, &QDockWidget::visibilityChanged, ac_csection, &QAction::setChecked );



    QAction* ac_topview = new QAction ( tr ( "Top-View Window" ) , this );
    ac_topview->setCheckable ( true );

    connect( ac_topview , &QAction::toggled, dw_topview , &QDockWidget::setVisible );
    connect( dw_topview, &QDockWidget::visibilityChanged, ac_topview, &QAction::setChecked );



    QAction* ac_sidebar = new QAction ( tr ( "Sidebar" ) , this );
    ac_sidebar->setCheckable ( true );
    connect ( ac_sidebar , &QAction::toggled , ac_show_sidebar , &QAction::setChecked );
    connect ( ac_show_sidebar, &QAction::toggled , ac_sidebar , &QAction::setChecked );



    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_new );
    mn_file->addAction ( ac_save_file );
    mn_file->addAction ( ac_load_file );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar()->addMenu ( tr ( "&View" ) );
    mn_windows->addAction ( ac_csection );
    mn_windows->addAction ( ac_topview );
    mn_windows->addAction ( ac_sidebar );

    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
    mn_help->addAction( ac_manual );
    mn_help->addAction( ac_about );

}




void MainWindow::createCrossSectionInterface()
{
    sketch_window = new SketchWindow( this );

    dw_csection = new QDockWidget( "Cross-Section" );
    dw_csection->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_csection->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_csection );


    connect( sketch_window, &SketchWindow::increaseSlider, sl_depth_csection,
             &RealSlider::increaseValue );
    connect( sketch_window, &SketchWindow::decreaseSlider, sl_depth_csection,
             &RealSlider::decreaseValue );
}


void MainWindow::createTopViewInterface()
{
    topview_window = new SketchWindow( this, false );

    dw_topview = new QDockWidget( "Top View" );
    dw_topview->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_topview->setWidget( topview_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_topview );

    connect( topview_window, &SketchWindow::increaseSlider, sl_depth_csection,
             &RealSlider::increaseValue );
    connect( topview_window, &SketchWindow::decreaseSlider, sl_depth_csection,
             &RealSlider::decreaseValue );
}


void MainWindow::createSidebar()
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

    connect( this, &MainWindow::resizedVolume, pages_sidebar,
             &PagesStack::changeVolumeSize );

}




void MainWindow::createController()
{
    if( controller != nullptr )
        delete controller;

    controller = new Controller();
}


void MainWindow::setupController()
{

    CSectionScene* csection_scene = ( CSectionScene* ) ( sketch_window->getScene() );
    TopViewScene* topview_scene = ( TopViewScene* ) ( topview_window->getScene() );
    Scene3d* scene3d = canvas3d->getScene();


    controller->setCSectionScene( csection_scene );
    controller->setTopViewScene( topview_scene );
    controller->setScene3d( scene3d );
    controller->setObjectTree( object_tree );




    connect( object_tree, &ObjectTree::setInputVolumeVisible, [=]( bool status )
                                              { controller->setVolumeVisibility( status ); } );

    connect( object_tree, &ObjectTree::setObjectVisible, [=]( std::size_t id, bool status )
                                              { controller->setObjectVisibility( id, status ); } );

    connect( object_tree, &ObjectTree::setObjectName, [=]( std::size_t id, const QString& n )
                                              { controller->setObjectName( id, n.toStdString() ); } );


    connect( object_tree, &ObjectTree::setObjectColor, [=](  std::size_t id_, const QColor& c ){
                                    controller->setObjectColor( id_, c.red(), c.green(), c.blue() );
                                 } );


    connect( sketch_window, &SketchWindow::createSurface, [=]() {
                                     bool status = controller->createObjectSurface();
                                     if( status == true )
                                        emit ( csection_scene->setUpColor() );
                                     checkUndoRedo(); } );


    connect( sketch_window, &SketchWindow::defineColorCurrent, [=]( const QColor& c ) {
                                     controller->setCurrentColor( c.red(), c.green(), c.blue() );
                                 } );

    connect( csection_scene, &CSectionScene::updateVolumeDimensions, [=]( double w, double h ){
                                    resizingVolumeWidth( w );
                                    resizingVolumeHeight( h );
                                } );


    connect( csection_scene, &CSectionScene::addCurveToObject, [=](  const Curve2D& curve ){
                                                controller->addCurveToObject( curve );
                                                bool status = controller->isVolumeResizable();
                                                setVolumeResizingEnabled( status ); } );


    connect( csection_scene, &CSectionScene::removeCurveFromObject, [=]( double depth ){
                                                controller->removeCurveFromObject( depth );
                                                bool status = controller->isVolumeResizable();
                                                setVolumeResizingEnabled( status );
    } );

    connect( csection_scene, &CSectionScene::createSurface, [=](){
                                    bool status = controller->createObjectSurface();
                                    if( status == true )
                                       emit ( csection_scene->setUpColor() );
                                    checkUndoRedo();  } );


    connect( csection_scene, &CSectionScene::selectedObject, [=]( std::size_t id ){
                                                controller->defineObjectSelected( id ); } );




    connect( topview_window, &SketchWindow::createSurface, [=]() {
                                     bool status = controller->createObjectSurface();
                                     if( status == true )
                                        emit ( csection_scene->setUpColor() );
                                     checkUndoRedo();  } );


    connect( topview_scene, &TopViewScene::addCurveToObject, [=](  const Curve2D& curve ){
                                                controller->addTrajectoryToObject( curve );
                                                bool status = controller->isVolumeResizable();
                                                setVolumeResizingEnabled( status ); } );


    connect( topview_scene, &TopViewScene::updateVolumeDimensions, [=]( double w, double d ){
                                            resizingVolumeWidth( w );
                                            resizingVolumeDepth( d );
                                        } );

    connect( topview_scene, &TopViewScene::removeTrajectory, [=](){
                                                controller->removeTrajectoryFromObject();
                                                bool status = controller->isVolumeResizable();
                                                setVolumeResizingEnabled( status ); } );

    connect( topview_scene, &CSectionScene::createSurface, [=](){
                                                controller->createObjectSurface();
                                                checkUndoRedo();
                                                emit ( csection_scene->setUpColor() );  } );



    connect( pages_sidebar, &PagesStack::widthVolumeChanged, [=]( int width ){
                                                resizingVolumeWidth( width ); } );

    connect( pages_sidebar, &PagesStack::heightVolumeChanged, [=]( int height ){
                                                resizingVolumeHeight( height ); } );

    connect( pages_sidebar, &PagesStack::depthVolumeChanged, [=]( int depth ){
                                                resizingVolumeDepth( depth ); } );


}



void MainWindow::setSimulationMode( bool status )
{
    if( status == true )
    {

        canvas3d->setVisible( false );
        ac_show_sidebar->setChecked( false );
        ac_show_topview->setChecked( false );
        hb_central_widget->insertWidget( 0, sketch_window );
        dw_csection->setVisible( false );

        return;
    }

    canvas3d->setVisible( true );
    ac_show_sidebar->setChecked( true );
    ac_show_topview->setChecked( true );
    hb_central_widget->removeWidget( sketch_window );

    dw_csection->setVisible( true );
    dw_csection->setWidget( sketch_window );

}


void MainWindow::run_app()
{
    controller->setCurrentColor( 255, 0, 0 );
    controller->init();

    checkUndoRedo();
    loadDefaultValues();

}



void MainWindow::setVolumeResizingEnabled( bool status )
{
    pages_sidebar->setEnabledVolumeResize( status );
    sketch_window->setEnabledVolumeResize( status );
}


void MainWindow::resizingVolumeWidth( double w )
{
    controller->setVolumeWidth( w );
    pages_sidebar->setVolumeWidth( w );

    double height = controller->getVolumeHeight();
    double depth = controller->getVolumeDepth();

    emit resizedVolume( w, height, depth );
}


void MainWindow::resizingVolumeHeight( double h )
{
    controller->setVolumeHeight( h );
    pages_sidebar->setVolumeHeight( h );

    double width = controller->getVolumeWidth();
    double depth = controller->getVolumeDepth();

    emit resizedVolume( width, h, depth );

}


void MainWindow::resizingVolumeDepth( double d )
{
    controller->setVolumeDepth( d );
    pages_sidebar->setVolumeDepth( d );

    double width = controller->getVolumeWidth();
    double height = controller->getVolumeHeight();

    emit resizedVolume( width, height, d );

    int disc = static_cast< int >( controller->setupCrossSectionDiscretization() );
    sl_depth_csection->setDiscretization( disc );
    sl_depth_csection->setRange( 0, d );


    bool status = controller->isVolumeResizable();
    setVolumeResizingEnabled( status );
}




void MainWindow::clear()
{
    controller->clear();
    clearInterface();

    run_app();
    loadDefaultValues();
}



void MainWindow::saveFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );


    if( filename.isEmpty() == true ) return;
    controller->saveFile( filename.toStdString() );

}


void MainWindow::loadFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );

    if( filename.isEmpty() == true ) return;

    clear();
    controller->loadFile( filename.toStdString() );
    loadDefaultValues();
    checkUndoRedo();
}



void MainWindow::undo()
{
    controller->undo();
    checkUndoRedo();
}


void MainWindow::redo()
{
    controller->redo();
    checkUndoRedo();
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
}


void MainWindow::loadDefaultValues()
{
    loadVolumeDimensions();
    loadDefaultRule();
    checkUndoRedo();
}


void MainWindow::loadVolumeDimensions()
{

    if( controller == nullptr ) return;

    double width = controller->getVolumeWidth();
    double height = controller->getVolumeHeight();
    double depth = controller->getVolumeDepth();

    emit resizedVolume( width, height, depth );

    int disc = static_cast< int >( controller->setupCrossSectionDiscretization() );
    sl_depth_csection->setDiscretization( disc );
    sl_depth_csection->setRange( 0, depth );


    bool status = controller->isVolumeResizable();
    setVolumeResizingEnabled( status );
}


void MainWindow::loadDefaultRule()
{

    Controller::StratigraphicRules rule = controller->getCurrentRule();

    if( rule == Controller::StratigraphicRules::REMOVE_ABOVE )
    {
        ac_remove_above->setChecked( true );
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
    {
        ac_ra_intersection->setChecked( true );
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_BELOW )
    {
        ac_remove_below->setChecked( true );
    }
    else if( rule == Controller::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
    {
        ac_rb_intersection->setChecked( true );
    }
}





void MainWindow::clearInterface()
{
    clearMenu();
    clearWindows();
}


void MainWindow::clearMenu()
{
    ac_sketch_above->setEnabled( CREATE_REGIONS_ALLOWED );
    ac_sketch_below->setEnabled( CREATE_REGIONS_ALLOWED );

    ac_sketch_above->setChecked( CREATE_REGIONS_DEFINED );
    ac_sketch_below->setChecked( CREATE_REGIONS_DEFINED );

    ac_remove_above->setChecked( NO_RULE_CHECKED );
    ac_ra_intersection->setChecked( NO_RULE_CHECKED );
    ac_remove_below->setChecked( NO_RULE_CHECKED );
    ac_rb_intersection->setChecked( NO_RULE_CHECKED );
}


void MainWindow::clearWindows()
{
    sketch_window->clear();
    topview_window->clear();
    pages_sidebar->clear();
}




void MainWindow::setDefaultValues()
{
    ac_show_sidebar->setChecked( SIDEBAR_VISIBLE );
    ac_show_topview->setChecked( TOPVIEW_VISIBLE );

    dw_topview->setVisible( TOPVIEW_VISIBLE );
    dw_csection->setVisible( CSECTION_VISIBLE );


}


void MainWindow::showHelp()
{
    QDir dir;
    QFileInfo file_info(dir.currentPath() + "/manual/rrm-manual.pdf");
    QDesktopServices::openUrl(QUrl::fromLocalFile(file_info.absoluteFilePath()));
}
