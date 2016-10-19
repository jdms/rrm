#include "SketchingWindow.h"


SketchingWindow::SketchingWindow( QWidget* parent )
{
	createWindow();
    createActions();
	createConnections();
}


void SketchingWindow::createWindow()
{

    setWindowTitle( "Sketch View" );


    sketching_canvas = new SketchingCanvas();
    setCentralWidget( sketching_canvas );

    lb_statusbar_status = new QLabel();

    statusBar()->addWidget( lb_statusbar_status );
    statusBar()->addPermanentWidget( sketching_canvas->lb_coordinates );

}


void SketchingWindow::createActions()
{



    ac_sketchabove = new QAction( tr( "Sketch Above" ), this );
    ac_sketchabove->setCheckable( true );

    ac_sketchbelow = new QAction( tr( "Sketch Below" ), this );
    ac_sketchbelow->setCheckable( true );


    ac_sketch = new QAction( tr( "Sketch" ), this );
    ac_sketch->setIcon( QIcon( ":/images/icons/sketchmode.png" ) );
    ac_sketch->setCheckable( true );
    ac_sketch->setChecked( true );


    // Section
    ac_removeabove = new QAction( tr( "Remove Above" ), this );
    ac_removeabove->setCheckable( true );


    ac_removeaboveintersection = new QAction( tr( "Remove Above Intersection" ), this );
    ac_removeaboveintersection->setIcon( QIcon( ":/images/icons/removeaboveintersection.png" ) );
    ac_removeaboveintersection->setCheckable( true );


    ac_removebelow = new QAction(tr( "Remove Below" ), this );
    ac_removebelow->setCheckable( true );

    ac_removebelowintersection = new QAction( tr( "Remove Below Intersection" ), this );
    ac_removebelowintersection->setIcon( QIcon( ":/images/icons/removeBelowintersection.png" ) );
    ac_removebelowintersection->setCheckable( true );




    ag_sketching_rules = new QActionGroup( this );
    ag_sketching_rules->addAction( ac_sketchabove );
    ag_sketching_rules->addAction( ac_sketchbelow );
    ag_sketching_rules->setExclusive( false );


    QAction* ac_separator = new QAction( this );
    ac_separator->setSeparator( true );

    ag_remove_rules = new QActionGroup( this );
    ag_remove_rules->addAction( ac_sketch );
    ag_remove_rules->addAction( ac_separator );
    ag_remove_rules->addAction( ac_removeabove );
    ag_remove_rules->addAction( ac_removeaboveintersection );
    ag_remove_rules->addAction( ac_removebelow );
    ag_remove_rules->addAction( ac_removebelowintersection );
    ag_remove_rules->setExclusive( true );


    tlb_rules = addToolBar( tr("Rules") );
    tlb_rules->addActions( ag_sketching_rules->actions() );
    tlb_rules->addSeparator();
    tlb_rules->addActions( ag_remove_rules->actions() );




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
    tlb_section->addSeparator();
    tlb_section->addAction( ac_undo );
    tlb_section->addAction( ac_redo );



	// Costumization

    ac_randomcolor = new QAction( tr("&Random Color" ), this );
    ac_randomcolor ->setCheckable( true );


	cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 0, 0, 128 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );

	tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( ":/images/icons/border_color.png" ) );

	mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_randomcolor );
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



    connect( ac_sketchabove, &QAction::toggled, [=]( bool status ){ emit defineSketchingAbove( status ); lb_statusbar_status->setText( "Sketching Above" ); uncheckRemoveRules(); } );
    connect( ac_sketchbelow, &QAction::toggled, [=]( bool status ){ emit defineSketchingBelow( status ); lb_statusbar_status->setText( "Sketching Below" ); uncheckRemoveRules(); } );


    connect( ac_sketch, &QAction::toggled, [=](){ emit updateStratigraphicRule( "SKETCHING" ); lb_statusbar_status->setText( "Sketch" ); } );


    connect( ac_removeabove, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RA_SKETCHING" );  lb_statusbar_status->setText( "Remove Above" ); } );
    connect( ac_removeaboveintersection, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RAI_SKETCHING" );  lb_statusbar_status->setText( "Remove Above Intersection" ); } );
    connect( ac_removebelow, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RB_SKETCHING" ); lb_statusbar_status->setText( "Remove Below" ); } );
    connect( ac_removebelowintersection, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RBI_SKETCHING" ); lb_statusbar_status->setText( "Remove Below Intersection" ); } );


    connect( ac_new, &QAction::triggered, [=](){ emit clear(); } );
    connect( ac_undo, &QAction::triggered, [=](){ emit undo(); } );
    connect( ac_redo, &QAction::triggered, [=](){ emit redo(); } );


    connect( mn_pickercolor, &QMenu::aboutToShow, cd_pickercolor, &QColorDialog::show );
    connect( cd_pickercolor, &QColorDialog::rejected, mn_pickercolor, &QMenu::hide );    
    connect( cd_pickercolor, &QColorDialog::accepted, mn_pickercolor, &QMenu::hide );
    connect( cd_pickercolor, &QColorDialog::colorSelected, [=]( const QColor& c ){ emit updateColor( c ); } );

    connect( ac_screenshot, &QAction::triggered, sketching_canvas, &SketchingCanvas::screenshot );

    connect( ac_add_sketch, &QAction::triggered, [=](){ emit addStratigraphy(); } );
    connect( ac_deny_curve, &QAction::triggered, [=](){ emit undoLastSketch(); } );
    connect( ac_new_boundary, &QAction::triggered, [=](){ emit setCurrentMode( Scene::InteractionMode::BOUNDARY ); } );


    connect( ac_randomcolor, &QAction::triggered, [=]( bool status ){ emit setRandomColor( status ); } );


}

