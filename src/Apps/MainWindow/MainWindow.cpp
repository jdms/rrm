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

#include "MainWindow.h"
//#include "MainWindow/ExtrusionRulesProcessor.hpp"

#include "ui_MainWindow.h"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    init();
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::init()
{

    setupWindowProperties();
    createWindow();


    controller = new Controller();
    controller->setScene3D( &scene3d );
    controller->setSketchScene( &sketch_scene );
    controller->setPathScene( &scene_path );
    controller->setObjectTree( object_tree );


    /*

    createWindow();
    createActions();
    createMenuBar();

    scene_initialized = false;
*/

}


void MainWindow::setupWindowProperties()
{
    setFocusPolicy( Qt::StrongFocus );
    setFocus();
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setDockNestingEnabled( true );
}


void MainWindow::createWindow()
{

    setMinimumSize( 1000, 800 );
    setWindowTitle( "Rapid Reservoir Modelling" );

    //TODO: add methods to create module 3d and 2d
    canvas3d = new Canvas3D();
    canvas3d->setScene( &scene3d );

    canvas2d = new SketchCanvas();
    canvas2d->setScene( &sketch_scene );
    canvas2d->setMaximumHeight( 350 );


    dw_sketch_canvas = new QDockWidget( "Sketching Canvas" );
    dw_sketch_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_canvas->setWidget( canvas2d );
    dw_sketch_canvas->setVisible( true );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketch_canvas );


    canvas_path = new CanvasPath();
    canvas_path->setScenePath( &scene_path );
    canvas_path->setMaximumSize(350, 200 );

    dw_sketch_path_canvas = new QDockWidget( "Sketching Path Canvas" );
    dw_sketch_path_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_path_canvas->setWidget( canvas_path );
    dw_sketch_path_canvas->setVisible( true );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketch_path_canvas );



    sl_depth_csection = new QSlider( Qt::Vertical );

    object_tree = new ObjectTree( this );
    object_tree->setMaximumWidth( 200 );
    object_tree->setHeaderLabel( "Objects" );
    object_tree->setColumnCount( 1 );


    ui->hl_mainwindow->addWidget( object_tree );
    ui->hl_mainwindow->addWidget( canvas3d );
    ui->hl_mainwindow->addWidget( sl_depth_csection );



    getCurrentDirectory();


    connect( sl_depth_csection, &QSlider::sliderReleased, [=](){
                      dw_sketch_canvas->setVisible( true );
                      controller->setCurrentCrossSection( (double)sl_depth_csection->value() ); } );

// TODO: uncomment these lines as the interface is ready
//    connect( tbt_colorsketch, &QToolButton::toggled, [=]( bool status_ ){
//                                                 randomColor( !status_ );
//    } );

//    connect( cd_pickercolor, &QColorDialog::colorSelected, [=]( const QColor& c_ ){
//                                                 tbt_colorsketch->setChecked( true );
//                                                 randomColor( false, c_ ); } );


    connect( &sketch_scene, &SketchScene::curveAccepted, [=]( const Curve2D& c_ ){
                                                 controller->addInputCurvetoCurrentObject( c_ );
                                                 } );



    connect( &sketch_scene, &SketchScene::interpolateObject, [=](){
                                                controller->interpolate();
                                                emit enableUndo( controller->canUndo() );
                                                emit enableRedo( controller->canRedo() );
                                                emit updateScenes();
                                                randomColor( true );  } );


    connect( &sketch_scene, &SketchScene::updateVolumeWidthHeight, [=]( double w_, double h_ ) {
                                                controller->setInputVolumeWidthHeight( w_, h_ );
                                                emit updateScenes();
                                                } );


    connect( &scene_path, &PathScene::updateVolumeWidthDepth, [=]( double w_, double d_ ) {
                                                controller->setInputVolumeWidthDepth( w_, d_ );
                                                emit updateScenes();
                                                } );


    connect( &scene_path, &PathScene::curveAccepted, [=]( const Curve2D& c_ ){
                                                 controller->setPathCurvetoCurrentObject( c_ );
                                                 } );


    connect( this, &MainWindow::updateScenes, &sketch_scene, &SketchScene::updateScene );
    connect( this, &MainWindow::updateScenes, &scene3d, &Scene3D::updateScene );



//    connect( object_tree, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_, int column_ ) {
//                                       ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* > item_;
//                                       controller->selectObject( obj_->getId() ); } );





    ac_undo = new QAction( "Undo", this );
//    ac_undo->setShortcut( QKeySequence(/*undo_*/ );

    QShortcut* undo_ = new QShortcut( QKeySequence::Undo, this );
    connect( undo_, &QShortcut::activated, ac_undo, &QAction::trigger );
    connect( ac_undo, &QAction::triggered, [=](){ undo(); } );


    ac_discard_sketch = new QAction( "Discard sketch", this );
    connect( ac_discard_sketch, &QAction::triggered, &sketch_scene, &SketchScene::clearSketch );


    ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, &sketch_scene, &SketchScene::screenshot );


    /*

    controller = new Controller();

    scene = new Scene();
    scene->setController( controller );

    createSketchingModule();
    create3DViewModule();
    createFlowDiagnosticsModule();

    aboutRRM = new AboutWidget( this );
    */

}


void MainWindow::getCurrentDirectory()
{

    QDir app_dir = QDir( qApp->applicationDirPath() );

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    QString current_dir ( app_dir.path ()+"\\" );

#elif defined(__linux__)               // Linux Directory Style
    QString current_dir ( app_dir.path ( ) + "/" );

#else
    /* Error, both can't be defined or undefined same time */
    std::cout << "Operate System not supported !"
    halt();

#endif

    scene3d.setCurrentDirectory( current_dir.toStdString() );
    //canvas->setCurrentDirectory( current_dir.toStdString() );

}


void MainWindow::run_app()
{
    controller->init();

    double w = 0.0f, h = 0.0f, d = 0.0f;
    controller->getInputVolumeDimensions( w, h, d );

    sl_depth_csection->setMinimum( 0 );
    sl_depth_csection->setMaximum( (int)d );

    randomColor( true );
}

void MainWindow::createActions()
{
//    createMainWindowActions();
//    createSketchingActions();
//    create3DWindowActions();

//    createFlowDiagnosticsActions();

}



void MainWindow::createMainWindowActions ()
{

//    ac_exit = new QAction( tr ( "E&xit" ) , this );
//    ac_exit->setIcon( QIcon( ":/images/icons/door_out.png" ) );

//    ac_rrmGuide = new QAction( tr ( "RRM Manual" ), this );
//    ac_about = new QAction( tr ( "&About" ) , this );


//    connect( ac_about, SIGNAL( triggered() ) , aboutRRM, SLOT( show() ) );
//	connect(ac_rrmGuide, &QAction::triggered, [=]()
//	{
//		QDir dir;
////		qDebug() << dir.currentPath() << "Help !!";
//		/// The QFileInfo class provides system-independent file information.
//		/// @see http://doc.qt.io/qt-5/qfileinfo.html#absoluteFilePath
//        QFileInfo file_info(dir.currentPath() + "/manual/rrm-manual.pdf");
//		QDesktopServices::openUrl(QUrl::fromLocalFile(file_info.absoluteFilePath()));
//	});
//    connect( ac_exit, SIGNAL( triggered() ) , this, SLOT( close() ) );

//    connect( this, SIGNAL( saveAsCPS3( const std::string& ) ) , scene, SLOT( exportToCPS3( const std::string& ) ) );
//    connect( this, SIGNAL( saveAsIrapGrid( const std::string& ) ) , scene, SLOT( exportToIrapGrid( const std::string& ) ) );

}


void MainWindow::createMenuBar()
{

//    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
//    mn_file->addAction ( ac_exit );

//    mn_windows = menuBar()->addMenu ( tr ( "&View" ) );

//    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
//    mn_help->addAction( ac_rrmGuide );
//    mn_help->addAction( ac_about );


//    createSketchingMenuBar();
//    create3DWindowMenuBar();
//    createFlowDiagnosticsMenuBar();

}





void MainWindow::createSketchingModule()
{

//    dw_sketching = new QDockWidget();
//    dw_sketching->setWindowTitle( "Sketching Window" );

//    sketching_window = new SketchingWindow( this );
//    dw_sketching->setWidget( sketching_window );

//    dw_sketching->setAllowedAreas( Qt::AllDockWidgetAreas );
//    addDockWidget( Qt::TopDockWidgetArea, dw_sketching );

}


void MainWindow::createSketchingMenuBar()
{
//    mn_windows->addAction ( ac_wdwsketching );
//    ac_wdwsketching->setCheckable ( true );
}


void MainWindow::createSketchingActions()
{


//    ac_wdwsketching = new QAction ( tr ( "Window Sketching" ) , this );
//    ac_wdwsketching->setCheckable ( true );
//    ac_wdwsketching->setChecked ( true );


//    connect( ac_wdwsketching , SIGNAL( toggled( bool ) ), dw_sketching , SLOT( setVisible( bool ) ) );
////    connect( dw_sketching, &QDockWidget::visibilityChanged, ac_wdwsketching, &QAction::setChecked );
//    connect( dw_sketching, &QDockWidget::visibilityChanged, [=]()
//    {
//        if( dw_sketching->isActiveWindow() && dw_sketching->isHidden()  )
//            ac_wdwsketching->setChecked( false );
//    });


//    connect ( sketching_window, &SketchingWindow::updateStratigraphicRule, controller, &Controller::setStratigraphicRule );


//    connect ( sketching_window, &SketchingWindow::defineSketchingAbove, scene, &Scene::enableSketchingAboveRegion );
//    connect ( sketching_window, &SketchingWindow::defineSketchingBelow, scene, &Scene::enableSketchingBelowRegion );


//    connect ( sketching_window, SIGNAL( undo() ), controller , SLOT( undo() ) );
//    connect ( sketching_window, SIGNAL( redo() ), controller , SLOT( redo() ) );
//    connect ( sketching_window, SIGNAL( clear() ), scene , SLOT( clearScene() ) );

//    connect ( sketching_window, SIGNAL( addStratigraphy() ), scene , SLOT( finishCurve() ) );
//    connect ( sketching_window, SIGNAL( undoLastSketch() ), scene , SLOT( undoLastSketch() ) );
//    connect ( sketching_window, SIGNAL( updateColor( const QColor& ) ), scene , SLOT( updateColor( const QColor& ) ) );
//    connect ( sketching_window, SIGNAL( setCurrentMode( const Scene::InteractionMode& ) ), scene , SLOT( setCurrentMode( const Scene::InteractionMode& ) ) );

//    connect ( sketching_window, SIGNAL( setRandomColor( bool ) ), scene, SLOT( setRandomColor ( bool ) ) );



//    connect( controller, SIGNAL( enableUndo( bool ) ) , sketching_window, SLOT( enableUndo( bool ) ) );
//    connect( controller, SIGNAL( enableRedo( bool ) ) , sketching_window, SLOT( enableRedo( bool ) ) );

//    connect( controller, SIGNAL( changeStratigraphyRulesStatus( const std::string& ) ) , sketching_window, SLOT( changeStratigraphyRulesStatus(const std::string& ) ) );
//    connect( controller, SIGNAL( changeDefineRegionStatus( const bool, const bool ) ) , sketching_window, SLOT( changeDefineRegionStatus( const bool, const bool ) ) );

//    connect( controller, SIGNAL( pickingRegion( bool ) ) , scene, SLOT( enablePickingRegion( bool ) ) );



//    connect( scene, &Scene::enableSketching, sketching_window, &SketchingWindow::enableSketching );
//    connect( scene, &Scene::sendCoordinates, sketching_window, &SketchingWindow::updateMousePosition );
//    connect( scene, &Scene::updateBoundGeometry, sketching_window, &SketchingWindow::updateBoundaryDimensions );


//    connect( sketching_window, SIGNAL( exportSurfaces() ), this, SLOT(  exportTo() ) );


}





void MainWindow::create3DViewModule()
{
//    dw_3dview = new QDockWidget();
//    dw_3dview->setWindowTitle( "3D Window" );

//    view3d_window = new View3DWindow ( this );
//    dw_3dview->setWidget( view3d_window );

//    dw_3dview->setAllowedAreas( Qt::AllDockWidgetAreas );
//    addDockWidget( Qt::BottomDockWidgetArea, dw_3dview );

}


void MainWindow::create3DWindowMenuBar()
{

//    mn_windows->addAction ( ac_3dview );
}


void MainWindow::create3DWindowActions()
{
//    ac_3dview = new QAction ( tr ( "Window 3D" ) , this );
//    ac_3dview->setCheckable ( true );
//    ac_3dview->setChecked ( true );

//    connect ( ac_3dview , SIGNAL( toggled( bool ) ) , dw_3dview , SLOT( setVisible( bool ) ) );
//    connect( dw_3dview, &QDockWidget::visibilityChanged, [=]()
//    {
//        if( dw_3dview->isActiveWindow() && dw_3dview->isHidden()  )
//            ac_3dview->setChecked( false );
//    });

//    connect( view3d_window, SIGNAL( initializeScene() ), this, SLOT( initScene() ) );
//    connect( view3d_window, SIGNAL( changeResolution( const int, const int ) ), controller, SLOT( changeResolution( const int, const int ) ) );
////    connect ( sketching_window, SIGNAL( clear() ), view3d_window , SLOT( clearScene() ) );

}


void MainWindow::createFlowDiagnosticsModule()
{
//    dw_flowdiagnostics = new QDockWidget();
//    dw_flowdiagnostics->setWindowTitle( "Flow Diagnostics Window" );

//    flow_window = new FlowWindow ( this );
//    dw_flowdiagnostics->setWidget( flow_window );

//    dw_flowdiagnostics->setAllowedAreas( Qt::AllDockWidgetAreas );
//    addDockWidget( Qt::BottomDockWidgetArea, dw_flowdiagnostics );

//    ac_flowwindow  = new QAction ( tr ( "Flow Diagnostics Window" ) , this );
//    ac_flowwindow->setCheckable ( true );
//    ac_flowwindow->setChecked ( true );

}


void MainWindow::createFlowDiagnosticsMenuBar()
{

//     mn_windows->addAction ( ac_flowwindow );
}


void MainWindow::createFlowDiagnosticsActions()
{

//    connect ( ac_flowwindow , SIGNAL( toggled( bool ) ) , dw_flowdiagnostics , SLOT( setVisible( bool ) ) );
//    connect( dw_flowdiagnostics, &QDockWidget::visibilityChanged, [=]()
//    {
//        if( dw_flowdiagnostics->isActiveWindow() && dw_flowdiagnostics->isHidden()  )
//            ac_flowwindow->setChecked( false );
//    });

//    connect( flow_window, &FlowWindow::getLegacyMeshes, scene, &Scene::getLegacyMeshes );

//	/// Send region coordinates
//    connect(scene, &Scene::sendRegionPoints,  flow_window, &FlowWindow::regionPoints);
//	/// Get number of region to ensure consistence
//	connect(scene, &Scene::requestNumberOfRegion, [=](){ scene->createRegions(flow_window->getNumberOfRegions()); });
//    connect(flow_window, &FlowWindow::sendNumberOfRegions, scene, &Scene::createRegions);

//    connect( scene, &Scene::volumeDimensions, flow_window, &FlowWindow::setVolumeDimensions );

}






void MainWindow::initScene()
{

//    if( scene_initialized == true ) return;

//    sketching_window->setScene( scene );
//    view3d_window->setScene( scene );

//    scene->init();
//    scene_initialized = true;

}


void MainWindow::clear()
{
}


void MainWindow::exportTo()
{
//    QString selected_format = "";
////    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./exported/",
////                                                         "CPS3 files (*.CPS3);;Irap Classic Grid (*.IRAPG)", &selected_format );

//    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./exported/irapg/",
//                                                         "Irap Classic Grid (*.IRAPG)", &selected_format );

//    if( filename.isEmpty() == true ) return;

//    if( selected_format == QString( "CPS3 files (*.CPS3)" ) )
//    {
//        emit saveAsCPS3( filename.toStdString() );
//    }

//    else if( selected_format == QString( "Irap Classic Grid (*.IRAPG)" ) )
//    {
//        emit saveAsIrapGrid( filename.toStdString() );
//    }


}


void MainWindow::keyPressEvent( QKeyEvent *event )
{

    switch ( event->key() )
    {

        case Qt::Key_A:
        {

            if( event->modifiers() & Qt::AltModifier )
            {
                controller->enableCreateAbove( false );
//                controller->defineSketchAbove( false );
            }

            else if( event->modifiers() & Qt::ControlModifier )
            {
                controller->updateRule( "RA_SKETCHING" );
            }

            else if( event->modifiers() & Qt::ShiftModifier )
            {
                controller->updateRule( "RAI_SKETCHING" );
            }

            else
            {
                controller->enableCreateAbove( true );
            }
        }
        break;

        case Qt::Key_B:
        {
            if( event->modifiers() & Qt::AltModifier )
            {
                controller->enableCreateBelow( false );
            }

            else if( event->modifiers() & Qt::ControlModifier )
            {
                controller->updateRule( "RB_SKETCHING" );
            }

            else if( event->modifiers() & Qt::ShiftModifier )
            {
                controller->updateRule( "RBI_SKETCHING" );
            }

            else
            {
                controller->enableCreateBelow( true );
            }
        }
        break;


        case Qt::Key_E:
        {
            ac_screenshot->trigger();
//            exportTo();
        }
        break;

        case Qt::Key_I:
        {
            controller->interpolate();

            emit enableUndo( controller->canUndo() );
            emit enableRedo( controller->canRedo() );

            emit updateScenes();
            std::cout << "Ordered to update secenes\n"  << std::flush;
        }
        break;



        case Qt::Key_S:
        {
            controller->updateRule( "SKETCHING" );

        }


        default:
            break;


    };


}


void MainWindow::undo()
{
    controller->undo();

}
