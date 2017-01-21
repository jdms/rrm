#include "MainWindow.h"
#include "MainWindow/ExtrusionRulesProcessor.hpp"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
    init();
}


MainWindow::~MainWindow()
{

}



void MainWindow::init()
{

    setFocusPolicy( Qt::StrongFocus );
    setFocus();
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs | QMainWindow::AllowTabbedDocks );
    setDockNestingEnabled( true );
    setMinimumSize ( 1000, 800 );
	setWindowTitle("Rapid Reservoir Modelling");
    createWindow();
    createActions();
    createMenuBar();

    scene_initialized = false;

}


void MainWindow::createWindow()
{

    controller = new Controller();

    scene = new Scene();
    scene->setController( controller );

    createSketchingModule();
    create3DViewModule();
    createFlowDiagnosticsModule();

    aboutRRM = new AboutWidget( this );

}


void MainWindow::createActions()
{
    createMainWindowActions();
    createSketchingActions();
    create3DWindowActions();

    createFlowDiagnosticsActions();

}



void MainWindow::createMainWindowActions ( )
{

    ac_exit = new QAction( tr ( "E&xit" ) , this );
    ac_exit->setIcon( QIcon( ":/images/icons/door_out.png" ) );

    ac_contents = new QAction( tr ( "Contents" ), this );
    ac_about = new QAction( tr ( "&About" ) , this );


    connect( ac_about, SIGNAL( triggered() ) , aboutRRM, SLOT( show() ) );
    connect( ac_contents, SIGNAL( triggered() ) , &help, SLOT( show() ) );
    connect( ac_exit, SIGNAL( triggered() ) , this, SLOT( close() ) );

    connect( this, SIGNAL( saveAsCPS3( const std::string& ) ) , scene, SLOT( exportToCPS3( const std::string& ) ) );
    connect( this, SIGNAL( saveAsIrapGrid( const std::string& ) ) , scene, SLOT( exportToIrapGrid( const std::string& ) ) );

}


void MainWindow::createMenuBar()
{

    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar()->addMenu ( tr ( "&View" ) );

    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
    mn_help->addAction( ac_contents );
    mn_help->addAction( ac_about );


    createSketchingMenuBar();
    create3DWindowMenuBar();

}





void MainWindow::createSketchingModule()
{

    dw_sketching = new QDockWidget();
    dw_sketching->setWindowTitle( "Sketching Window" );

    sketching_window = new SketchingWindow( this );
    dw_sketching->setWidget( sketching_window );

    dw_sketching->setAllowedAreas( Qt::AllDockWidgetAreas );
    addDockWidget( Qt::TopDockWidgetArea, dw_sketching );

}


void MainWindow::createSketchingMenuBar()
{
    mn_windows->addAction ( ac_wdwsketching );
    ac_wdwsketching->setCheckable ( true );
}


void MainWindow::createSketchingActions()
{


    ac_wdwsketching = new QAction ( tr ( "Window Sketching" ) , this );
    ac_wdwsketching->setCheckable ( true );
    ac_wdwsketching->setChecked ( true );


    connect( ac_wdwsketching , SIGNAL( toggled( bool ) ), dw_sketching , SLOT( setVisible( bool ) ) );
    connect( dw_sketching, &QDockWidget::visibilityChanged, ac_wdwsketching, &QAction::setChecked );


    connect ( sketching_window, &SketchingWindow::updateStratigraphicRule, controller, &Controller::setStratigraphicRule );


    connect ( sketching_window, &SketchingWindow::defineSketchingAbove, scene, &Scene::enableSketchingAboveRegion );
    connect ( sketching_window, &SketchingWindow::defineSketchingBelow, scene, &Scene::enableSketchingBelowRegion );


    connect ( sketching_window, SIGNAL( undo() ), controller , SLOT( undo() ) );
    connect ( sketching_window, SIGNAL( redo() ), controller , SLOT( redo() ) );
    connect ( sketching_window, SIGNAL( clear() ), scene , SLOT( clearScene() ) );

    connect ( sketching_window, SIGNAL( addStratigraphy() ), scene , SLOT( finishCurve() ) );
    connect ( sketching_window, SIGNAL( undoLastSketch() ), scene , SLOT( undoLastSketch() ) );
    connect ( sketching_window, SIGNAL( updateColor( const QColor& ) ), scene , SLOT( updateColor( const QColor& ) ) );
    connect ( sketching_window, SIGNAL( setCurrentMode( const Scene::InteractionMode& ) ), scene , SLOT( setCurrentMode( const Scene::InteractionMode& ) ) );

    connect ( sketching_window, SIGNAL( setRandomColor( bool ) ), scene, SLOT( setRandomColor ( bool ) ) );



    connect( controller, SIGNAL( enableUndo( bool ) ) , sketching_window, SLOT( enableUndo( bool ) ) );
    connect( controller, SIGNAL( enableRedo( bool ) ) , sketching_window, SLOT( enableRedo( bool ) ) );

    connect( controller, SIGNAL( changeStratigraphyRulesStatus( const std::string& ) ) , sketching_window, SLOT( changeStratigraphyRulesStatus(const std::string& ) ) );
    connect( controller, SIGNAL( changeDefineRegionStatus( const bool, const bool ) ) , sketching_window, SLOT( changeDefineRegionStatus( const bool, const bool ) ) );

    connect( controller, SIGNAL( pickingRegion( bool ) ) , scene, SLOT( enablePickingRegion( bool ) ) );



    connect( scene, &Scene::enableSketching, sketching_window, &SketchingWindow::enableSketching );
    connect( scene, &Scene::sendCoordinates, sketching_window, &SketchingWindow::updateMousePosition );
    connect( scene, &Scene::updateBoundGeometry, sketching_window, &SketchingWindow::updateBoundaryDimensions );


    connect( sketching_window, SIGNAL( exportSurfaces() ), this, SLOT(  exportTo() ) );


}





void MainWindow::create3DViewModule()
{
    dw_3dview = new QDockWidget();
    dw_3dview->setWindowTitle( "3D Window" );

    view3d_window = new View3DWindow ( this );
    dw_3dview->setWidget( view3d_window );

    dw_3dview->setAllowedAreas( Qt::AllDockWidgetAreas );
    addDockWidget( Qt::BottomDockWidgetArea, dw_3dview );

}


void MainWindow::create3DWindowMenuBar()
{

    mn_windows->addAction ( ac_3dview );
}


void MainWindow::create3DWindowActions()
{
    ac_3dview = new QAction ( tr ( "Window 3D" ) , this );
    ac_3dview->setCheckable ( true );
    ac_3dview->setChecked ( true );

    connect ( ac_3dview , SIGNAL( toggled( bool ) ) , dw_3dview , SLOT( setVisible( bool ) ) );

    connect( dw_3dview, &QDockWidget::visibilityChanged, ac_3dview, &QAction::setChecked );
    connect( view3d_window, SIGNAL( initializeScene() ), this, SLOT( initScene() ) );
    connect( view3d_window, SIGNAL( changeResolution( const int, const int ) ), controller, SLOT( changeResolution( const int, const int ) ) );

}


void MainWindow::createFlowDiagnosticsModule()
{
    dw_flowdiagnostics = new QDockWidget();
    dw_flowdiagnostics->setWindowTitle( "Flow Diagnostics Window" );

    flow_window = new FlowWindow ( this );
    dw_flowdiagnostics->setWidget( flow_window );

    dw_flowdiagnostics->setAllowedAreas( Qt::AllDockWidgetAreas );
    addDockWidget( Qt::BottomDockWidgetArea, dw_flowdiagnostics );
}



void MainWindow::createFlowDiagnosticsActions()
{

    connect( flow_window, &FlowWindow::getLegacyMeshes, scene, &Scene::getLegacyMeshes );

	/// Send region coordinates
    connect(scene, &Scene::sendRegionPoints,  flow_window, &FlowWindow::regionPoints);
	/// Get number of region to ensure consistence
	connect(scene, &Scene::requestNumberOfRegion, [=](){ scene->createRegions(flow_window->getNumberOfRegions()); });
    connect(flow_window, &FlowWindow::sendNumberOfRegions, scene, &Scene::createRegions);

//	connect(flow_window, &FlowWindow::get2Dto3DMatrix, scene, &Scene::send2Dto3DMatrix);
//	connect(flow_window, &FlowWindow::get3Dto2DMatrix, scene, &Scene::send3Dto2DMatrix);
}






void MainWindow::initScene()
{

    if( scene_initialized == true ) return;

    sketching_window->setScene( scene );
    view3d_window->setScene( scene );

    scene->init();
    scene_initialized = true;

}



void MainWindow::clear()
{
}



void MainWindow::exportTo()
{


    QString selected_format = "";
//    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "/bin/exported/",
//                                                         "CPS3 files (*.CPS3);;Irap Classic Grid (*.IRAPG)", &selected_format );

    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "/bin/exported/",
                                                         "Irap Classic Grid (*.IRAPG)", &selected_format );

    if( filename.isEmpty() == true ) return;

    if( selected_format == QString( "CPS3 files (*.CPS3)" ) )
    {
        emit saveAsCPS3( filename.toStdString() );
    }

    else if( selected_format == QString( "Irap Classic Grid (*.IRAPG)" ) )
    {
        emit saveAsIrapGrid( filename.toStdString() );
    }


}


void MainWindow::keyPressEvent( QKeyEvent *event )
{

    switch ( event->key() )
    {
        case Qt::Key_E:
        {

            exportTo();

        }
        break;

        default:
            break;


    };


}
