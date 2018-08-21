#include "mainwindow.h"

MainWindow::MainWindow( QWidget* parent_ ): QMainWindow( parent_ )
{

    setWindowProperties();
    createWindow();

    run();

}


void MainWindow::setWindowProperties()
{

    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setDockNestingEnabled( true );

    setWindowTitle( "Rapid Reservoir Modelling" );

}


void MainWindow::createWindow()
{

    createActions();
    createMenuBar();
    createToolbar();
    createController();

    createObjectTree();

    plug3dInterface();    
    plugSketchInterface();

}


void MainWindow::createActions()
{
    ac_save = new QAction( "Save", this );
    ac_save->setIcon(QIcon(":/images/icons/save.png"));

    ac_load = new QAction( "Load", this );
    ac_load->setIcon(QIcon(":/images/icons/load.png"));

    ac_clear = new QAction( "New", this );
    ac_clear->setIcon(QIcon(":/images/icons/new.png"));

    ac_export = new QAction( "Export", this );
    ac_exit = new QAction( tr ( "E&xit" ) , this );

    ac_manual = new QAction( tr ( "RRM Manual" ), this );
    ac_about = new QAction( tr ( "&About" ) , this );

    ac_undo = new QAction( "Undo", this );
    ac_undo->setIcon(QIcon(":/images/icons/undo.png"));

    ac_redo = new QAction( "Redo", this );
    ac_redo->setIcon(QIcon(":/images/icons/redo.png"));

    ac_sketch_above = new QAction( "PA", this ); // preserve above!
    ac_sketch_above->setCheckable( true );

    ac_sketch_region = new QAction( "PR", this ); // preserve region!
    ac_sketch_region->setCheckable( true );
    ac_sketch_region->setVisible( false );

    ac_sketch_below = new QAction( "PB", this ); // preserve below!
    ac_sketch_below->setCheckable( true );

    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );

    ac_remove_above_int = new QAction( "RAI", this );
    ac_remove_above_int->setCheckable( true );
    ac_remove_above_int->setChecked( true );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );

    ac_remove_below_int = new QAction( "RBI", this );
    ac_remove_below_int->setCheckable( true );

    ac_screenshot = new QAction( "Screenshot", this );
    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));

    ac_direction_x = new QAction( "Width", this );
    ac_direction_x->setCheckable( true );

    ac_direction_y = new QAction( "Height", this );
    ac_direction_y->setCheckable( true );

    ac_direction_z = new QAction( "Lenght", this );
    ac_direction_z->setCheckable( true );
    ac_direction_z->setChecked( true );


    ac_stratigraphy = new QAction( "Stratigraphy", this );
    ac_stratigraphy->setCheckable( true );
    ac_stratigraphy->setChecked( true );

    ac_structural = new QAction( "Structural", this );
    ac_structural->setCheckable( true );


    connect( ac_clear, &QAction::triggered, [=](){ app->reset(); } );

    connect( ac_undo, &QAction::triggered, [=](){ app->undo(); } );

    connect( ac_redo, &QAction::triggered, [=](){ app->redo(); } );

    connect( ac_save, &QAction::triggered, [=](){ save(); } );

    connect( ac_load, &QAction::triggered, [=](){ load(); } );


    connect( ac_remove_above, &QAction::triggered, [=]()
    { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE ); } );

    connect( ac_remove_above_int, &QAction::triggered, [=]()
    { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION ); } );

    connect( ac_remove_below, &QAction::triggered, [=]()
    { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW ); } );

    connect( ac_remove_below_int, &QAction::triggered, [=]()
    { app->setStratigraphicRule( Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION ); } );


    connect( ac_sketch_above, &QAction::triggered, [=]( bool status_ ){ app->setSketchAbove( status_ );
    } );

    connect( ac_sketch_below, &QAction::triggered, [=]( bool status_ ){ app->setSketchBelow( status_ );
    } );

    connect( ac_sketch_region, &QAction::triggered, [=]( bool status_ ){ app->setSketchRegion( status_ );
    } );

    connect( ac_direction_x, &QAction::triggered, [=]()
    { app->changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections::X );} );

    connect( ac_direction_y, &QAction::triggered, [=]()
    { app->changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections::Y );} );

    connect( ac_direction_z, &QAction::triggered, [=]()
    { app->changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections::Z );} );

    connect( ac_stratigraphy, &QAction::triggered, [=](){ app->setCurrentObjectType( Settings::Objects::ObjectType::STRATIGRAPHY ); } );

    connect( ac_structural, &QAction::triggered, [=](){ app->setCurrentObjectType( Settings::Objects::ObjectType::STRUCTURAL ); } );

}


void MainWindow::createMenuBar()
{

    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_clear );
    mn_file->addAction ( ac_save );
    mn_file->addAction ( ac_load );
    mn_file->addSeparator();
    mn_file->addAction( ac_export );
    mn_file->addAction ( ac_exit );

    mn_windows = menuBar()->addMenu ( tr ( "&Windows" ) );


    mn_help = menuBar()->addMenu ( tr ( "&Help" ) );
    mn_help->addAction( ac_manual );
    mn_help->addAction( ac_about );


}


void MainWindow::createToolbar()
{

    QActionGroup* ag_rules = new QActionGroup( this );
    ag_rules->setExclusive( true );
    ag_rules->addAction( ac_remove_above );
    ag_rules->addAction( ac_remove_above_int );
    ag_rules->addAction( ac_remove_below );
    ag_rules->addAction( ac_remove_below_int );


    QActionGroup* ag_directions = new QActionGroup( this );
    ag_directions->setExclusive( true );
    ag_directions->addAction( ac_direction_x );
    ag_directions->addAction( ac_direction_z );
    ag_directions->addAction( ac_direction_y );

    QActionGroup* ag_objects = new QActionGroup( this );
    ag_objects->setExclusive( true );
    ag_objects->addAction( ac_stratigraphy );
    ag_objects->addAction( ac_structural );

    tb_mainwindow = addToolBar( "");
    tb_mainwindow->addAction( ac_clear );
    tb_mainwindow->addAction( ac_save );
    tb_mainwindow->addAction( ac_load );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_undo );
    tb_mainwindow->addAction( ac_redo );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_sketch_above );
    tb_mainwindow->addAction( ac_sketch_region );
    tb_mainwindow->addAction( ac_sketch_below );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_rules->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_directions->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_objects->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_screenshot );


}


void MainWindow::createController()
{
    controller = new Controller();
    app = new RRMApplication();
    app->setMainWindow( this );

    connect( app, &RRMApplication::lockDirection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_ )
    { lockDirection( dir_ );} );

    connect( app, &RRMApplication::unlockDirections, [=]()
    {
        ac_direction_x->setEnabled( true );
        ac_direction_y->setEnabled( true );
        ac_direction_z->setEnabled( true );
    });


    connect( app, &RRMApplication::selectEnabled, [=]( const std::string option_ )
    {
        lockPreserve( option_ );
    } );

}


void MainWindow::createObjectTree()
{
    object_tree = new ObjectTree( this );
    dw_object_tree = new QDockWidget( "" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_tree );


    connect( object_tree, &ObjectTree::setObjectVisible, [=]( std::size_t index_, bool status_ )
    {
        app->setObjectVisible( index_, status_ );
    } );

    connect( object_tree, &ObjectTree::setObjectName, [=]( std::size_t index_, const std::string& name_ )
    {
        app->setObjectName( index_, name_ );
    } );

    connect( object_tree, &ObjectTree::setObjectColor, [=]( std::size_t index_, const QColor& color_ )
    {
        app->setObjectColor( index_, color_.red(), color_.green(), color_.blue() );
    } );

    connect( object_tree, &ObjectTree::setVolumeName, [=]( std::size_t index_, const std::string& name_ )
    {
        app->setVolumeName( name_ );
    } );

    connect( object_tree, &ObjectTree::setVolumeVisible, [=]( std::size_t index_, bool status_ )
    {
        app->setVolumeVisible( status_ );
    } );


    connect( app, &RRMApplication::resetApplication, object_tree, &ObjectTree::clear );





}


void MainWindow::plugSketchInterface()
{
    sketchapp = std::make_shared< SketchInterface >( this );
    sketchapp->createInterface();

}


void MainWindow::plug3dInterface()
{
    view3dapp = std::make_shared< View3dInterface >( this );
    view3dapp->createInterface();

}


void MainWindow::run()
{
    app->init();
}


void MainWindow::save()
{
    QString selected_format = "";
    QString filename_ = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );


    if( filename_.isEmpty() == true ) return;
    app->save( filename_.toStdString() );
}


void MainWindow::load()
{

    QString selected_format = "";
    QString filename_ = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );

    if( filename_.isEmpty() == true ) return;
    app->load( filename_.toStdString() );

}


void MainWindow::lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    switch( dir_)
    {

        case Settings::CrossSection::CrossSectionDirections::X:
        {
            ac_direction_y->setEnabled( false );
            ac_direction_z->setEnabled( false );
        }
        break;

        case Settings::CrossSection::CrossSectionDirections::Y:
        {
            ac_direction_x->setEnabled( false );
            ac_direction_z->setEnabled( false );
        }
        break;

        case Settings::CrossSection::CrossSectionDirections::Z:
        {
            ac_direction_x->setEnabled( false );
            ac_direction_y->setEnabled( false );
        }
        break;

    default:
        break;

    };

}


void MainWindow::lockPreserve( const std::string& option_ )
{
    if( option_.compare( "ABOVE" ) == 0 )
    {
        ac_sketch_below->setEnabled( false );
        ac_sketch_region->setEnabled( false );
    }
    else if( option_.compare( "REGION" ) == 0 )
    {
        ac_sketch_below->setEnabled( false );
        ac_sketch_above->setEnabled( false );
    }
    else if( option_.compare( "BELOW" ) == 0 )
    {
        ac_sketch_above->setEnabled( false );
        ac_sketch_region->setEnabled( false );
    }
    else
    {
        ac_sketch_above->setEnabled( true );
        ac_sketch_region->setEnabled( true );
        ac_sketch_below->setEnabled( true );
    }
}


MainWindow::~MainWindow()
{

    if( object_tree != nullptr )
        delete object_tree;

    if( dw_object_tree != nullptr )
        delete dw_object_tree;


    if( controller != nullptr )
        delete controller;

    if( app != nullptr )
        delete app;


    object_tree = nullptr;
    dw_object_tree = nullptr;

    controller = nullptr;
    app = nullptr;


}
