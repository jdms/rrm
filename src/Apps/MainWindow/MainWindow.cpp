#include "MainWindow.h"
#include "MainWindow/ExtrusionRulesProcessor.hpp"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
    setFocusPolicy( Qt::StrongFocus );
    setFocus();
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs | QMainWindow::AllowTabbedDocks );
    setDockNestingEnabled( true );
    setMinimumSize ( 800 , 600 );

    createWindow();
    createActions();
    createMenuBar();

    scene_initialized = false;


}


MainWindow::~MainWindow()
{

}



void MainWindow::createWindow()
{

    controller = new Controller();

    scene = new Scene();
    scene->setController( controller );

    createSketchingModule();
    create3DViewModule();

    aboutRRM = new AboutWidget( this );

}




void MainWindow::createActions()
{
    createMainWindowActions();
    createSketchingActions();
    create3DWindowActions();

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


//    connect ( sketching_window, SIGNAL( updateStratigraphicRule( const std::string& ) ) , controller, SLOT( setCurrentStratigraphicRule( const std::string& ) ) );
    connect ( sketching_window, &SketchingWindow::updateStratigraphicRule, [=]( const std::string& rule ){
                                                                                                std::vector< unsigned int > items = scene->getAllSelectedItems();
                                                                                                controller->setCurrentStratigraphicRule( rule, items ); } );


    connect ( sketching_window, SIGNAL( undo() ), controller , SLOT( undo() ) );
    connect ( sketching_window, SIGNAL( redo() ), controller , SLOT( redo() ) );
    connect ( sketching_window, SIGNAL( clear() ), scene , SLOT( clearScene() ) );

    connect ( sketching_window, SIGNAL( addStratigraphy() ), scene , SLOT( addCurve() ) );
    connect ( sketching_window, SIGNAL( undoLastSketch() ), scene , SLOT( undoLastSketch() ) );
    connect ( sketching_window, SIGNAL( updateColor( const QColor& ) ), scene , SLOT( updateColor( const QColor& ) ) );
    connect ( sketching_window, SIGNAL( setCurrentMode( const Scene::InteractionMode& ) ), scene , SLOT( setCurrentMode( const Scene::InteractionMode& ) ) );

    connect ( sketching_window, SIGNAL( setRandomColor( bool ) ), scene, SLOT( setRandomColor ( bool ) ) );


    connect( controller, SIGNAL( enableUndo( bool ) ) , sketching_window, SLOT( enableUndo( bool ) ) );
    connect( controller, SIGNAL( enableRedo( bool ) ) , sketching_window, SLOT( enableRedo( bool ) ) );

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
    connect( view3d_window, SIGNAL( initializeScene() ), this, SLOT( initProgram() ) );



}



void MainWindow::initProgram()
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
