#include "SketchingWindow.h"


SketchingWindow::SketchingWindow( QWidget* parent )
{
    init();
}



void SketchingWindow::init()
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

    tlb_section = addToolBar( tr( "New" ) );
    createNewActions();


    tlb_sketchregions = addToolBar( tr( "SketchRegions" ) );
    createDefineSketchRegionsActions();


    tlb_rules = addToolBar( tr("Rules") );
    createRulesActions();


    tlb_edit = addToolBar( tr( "Edit" ) );
    createEditActions();



    tlb_customization = addToolBar( tr( "Customize" ) );
    createColorActions();
    createMiscActions();

}





void SketchingWindow::createDefineSketchRegionsActions()
{

    ac_sketchabove = new QAction( tr( "SA" ), this );
    ac_sketchabove->setCheckable( true );

    ac_sketchbelow = new QAction( tr( "SB" ), this );
    ac_sketchbelow->setCheckable( true );


    ag_sketching_rules = new QActionGroup( this );
    ag_sketching_rules->addAction( ac_sketchabove );
    ag_sketching_rules->addAction( ac_sketchbelow );
    ag_sketching_rules->setExclusive( false );


    tlb_sketchregions->addActions( ag_sketching_rules->actions() );

}



void SketchingWindow::createRulesActions()
{


    ac_sketch = new QAction( tr( "TS" ), this );
//    ac_sketch->setIcon( QIcon( ":/images/icons/sketchmode.png" ) );
    ac_sketch->setCheckable( true );


    ac_removeabove = new QAction( tr( "RA" ), this );
    ac_removeabove->setCheckable( true );
    ac_removeabove->setChecked( true );


    ac_removeaboveintersection = new QAction( tr( "RAI" ), this );
//    ac_removeaboveintersection->setIcon( QIcon( ":/images/icons/removeaboveintersection.png" ) );
    ac_removeaboveintersection->setCheckable( true );


    ac_removebelow = new QAction(tr( "RB" ), this );
    ac_removebelow->setCheckable( true );

    ac_removebelowintersection = new QAction( tr( "RBI" ), this );
//    ac_removebelowintersection->setIcon( QIcon( ":/images/icons/removebelowintersection.png" ) );
    ac_removebelowintersection->setCheckable( true );



    QAction* ac_separator = new QAction( this );
    ac_separator->setSeparator( true );

    ag_remove_rules = new QActionGroup( this );

    ag_remove_rules->addAction( ac_removeabove );
    ag_remove_rules->addAction( ac_removeaboveintersection );
    ag_remove_rules->addAction( ac_removebelow );
    ag_remove_rules->addAction( ac_removebelowintersection );
    ag_remove_rules->addAction( ac_separator );
    ag_remove_rules->addAction( ac_sketch );
    ag_remove_rules->setExclusive( true );


    tlb_rules->addActions( ag_remove_rules->actions() );



}



void SketchingWindow::createNewActions()
{

    ac_new = new QAction( tr( "&New" ), this );
    ac_new->setIcon( QIcon( ":/images/icons/page_white.png" ) );
    ac_new->setShortcut( Qt::Key_N );


    tlb_section->addAction( ac_new );

}



void SketchingWindow::createEditActions()
{


    ac_new_boundary = new QAction( tr( "&Edit Boundary" ), this );
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


    tlb_edit->addAction( ac_new_boundary );
    tlb_edit->addSeparator();

    tlb_edit->addAction( ac_add_sketch );
    tlb_edit->addAction( ac_deny_curve );
    tlb_edit->addSeparator();

    tlb_edit->addAction( ac_undo );
    tlb_edit->addAction( ac_redo );

}





void SketchingWindow::createColorActions()
{


    cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 0, 0, 128 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );



    mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_sketchcolor );


    tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( ":/images/icons/border_color.png" ) );
    tbt_colorsketch->setMenu( mn_pickercolor );
    tbt_colorsketch ->setCheckable( true );


    tlb_customization->addWidget( tbt_colorsketch );


}



void SketchingWindow::createMiscActions()
{

    ac_screenshot = new QAction( tr( "&Screenshot" ), this );
    ac_screenshot->setIcon( QIcon( ":/images/icons/Camera.png" ) );
    ac_screenshot->setShortcut( Qt::Key_P );

    tlb_customization->addAction( ac_screenshot );

}



void SketchingWindow::resetActions()
{


    ac_undo->setEnabled( true );
    ac_redo->setEnabled( true );

    ac_removeabove->setChecked( true );
    ac_removebelow->setChecked( false );
    ac_removeaboveintersection->setChecked( false );
    ac_removebelowintersection->setChecked( false );
    ac_sketchabove->setChecked( false );
    ac_sketchbelow->setChecked( false );
    ac_sketch->setChecked( false );

    tbt_colorsketch->setChecked( false );

}



void SketchingWindow::createConnections()
{



    connect( ac_sketchabove, &QAction::toggled, [=]( bool status ){ emit defineSketchingAbove( status ); lb_statusbar_status->setText( "Sketching Above" ); } );
    connect( ac_sketchbelow, &QAction::toggled, [=]( bool status ){ emit defineSketchingBelow( status ); lb_statusbar_status->setText( "Sketching Below" ); } );


    connect( ac_sketch, &QAction::toggled, [=](){ emit updateStratigraphicRule( "SKETCHING" ); lb_statusbar_status->setText( "Sketch" ); } );


    connect( ac_removeabove, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RA_SKETCHING" );  lb_statusbar_status->setText( "Remove Above" ); } );
    connect( ac_removeaboveintersection, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RAI_SKETCHING" );  lb_statusbar_status->setText( "Remove Above Intersection" ); } );
    connect( ac_removebelow, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RB_SKETCHING" ); lb_statusbar_status->setText( "Remove Below" ); } );
    connect( ac_removebelowintersection, &QAction::toggled, [=](){ emit updateStratigraphicRule( "RBI_SKETCHING" ); lb_statusbar_status->setText( "Remove Below Intersection" ); } );


    connect( ac_new, &QAction::triggered, [=](){ emit clear(); resetActions(); } );
    connect( ac_undo, &QAction::triggered, [=](){ emit undo(); } );
    connect( ac_redo, &QAction::triggered, [=](){ emit redo(); } );


    connect( mn_pickercolor, &QMenu::aboutToShow, cd_pickercolor, &QColorDialog::show );
    connect( cd_pickercolor, &QColorDialog::rejected, mn_pickercolor, &QMenu::hide );    
    connect( cd_pickercolor, &QColorDialog::accepted, mn_pickercolor, &QMenu::hide );
    connect( cd_pickercolor, &QColorDialog::colorSelected, [=]( const QColor& c ){ tbt_colorsketch->setChecked( true ); emit updateColor( c ); } );

    connect( ac_screenshot, &QAction::triggered, sketching_canvas, &SketchingCanvas::screenshot );

    connect( ac_add_sketch, &QAction::triggered, [=](){ emit addStratigraphy(); } );
    connect( ac_deny_curve, &QAction::triggered, [=](){ emit undoLastSketch(); } );
    connect( ac_new_boundary, &QAction::triggered, [=](){ emit setCurrentMode( Scene::InteractionMode::BOUNDARY ); } );


    connect( tbt_colorsketch, &QToolButton::toggled, [=]( bool status ){ emit setRandomColor( !status ); } );

}




void SketchingWindow::changeDefineRegionStatus( const bool above, const bool below )
{
    ac_sketchabove->setChecked( above );
    ac_sketchbelow->setChecked( below );

}



void SketchingWindow::changeStratigraphyRulesStatus( const std::string& rule )
{


    if( rule.compare( "SKETCHING" ) == 0 )
    {
        ac_sketch->toggle();
    }
    else if( rule.compare( "RA_SKETCHING" ) == 0 )
    {
        ac_removeabove->toggle();
    }
    else if( rule.compare( "RAI_SKETCHING" ) == 0 )
    {
        ac_removeaboveintersection->toggle();
    }
    else if( rule.compare( "RB_SKETCHING" ) == 0 )
    {
        ac_removebelow->toggle();
    }
    else if( rule.compare( "RBI_SKETCHING" ) == 0 )
    {
        ac_removebelowintersection->toggle();
    }

}




