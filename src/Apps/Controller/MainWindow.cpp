#include "MainWindow.h"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
	this->setFocusPolicy ( Qt::StrongFocus );
	this->setFocus ( );
	this->setAcceptDrops ( true );

	this->setMinimumSize ( 800 , 600 );

	create3DModule ( );
	create2DModule ( );

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

	mn_sketching = menuBar ( )->addMenu ( tr ( "&Sketching" ) );

	ac_newBoundary = new QAction ( tr ( "Sketch a New Boundary" ) , this );
	ac_newBoundary->setShortcut ( QKeySequence ( tr ( "Ctrl+B" ) ) );

	mn_sketching->addAction ( ac_newBoundary );

	mn_windows = menuBar ( )->addMenu ( tr ( "&Windows" ) );
	mn_help = menuBar ( )->addMenu ( tr ( "&Help" ) );
}

void MainWindow::createMainWindowMenuBar ( )
{

	mn_file->addAction ( ac_new );
	mn_file->addAction ( ac_open );
	mn_file->addAction ( ac_save );
	mn_file->addAction ( ac_export );
	mn_file->addAction ( ac_exit );
	mn_help->addAction ( ac_contents );
	mn_help->addAction ( ac_about );

}

void MainWindow::createMainWindowToolbar ( )
{
	tlb_section = addToolBar ( tr ( "Section" ) );
	tlb_section->addAction ( ac_new );
//	tlb_section->addAction ( ac_open );
//	tlb_section->addAction ( ac_save );
//	tlb_section->addAction ( ac_export );

}

void MainWindow::createMainWindowActions ( )
{
	ac_new = new QAction ( tr ( "&New" ) , this );
	ac_new->setIcon ( QIcon ( ":/images/icons/page_white.png" ) );

	ac_open = new QAction ( tr ( "&Open" ) , this );
	ac_open->setIcon ( QIcon ( ":/images/icons/folder.png" ) );
	ac_open->setDisabled ( true );

	ac_save = new QAction ( tr ( "&Save" ) , this );
	ac_save->setIcon ( QIcon ( ":/images/icons/diskette.png" ) );
	ac_save->setDisabled ( true );

	ac_export = new QAction ( tr ( "&Export" ) , this );
	ac_export->setIcon ( QIcon ( ":/images/icons/document_export.png" ) );
	ac_export->setDisabled ( true );

	ac_exit = new QAction ( tr ( "E&xit" ) , this );
	ac_exit->setIcon ( QIcon ( ":/images/icons/door_out.png" ) );

	ac_contents = new QAction ( tr ( "Contents" ) , this );
	ac_contents->setDisabled ( true );

	ac_about = new QAction ( tr ( "&About" ) , this );

	// About Widget
	aboutRRM = new AboutWidget( this );
	connect(ac_about, SIGNAL(triggered()), aboutRRM, SLOT(show()));

	connect ( ac_exit , SIGNAL( triggered() ) , this , SLOT( close() ) );

}

//void MainWindow::newSection ( )
//{
////	this->sketchSession_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View
////	this->cross_section_.initialization ( 0.0 , 0.0 , 700 , 400 );   // The  Model
//}

void MainWindow::changeColorLine ( )
{
}

void MainWindow::createSketchingMenuBar ( )
{

	mn_windows->addAction ( ac_wdwsketching );
	ac_wdwsketching->setCheckable ( true );

}

void MainWindow::createSketchingToolbar ( )
{

//	tlb_rules = addToolBar ( tr ( "Rules" ) );
//	tlb_rules->addAction ( ac_removeabove );
//	tlb_rules->addAction ( ac_removebelow );
//
//	tbt_colorsketch = new QToolButton;
//	tbt_colorsketch->setPopupMode ( QToolButton::MenuButtonPopup );
//	tbt_colorsketch->setIcon ( QIcon ( ":/images/icons/border_color.png" ) );
//
//	mn_pickercolor = new QMenu ( );
//	mn_pickercolor->addAction ( ac_sketchcolor );
//	tbt_colorsketch->setMenu ( mn_pickercolor );
//
//	tlb_customization = addToolBar ( tr ( "Customize" ) );
//	tlb_customization->addAction ( ac_select );
//	tlb_customization->addWidget ( tbt_colorsketch );

//	connect ( mn_pickercolor , SIGNAL( aboutToShow() ) , cd_pickercolor , SLOT( show() ) );
//	connect ( cd_pickercolor , SIGNAL( rejected() ) , mn_pickercolor , SLOT( hide() ) );
//	connect ( cd_pickercolor , SIGNAL( accepted() ) , this , SLOT( changeColorLine() ) );

}

void MainWindow::createSketchingActions ( )
{
	ac_removeabove = new QAction ( tr ( "Remove Above Intersection" ) , this );

	ac_removeabove->setIcon ( QIcon ( ":/images/icons/removeabove.png" ) );
	ac_removeabove->setCheckable ( true );

	ac_removebelow = new QAction ( tr ( "Remove Below Intersection" ) , this );

	ac_removebelow->setIcon ( QIcon ( ":/images/icons/removebelow.png" ) );
	ac_removebelow->setCheckable ( true );

	ac_select = new QAction ( tr ( "Select" ) , this );

	ac_select->setIcon ( QIcon ( ":/images/icons/pointer.png" ) );
	ac_select->setCheckable ( true );

	cd_pickercolor = new QColorDialog ( );
	cd_pickercolor->setWindowFlags ( Qt::Widget );
	cd_pickercolor->setCurrentColor ( QColor ( 0 , 0 , 128 ) );

	ac_sketchcolor = new QWidgetAction ( this );
	ac_sketchcolor->setDefaultWidget ( cd_pickercolor );

	ac_wdwsketching = new QAction ( tr ( "Window Sketching" ) , this );
	ac_wdwsketching->setCheckable ( true );
	ac_wdwsketching->setChecked ( true );

//	connect ( ac_removeabove , SIGNAL( triggered() ) , dc_2DModule , SLOT( applyRemoveAbove() ) );
//	connect ( ac_removebelow , SIGNAL( triggered() ) , dc_2DModule , SLOT( applyRemoveBelow() ) );

	connect ( ac_select , SIGNAL( toggled(bool) ) , dc_2DModule , SLOT( pointerSelection( bool ) ) );
	connect ( ac_wdwsketching , SIGNAL( toggled(bool) ) , dc_2DModule , SLOT( setVisible(bool) ) );

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
}

