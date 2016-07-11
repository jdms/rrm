#include "Sketching2DModule.h"

Sketching2DModule::Sketching2DModule ( QWidget* parent )
{
	createWindow ( );
	createActions ( parent );
	createConnections();

	ac_sketch->setChecked(true);
			
	emit setColor ( 0.0f , 0.0f , 128.0f );
}

void Sketching2DModule::setCentralWidget(QWidget * _centra_widget)
{
	if (mainWidonw_widget_)
	{
		mainWidonw_widget_->setCentralWidget(_centra_widget);
	}

}

void Sketching2DModule::createWindow ( )
{
	this->mainWidonw_widget_ = new QMainWindow();
	this->setWidget(this->mainWidonw_widget_);
	this->setAllowedAreas ( Qt::LeftDockWidgetArea );
	this->setWindowTitle("Sketch View");

	// StatusBar Text	
	//this->label_status_bar_text_ = new QLabel();
	//this->mainWidonw_widget_->statusBar()->addWidget(label_status_bar_text_);
	
	sketch_board_ = new SketchBoard();
	// Default Sketch Window
	sketch_board_->newSession(0.0, 0.0, 700, 400);
	this->mainWidonw_widget_->setCentralWidget(sketch_board_);

	sketch_board_->status_text_->setAlignment(Qt::AlignLeft);
	sketch_board_->sketchSession_->coordinates_->setAlignment(Qt::AlignRight);

	this->mainWidonw_widget_->statusBar()->addWidget(sketch_board_->status_text_);
	this->mainWidonw_widget_->statusBar()->addPermanentWidget(sketch_board_->sketchSession_->coordinates_);
}

void Sketching2DModule::createActions ( QWidget* parent )
{
	// Section
	ac_new = new QAction(tr("&New"), this);
	ac_new->setIcon(QIcon(":/images/icons/page_white.png"));
	ac_new->setShortcut(Qt::Key_N);

	ac_insertCurve = new QAction(tr("&Insert Curve"), this);
	ac_insertCurve->setIcon(QIcon(":/images/icons/InsertCurve.png"));
	ac_insertCurve->setShortcut(Qt::Key_I);

	ac_denyCurve = new QAction(tr("&Undo Curve"), this);
	ac_denyCurve->setIcon(QIcon(":/images/icons/denyCurve.png"));

	ac_undo = new QAction(tr("&Undo"), this);
	ac_undo->setIcon(QIcon(":/images/icons/undo.png"));

	ac_redo = new QAction(tr("&Redo"), this);
	ac_redo->setIcon(QIcon(":/images/icons/redo.png"));

	// Section
	ac_newBoundary = new QAction(tr("&New Boundary"), this);
	ac_newBoundary->setIcon(QIcon(":/images/icons/NewBoundary.png"));
	ac_newBoundary->setShortcut(QKeySequence::New);

	ac_screenShot = new QAction(tr("&Screenshot"), this);
	ac_screenShot->setIcon(QIcon(":/images/icons/Camera.png"));
	ac_screenShot->setShortcut(Qt::Key_P);

	ac_removeabove = new QAction(tr("Remove Above Intersection"), this);
	ac_removeabove->setIcon(QIcon(":/images/icons/removeaboveintersection.png"));
	ac_removeabove->setCheckable(true);

	ac_removebelow = new QAction(tr("Remove Below Intersection"), this);
	ac_removebelow->setIcon(QIcon(":/images/icons/removebelowintersection.png"));
	ac_removebelow->setCheckable(true);

	ac_sketch = new QAction(tr("Sketch"), this);
	ac_sketch->setIcon(QIcon(":/images/icons/sketchmode.png"));
	ac_sketch->setCheckable(true);

	ac_region_point_ = new QAction(tr("Set Region Point"), this);
	ac_region_point_->setIcon(QIcon(":/images/icons/regionPointmode.png"));
	ac_region_point_->setCheckable(true);
	
	tlb_rules = this->mainWidonw_widget_->addToolBar(tr("Rules"));//addToolBar ( tr ( "Rules" ) );
	action_group_rules_ = new QActionGroup(tlb_rules);

	action_group_rules_->addAction(ac_removeabove);
	action_group_rules_->addAction(ac_removebelow);
	action_group_rules_->addAction(ac_sketch);
	action_group_rules_->addAction(ac_region_point_);
	action_group_rules_->setExclusive(true);

	tlb_rules->addAction(ac_sketch);
	tlb_rules->addAction(ac_removeabove);
	tlb_rules->addAction(ac_removebelow);
	//tlb_rules->addAction(ac_region_point_);
		
	tlb_section = this->mainWidonw_widget_->addToolBar(tr("Section"));
	tlb_section->addAction(ac_new);
	tlb_section->addAction(ac_newBoundary);
	tlb_section->addAction(ac_screenShot);
	tlb_section->addAction(ac_insertCurve);
	tlb_section->addAction(ac_denyCurve);
	tlb_section->addAction(ac_undo);
	tlb_section->addAction(ac_redo);

	//ac_select = new QAction(tr("Select"), this);

	//ac_select->setIcon(QIcon(":/images/icons/pointer.png"));
	//ac_select->setCheckable(true);

	// Costumization
	cd_pickercolor = new QColorDialog();
	cd_pickercolor->setWindowFlags(Qt::Widget);
	cd_pickercolor->setCurrentColor(QColor(0, 0, 128));

	ac_sketchcolor = new QWidgetAction(this);
	ac_sketchcolor->setDefaultWidget(cd_pickercolor);
	
	tbt_colorsketch = new QToolButton;
	tbt_colorsketch->setPopupMode(QToolButton::MenuButtonPopup);
	tbt_colorsketch->setIcon(QIcon(":/images/icons/border_color.png"));
	
	mn_pickercolor = new QMenu();
	mn_pickercolor->addAction(ac_sketchcolor);
	tbt_colorsketch->setMenu(mn_pickercolor);
	
	tlb_customization = getMainWidow()->addToolBar(tr("Customize"));
	//tlb_customization->addAction(ac_select);
	tlb_customization->addWidget(tbt_colorsketch);

}

void Sketching2DModule::createConnections()
{
	connect(Sketching2DModule::ac_removebelow, &QAction::toggled, [=](bool isToogled) { if (isToogled) { this->sketch_board_->setModeRemoveBelowIntersection(); } });
	connect(Sketching2DModule::ac_removeabove, &QAction::toggled, [=](bool isToogled) { if (isToogled) { this->sketch_board_->setModeRemoveAboveIntersection(); } });
	connect(Sketching2DModule::ac_sketch, &QAction::toggled, [=](bool isToogled) { if (isToogled) { this->sketch_board_->setModeSketch(); } });
	connect(Sketching2DModule::ac_region_point_, &QAction::toggled, [=](bool isToogled) { if (isToogled) { this->sketch_board_->setModeRegionPoint(); } });
	// Color
	connect(Sketching2DModule::mn_pickercolor, &QMenu::aboutToShow, Sketching2DModule::cd_pickercolor, &QColorDialog::show );
	connect(Sketching2DModule::cd_pickercolor, &QColorDialog::rejected, Sketching2DModule::mn_pickercolor, &QMenu::hide);
	connect(Sketching2DModule::cd_pickercolor, &QColorDialog::rejected, Sketching2DModule::mn_pickercolor, &QMenu::hide);
	connect(Sketching2DModule::cd_pickercolor, &QColorDialog::colorSelected, [=](const QColor& _color){ sketch_board_->sketchSession_->setColor(_color); mn_pickercolor->hide(); });

	// Selection
	//connect(Sketching2DModule::ac_select, &QAction::toggled, [=](bool isToogled) { if (isToogled) { this->sketch_board_->setModeEdition(); } });
	connect(Sketching2DModule::ac_new, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::clear);
	connect(Sketching2DModule::ac_screenShot, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::screenShot);
	connect(Sketching2DModule::ac_newBoundary, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::newBoundary);
	connect(Sketching2DModule::ac_insertCurve, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::newSketch);

	connect(Sketching2DModule::ac_denyCurve, &QAction::triggered, Sketching2DModule::sketch_board_->sketchSession_, &SketchSessionTesting::clearSketch);

	connect(Sketching2DModule::ac_undo, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::undo);
	connect(Sketching2DModule::ac_redo, &QAction::triggered, Sketching2DModule::sketch_board_, &SketchBoard::redo);


}

QMainWindow * Sketching2DModule::getMainWidow()
{
	return mainWidonw_widget_;
}

QLabel * Sketching2DModule::getStatusBarText()
{
	return label_status_bar_text_;
}

void Sketching2DModule::applyRemoveAbove ( )
{
//    bool flag = ac_removeabove->isChecked();
//    if( flag == false ) return;

//    ac_removebelow->setChecked( false );
	emit applyremoveabove ( );
//    statusBar()->showMessage( "Applying remove above intersection" );

}

void Sketching2DModule::applyRemoveBelow ( )
{
//    bool flag = ac_removebelow->isChecked();
//    if( flag == false ) return;

//    ac_removeabove->setChecked( false );
	emit applyremovebelow ( );

//    statusBar()->showMessage( "Applying remove below intersection" );
}

void Sketching2DModule::pointerSelection ( bool flag )
{
//    bool flag = ac_select->isChecked();

	if ( flag == false )
	{
		emit sketchingMode ( );
		return;
	}

	emit selectMode ( );
}

void Sketching2DModule::emitColor ( int R , int G , int B )
{

	emit setColor ( R , G , B );
}

void Sketching2DModule::emitModeSelect ( )
{
	emit selectMode ( );
}

void Sketching2DModule::emitModeSketching ( )
{
	emit sketchingMode ( );
}

void Sketching2DModule::clearCanvas2D ( )
{

}
