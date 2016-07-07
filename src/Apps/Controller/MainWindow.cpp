#include "MainWindow.h"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
	this->setFocusPolicy ( Qt::StrongFocus );
	this->setFocus ( );
	this->setAcceptDrops ( true );

	this->setMinimumSize ( 800 , 600 );

	create2DModule ( );
	create3DModule ( );

	createMenuBar ( );
	createMainWindowActions ( );
	createMainWindowMenuBar ( );
	createMainWindowToolbar ( );

	createSketchingActions ( );
	createSketchingMenuBar ( );
	createSketchingToolbar ( );

	create3DWindowActions ( );
	create3DWindowMenuBar ( );

	createFlowComputationModule ( );
	createFlowComputationActions ( );
	createFlowComputationMenuBar ( );
}

MainWindow::~MainWindow ( )
{

}

void MainWindow::createWindow ( )
{

}

void MainWindow::createMenuBar ( )
{
	mn_file = menuBar ( )->addMenu ( tr ( "&File" ) );

	//mn_sketching = menuBar ( )->addMenu ( tr ( "&Sketching" ) );

	//ac_newBoundary = new QAction ( tr ( "Sketch a New Boundary" ) , this );
	//ac_newBoundary->setShortcut ( QKeySequence ( tr ( "Ctrl+B" ) ) );

	//mn_sketching->addAction ( ac_newBoundary );

	mn_windows = menuBar ( )->addMenu ( tr ( "&Windows" ) );
	mn_help = menuBar ( )->addMenu ( tr ( "&Help" ) );
}

void MainWindow::createMainWindowMenuBar ( )
{

	//mn_file->addAction ( ac_new );
	//mn_file->addAction ( ac_open );
	//mn_file->addAction ( ac_save );
	//mn_file->addAction ( ac_export );
	mn_file->addAction ( ac_exit );
	//mn_help->addAction ( ac_contents );
	mn_help->addAction ( ac_about );

}

void MainWindow::createMainWindowToolbar ( )
{

//	tlb_section->addAction ( ac_open );
//	tlb_section->addAction ( ac_save );
//	tlb_section->addAction ( ac_export );

}

void MainWindow::createMainWindowActions ( )
{

	//ac_open = new QAction ( tr ( "&Open" ) , this );
	//ac_open->setIcon ( QIcon ( ":/images/icons/folder.png" ) );
	//ac_open->setDisabled ( true );

	//ac_save = new QAction ( tr ( "&Save" ) , this );
	//ac_save->setIcon ( QIcon ( ":/images/icons/diskette.png" ) );
	//ac_save->setDisabled ( true );

	//ac_export = new QAction ( tr ( "&Export" ) , this );
	//ac_export->setIcon ( QIcon ( ":/images/icons/document_export.png" ) );
	//ac_export->setDisabled ( true );

	ac_exit = new QAction ( tr ( "E&xit" ) , this );
	ac_exit->setIcon ( QIcon ( ":/images/icons/door_out.png" ) );

	//ac_contents = new QAction ( tr ( "Contents" ) , this );
	//ac_contents->setDisabled ( true );

	ac_about = new QAction ( tr ( "&About" ) , this );

	// About Widget
	aboutRRM = new AboutWidget( this );
	connect ( ac_about , SIGNAL( triggered() ) , aboutRRM , SLOT( show() ) );
	connect ( ac_exit , SIGNAL( triggered() ) , this , SLOT( close() ) );
	//connect ( ac_new , SIGNAL( triggered(bool) ) , this->sketch_board_ , SLOT( clear() ) );
}

void MainWindow::changeColorLine ( )
{
}

void MainWindow::createSketchingMenuBar ( )
{
	mn_windows->addAction ( ac_wdwsketching );
	ac_wdwsketching->setCheckable ( true );
	//mn_windows->addAction(ac_wdwseismic);
	//ac_wdwseismic->setCheckable(false);
}

void MainWindow::createSketchingToolbar ( )
{


}

void MainWindow::createSketchingActions ( )
{

	ac_wdwsketching = new QAction ( tr ( "Window Sketching" ) , this );
	ac_wdwsketching->setCheckable ( true );
	ac_wdwsketching->setChecked ( true );

	ac_wdwseismic = new QAction(tr("Window Seismic"), this);
	ac_wdwseismic->setCheckable(true);
	ac_wdwseismic->setChecked(true);

//	ac_wdwflow = new QAction(tr("Flow Visualization Window "), this);
//	ac_wdwflow->setCheckable(true);
//	ac_wdwflow->setChecked(true);


//	connect ( ac_removeabove , SIGNAL( triggered() ) , dc_2DModule , SLOT( applyRemoveAbove() ) );
//	connect ( ac_removebelow , SIGNAL( triggered() ) , dc_2DModule , SLOT( applyRemoveBelow() ) );
		
	connect ( ac_wdwsketching , SIGNAL( toggled(bool) ) , dc_2DModule , SLOT( setVisible(bool) ) );
		connect(dc_2DModule, &Sketching2DModule::visibilityChanged, ac_wdwsketching, &QAction::setChecked );
	connect(ac_wdwseismic, SIGNAL(toggled(bool)), dc_Seismic_Module_, SLOT(setVisible(bool)));
		connect(dc_Seismic_Module_, &Sketching2DModule::visibilityChanged, ac_wdwseismic, &QAction::setChecked);

}

void MainWindow::create3DWindowMenuBar ( )
{

	mn_windows->addAction ( ac_window3d );
}

void MainWindow::create3DWindowActions ( )
{
	ac_window3d = new QAction ( tr ( "Window 3D" ) , this );
	ac_window3d->setCheckable ( true );
	ac_window3d->setChecked ( true );
		
	connect ( ac_window3d , SIGNAL( toggled(bool) ) , dc_3DModule , SLOT( setVisible(bool) ) );
	connect(dc_3DModule, &View3DModule::visibilityChanged, ac_window3d, &QAction::setChecked);
}

