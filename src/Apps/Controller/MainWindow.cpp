#include "MainWindow.h"


MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
    this->setFocusPolicy ( Qt::StrongFocus );
    this->setFocus ( );
	this->setAcceptDrops ( true );
	this->setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs | QMainWindow::AllowTabbedDocks);
	this->setDockNestingEnabled(true);
	this->setMinimumSize ( 800 , 600 );

    createWindow();
    createActions();
    createMenuBar();



}


MainWindow::~MainWindow()
{

}




void MainWindow::createWindow()
{
    createSketchingModule();
    createSeismicModule();
    create3DViewModule();
    createFlowDiagnosticsModule();

    std::cout << "MainWindow : " << width() << ", " << height() << std::endl;


    aboutRRM = new AboutWidget( this );

}


void MainWindow::createActions()
{
    createMainWindowActions();
    createSketchingActions();
    createSeismicgActions();
    create3DWindowActions();
    createFlowDiagnosticsActions();

}


void MainWindow::createMainWindowActions ( )
{

    ac_exit = new QAction ( tr ( "E&xit" ) , this );
    ac_exit->setIcon ( QIcon ( ":/images/icons/door_out.png" ) );

    ac_contents = new QAction ( tr ( "Contents" ) , this );
    ac_about = new QAction ( tr ( "&About" ) , this );


    connect ( ac_about , SIGNAL( triggered() ) , aboutRRM , SLOT( show() ) );
    connect ( ac_contents , SIGNAL( triggered() ) , &help , SLOT( show() ) );
    connect ( ac_exit , SIGNAL( triggered() ) , this , SLOT( close() ) );
}


void MainWindow::createMenuBar()
{

    mn_file = menuBar ( )->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar ( )->addMenu ( tr ( "&View" ) );

    mn_help = menuBar ( )->addMenu ( tr ( "&Help" ) );
    mn_help->addAction ( ac_contents );
    mn_help->addAction ( ac_about );


    createSketchingMenuBar();
    create3DWindowMenuBar();
    createFlowDiagnosticsMenuBar();

}




void MainWindow::createSketchingModule()
{

    dw_sketching = new QDockWidget();
    dw_sketching->setWindowTitle( "Sketching Window" );

    dc_sketching_module = new SketchingWindow( this );
    dw_sketching->setWidget( dc_sketching_module );

    dw_sketching->setAllowedAreas( Qt::AllDockWidgetAreas );
    this->addDockWidget( Qt::TopDockWidgetArea, dw_sketching );

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

	ac_wdwseismic = new QAction(tr("Window Seismic"), this);
	ac_wdwseismic->setCheckable(true);
	ac_wdwseismic->setChecked(true);



    connect ( ac_wdwsketching , SIGNAL( toggled( bool ) ) , dw_sketching , SLOT( setVisible( bool ) ) );
    connect( dw_sketching, &QDockWidget::visibilityChanged, ac_wdwsketching, &QAction::setChecked );

    /// Black Screen Module
    connect( dc_sketching_module->sketching_canvas, SIGNAL( currentCrossSection( const CrossSection& ) ), dc_3DView_Module->glWidget, SLOT( updateBlackScreen( const CrossSection& ) ) );

    /// Notify the Seismic Module with the new CrossSection configuration
    connect( dc_sketching_module->sketching_canvas, SIGNAL( currentCrossSection( const CrossSection& ) ), seismic_window, SLOT( updateCrossSection( const CrossSection& ) ) );



}




void MainWindow::createSeismicModule()
{
    QDockWidget* dw_seismic = new QDockWidget();
    dw_seismic->setWindowTitle( "Sketching Window" );

    dc_Seismic_Module = new SketchingWindow( this );
    dw_seismic->setWidget( dc_Seismic_Module );

    dw_seismic->setAllowedAreas( Qt::AllDockWidgetAreas );
    this->addDockWidget( Qt::TopDockWidgetArea, dw_seismic );

    dw_seismic->setVisible( false );

    seismic_window = new RRM::SeismicWindow(this);

}


void MainWindow::createSeismicgActions()
{
    connect( ac_wdwseismic, SIGNAL( toggled( bool ) ), dc_Seismic_Module, SLOT( setVisible( bool ) ) );
//		connect( dc_Seismic_Module, &SketchingWindow::visibilityChanged, ac_wdwseismic, &QAction::setChecked);

    /// Equip the Sketch Board with the current slice CrossSection
    connect( seismic_window, SIGNAL(currentCrossSection(const CrossSection&)), dc_sketching_module->sketching_canvas, SLOT( setCrossSection( const CrossSection& ) ) );

    /// Notify the 3D the current slice position
    connect( seismic_window->ui->seismic_slices_verticalSlider_ , SIGNAL( valueChanged( int ) ) , dc_3DView_Module->glWidget , SLOT( setPlanePosition( int ) ) );

    /// Notify the 3D View to update the surfaces
    connect( seismic_window->ui->seismic_pushButton_interpolate , SIGNAL( clicked( bool ) ) , this , SLOT( interpolate() ) );

    /// Notify the 3D View to update the surfaces
    connect ( seismic_window->ui->seismic_pushButton_clear_ , SIGNAL( pressed() ) , this , SLOT( clear() ) );

}




void MainWindow::create3DViewModule()
{
    dw_3dview = new QDockWidget();
    dw_3dview->setWindowTitle( "3D Window" );

    dc_3DView_Module = new View3DModule ( this );
    dw_3dview->setWidget( dc_3DView_Module );

    dw_3dview->setAllowedAreas( Qt::AllDockWidgetAreas );
    this->addDockWidget( Qt::BottomDockWidgetArea, dw_3dview );

    dc_3DView_Module->glWidget->updateSeismicSlices( seismic_window->getSeimicSlices() );

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

    connect ( dc_3DView_Module->horizontalSlider_curve, SIGNAL( valueChanged( int ) ) , dc_3DView_Module->glWidget , SLOT( black_screen_stepx( int ) ) );
    connect ( dc_3DView_Module->horizontalSlider_surface, SIGNAL( valueChanged( int ) ) , dc_3DView_Module->glWidget , SLOT( black_screen_stepz( int ) ) );
    connect ( dc_3DView_Module->horizontalSlider_extrusion, SIGNAL( valueChanged( int ) ) , dc_3DView_Module->glWidget , SLOT( black_screen_volumeWidth( int ) ) );

}




void MainWindow::createFlowDiagnosticsModule()
{

    dw_flowdiagnostics = new QDockWidget();
    dw_flowdiagnostics->setWindowTitle( "Flow Diagnostics Window" );


    flowdiagnostics_window = new FlowWindow();
    dw_flowdiagnostics->setWidget( flowdiagnostics_window );


    dw_flowdiagnostics->setAllowedAreas( Qt::AllDockWidgetAreas );
    this->addDockWidget( Qt::BottomDockWidgetArea, dw_flowdiagnostics );

}


void MainWindow::createFlowDiagnosticsMenuBar()
{
    mn_windows->addAction( ac_flowdiagnostics );
    ac_flowdiagnostics->setCheckable( true );
}


void MainWindow::createFlowDiagnosticsActions()
{
    ac_flowdiagnostics = new QAction ( tr ( "Window Flow Computation" ) , this );
    ac_flowdiagnostics->setCheckable ( true );

    connect ( ac_flowdiagnostics , SIGNAL( toggled(bool) ) , dw_flowdiagnostics , SLOT( setVisible(bool) ) );
    connect( dw_flowdiagnostics, &QDockWidget::visibilityChanged, ac_flowdiagnostics, &QAction::setChecked );
    connect( flowdiagnostics_window, &FlowWindow::getCrossSection, this, [=](){ flowdiagnostics_window->setCrossSection( dc_sketching_module->sketching_canvas->sketch_controller->getCrossSection() ); } );

}




void MainWindow::interpolate( )
{
    // precisa????
    std::cout << seismic_window->getSeimicSlices().size() << std::endl;

    seismic_window->updateCrossSection( dc_sketching_module->sketching_canvas->sketch_controller->getCrossSection() ) ;

    dc_3DView_Module->glWidget->updateSeismicSlices( seismic_window->getSeimicSlices() );
    dc_3DView_Module->glWidget->updateRendering();
}


void MainWindow::clear()
{
    // precisa????
    dc_3DView_Module->glWidget->clear();
}





void MainWindow::keyPressEvent ( QKeyEvent *event )
{
    event->ignore();
}


void MainWindow::mousePressEvent ( QMouseEvent* event )
{
    event->ignore();
}
