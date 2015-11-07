#include "MainWindow.h"

MainWindow::MainWindow ( QWidget *parent ) :
																QMainWindow ( parent )
{
	createWindow ( );

	this->setFocusPolicy ( Qt::StrongFocus );
	this->setFocus ( );
	this->setAcceptDrops ( true );
}

MainWindow::~MainWindow ( )
{
//    canvas2D->clear();
}

void MainWindow::createWindow ( )
{
	this->setMinimumSize ( 800 , 600 );

	createActions ( );
	createMenuBar ( );
	createToolbar ( );
	create2DModule ( );
	create3DModule ( );

	emit setColor ( 0 , 0 , 128 );

	/// Status Bar view
	this->status_bar_ = new QStatusBar ( this );
	this->status_text = new QLabel ( "Sketch" , this );
	this->cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::Sketch );
	this->status_bar_->addWidget ( status_text );
	this->setStatusBar ( this->status_bar_ );

}

void MainWindow::createActions ( )
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
	ac_window3d = new QAction ( tr ( "Window 3D" ) , this );

	connect ( ac_new , SIGNAL( triggered() ) , this , SLOT( newSection() ) );
	connect ( ac_removeabove , SIGNAL( triggered() ) , this , SLOT( applyRemoveAbove() ) );
	connect ( ac_removebelow , SIGNAL( triggered() ) , this , SLOT( applyRemoveBelow() ) );
	connect ( ac_select , SIGNAL( triggered() ) , this , SLOT( pointerSelection() ) );
	connect ( ac_exit , SIGNAL( triggered() ) , this , SLOT( close() ) );

	/// Sketching
	ac_newBoundary = new QAction ( tr ( "Sketch a New Boundary" ) , this );
	ac_newBoundary->setShortcut ( QKeySequence ( tr ( "Ctrl+B" ) ) );

	//connect(this , SIGNAL( newBoundarySignal() ), this->, SLOT( newBoundarySlot()) );

}

void MainWindow::createMenuBar ( )
{

	QMenu *mn_file = menuBar ( )->addMenu ( tr ( "&File" ) );
	mn_file->addAction ( ac_new );
	mn_file->addAction ( ac_open );
	mn_file->addAction ( ac_save );
	mn_file->addAction ( ac_export );
	mn_file->addAction ( ac_exit );

	QMenu *mn_tools = menuBar ( )->addMenu ( tr ( "&Tools" ) );
	mn_tools->addAction ( ac_newBoundary );

	QMenu *mn_windows = menuBar ( )->addMenu ( tr ( "&Windows" ) );
	mn_windows->addAction ( ac_wdwsketching );
	mn_windows->addAction ( ac_window3d );

	ac_wdwsketching->setCheckable ( true );
	ac_window3d->setCheckable ( true );

	QMenu *mn_help = menuBar ( )->addMenu ( tr ( "&Help" ) );
	mn_help->addAction ( ac_contents );
	mn_help->addAction ( ac_about );

}

void MainWindow::createToolbar ( )
{
	tlb_section = addToolBar ( tr ( "Section" ) );
	tlb_section->addAction ( ac_new );
	tlb_section->addAction ( ac_open );
	tlb_section->addAction ( ac_save );
	tlb_section->addAction ( ac_export );


	tlb_rules = addToolBar ( tr ( "Rules" ) );
	tlb_rules->addAction ( ac_removeabove );
	tlb_rules->addAction ( ac_removebelow );

	tbt_colorsketch = new QToolButton;
	tbt_colorsketch->setPopupMode ( QToolButton::MenuButtonPopup );
	tbt_colorsketch->setIcon ( QIcon ( ":/images/icons/border_color.png" ) );

	mn_pickercolor = new QMenu ( );
	mn_pickercolor->addAction ( ac_sketchcolor );
	tbt_colorsketch->setMenu ( mn_pickercolor );

	tlb_customization = addToolBar ( tr ( "Customize" ) );
	tlb_customization->addAction ( ac_select );
	tlb_customization->addWidget ( tbt_colorsketch );

	connect ( mn_pickercolor , SIGNAL( aboutToShow() ) , cd_pickercolor , SLOT( show() ) );
	connect ( cd_pickercolor , SIGNAL( rejected() ) , mn_pickercolor , SLOT( hide() ) );
	connect ( cd_pickercolor , SIGNAL( accepted() ) , this , SLOT( changeColorLine() ) );

}

void MainWindow::create2DModule ( )
{

	dc_2DModule = new QDockWidget ( this );
	dc_2DModule->setAllowedAreas ( Qt::LeftDockWidgetArea );
	dc_2DModule->setWindowTitle ( "2D View" );

	/// XXX Just the Canvas
	this->sketch_board_ = new SketchBoard ( );
	/// XXX GraphScene where we can add Entities ( Curves, Icons ... )
	this->sketchSession_ = new SketchSession ( sketch_board_ );
	this->sketchSession_->initialization ( 0.0 , 0.0 , 700 , 400 );  /// THE VIEW
	this->cross_section_.initialization ( 0.0 , 0.0 , 700 , 400 );   /// THE MODEL
	/// FIXME
	this->sketch_board_->setScene ( sketchSession_ );
	this->setCentralWidget ( sketch_board_ );

	dc_2DModule->setWidget ( sketch_board_ );

	addDockWidget ( Qt::LeftDockWidgetArea , dc_2DModule );
}

void MainWindow::newSection ( )
{

	this->sketchSession_->initialization ( 0.0 , 0.0 , 700 , 400 );  /// THE VIEW
	this->cross_section_.initialization ( 0.0 , 0.0 , 700 , 400 );   /// THE MODEL
//      clearCanvas2D ( );
//      clearCanvas3D();
//      clearComputation();
}

void MainWindow::applyRemoveAbove ( )
{
	bool flag = ac_removeabove->isChecked ( );
	if ( flag == false )
		return;

	ac_removebelow->setChecked ( false );
	emit applyremoveabove ( );

	statusBar ( )->showMessage ( "Applying remove above intersection" );

}

void MainWindow::applyRemoveBelow ( )
{
	bool flag = ac_removebelow->isChecked ( );
	if ( flag == false )
		return;

	ac_removeabove->setChecked ( false );
	emit applyremovebelow ( );

	statusBar ( )->showMessage ( "Applying remove below intersection" );
}

void MainWindow::pointerSelection ( )
{
	bool flag = ac_select->isChecked ( );

	if ( flag == false )
	{

		return;
	}

	emit selectMode ( );

}

void MainWindow::clearCanvas2D ( )
{

}

void MainWindow::changeColorLine ( )
{
	mn_pickercolor->hide ( );

	QColor c = cd_pickercolor->selectedColor ( );
	emit setColor ( c.red ( ) , c.green ( ) , c.blue ( ) );

}

