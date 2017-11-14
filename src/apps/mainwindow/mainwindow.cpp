#include "mainwindow.h"
#include "3dview/canvas3d.h"
#include "sketching/sketchwindow.h"
#include "widgets/realfeaturedslider.h"
#include "widgets/objecttree.h"
#include "widgets/pages_stack.h"


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
    createBottombar();
    createToolbar();
    createSketchingWindow();
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


    connect( this, &MainWindow::updateVolume, [=](){ double width_ = 0, height_ = 0, length_ = 0;
                                                     controller->getVolumeDimensions( width_, height_, length_ );
                                                     sl_depth_csection->setRange( 0, length_ ); } );

    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( const double& value_ )
                                                                  { controller->setCurrentCrossSection( value_ ); } );

}



void MainWindow::createToolbar()
{

    ac_clear = new QAction( "New", this );
    connect( ac_clear, &QAction::triggered, this, &MainWindow::clear );

    QAction* ac_save = new QAction( "Save", this );
    QAction* ac_load = new QAction( "Load", this );

    ac_undo = new QAction( "Undo", this );
    connect( ac_undo, &QAction::triggered, [=](){ controller->undo(); checkUndoRedo(); } );

    ac_redo = new QAction( "Redo", this );
    connect( ac_redo, &QAction::triggered, [=](){ controller->redo(); checkUndoRedo(); } );


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


    connect( ag_rules, &QActionGroup::triggered, [=]( QAction* action_ ) {
                                                 if( action_ == ac_remove_above )
                                                     controller->setRemoveAbove();
                                                 else if( action_ == ac_remove_above_int )
                                                     controller->setRemoveAboveIntersection();
                                                 else if( action_ == ac_remove_below )
                                                     controller->setRemoveBelow();
                                                 else if( action_ == ac_remove_below_int )
                                                     controller->setRemoveBelowIntersection();
    } );


    connect( this, &MainWindow::resetMenus, [=](){ ac_undo->setEnabled( false ); ac_redo->setEnabled( false );
                                                   ac_remove_above->setChecked( true );  ac_sketch_above->setCheckable( false );
                                                   ac_sketch_below->setCheckable( false ); } );



    QAction* ac_output_volume = new QAction( "Get Regions", this );
    connect( ac_output_volume, &QAction::triggered, [=](){ controller->getOutputVolume(); } );

    QToolBar* tb_mainwindow = new QToolBar();
    tb_mainwindow->addAction( ac_clear );
    tb_mainwindow->addAction( ac_save );
    tb_mainwindow->addAction( ac_load );
    tb_mainwindow->addAction( ac_undo );
    tb_mainwindow->addAction( ac_redo );
    tb_mainwindow->addAction( ac_sketch_above );
    tb_mainwindow->addAction( ac_sketch_below );
    tb_mainwindow->addActions( ag_rules->actions() );
    tb_mainwindow->addAction( ac_output_volume );

    addToolBar( tb_mainwindow );


}


void MainWindow::createSidebar()
{

    object_tree = new ObjectTree( this );
    dw_object_tree = new QDockWidget( "" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_tree );

    connect( object_tree, &ObjectTree::setVolumeVisible, [=]( std::size_t index_, bool status_ )
                                                         { controller->setVolumeVisibility( status_ ); } );


    connect( object_tree, &ObjectTree::setObjectVisible, [=]( std::size_t index_, bool status_ )
                                                         { controller->setObjectVisibility( index_, status_ ); } );


    connect( object_tree, &ObjectTree::setObjectName, [=]( std::size_t index_, const std::string& name_ )
                                                       { controller->setObjectName( index_, name_ ); } );




}



void MainWindow::createBottombar()
{

    object_properties = new PagesStack();
    dw_object_properties = new QDockWidget( "" );
    dw_object_properties->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_properties->setWidget( object_properties );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_properties );

    connect( this, &MainWindow::defineVolumeDimensions, object_properties, &PagesStack::changeVolumeSize );

    connect( object_properties, &PagesStack::heightVolumeChanged, [=]( double height_ ){
                                                                      double w = object_properties->getWidth();
                                                                      double d = object_properties->getLenght();
                                                                      controller->setVolumeDimensions( w, height_, d );
                                                                      emit updateVolume(); } );
    connect( object_properties, &PagesStack::widthVolumeChanged, [=]( double width_ ){
                                                                      double h = object_properties->getHeight();
                                                                      double d = object_properties->getLenght();
                                                                      controller->setVolumeDimensions( width_, h, d );
                                                                      emit updateVolume(); } );

    connect( object_properties, &PagesStack::depthVolumeChanged, [=]( double lenght_ ){
                                                                      double w = object_properties->getWidth();
                                                                      double h = object_properties->getHeight();
                                                                      controller->setVolumeDimensions( w, h, lenght_ );
                                                                      emit updateVolume(); } );

    connect( object_tree, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_, int col_ )
    {
        ObjectTreeItem* obj_item_ = static_cast< ObjectTreeItem* >( item_ );
        if( obj_item_->getType() == ObjectTreeItem::Type::VOLUME )
            object_properties->setCurrentIndex( 0 );
        else
            object_properties->setCurrentIndex( 1 );

        std::string text_ = controller->getObjectInformation( obj_item_->getIndex() );
        object_properties->loadObjectInformation( text_ );
    } );


    connect( object_properties, &PagesStack::saveText, [=]( const QString& text_ )
    {
        QList< QTreeWidgetItem* > list = object_tree->selectedItems();
        if (list.empty() == true ) return;
        ObjectTreeItem* item_ = static_cast< ObjectTreeItem* >( list[ 0 ] );
        std::cout << item_->getIndex() << std::endl << std::flush;
        std::size_t index_ = item_->getIndex();
        controller->saveObjectInformation( index_, text_.toStdString() );
    } );



}



void MainWindow::createSketchingWindow()
{

    sketch_window = new SketchWindow();
    dw_sketchwindow = new QDockWidget( "Cross-Section" );
    dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketchwindow->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketchwindow );

    connect( this, &MainWindow::defineMainCrossSection, [=]( double v ){ controller->setMainCrossSection( CrossSection::Direction::Z, v );
                                                                         sketch_window->setMainCanvas( controller->getCrossSection( v ) );
                                                                         sketch_window->setCurrentCrossSection( v );
                                                                         /*controller->addTopViewCrossSection();
                                                                         sketch_topview_window->setMainCanvas( controller->getTopViewCrossSection() );*/ } );



    connect( ac_sketch_below, &QAction::triggered, [=]( bool status_ ) {
                                                 bool enabled_ = controller->enableCreateBelow( status_ );
                                                 if( ( status_ == false ) && ( enabled_ == true ) )
                                                 {
                                                     std::cout << "Stop failed, so keep turned it on" << std::endl << std::flush;
                                                     ac_sketch_below->setChecked( true );
                                                 }
                                                 if( ( status_ == true ) && ( enabled_ == false ) )
                                                 {
                                                     std::cout << "Request failed, so keep turned it off" << std::endl << std::flush;
                                                     ac_sketch_below->setChecked( false );
                                                 }

                                                 if( ( status_ == true ) && ( enabled_ == true ) )
                                                 {
                                                     std::cout << "Request accepted, get the selectable objects" << std::endl << std::flush;
                                                     std::cout << "and mark them as selectable" << std::endl << std::flush;
                                                     sketch_window->setModeSelecting();
                                                     sketch_window->updateCanvas();

                                                 }
                                                 if( ( status_ == false ) && ( enabled_ == false ) )
                                                 {
                                                     std::cout << "Stop accepted, get the selectable objects" << std::endl << std::flush;
                                                     std::cout << "and mark them as non-selectable" << std::endl << std::flush;
                                                     sketch_window->setModeSketching();
                                                     sketch_window->updateCanvas();
                                                 }
    } );



//    sketch_topview_window = new SketchWindow();
//    dw_topview_window = new QDockWidget( "Top-View" );
//    dw_topview_window->setAllowedAreas( Qt::AllDockWidgetAreas );
//    dw_topview_window->setWidget( sketch_topview_window );
//    dw_topview_window->setVisible( true );
//    addDockWidget( Qt::BottomDockWidgetArea, dw_topview_window );


//    connect( this, &MainWindow::updateVolume, sketch_topview_window, &SketchWindow::updateVolumes );
//    connect( this, &MainWindow::addObject, sketch_topview_window, &SketchWindow::addTrajectory );
//    connect( this, &MainWindow::updateObject, sketch_topview_window, &SketchWindow::updateTrajectory );
//    connect( this, &MainWindow::updateObjects, sketch_topview_window, &SketchWindow::updateTrajectories );
//    connect( this, &MainWindow::addCrossSection, sketch_topview_window, &SketchWindow::addCrossSection );


//    connect( sketch_topview_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_ ){ bool status_ = controller->addObjectTrajectory( curve_ );
//                                                                                        if( status_ == false ) return;
//                                                                                        emit updateObjects(); } );


//    connect( sketch_topview_window, &SketchWindow::commitObject, [=](){ bool status_ = controller->createObjectSurface();
//                                                                        if( status_ == false ) return;

//                                                                        controller->addObject();
//                                                                        object_properties->setEnabledVolumeResize( controller->isVolumeResizable() );

//                                                                        emit addObject( controller->getCurrentObject() );
//                                                                        emit setUpColor();

//                                                                        checkUndoRedo(); } );

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
    controller->init();
    setupSlider();


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


