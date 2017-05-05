/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>

#include "MainWindow.h"

MainWindow::MainWindow ( QWidget *parent ) : QMainWindow ( parent )
{
    setupUi( this );

    sl_depth_csection = nullptr;
    object_tree = nullptr;

    canvas3d = nullptr;

    dw_sketch_path_canvas = nullptr;
    canvas_path = nullptr;

    dw_sketch_canvas = nullptr;
    canvas2d = nullptr;

    ac_undo = nullptr;
    ac_redo = nullptr;
    ac_discard_sketch = nullptr;
    ac_screenshot = nullptr;
    ac_clear = nullptr;


    init();
}


MainWindow::~MainWindow()
{
}



void MainWindow::init()
{

    setupWindowProperties();
    createWindow();

    startController();
    createActions();

}


void MainWindow::setupWindowProperties()
{
    setFocusPolicy( Qt::StrongFocus );
    setFocus();
    setAcceptDrops( true );
    setDockOptions( QMainWindow::AllowNestedDocks | QMainWindow::VerticalTabs |
                    QMainWindow::AllowTabbedDocks );

    setDockNestingEnabled( true );

    QRect rect = QGuiApplication::primaryScreen()->geometry();
    app_height = rect.height()* 0.8;
    app_width = rect.width()* 0.8;

    app_orig_x = ( rect.width() - app_width )*0.5 ;
    app_orig_y = ( rect.height() - app_height )*0.5 ;


    setGeometry( app_orig_x, app_orig_y, app_width, app_height );
    setWindowTitle( "Rapid Reservoir Modelling" );
}


void MainWindow::createWindow()
{

    create3dSection();
    createObjectTreeSection();
    createSketchSection();



    getCurrentDirectory();


    connect( sl_depth_csection, &QSlider::sliderMoved, [=]( int v ){


                   double depth_ = controller->depthFromRowIndex( (std::size_t ) v );
                   controller->setCurrentCrossSection( depth_ );
                   emit updateScenes(); } );




    connect( this, &MainWindow::updateScenes, &scene3d, &Scene3D::updateScene );



//    connect( object_tree, &ObjectTree::itemClicked, [=]( QTreeWidgetItem* item_, int column_ ) {
//                                       ObjectTreeItem* obj_ = static_cast< ObjectTreeItem* > item_;
//                                       controller->selectObject( obj_->getId() ); } );






}


void MainWindow::create3dSection()
{

    canvas3d = new Canvas3D();
    canvas3d->setScene( &scene3d );

    sl_depth_csection = new QSlider( Qt::Vertical );


    QHBoxLayout* hl_window3d = new QHBoxLayout( this );
    hl_window3d->addWidget( canvas3d );
    hl_window3d->addWidget( sl_depth_csection );

    QWidget *wd_window3d = new QWidget( this );
    wd_window3d->setLayout( hl_window3d );


    connect( this, SIGNAL( updateCanvas() ), canvas3d, SLOT( update() ) );


    setCentralWidget( wd_window3d );

}


void MainWindow::createObjectTreeSection()
{
    object_tree = new ObjectTree( this );
    object_tree->setMaximumWidth( 0.2*app_width );

    dw_object_tree = new QDockWidget( "Sketching Canvas" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    dw_object_tree->setVisible( true );
    addDockWidget( Qt::LeftDockWidgetArea, dw_object_tree );

    connect( object_tree, &ObjectTree::setInputVolumeVisible,
                        [=]( bool status_ ) { controller->setInputVolumeVisibility( status_ );
                                              emit updateScenes();
                                              emit updateCanvas();
                                            } );

    connect( object_tree, &ObjectTree::setObjectVisible,
                        [=]( std::size_t id_, bool status_ ) {
                                        controller->setVisibilityofObjectofId( id_, status_ );
                                        emit updateScenes();
                                        emit updateCanvas();
                            } );


}



void MainWindow::createSketchSection()
{


    canvas2d = new SketchCanvas();
    canvas2d->setScene( &sketch_scene );
    canvas2d->setMinimumHeight( 0.4f* ( app_height - 10 ) );

    dw_sketch_canvas = new QDockWidget( "Sketching Canvas" );
    dw_sketch_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_canvas->setWidget( canvas2d );
    dw_sketch_canvas->setVisible( true );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketch_canvas );

    canvas_path = new CanvasPath();
    canvas_path->setScenePath( &scene_path );

    dw_sketch_path_canvas = new QDockWidget( "Sketching Path Canvas" );
    dw_sketch_path_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_path_canvas->setWidget( canvas_path );
    dw_sketch_path_canvas->setVisible( false );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketch_path_canvas );


    connect( &sketch_scene, &SketchScene::updateVolumeWidthHeight, [=]( double w_, double h_ ) {
                                                controller->setInputVolumeWidthHeight( w_, h_ );
                                                emit updateScenes();
                                                } );

    connect( &sketch_scene, &SketchScene::selectedSurface, [=]( const std::size_t&id_  ) {
                                                        controller->sendSelectedSurface( id_ ); } );


    connect( &scene_path, &PathScene::updateVolumeWidthDepth, [=]( double w_, double d_ ) {
                                                controller->setInputVolumeWidthDepth( w_, d_ );
                                                emit updateScenes();
                                                } );


    connect( &scene_path, &PathScene::curveAccepted, [=]( const Curve2D& c_ ){
                                                 controller->setPathCurvetoCurrentObject( c_ );
                                                 } );


    connect( this, &MainWindow::updateScenes, &sketch_scene, &SketchScene::updateScene );

    connect( this, SIGNAL( updateCanvas() ), canvas2d, SLOT( update() ) );

}


void MainWindow::getCurrentDirectory()
{

    QDir app_dir = QDir( qApp->applicationDirPath() );

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    QString current_dir ( app_dir.path ()+"\\" );

#elif defined(__linux__)               // Linux Directory Style
    QString current_dir ( app_dir.path ( ) + "/" );

#else
    /* Error, both can't be defined or undefined same time */
    std::cout << "Operate System not supported !"
    halt();

#endif

    scene3d.setCurrentDirectory( current_dir.toStdString() );

}


void MainWindow::run_app()
{

    defineColor( true );
    controller->init();
    setupCrossSectionsDiscretization();

    emit updateCanvas();

}


void MainWindow::setupCrossSectionsDiscretization()
{
    std::size_t depth_ = 1;
    std::size_t step_ = 1;
    controller->setupDepthResolution( depth_, step_ );

    std::cout << "Slider goes from 0 to " << depth_ << std::endl << std::flush;

    sl_depth_csection->setMinimum( 0 );
    sl_depth_csection->setMaximum( (int)depth_);
    sl_depth_csection->setValue( depth_ );
}


void MainWindow::startController()
{
    controller = new Controller();
    controller->setScene3D( &scene3d );
    controller->setSketchScene( &sketch_scene );
    controller->setPathScene( &scene_path );
    controller->setObjectTree( object_tree );
}


void MainWindow::createActions()
{
    createGeneralActions();
    createAppRelatedActions();
}


void MainWindow::createGeneralActions()
{

    ac_clear = new QAction( "Clear", this );
    connect( ac_clear, &QAction::triggered, this, &MainWindow::clear );

    ac_undo = new QAction( "Undo", this );
    connect( ac_undo, &QAction::triggered, this, &MainWindow::undo );


    ac_redo = new QAction( "Redo", this );
    connect( ac_redo, &QAction::triggered, this, &MainWindow::redo );


    ac_discard_sketch = new QAction( "Discard sketch", this );
    connect( ac_discard_sketch, &QAction::triggered, &sketch_scene, &SketchScene::clearSketch );


    ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, this, &MainWindow::screenshot );


    tb_general = new QToolBar( this );
    tb_general->addAction( ac_clear );
    tb_general->addAction( ac_undo );
    tb_general->addAction( ac_redo );
    tb_general->addAction( ac_screenshot );
    addToolBar( tb_general );

}


void MainWindow::createAppRelatedActions()
{
    ac_stratigraphy = new QAction( "Stratigraphy", this );
    ac_stratigraphy->setCheckable( true );
    connect( ac_stratigraphy, &QAction::triggered,
             [=](){ controller->setTypeCurrentObject( Object::TYPE::Stratigraphy );
                    dw_sketch_path_canvas->setVisible( false ); } );

    ac_channel = new QAction( "Channel", this );
    ac_channel->setCheckable( true );
    connect( ac_channel, &QAction::triggered,
             [=](){ controller->setTypeCurrentObject( Object::TYPE::Channel );
                    dw_sketch_path_canvas->setVisible( true ); } );


    ag_surface_type = new QActionGroup( this );
    ag_surface_type->addAction( ac_stratigraphy );
    ag_surface_type->addAction( ac_channel );
    ac_stratigraphy->setChecked( true );



    ac_sketch_above = new QAction( "SA", this );
    ac_sketch_above->setCheckable( true );
    connect( ac_sketch_above, &QAction::triggered, [=]( bool status_ ){
                                                   controller->enableCreateAbove( status_ ); } );

    ac_sketch_below = new QAction( "SB", this );
    ac_sketch_below->setCheckable( true );
    connect( ac_sketch_below, &QAction::triggered, [=]( bool status_ ){
                                                   controller->enableCreateBelow( status_ ); } );



    ac_remove_above  = new QAction( "RA", this );
    ac_remove_above->setCheckable( true );
    connect( ac_remove_above, &QAction::triggered,
             [=](){ controller->updateRule( "RA_SKETCHING" ); } );

    ac_remove_above_int  = new QAction( "RAI", this );
    ac_remove_above_int->setCheckable( true );
    connect( ac_remove_above_int, &QAction::triggered,
             [=](){ controller->updateRule( "RAI_SKETCHING" ); } );

    ac_remove_below  = new QAction( "RB", this );
    ac_remove_below->setCheckable( true );
    connect( ac_remove_below, &QAction::triggered,
             [=](){ controller->updateRule( "RB_SKETCHING" ); } );

    ac_remove_below_int  = new QAction( "RBI", this );
    ac_remove_below_int->setCheckable( true );
    connect( ac_remove_below_int, &QAction::triggered,
             [=](){ controller->updateRule( "RBI_SKETCHING" ); } );


    ag_stratigraphy_rules = new QActionGroup( this );
    ag_stratigraphy_rules->addAction( ac_remove_above );
    ag_stratigraphy_rules->addAction( ac_remove_above_int );
    ag_stratigraphy_rules->addAction( ac_remove_below );
    ag_stratigraphy_rules->addAction( ac_remove_below_int );
    ac_remove_above_int->setChecked( true );




    ac_discard_sketch = new QAction( "Discard", this );
    connect( ac_discard_sketch, &QAction::triggered, [=](){ sketch_scene.clearSketch(); } );

    ac_commit_sketch = new QAction( "Commit", this );
    connect( ac_commit_sketch, &QAction::triggered, [=](){ sketch_scene.finishSketch(); } );

    connect( &sketch_scene, &SketchScene::curveAccepted, [=]( const Curve2D& c_ ){
                                                 controller->addInputCurvetoCurrentObject( c_ ); } );

    ac_interpolate = new QAction( "Generate", this );
    connect( ac_interpolate, &QAction::triggered, [=](){ emit sketch_scene.interpolateObject(); } );

    connect( &sketch_scene, &SketchScene::interpolateObject, this, &MainWindow::interpolate );


    cd_pickercolor = new QColorDialog();
    cd_pickercolor->setWindowFlags( Qt::Widget );
    cd_pickercolor->setCurrentColor( QColor( 255, 0, 0 ) );

    ac_sketchcolor = new QWidgetAction( this );
    ac_sketchcolor->setDefaultWidget( cd_pickercolor );


    mn_pickercolor = new QMenu();
    mn_pickercolor->addAction( ac_sketchcolor );

    tbt_colorsketch = new QToolButton;
    tbt_colorsketch->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_colorsketch->setIcon( QIcon( ":/images/icons/border_color.png" ) );
    tbt_colorsketch->setMenu( mn_pickercolor );
    tbt_colorsketch ->setCheckable( true );


    connect( tbt_colorsketch, &QToolButton::toggled, [=]( bool status_ ){ defineColor( !status_ ); } );

    connect( cd_pickercolor, &QColorDialog::colorSelected, [=]( const QColor& c_ ){
        tbt_colorsketch->setChecked( true );
        defineColor( false, c_ ); } );


    tb_sketch = new QToolBar( this );
    tb_sketch->addActions( ag_surface_type->actions() );
    tb_sketch->addSeparator();
    tb_sketch->addAction( ac_sketch_above );
    tb_sketch->addAction( ac_sketch_below );
    tb_sketch->addSeparator();
    tb_sketch->addActions( ag_stratigraphy_rules->actions() );
    tb_sketch->addSeparator();
    tb_sketch->addAction( ac_discard_sketch );
    tb_sketch->addAction( ac_commit_sketch );
    tb_sketch->addAction( ac_interpolate );
    tb_sketch->addSeparator();
    tb_sketch->addWidget( tbt_colorsketch );
    addToolBar( tb_sketch );
}


void MainWindow::clear()
{
    controller->clear();
    resetInterface();
    run_app();
}


void MainWindow::resetInterface()
{
    dw_sketch_canvas->setVisible( true );
    dw_sketch_path_canvas->setVisible( false );
    ac_stratigraphy->setChecked( true );
    tbt_colorsketch->setChecked( true );
    ac_remove_above_int->setChecked( true );
    ac_undo->setEnabled( true );
    ac_redo->setEnabled( true );
}


void MainWindow::undo()
{
    bool undo_ok = controller->undo();
    if( undo_ok == false ) return;

    ac_undo->setEnabled( controller->canUndo() );
    ac_redo->setEnabled( controller->canRedo() );
    emit updateScenes();
}


void MainWindow::redo()
{
    //TODO: update scenes should be called from controller
    bool redo_ok = controller->redo();
    if( redo_ok == false ) return;

    ac_undo->setEnabled( controller->canUndo() );
    ac_redo->setEnabled( controller->canRedo() );
    emit updateScenes();
}


void MainWindow::screenshot()
{
    sketch_scene.screenshot();
//    scene3d.screenshot();
}


void MainWindow::interpolate()
{

    bool created = controller->interpolate();
    if( created == true )
    {
        defineColor( true );
        controller->createObject();
    }

    ac_undo->setEnabled( controller->canUndo() );
    ac_redo->setEnabled( controller->canRedo() );

    emit updateScenes();
}


void MainWindow::defineColor( bool aleatory_, QColor c_ )
{
    QColor color = c_;

    if( aleatory_ == true ){
        int r_ = 0, g_ = 0, b_ = 0;
        Model3DUtils::randomColor( r_, g_, b_ );
        color = QColor( r_, g_, b_ );
    }

    controller->setCurrentColor( color.red(), color.green(), color.blue() );

}


void MainWindow::exportTo()
{
//    QString selected_format = "";
////    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./exported/",
////                                                         "CPS3 files (*.CPS3);;Irap Classic Grid (*.IRAPG)", &selected_format );

//    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./exported/irapg/",
//                                                         "Irap Classic Grid (*.IRAPG)", &selected_format );

//    if( filename.isEmpty() == true ) return;

//    if( selected_format == QString( "CPS3 files (*.CPS3)" ) )
//    {
//        emit saveAsCPS3( filename.toStdString() );
//    }

//    else if( selected_format == QString( "Irap Classic Grid (*.IRAPG)" ) )
//    {
//        emit saveAsIrapGrid( filename.toStdString() );
//    }


}


void MainWindow::keyPressEvent( QKeyEvent *event )
{

    switch ( event->key() )
    {

        case Qt::Key_A:
        {

            if( event->modifiers() & Qt::AltModifier )
            {
                controller->enableCreateAbove( false );
            }

            else if( event->modifiers() & Qt::ControlModifier )
            {
                controller->updateRule( "RA_SKETCHING" );
            }

            else if( event->modifiers() & Qt::ShiftModifier )
            {
                controller->updateRule( "RAI_SKETCHING" );
            }

            else
            {
                controller->enableCreateAbove( true );
            }
        }
        break;

        case Qt::Key_B:
        {
            if( event->modifiers() & Qt::AltModifier )
            {
                controller->enableCreateBelow( false );
            }

            else if( event->modifiers() & Qt::ControlModifier )
            {
                controller->updateRule( "RB_SKETCHING" );
            }

            else if( event->modifiers() & Qt::ShiftModifier )
            {
                controller->updateRule( "RBI_SKETCHING" );
            }

            else
            {
                controller->enableCreateBelow( true );
            }
        }
        break;


        case Qt::Key_E:
        {
            ac_screenshot->trigger();
//            exportTo();
        }
        break;

        case Qt::Key_I:
        {
            controller->interpolate();

            emit enableUndo( controller->canUndo() );
            emit enableRedo( controller->canRedo() );

            emit updateScenes();
        }
        break;



        case Qt::Key_S:
        {
            controller->updateRule( "SKETCHING" );
        }
        break;

        case Qt::Key_N:
        {
            ac_clear->trigger();
        }
        break;



        default:
            break;


    };


}

