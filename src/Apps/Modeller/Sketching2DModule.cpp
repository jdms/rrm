#include "Sketching2DModule.h"

Sketching2DModule::Sketching2DModule(QWidget* parent)
{
	createWindow();
    createActions();
	createConnections();

	emit setColor(0.0f, 0.0f, 128.0f);
}


void Sketching2DModule::createWindow()
{

    setWindowTitle( "Sketch View" );

    sketching_canvas = new SketchBoard();
    sketching_canvas->newSession( 0.0, 0.0, 700, 400 );

    setCentralWidget( sketching_canvas );

//    sketching_canvas->status_text_->setAlignment( Qt::AlignLeft );
//    sketching_canvas->sketchSession_->coordinates_->setAlignment( Qt::AlignRight );

//    statusBar()->addWidget( sketching_canvas->status_text_ );
//    statusBar()->addPermanentWidget( sketching_canvas->sketchSession_->coordinates_ );


    lb_statusbar_status = new QLabel();

    statusBar()->addWidget( lb_statusbar_status );
    statusBar()->addPermanentWidget( sketching_canvas->lb_coordinates );

}


void Sketching2DModule::createActions()
{
	// View
    ac_zoom_in = new QAction(tr("&Zoom In"), this);
    ac_zoom_in->setShortcut( QKeySequence::ZoomIn );

    ac_zoom_out = new QAction(tr("&Zoom Out"), this);
    ac_zoom_out->setShortcut(QKeySequence::ZoomOut);

	// Section
    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( ":/images/icons/page_white.png" ) );
    ac_new->setShortcut( Qt::Key_N );

    ac_add_sketch = new QAction( tr( "&Insert Curve" ), this );
    ac_add_sketch->setIcon( QIcon( ":/images/icons/InsertCurve.png" ) );
    ac_add_sketch->setShortcut( Qt::Key_I );

    ac_deny_curve = new QAction( tr( "&Undo Curve" ), this );
    ac_deny_curve->setIcon( QIcon( ":/images/icons/denyCurve.png" ) );

    ac_undo = new QAction( tr("&Undo" ), this );
    ac_undo->setIcon( QIcon( ":/images/icons/undo.png" ) );

    ac_redo = new QAction( tr( "&Redo" ), this );
    ac_redo->setIcon(QIcon( ":/images/icons/redo.png" ) );

	// Section
    ac_new_boundary = new QAction( tr( "&New Boundary" ), this );
    ac_new_boundary->setIcon( QIcon( ":/images/icons/NewBoundary.png" ) );
    ac_new_boundary->setShortcut( QKeySequence::New );

    ac_screenshot = new QAction( tr( "&Screenshot" ), this );
    ac_screenshot->setIcon( QIcon( ":/images/icons/Camera.png" ) );
    ac_screenshot->setShortcut( Qt::Key_P );

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

    ac_region_point = new QAction( tr( "Set Region Point" ), this );
    ac_region_point->setIcon(QIcon( ":/images/icons/regionPointmode.png"  ));
    ac_region_point->setCheckable(true);

    tlb_rules = addToolBar( tr("Rules") );//addToolBar ( tr ( "Rules" ) );
    action_group_rules = new QActionGroup( tlb_rules );

	// View
    addAction( ac_zoom_out );
    addAction( ac_zoom_in );

    action_group_rules->addAction( ac_removeabove );
    action_group_rules->addAction( ac_removebelow );
    action_group_rules->addAction( ac_sketch );
    action_group_rules->addAction( ac_region_point );
    action_group_rules->setExclusive( true );

    tlb_rules->addAction( ac_sketch );
    tlb_rules->addAction( ac_removeabove );
    tlb_rules->addAction( ac_removebelow );

    tlb_section = addToolBar( tr( "Section" ) );
    tlb_section->addAction( ac_new );
    tlb_section->addAction( ac_new_boundary );
    tlb_section->addAction( ac_screenshot );
    tlb_section->addAction( ac_add_sketch );
    tlb_section->addAction( ac_deny_curve );
    tlb_section->addAction( ac_undo );
    tlb_section->addAction( ac_redo );

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

    tlb_customization = addToolBar( tr( "Customize" ) );
    tlb_customization->addWidget( tbt_colorsketch );

}


void Sketching2DModule::createConnections()
{

    connect( Sketching2DModule::ac_removebelow, &QAction::toggled, [=]( bool isToogled ) { if( isToogled ) { this->sketching_canvas->setModeRemoveBelowIntersection(); lb_statusbar_status->setText( "Remove Below Intersection" ); } } );
    connect( Sketching2DModule::ac_removeabove, &QAction::toggled, [=]( bool isToogled ) { if( isToogled ) { this->sketching_canvas->setModeRemoveAboveIntersection(); lb_statusbar_status->setText( "Remove Above Intersection" ); } } );
    connect( Sketching2DModule::ac_sketch, &QAction::toggled, [=]( bool isToogled ) { if ( isToogled ) { this->sketching_canvas->setModeSketch(); lb_statusbar_status->setText( "Sketch" ); } } );
    connect( Sketching2DModule::ac_region_point, &QAction::toggled, [=]( bool isToogled ) { if ( isToogled ) { this->sketching_canvas->setModeRegionPoint(); lb_statusbar_status->setText( "Select Region Points" ); } });

    // Color
    connect( Sketching2DModule::mn_pickercolor, &QMenu::aboutToShow, Sketching2DModule::cd_pickercolor, &QColorDialog::show );
    connect( Sketching2DModule::cd_pickercolor, &QColorDialog::rejected, Sketching2DModule::mn_pickercolor, &QMenu::hide );
    connect( Sketching2DModule::cd_pickercolor, &QColorDialog::colorSelected, [=](const QColor& _color){ sketching_canvas->sketchSession_->setColor(_color); mn_pickercolor->hide(); } );


    connect(Sketching2DModule::ac_new, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::clear);
    connect(Sketching2DModule::ac_new_boundary, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::newBoundary);



    connect(Sketching2DModule::ac_add_sketch, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::newSketch);
    connect(Sketching2DModule::ac_deny_curve, &QAction::triggered, Sketching2DModule::sketching_canvas->sketchSession_, &SketchSessionTesting::clearSketch);

    connect(Sketching2DModule::ac_undo, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::undo);
    connect(Sketching2DModule::ac_redo, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::redo);


    connect(Sketching2DModule::ac_screenshot, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::screenShot);


	// View
    connect(Sketching2DModule::ac_zoom_in, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::zoomIn);
    connect(Sketching2DModule::ac_zoom_out, &QAction::triggered, Sketching2DModule::sketching_canvas, &SketchBoard::zoomOut);


    /*


    // Refactoring

    connect( ac_removeabove, &QAction::triggered, [=](){ controller->updateRule( "REMOVEABOVE" ); lb_statusbar_status->setText( "Remove Above Intersection" ); } );
    connect( ac_removebelow, &QAction::triggered, [=](){ controller->updateRule( "REMOVEBELOW" ); lb_statusbar_status->setText( "Remove Below Intersection" ); } );
    connect( ac_sketch, &QAction::triggered, [=](){ controller->updateRule( "SKETCH" ); lb_statusbar_status->setText( "Sketch" ); } );

    connect( mn_pickercolor, &QMenu::aboutToShow, cd_pickercolor, &QColorDialog::show );
    connect( cd_pickercolor, &QColorDialog::rejected, mn_pickercolor, &QMenu::hide );
    connect( cd_pickercolor, &QColorDialog::colorSelected, [=]( const QColor& color ){ controller->setCurrentColor( color.redF(), color.blueF(), color.greenF() ); mn_pickercolor->hide(); } );


    // a cena ou sketching_canvas deve ter status para sketching, boundary, selecting, etc...
    connect( ac_new_boundary, &QAction::triggered, sketching_canvas, &SketchBoard::statusBoundary );


    connect( ac_add_sketch, &QAction::triggered, controller, &Controller::addSketch );
    connect( ac_deny_curve, &QAction::triggered, controller , &Controller::denySketch );


    connect( ac_undo, &QAction::triggered, controller, &Controller::undo );
    connect( ac_redo, &QAction::triggered, controller, &Controller::redo );


    connect( ac_screenshot, &QAction::triggered, sketching_canvas, &SketchBoard::screenShot );


connect( this->parent(), SIGNAL( sendController( Controller& ) ), [=]( Controller& cont ){ sc->setController( cont ); } );

*/



}
