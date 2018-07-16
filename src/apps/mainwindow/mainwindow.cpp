/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "mainwindow.h"

#include <QtWidgets>
#include <QActionGroup>
#include <QSlider>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QResizeEvent>
///@ http://doc.qt.io/qt-5/qdesktopservices.html
#include <QtGui/QDesktopServices>



MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent )
{
    controller = nullptr;

    setupWindowProperties();
    createWindow();

    show();

    run_app();
    run_app_sketch();
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
//    createFlowWindow();

    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_screenshot );

    createMenuBar();
}




void MainWindow::createMainInterface()
{

    canvas3d = new Canvas3d();
//    canvas3d->show();`
    sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
    sl_depth_csection->setDiscretization( 500 );
    sl_depth_csection->setRange( 0, 500 );
    sl_depth_csection->setValue( 500 );



    hb_central_widget = new QHBoxLayout();
    hb_central_widget->addWidget( canvas3d );
    hb_central_widget->addWidget( sl_depth_csection );


    central_widget = new QWidget();
    central_widget->setLayout( hb_central_widget );
    setCentralWidget( central_widget );

}


void MainWindow::createMenuBar()
{

    QAction* ac_exit = new QAction( tr ( "E&xit" ) , this );
    QAction* ac_manual = new QAction( tr ( "RRM Manual" ), this );
    QAction* ac_about = new QAction( tr ( "&About" ) , this );

//    connect( ac_about, &QAction::triggered, aboutRRM, &AboutWidget::show );
    connect( ac_manual, &QAction::triggered, this, &MainWindow::showHelp );
    connect( ac_exit, &QAction::triggered , this, &MainWindow::close );



    QAction* ac_csection = new QAction ( tr ( "Cross-Section Window" ) , this );
    ac_csection->setCheckable ( true );
    ac_csection->setChecked ( Settings::Application::DEFAULT_CSECTION_VISIBILITY );

    connect( ac_csection, &QAction::toggled, dw_sketchwindow , &QDockWidget::setVisible );
    connect( dw_sketchwindow, &QDockWidget::visibilityChanged, [=]( bool status )
    {
        if( ( status == false ) && ( dw_sketchwindow->isVisible() == false ) )
            ac_csection->setChecked( false );
    } );


    QAction* ac_topview = new QAction ( tr ( "Top-View Window" ) , this );
    ac_topview->setCheckable ( true );
    ac_topview->setChecked ( Settings::Application::DEFAULT_TOPVIEW_VISIBILITY );

    connect( ac_topview , &QAction::toggled, dw_topview_window , &QDockWidget::setVisible );
    connect( dw_topview_window, &QDockWidget::visibilityChanged, [=]( bool status )
    {
        if( ( status == false ) && ( dw_topview_window->isVisible() == false ) )
            ac_topview->setChecked( false );
    } );



//    QAction* ac_sidebar = new QAction ( tr ( "Sidebar" ) , this );
//    ac_sidebar->setCheckable ( true );
//    connect ( ac_sidebar , &QAction::toggled , ac_show_sidebar , &QAction::setChecked );
//    connect ( ac_show_sidebar, &QAction::toggled , ac_sidebar , &QAction::setChecked );


    QAction *ac_export = new QAction( "Export", this );
//    connect( ac_export, &QAction::triggered, [=](){ app->exportToIRAP(); } );

    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_clear );
    mn_file->addAction ( ac_save );
    mn_file->addAction ( ac_load );
    mn_file->addSeparator();
    mn_file->addAction( ac_export );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar()->addMenu ( tr ( "&Windows" ) );
    mn_windows->addAction ( ac_csection );
    mn_windows->addAction ( ac_topview );
//    mn_windows->addAction ( ac_sidebar );

    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
    mn_help->addAction( ac_manual );
    mn_help->addAction( ac_about );

}



void MainWindow::createToolbar()
{

    ac_save = new QAction( "Save", this );
    ac_save->setIcon(QIcon(":/images/icons/save.png"));

    ac_load = new QAction( "Load", this );
    ac_load->setIcon(QIcon(":/images/icons/load.png"));

    ac_clear = new QAction( "New", this );
    ac_clear->setIcon(QIcon(":/images/icons/new.png"));

    ac_undo = new QAction( "Undo", this );
    ac_undo->setIcon(QIcon(":/images/icons/undo.png"));

    ac_redo = new QAction( "Redo", this );
    ac_redo->setIcon(QIcon(":/images/icons/redo.png"));

    ac_sketch_above = new QAction( "PA", this ); // create above!
//    ac_sketch_above->setIcon(QIcon(":/images/icons/sketchabove.png"));
    ac_sketch_above->setCheckable( true );

    ac_sketch_below = new QAction( "PB", this ); // create below!
//    ac_sketch_below->setIcon(QIcon(":/images/icons/sketchbelow.png"));
    ac_sketch_below->setCheckable( true );

    ac_truncate = new QAction( "T", this );
//    ac_truncate->setIcon(QIcon(":/images/icons/truncate.png"));
    ac_truncate->setCheckable( true );


    ac_remove_above = new QAction( "RA", this );
//    ac_remove_above->setIcon(QIcon(":/images/icons/removeabove.png"));
    ac_remove_above->setCheckable( true );

    ac_remove_above_int = new QAction( "RAI", this );
//    ac_remove_above_int->setIcon(QIcon(":/images/icons/removeaboveintersecion.png"));
    ac_remove_above_int->setCheckable( true );

    ac_remove_below = new QAction( "RB", this );
//    ac_remove_below->setIcon(QIcon(":/images/icons/removebelow.png"));
    ac_remove_below->setCheckable( true );

    ac_remove_below_int = new QAction( "RBI", this );
//    ac_remove_below_int->setIcon(QIcon(":/images/icons/removebelowintersecion.png"));
    ac_remove_below_int->setCheckable( true );



    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->setExclusive( true );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_remove_above_int );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_remove_below_int );
    ag_rules->addAction( ac_truncate );


    ac_screenshot = new QAction( "Screenshot", this );
    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));

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

//    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, [=]( double d_ ){ app->setCurrentCrossSection( d_ ); } );


//    connect( ac_sketch_above, &QAction::triggered, [=]( bool status_ ){ app->setSketchAbove( status_ ); } );


//    connect( ac_sketch_below, &QAction::triggered, [=]( bool status_ ){ app->setSketchBelow( status_ ); } );


//    connect( ac_remove_above, &QAction::triggered, [=]()
//                                                   { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE ); } );

//    connect( ac_remove_above_int, &QAction::triggered, [=]()
//                                                   { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION ); } );

//    connect( ac_remove_below, &QAction::triggered, [=]()
//                                                   { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW ); } );

//    connect( ac_remove_below_int, &QAction::triggered, [=]()
//                                                   { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION ); } );


//    connect( ac_truncate, &QAction::triggered, [=]()
//                                                   { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::TRUNCATE ); } );


//    connect( ac_clear, &QAction::triggered, [=](){ app->restart(); } );


//    connect( ac_save, &QAction::triggered, [=](){ save(); } );


//    connect( ac_load, &QAction::triggered, [=](){ load(); } );


//    connect( ac_undo, &QAction::triggered, [=](){ app->undo(); } );


//    connect( ac_redo, &QAction::triggered, [=](){ app->redo(); } );

//    connect( ac_screenshot, &QAction::triggered, [=](){ app->screenshot(); } );

}



void MainWindow::createSidebarActions()
{

//    connect( object_tree, &ObjectTree::setVolumeName, [=]( std::size_t index_, const std::string& name_ ){ app->setVolumeName( index_, name_ ); } );


//    connect( object_tree, &ObjectTree::setVolumeVisible, [=]( std::size_t index_, bool status_ ){ app->setVolumeVisible( index_, status_ ); } );


//    connect( object_tree, &ObjectTree::setObjectName, [=]( std::size_t index_, const std::string& name_ ){ app->setObjectName( index_, name_ ); } );


//    connect( object_tree, &ObjectTree::setObjectVisible, [=]( std::size_t index_, bool status_ ){ app->setObjectVisible( index_, status_ ); } );


//    connect( object_tree, &ObjectTree::setObjectColor, [=]( std::size_t index_, const QColor& color_ )
//                                                       { app->setObjectColor( index_, color_.red(), color_.green(), color_.blue() ); } );


//    connect( object_tree, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_ ) { app->getObjectInformation( item_ ); } );





//    connect( object_tree, &ObjectTree::setRegionName, [=]( std::size_t index_, const std::string& name_ ){ app->setRegionName( index_, name_ ); } );


//    connect( object_tree, &ObjectTree::setRegionVisible, [=]( std::size_t index_, bool status_ ){ app->setRegionVisible( index_, status_ ); } );


//    connect( object_tree, &ObjectTree::setRegionColor, [=]( std::size_t index_, const QColor& color_ )
//                                                       { app->setRegionColor( index_, color_.red(), color_.green(), color_.blue() ); } );




//    connect( object_properties, &PagesStack::widthVolumeChanged, [=]( double w_ )
//                                                                 { app->changeVolumeDimension( Settings::CrossSection::CrossSectionDirections::X, w_ ); } );

//    connect( object_properties, &PagesStack::heightVolumeChanged, [=]( double h_ )
//                                                                 { app->changeVolumeDimension( Settings::CrossSection::CrossSectionDirections::Y, h_ ); } );


//    connect( object_properties, &PagesStack::depthVolumeChanged, [=]( double l_ )
//                                                                 { app->changeVolumeDimension( Settings::CrossSection::CrossSectionDirections::Z, l_ ); } );


//    connect( object_properties, &PagesStack::saveText, [=]( const QString& text_ ){ app->saveObjectInformation( text_.toStdString() ); } );



//    connect( object_properties, &PagesStack::setLowResolution, [=](){ app->setLowResolution(); } );
//    connect( object_properties, &PagesStack::setMediumResolution, [=](){ app->setMediumResolution(); } );
//    connect( object_properties, &PagesStack::setHighResolution, [=](){ app->setHighResolution(); } );

}




void MainWindow::createSketchingWindow()
{

    sketch_window = new SketchWindow();
    dw_sketchwindow = new QDockWidget( "Cross-Section" );
    dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketchwindow->setWidget( sketch_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketchwindow );

    scontroller = new SketchingController();

    sketch_topview_window = new SketchWindow();
    dw_topview_window = new QDockWidget( "Top-View" );
    dw_topview_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_topview_window->setWidget( sketch_topview_window );
    addDockWidget( Qt::BottomDockWidgetArea, dw_topview_window );

}


void MainWindow::updateSketchingWindowGeometry(  int width, int height )
{
    dw_sketchwindow->setMaximumSize( app_width*0.6, app_height*0.5 );
    dw_topview_window->setMaximumSize( app_width*0.6, app_height*0.5 );
}


void MainWindow::createSketchingActions()
{

    connect( sketch_window, &SketchWindow::addCurve, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ app->addCurveToObject( curve_, dir_, depth_ ); } );


    connect( ac_redo, &QAction::triggered, [=]{ app->moveMainCrossSection( 10. ); } );
    connect( ac_undo, &QAction::triggered, [=]{ app->moveMainCrossSection( -10. ); } );

    connect( ac_load, &QAction::triggered, [=]{ app->changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections::X ); } );

    connect( ac_save, &QAction::triggered, [=]{ app->changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections::Z ); } );


//    connect( sl_depth_csection, &RealFeaturedSlider::markValue, [=]( const double& v, QColor c_ )
//                                                         { app->addCrossSectionCanvas( v, c_ );  } );

//    connect( sl_depth_csection, &RealFeaturedSlider::unmarkValue, [=]( double v ){ app->removeCrossSectionCanvas( v ); } );


//    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, sketch_window, &SketchWindow::setCurrentCrossSection );



//    connect( sketch_window, &SketchWindow::updateVolume, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w_, double h_ )
//                                                         { app->changeVolumeDimensions( dir_, w_, h_ ); } );

//    connect( sketch_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_, double depth_ )
//                                                         { app->acceptSketchingCurve( curve_, depth_ ); } );

//    connect( sketch_window, &SketchWindow::commitObject, [=](){ app->createObjectSurface(); } );


//    connect( sketch_window, &SketchWindow::objectSelected, [=]( std::size_t index_ ){ app->setObjectAsBoundering( index_ ); } );


//    connect( sketch_window, &SketchWindow::defineColorCurrent, [=]( const QColor& color_ )
//                                                          { app->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );


//    connect( sketch_window, &SketchWindow::setImageCrossSection, [=](  double depth_, const QString& file, double ox_, double oy_, double x_, double y_ )
//                                                           { app->setImageToCrossSection( depth_, file.toStdString(), ox_, oy_, x_, y_ ); } );

//    connect( sketch_window, &SketchWindow::removeCurveFromObject, [=](  double depth_, std::size_t index_ )
//                                                           { app->removeCurveFromObject( depth_, index_ ); } );

//    connect( sketch_window, &SketchWindow::removeImageFromCrossSection, [=](  double depth_ )
//                                                           { app->removeImageFromCrossSection( depth_ ); } );

//    connect( sketch_window, &SketchWindow::addFixedCrossSection, sl_depth_csection, &RealFeaturedSlider::addMarker );

//    connect( sketch_window, &SketchWindow::removeFixedCrossSection, sl_depth_csection, &RealFeaturedSlider::removeMarker );


//    connect( sketch_window, &SketchWindow::enablePreview, [=]( bool status_ ){ app->enablePreview( status_ ); } );


//    connect( object_properties, &PagesStack::widthVolumeChanged, [=]()
//                                                           { app->updateSketchingCanvas(); } );

//    connect( object_properties, &PagesStack::heightVolumeChanged, [=]()
//                                                           {  app->updateSketchingCanvas(); } );

//    connect( object_properties, &PagesStack::depthVolumeChanged, [=]()
//                                                            {  app->updateSketchingCanvas(); } );




//    connect( sl_depth_csection, &RealFeaturedSlider::sliderMoved, sketch_topview_window, &SketchWindow::setCurrentCrossSection );


//    connect( sketch_topview_window, &SketchWindow::acceptCurve, [=]( const PolyCurve& curve_ )
//                                                        { app->acceptSketchingTrajectory( curve_ ); } );

//    connect( sketch_topview_window, &SketchWindow::commitObject, [=](){ app->createObjectSurface(); } );


//    connect( sketch_topview_window, &SketchWindow::getHeightMap, [=](){ app->getHeightMapTopView(); } );


//    connect( sketch_topview_window, &SketchWindow::setImageToTopView, [=]( const QString& file_, double ox_, double oy_, double x_, double y_ )
//                                                            { app->setImageToTopView( file_.toStdString(), ox_, oy_, x_, y_ ); } );

//    connect( sketch_topview_window, &SketchWindow::removeImageFromTopView, [=](){ app->removeImageFromTopView(); } );


}



void MainWindow::createFlowWindow()
{
//    flow_window = new FlowWindow();
//    dw_flow_window = new QDockWidget( "Flow Diagnostics" );
//    dw_flow_window->setAllowedAreas( Qt::AllDockWidgetAreas );
//    dw_flow_window->setWidget( flow_window );
//    dw_flow_window->setVisible( false );
//    addDockWidget( Qt::BottomDockWidgetArea, dw_flow_window );

////    connect( flow_window, &FlowWindow::getLegacyMeshes, this, [=]( std::vector<double> &points, std::vector<size_t> &nu,
////                                                                   std::vector<size_t> &nv, size_t num_extrusion_steps ){
////                                                                    app->getLegacyMeshes( points, nu, nv, num_extrusion_steps ) ; } );

////    connect( flow_window, &FlowWindow::getSurfacesMeshes, this, [=]( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
////                                                std::vector< FlowWindow::CurveMesh>& left_curves,
////                                                std::vector< FlowWindow::CurveMesh >& right_curves,
////                                                std::vector< FlowWindow::CurveMesh > & front_curves,
////                                                std::vector< FlowWindow::CurveMesh >& back_curves ) {
////                                                app->getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves ); } );

////    connect( flow_window, &FlowWindow::sendSimplifiedMesh, [=]( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces ){
////                                                app->getTetrahedronsRegions( vertices, edges, faces ); } );


//    ac_output_volume = new QAction( "Diagnostics", this );
////    ac_output_volume->setIcon(QIcon(":/images/icons/image71.png"));
//    ac_output_volume->setCheckable( true );

////    connect( ac_output_volume, &QAction::toggled, [=]( bool status_ )
////    {  if( status_ == true )
////        {
////            app->startFlowDiagnostics();
////            std::cout << "Start flow diagnostics" << std::endl << std::flush;
////        }
////        else
////        {
////            app->closeFlowDiagnostics();
////            std::cout << "Stop flow diagnostics" << std::endl << std::flush;
////        }
////        //             else
////    } );

//    tb_mainwindow->addAction( ac_output_volume );




}



void MainWindow::run_app()
{

    controller = new Controller();

    app = new RRMApplication();
    app->setController( controller );
    app->init();

//    app = new RRMApplication( this );
//    app->init();


}



void MainWindow::run_app_sketch()
{
    scontroller->setMainWindow( std::shared_ptr< SketchWindow > ( sketch_window ) );
    scontroller->setController( std::shared_ptr< Controller > ( controller ) );
    app->setSketchingController( scontroller );
    app->initSketchingApp();

}


void MainWindow::save()
{
    QString selected_format = "";
    QString filename_ = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );


    if( filename_.isEmpty() == true ) return;
//    app->save( filename_.toStdString() );
}


void MainWindow::load()
{

    QString selected_format = "";
    QString filename_ = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );

    if( filename_.isEmpty() == true ) return;
//    app->load( filename_.toStdString() );

}


void MainWindow::showHelp()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile("manual/rrm-manual.pdf"));
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
//    std::cout << "Resizing window" << std::endl << std::flush;

//    QRect rect = QGuiApplication::primaryScreen()->geometry();

//    app_width  = static_cast< int > (  event->size().width() /** 0.8f*/ );
//    app_height = static_cast< int > ( event->size().height() /** 0.8f*/ );

//    resize( app_width, app_height );
}

