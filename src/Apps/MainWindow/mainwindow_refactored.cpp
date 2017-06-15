#include "mainwindow_refactored.h"
#include "3dView/canvas3d_refactored.h"
#include "controller_refactored.h"
#include "Sketching/sketchwindow.h"
#include "Widgets/pagesstack.h"
#include "Widgets/realslider.h"
#include "Object_Tree/ObjectTree.h"
#include "Sketching/csectionscene.h"
#include "Sketching/topviewscene.h"

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
    run_app();

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

    sl_depth_csection = new RealSlider( Qt::Vertical );
    sl_depth_csection->setTickPosition( QSlider::TicksRight );
    sl_depth_csection->setInvertedAppearance( true );


    connect( sl_depth_csection, &RealSlider::sliderMoved, [=]( const double& v )
                                      { controller->setCurrentCrossSection( v ); } );


    connect( this, &MainWindow_Refactored::resizedVolume, [=]( double w, double h, double d )
                                            { sl_depth_csection->setRange( 0, d );
                                              sl_depth_csection->setValue( d ); } );


    connect( canvas3d, &Canvas3d_Refactored::increaseSlider, sl_depth_csection,
             &RealSlider::increaseValue );
    connect( canvas3d, &Canvas3d_Refactored::decreaseSlider, sl_depth_csection,
             &RealSlider::decreaseValue );

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
    connect( ac_new, &QAction::triggered, this, &MainWindow_Refactored::clear );

    QAction* ac_save_file = new QAction( "Save", this );
    connect( ac_save_file, &QAction::triggered, this, &MainWindow_Refactored::saveFile );
    QAction* ac_load_file = new QAction( "Load", this );
    connect( ac_load_file, &QAction::triggered, this, &MainWindow_Refactored::loadFile );


    ac_undo = new QAction( "Undo", this );
    connect( ac_undo, &QAction::triggered, this, &MainWindow_Refactored::undo );

    ac_redo = new QAction( "Redo", this );
    connect( ac_redo, &QAction::triggered, this, &MainWindow_Refactored::redo );


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


    ac_sketch_above = new QAction( "SA", this );
    ac_sketch_above->setCheckable( true );
    connect( ac_sketch_above, &QAction::toggled, [=]( bool status ){
          controller->enableCreateAbove( status ); } );

    ac_sketch_below = new QAction( "SB", this );
    ac_sketch_below->setCheckable( true );
    connect( ac_sketch_below, &QAction::toggled, [=]( bool status ){
          controller->enableCreateBelow( status ); } );


    QActionGroup* ag_region_sketching = new QActionGroup( this );
    ag_region_sketching->addAction( ac_sketch_above );
    ag_region_sketching->addAction( ac_sketch_below );
    ag_region_sketching->setExclusive( false );


    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );
    connect( ac_remove_above, &QAction::toggled, [=](){
          controller->setCurrentRule( Controller_Refactored::StratigraphicRules::REMOVE_ABOVE );} );

    ac_ra_intersection = new QAction( "RAI", this );
    ac_ra_intersection->setCheckable( true );
    connect( ac_ra_intersection, &QAction::toggled, [=](){
          controller->setCurrentRule( Controller_Refactored::StratigraphicRules::REMOVE_ABOVE_INTERSECTION );} );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );
    connect( ac_remove_below, &QAction::toggled, [=](){
          controller->setCurrentRule( Controller_Refactored::StratigraphicRules::REMOVE_BELOW );} );

    ac_rb_intersection = new QAction( "RBI", this );
    ac_rb_intersection->setCheckable( true );
    connect( ac_rb_intersection, &QAction::toggled, [=](){
          controller->setCurrentRule( Controller_Refactored::StratigraphicRules::REMOVE_BELOW_INTERSECTION );} );


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
    connect( ac_screenshot, &QAction::triggered, canvas3d, &Canvas3d_Refactored::screenshot );

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


    connect( sketch_window, &SketchWindow::increaseSlider, sl_depth_csection,
             &RealSlider::increaseValue );
    connect( sketch_window, &SketchWindow::decreaseSlider, sl_depth_csection,
             &RealSlider::decreaseValue );
}


void MainWindow_Refactored::createTopViewInterface()
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
                                     controller->createObjectSurface();
                                     checkUndoRedo();
                                     emit ( csection_scene->setUpColor() );
                                 } );


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
                                                controller->createObjectSurface();                                                
                                                checkUndoRedo();
                                                emit ( csection_scene->setUpColor() );  } );


    connect( csection_scene, &CSectionScene::selectedObject, [=]( std::size_t id ){
                                                controller->defineObjectSelected( id ); } );




    connect( topview_window, &SketchWindow::createSurface, [=]() {
                                     controller->createObjectSurface();
                                     checkUndoRedo();
                                     emit ( csection_scene->setUpColor() );
                                 } );


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
                                                resizingVolumeWidth( width );
                                                /*controller->setVolumeWidth( width );*/ } );

    connect( pages_sidebar, &PagesStack::heightVolumeChanged, [=]( int height ){
                                                resizingVolumeHeight( height );
                                                /*controller->setVolumeHeight( height );*/ } );

    connect( pages_sidebar, &PagesStack::depthVolumeChanged, [=]( int depth ){
                                                resizingVolumeDepth( depth );
                                                /*controller->setVolumeDepth( depth );*/ } );


}



void MainWindow_Refactored::run_app()
{
    controller->setCurrentColor( 255, 0, 0 );
    controller->init();

    checkUndoRedo();
    loadDefaultValues();
}



void MainWindow_Refactored::setVolumeResizingEnabled( bool status )
{
    pages_sidebar->setEnabledVolumeResize( status );
    sketch_window->setEnabledVolumeResize( status );
}


void MainWindow_Refactored::resizingVolumeWidth( double w )
{
    controller->setVolumeWidth( w );
    pages_sidebar->setVolumeWidth( w );

    double height = controller->getVolumeHeight();
    double depth = controller->getVolumeDepth();

    emit resizedVolume( w, height, depth );
}


void MainWindow_Refactored::resizingVolumeHeight( double h )
{
    controller->setVolumeHeight( h );
    pages_sidebar->setVolumeHeight( h );

    double width = controller->getVolumeWidth();
    double depth = controller->getVolumeDepth();

    emit resizedVolume( width, h, depth );

}


void MainWindow_Refactored::resizingVolumeDepth( double d )
{
    controller->setVolumeDepth( d );
    pages_sidebar->setVolumeDepth( d );

    double width = controller->getVolumeWidth();
    double height = controller->getVolumeHeight();

    emit resizedVolume( width, height, d );
}




void MainWindow_Refactored::clear()
{
    controller->clear();
    clearInterface();

    run_app();
    loadDefaultValues();
}



void MainWindow_Refactored::saveFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );


    if( filename.isEmpty() == true ) return;
    controller->saveFile( filename.toStdString() );

}


void MainWindow_Refactored::loadFile()
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



void MainWindow_Refactored::undo()
{
    controller->undo();
    checkUndoRedo();
}


void MainWindow_Refactored::redo()
{
    controller->redo();
    checkUndoRedo();
}



void MainWindow_Refactored::checkUndoRedo()
{
    if( controller == nullptr ) return;

    ac_undo->setEnabled( controller->canUndo() );
    ac_redo->setEnabled( controller->canRedo() );
}




void MainWindow_Refactored::loadDefaultValues()
{
    loadVolumeDimensions();
    loadDefaultRule();
    checkUndoRedo();
}


void MainWindow_Refactored::loadVolumeDimensions()
{
    double width = controller->getVolumeWidth();
    double height = controller->getVolumeHeight();
    double depth = controller->getVolumeDepth();

    pages_sidebar->changeRangeSize( 2*width, 2*height, 2*depth );
    emit resizedVolume( width, height, depth );

    int disc = static_cast< int >( controller->setupCrossSectionDiscretization() );
    sl_depth_csection->setDiscretization( disc );
    sl_depth_csection->setRange( 0, depth );


    bool status = controller->isVolumeResizable();
    setVolumeResizingEnabled( status );
}


void MainWindow_Refactored::loadDefaultRule()
{

    Controller_Refactored::StratigraphicRules rule = controller->getCurrentRule();

    if( rule == Controller_Refactored::StratigraphicRules::REMOVE_ABOVE )
    {
        ac_remove_above->setChecked( true );
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
    {
        ac_ra_intersection->setChecked( true );
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_BELOW )
    {
        ac_remove_below->setChecked( true );
    }
    else if( rule == Controller_Refactored::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
    {
        ac_rb_intersection->setChecked( true );
    }
}






void MainWindow_Refactored::clearInterface()
{
    clearMenu();
    clearWindows();
}


void MainWindow_Refactored::clearMenu()
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


void MainWindow_Refactored::clearWindows()
{
    sketch_window->clear();
    topview_window->clear();
    pages_sidebar->clear();
}




void MainWindow_Refactored::setDefaultValues()
{
    ac_show_sidebar->setChecked( SIDEBAR_VISIBLE );
    ac_show_topview->setChecked( TOPVIEW_VISIBLE );

    dw_topview->setVisible( TOPVIEW_VISIBLE );
    dw_csection->setVisible( CSECTION_VISIBLE );

}
