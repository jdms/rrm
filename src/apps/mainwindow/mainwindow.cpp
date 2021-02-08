/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file mainwindow.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class MainWindow
 */


#include <QDesktopServices>

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

    about_rrm = new AboutWidget( this );

}


void MainWindow::createWindow()
{
    createActions();
    createMenuBar();
    createToolbar();
    createController();

    createObjectTree();
    createSideBar();

    plug3dInterface();    
    plugSketchInterface();
    plug3dDiagnosticsInterface();

}


void MainWindow::createActions()
{
    ac_save = new QAction( "Save", this );
    ac_save->setToolTip( "Save" );
    ac_save->setIcon(QIcon(":/images/icons/save.png"));

    ac_load = new QAction( "Load", this );
    ac_load->setToolTip( "Open" );
    ac_load->setIcon(QIcon(":/images/icons/load.png"));

    ac_clear = new QAction( "New", this );
    ac_clear ->setToolTip( "New" );
    ac_clear->setIcon(QIcon(":/images/icons/new.png"));

    ac_export = new QAction( "Export", this );
    ac_export->setToolTip( "Export to" );

    ac_exit = new QAction( tr ( "E&xit" ) , this );
    ac_exit->setToolTip("Exit");

    ac_manual = new QAction( tr ( "RRM Manual" ), this );
    ac_manual->setToolTip( "Manual" );

    ac_about = new QAction( tr ( "&About" ) , this );
    ac_about->setToolTip( "About" );

    ac_undo = new QAction( "Undo", this );
    ac_undo->setToolTip( "Undo" );
    ac_undo->setIcon(QIcon(":/images/icons/undo.png"));

    ac_redo = new QAction( "Redo", this );
    ac_redo->setToolTip( "Redo" );
    ac_redo->setIcon(QIcon(":/images/icons/redo.png"));

    ac_sketch_above = new QAction( "SA", this ); // preserve above!
    ac_sketch_above->setToolTip( "Sketch Above" );
    ac_sketch_above->setCheckable( true );

    ac_sketch_region = new QAction( "SR", this ); // preserve region!
    ac_sketch_region->setToolTip( "Sketch Region" );
    ac_sketch_region->setCheckable( true );
    ac_sketch_region->setVisible( true );

    ac_sketch_below = new QAction( "SB", this ); // preserve below!
    ac_sketch_below->setToolTip( "Sketch Below" );
    ac_sketch_below->setCheckable( true );

    ac_remove_above = new QAction( "RA", this );
    ac_remove_above->setToolTip( "Remove Above" );
    ac_remove_above->setCheckable( true );

    ac_remove_above_int = new QAction( "RAI", this );
    ac_remove_above_int->setToolTip( "Remove Above Intersection" );
    ac_remove_above_int->setCheckable( true );
    ac_remove_above_int->setChecked( true );

    ac_remove_below = new QAction( "RB", this );
    ac_remove_below->setToolTip( "Remove Below" );
    ac_remove_below->setCheckable( true );

    ac_remove_below_int = new QAction( "RBI", this );
    ac_remove_below_int->setToolTip( "Remove Below Intersection" );
    ac_remove_below_int->setCheckable( true );

    ac_screenshot = new QAction( "Screenshot", this );
    ac_screenshot->setToolTip( "Screenshot" );
    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));

    ac_direction_x = new QAction( "Width", this );
    ac_direction_x->setToolTip( "Widthwise Direction" );
    ac_direction_x->setCheckable( true );

    ac_direction_y = new QAction( "Height", this );
    ac_direction_y->setToolTip( "Heightwise Direction" );
    ac_direction_y->setCheckable( true );

    ac_direction_z = new QAction( "Length", this );
    ac_direction_z->setToolTip( "Lenghtwise Direction" );
    ac_direction_z->setCheckable( true );
    ac_direction_z->setChecked( true );


    ac_stratigraphy = new QAction( "Stratigraphy", this );
    ac_stratigraphy->setToolTip( "Stratigraphy" );
    ac_stratigraphy->setCheckable( true );
    ac_stratigraphy->setChecked( true );

    ac_structural = new QAction( "Structural", this );
    ac_structural->setToolTip( "Structural" );
    ac_structural->setCheckable( true );

    ac_regions = new QAction( "Regions", this );
    ac_regions->setToolTip( "Show Regions" );
    ac_regions->setCheckable( true );
    ac_regions->setChecked( false );

    ac_diagnostics = new QAction( "Diagnostics", this );
    ac_diagnostics->setToolTip( "Run Diagnostics" );
    ac_diagnostics->setCheckable( true );
    ac_diagnostics->setChecked( false );
    ac_diagnostics->setEnabled( false );


    connect( ac_clear, &QAction::triggered, [=](){ app->reset(); } );

    connect( ac_export, &QAction::triggered, [=](){ app->exportToIRAP(); } );


    connect( ac_undo, &QAction::triggered, [=](){ app->undo(); } );

    connect( ac_redo, &QAction::triggered, [=](){ app->redo(); } );

    connect( ac_save, &QAction::triggered, [=](){ save(); } );

    connect( ac_load, &QAction::triggered, [=](){ load(); } );

    connect( ac_exit, &QAction::triggered, this, &MainWindow::close );


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

    connect( ac_stratigraphy, &QAction::triggered, [=]()
    { app->setCurrentObjectType( Settings::Objects::ObjectType::STRATIGRAPHY ); } );

    connect( ac_structural, &QAction::triggered, [=]()
    { app->setCurrentObjectType( Settings::Objects::ObjectType::STRUCTURAL ); } );


    connect( ac_regions, &QAction::triggered, [=]( bool status_ )
    { app->getRegions( status_ ); ac_diagnostics->setEnabled( status_ && diagapp->isImplemented() ); lockUndoRedo( status_ ); } );

    connect( ac_diagnostics, &QAction::triggered, [=]( bool status_ )
    {  emit runDiagnostics( status_ );
    } );

    connect( ac_screenshot, &QAction::triggered, [=](){ emit takeScreenshot();  } );

    connect( ac_about, &QAction::triggered, about_rrm, &AboutWidget::show );

    connect( ac_manual, &QAction::triggered, this, &MainWindow::showHelp );

}


void MainWindow::createMenuBar()
{

    mn_file = menuBar()->addMenu ( tr ( "&File" ) );
    mn_file->addAction ( ac_clear );
    mn_file->addSeparator();
    mn_file->addAction ( ac_load );
    mn_file->addAction ( ac_save );
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
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_load );
    tb_mainwindow->addAction( ac_save );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_undo );
    tb_mainwindow->addAction( ac_redo );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_sketch_region );
    tb_mainwindow->addAction( ac_sketch_above );
    tb_mainwindow->addAction( ac_sketch_below );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_rules->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_directions->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addActions( ag_objects->actions() );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_regions );
    tb_mainwindow->addAction( ac_diagnostics );
    tb_mainwindow->addSeparator();
    tb_mainwindow->addAction( ac_screenshot );


}


void MainWindow::createSideBar()
{
    ps_objectdata = new PagesStack();
    dw_object_properties = new QDockWidget( "" );
    dw_object_properties->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_properties->setWidget( ps_objectdata );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_properties );


    connect( ps_objectdata, &PagesStack::widthVolumeChanged, [=]( int width_ )
    {
       app->setVolumeWidth( static_cast< double >( width_ ) );
    });

    connect( ps_objectdata, &PagesStack::heightVolumeChanged, [=]( int height_ )
    {
       app->setVolumeHeight( static_cast< double >( height_ ) );
    });

    connect( ps_objectdata, &PagesStack::depthVolumeChanged, [=]( int depth_ )
    {
       app->setVolumeDepth( static_cast< double >( depth_ ) );
    });


    connect( ps_objectdata, &PagesStack::setHighResolution, [=]()
    { app->setMeshResolution( "HIGH" ); } );

    connect( ps_objectdata, &PagesStack::setMediumResolution, [=]()
    { app->setMeshResolution( "MEDIUM" ); } );

    connect( ps_objectdata, &PagesStack::setLowResolution, [=]()
    { app->setMeshResolution( "LOW" ); } );


    connect( object_tree, &ObjectTree::objectSelected, ps_objectdata, &PagesStack::selectObjectPage );

    connect( app, &RRMApplication::defineVolumeGeometry, [=]( double ox_, double oy_, double oz_, double w_, double h_, double d_ )
    {
        ps_objectdata->changeRangeSize( 20* ( ox_ + w_ ), 20* ( oy_ + h_ ), 20* ( oz_ + d_ ) );
        ps_objectdata->changeVolumeSize( ( ox_ + w_ ), ( oy_ + h_ ), ( oz_ + d_ ) );
    } );

    connect( app, &RRMApplication::disableVolumeResizing, [=]
    {
        ps_objectdata->setEnabledVolumeResize( false );
        dw_object_properties->setVisible( false );
    } );

    connect( app, &RRMApplication::enableVolumeResizing, [=]
    {
        ps_objectdata->setEnabledVolumeResize( true );
        dw_object_properties->setVisible( true );
    } );




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


    connect( app, &RRMApplication::selectEnabled, [=]( const std::string& option_, bool status_ )
    {
        lockPreserve( option_ );
    } );

    connect( app, &RRMApplication::addRegions, [=]()
    {
        ac_regions->setChecked( true );
        ac_diagnostics->setEnabled( true && diagapp->isImplemented() );
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
        if( index_ == 0 )
            app->setVolumeVisible( status_ );
        else
            app->setRegionsVisible( status_ );
    } );


    connect( app, &RRMApplication::resetApplication, object_tree, &ObjectTree::clear );


    connect( object_tree, &ObjectTree::setRegionVisible, [=]( std::size_t index_, bool status_ )
    {
        app->setRegionVisible( index_, status_ );
    } );

    connect( object_tree, &ObjectTree::setRegionColor, [=]( std::size_t index_, const QColor& c_ )
    {
        app->setRegionColor( index_, c_.red(), c_.green(), c_.blue() );
    } );

    connect( object_tree, &ObjectTree::setDomainName, [=]( std::size_t index_, const std::string& name_ )
    {
        app->setDomainName( index_, name_ );
    } );

    connect( object_tree, &ObjectTree::createDomainOfRegions, [=]()
    {
        app->createDomain();
    } );

    connect( object_tree, &ObjectTree::addRegionToDomain, [=]( std::size_t reg_id_, std::size_t domain_id_ )
    {
        app->addRegionToDomain( reg_id_, domain_id_ );
    } );



    connect( object_tree, &ObjectTree::removeRegionFromDomain, [=]( std::size_t reg_id_, std::size_t domain_id_ )
    {
        app->removeRegionFromDomain( reg_id_, domain_id_ );
    } );

    connect( object_tree, &ObjectTree::removeDomain, [=]( std::size_t index_ )
    {
        app->removeDomain( index_ );
    } );




    connect( object_tree, &ObjectTree::addRegionsToDomain, [=]( std::size_t domain_id_, std::vector< std::size_t > regions_ )
    {
        app->addRegionsToDomain( domain_id_, regions_ );
    } );

    connect( object_tree, &ObjectTree::removeRegionsFromTheirDomains, [=]( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ )
    {
        app->removeRegionsFromDomains( regions_, domains_, true );
    } );



    connect( object_tree, &ObjectTree::removeRegionsFromTheirDomainsNoDelete, [=]( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_ )
        {
            app->removeRegionsFromDomains( regions_, domains_, false );
        } );

    connect( object_tree, &ObjectTree::saveSurfaceLog, [=]( int id_, const QString& log_ )
    {
        app->setObjectLog( static_cast< int >( id_ ), log_ );
    } );


        connect( object_tree, &ObjectTree::getSurfaceLog, [=]( int id_, QString& log_ )
    {
        app->getObjectLog( static_cast< int >( id_ ), log_ );
    } );

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


void MainWindow::plug3dDiagnosticsInterface()
{
    diagapp = std::make_shared< DiagnosticsInterface >( this );
    diagapp->createInterface();
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
    }
    else if( option_.compare( "REGION" ) == 0 )
    {
    }
    else if( option_.compare( "BELOW" ) == 0 )
    {
        ac_sketch_above->setEnabled( false );
    }
    else
    {
        ac_sketch_above->setEnabled( true );
        ac_sketch_region->setEnabled( true );
        ac_sketch_below->setEnabled( true );
    }
}


void MainWindow::lockUndoRedo( bool status_ )
{
    ac_undo->setDisabled( status_ );
    ac_redo->setDisabled( status_ );
}


void  MainWindow::activatePreserveAbove( bool status_ )
{
    ac_sketch_above->setEnabled( true );
    ac_sketch_above->setChecked( status_ );
}


void  MainWindow::activatePreserveBelow( bool status_ )
{
    ac_sketch_below->setEnabled( true );
    ac_sketch_below->setChecked( status_ );
}


void  MainWindow::activatePreserveRegion( bool status_ )
{
    ac_sketch_region->setChecked( status_ );
}


bool MainWindow::isRegionEnabled() const
{
    return ac_regions->isChecked();
}


void MainWindow::initializeInterface()
{
    ac_undo->setEnabled( false );
    ac_redo->setEnabled( false );

    ac_sketch_region->setChecked( false );
    ac_sketch_above->setChecked( false );
    ac_sketch_below->setChecked( false );
    ac_remove_above->setChecked( false );
    ac_remove_above_int->setChecked( true );
    ac_remove_below->setChecked( false );
    ac_remove_below_int->setChecked( false );

    ac_direction_x->setEnabled( true );
    ac_direction_y->setEnabled( true );
    ac_direction_z->setEnabled( true );

    ac_direction_x->setChecked( false );
    ac_direction_y->setChecked( false );
    ac_direction_z->setChecked( true );

    ac_stratigraphy->setChecked( true );
    ac_structural->setChecked( false );

    ac_regions->setChecked( false );
    ac_regions->setEnabled( true );

    ac_diagnostics->setChecked( false );
    ac_diagnostics->setEnabled( false );

    if( ps_objectdata == nullptr ) return;
    ps_objectdata->selectObjectPage( Settings::Objects::ObjectType::VOLUME );
}


void MainWindow::showHelp()
{
    QDir dir;
    QFileInfo file_info(dir.currentPath() + "/manual/rrm-manual.pdf");
    QDesktopServices::openUrl(QUrl::fromLocalFile(file_info.absoluteFilePath()));
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
