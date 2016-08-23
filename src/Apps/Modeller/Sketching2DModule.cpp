#include "Sketching2DModule.h"

SketchingWindow::SketchingWindow(QWidget* parent)
{
	createWindow();
    createActions();
	createConnections();
}


void SketchingWindow::createWindow()
{

    setWindowTitle( "Sketch View" );

    sketching_canvas = new SketchBoard();
    sketching_canvas->newSession( 0.0, 0.0, 700, 400 );


    sketching_canvas_ = new SketchingCanvas();
    setCentralWidget( sketching_canvas_ );

    lb_statusbar_status = new QLabel();

    statusBar()->addWidget( lb_statusbar_status );
    statusBar()->addPermanentWidget( sketching_canvas_->lb_coordinates );

}


void SketchingWindow::createActions()
{

    // Section
    ac_removeabove = new QAction( tr( "Remove Above Intersection" ), this );
    ac_removeabove->setIcon( QIcon( ":/images/icons/removeaboveintersection.png" ) );
    ac_removeabove->setCheckable( true );

    ac_removebelow = new QAction(tr( "Remove Below Intersection" ), this );
    ac_removebelow->setIcon( QIcon( ":/images/icons/removebelowintersection.png" ) );
    ac_removebelow->setCheckable( true );

    ac_sketch = new QAction( tr( "Sketch" ), this );
    ac_sketch->setIcon( QIcon( ":/images/icons/sketchmode.png" ) );
    ac_sketch->setCheckable( true );
    ac_sketch->setChecked( true );


    tlb_rules = addToolBar( tr("Rules") );//addToolBar ( tr ( "Rules" ) );

    action_group_rules = new QActionGroup( tlb_rules );
    action_group_rules->addAction( ac_removeabove );
    action_group_rules->addAction( ac_removebelow );
    action_group_rules->addAction( ac_sketch );
    action_group_rules->setExclusive( true );

    tlb_rules->addActions( action_group_rules->actions() );


    // New/Edit scene
    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( ":/images/icons/page_white.png" ) );
    ac_new->setShortcut( Qt::Key_N );

    ac_new_boundary = new QAction( tr( "&New Boundary" ), this );
    ac_new_boundary->setIcon( QIcon( ":/images/icons/NewBoundary.png" ) );
    ac_new_boundary->setShortcut( QKeySequence::New );


    ac_add_sketch = new QAction( tr( "&Insert Curve" ), this );
    ac_add_sketch->setIcon( QIcon( ":/images/icons/InsertCurve.png" ) );
    ac_add_sketch->setShortcut( Qt::Key_I );

    ac_deny_curve = new QAction( tr( "&Undo Curve" ), this );
    ac_deny_curve->setIcon( QIcon( ":/images/icons/denyCurve.png" ) );

    ac_undo = new QAction( tr("&Undo" ), this );
    ac_undo->setIcon( QIcon( ":/images/icons/undo.png" ) );

    ac_redo = new QAction( tr( "&Redo" ), this );
    ac_redo->setIcon(QIcon( ":/images/icons/redo.png" ) );


    tlb_section = addToolBar( tr( "Section" ) );
    tlb_section->addAction( ac_new );
    tlb_section->addAction( ac_new_boundary );
    tlb_section->addAction( ac_add_sketch );
    tlb_section->addAction( ac_deny_curve );
    tlb_section->addAction( ac_undo );
    tlb_section->addAction( ac_redo );



    /*
    ac_region_point = new QAction( tr( "Set Region Point" ), this );
    ac_region_point->setIcon(QIcon( ":/images/icons/regionPointmode.png"  ));
    ac_region_point->setCheckable(true);
    */




	// Costumization
	cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 0, 0, 128 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );

	tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( ":/images/icons/border_color.png" ) );

	mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_sketchcolor );
    tbt_colorsketch->setMenu( mn_pickercolor );

    ac_screenshot = new QAction( tr( "&Screenshot" ), this );
    ac_screenshot->setIcon( QIcon( ":/images/icons/Camera.png" ) );
    ac_screenshot->setShortcut( Qt::Key_P );

    tlb_customization = addToolBar( tr( "Customize" ) );
    tlb_customization->addAction( ac_screenshot );
    tlb_customization->addWidget( tbt_colorsketch );


}


void SketchingWindow::createConnections()
{

    /*


    // Refactoring

    connect( ac_removeabove, &QAction::triggered, [=](){ controller->updateRule( "REMOVEABOVE" ); lb_statusbar_status->setText( "Remove Above Intersection" ); } );
    connect( ac_removebelow, &QAction::triggered, [=](){ controller->updateRule( "REMOVEBELOW" ); lb_statusbar_status->setText( "Remove Below Intersection" ); } );
    connect( ac_sketch, &QAction::triggered, [=](){ controller->updateRule( "SKETCH" ); lb_statusbar_status->setText( "Sketch" ); } );

    connect( ac_undo, &QAction::triggered, controller, &Controller::undo );
    connect( ac_redo, &QAction::triggered, controller, &Controller::redo );


    connect( this->parent(), SIGNAL( sendController( Controller& ) ), [=]( Controller& cont ){ sc->setController( cont ); } );

*/

    connect( mn_pickercolor, &QMenu::aboutToShow, cd_pickercolor, &QColorDialog::show );
    connect( cd_pickercolor, &QColorDialog::rejected, mn_pickercolor, &QMenu::hide );
    connect( cd_pickercolor, &QColorDialog::colorSelected, sketching_canvas_, &SketchingCanvas::updateColor );
    connect( cd_pickercolor, &QColorDialog::accepted, mn_pickercolor, &QMenu::hide );

    connect( ac_screenshot, &QAction::triggered, sketching_canvas_, &SketchingCanvas::screenshot );
    connect( ac_add_sketch, &QAction::triggered, sketching_canvas_, &SketchingCanvas::insertSketch );
    connect( ac_deny_curve, &QAction::triggered, sketching_canvas_, &SketchingCanvas::undoLastSketch );

    connect( ac_new_boundary, &QAction::triggered, [=](){ emit sketching_canvas_->newBoundary(); } );


}
