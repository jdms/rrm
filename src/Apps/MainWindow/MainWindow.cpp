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

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);


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

    createVolumePropertiesSection();
    createObjectPropertiesSection();


    ///////////////////////////////////////////


    setFocusProxy( sl_depth_csection );
    getCurrentDirectory();

    connect( sl_depth_csection, &QSlider::valueChanged, [=]( int v ){
                   double depth_ = controller->depthFromRowIndex( (std::size_t ) v );
                   controller->setCurrentCrossSection( depth_ );
                   emit updateScenes(); } );

    connect( canvas3d, &Canvas3D::increaseSlider, [=](){
                                                        int value = sl_depth_csection->value() + 1;
                                                        sl_depth_csection->setValue( value );
                                                  } );

    connect( canvas3d, &Canvas3D::decreaseSlider, [=](){
                                                        int value = sl_depth_csection->value() - 1;
                                                        sl_depth_csection->setValue( value );
                                                  } );


    connect( canvas_path, &CanvasPath::increaseSlider, [=](){
                                                        int value = sl_depth_csection->value() + 1;
                                                        sl_depth_csection->setValue( value );
                                                  } );

    connect( canvas_path, &CanvasPath::decreaseSlider, [=](){
                                                        int value = sl_depth_csection->value() - 1;
                                                        sl_depth_csection->setValue( value );
                                                  } );


}


void MainWindow::create3dSection()
{

    canvas3d = new Canvas3D();
    canvas3d->setScene( &scene3d );

    sl_depth_csection = new QSlider( Qt::Vertical );
    sl_depth_csection->setTickPosition( QSlider::TicksRight );
    sl_depth_csection->setInvertedAppearance( true );
    sl_depth_csection->setFocusPolicy( Qt::WheelFocus );
    sl_depth_csection->setFocus();

    QHBoxLayout* hl_window3d = new QHBoxLayout( this );
    hl_window3d->addWidget( canvas3d );
    hl_window3d->addWidget( sl_depth_csection );

    QWidget *wd_window3d = new QWidget( this );
    wd_window3d->setLayout( hl_window3d );


    connect( this, SIGNAL( updateCanvas() ), canvas3d, SLOT( update() ) );
    connect( this, &MainWindow::updateScenes, &scene3d, &Scene3D::updateScene );

    setCentralWidget( wd_window3d );

}


void MainWindow::createObjectTreeSection()
{
    object_tree = new ObjectTree( this );
    object_tree->setMaximumWidth( 0.2*app_width );


    dw_object_tree = new QDockWidget( "" );
    dw_object_tree->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_object_tree->setWidget( object_tree );
    dw_object_tree->setVisible( false );
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


    connect( object_tree, &ObjectTree::itemClicked, this, &MainWindow::editObjectTreeItem );

}


void MainWindow::createSketchSection()
{
    canvas2d = new SketchCanvas();
    canvas2d->setScene( &sketch_scene );
    canvas2d->setMinimumHeight( 0.4f* ( app_height - 10 ) );


    QMainWindow* mw_sketch_window = new QMainWindow();
    mw_sketch_window->setCentralWidget( canvas2d );

    ac_discard_sketch = new QAction( "Discard", this );
    connect( ac_discard_sketch, &QAction::triggered, [=](){ sketch_scene.clearSketch(); } );

    ac_commit_sketch = new QAction( "Commit", this );
    connect( ac_commit_sketch, &QAction::triggered, [=](){ sketch_scene.finishSketch(); } );

    connect( &sketch_scene, &SketchScene::curveAccepted, [=]( const Curve2D& c_ ){
                                                 controller->addInputCurvetoCurrentObject( c_ );
                                                 sl_depth_volume->setDisabled( true );
                                                 sl_height_volume->setDisabled( true );
                                                 sl_width_volume->setDisabled( true );
                                                 sp_depth_volume->setEnabled( false );
                                                 sp_width_volume->setEnabled( false );
                                                 sp_height_volume->setEnabled( false );
                                                 ac_edit_boundary->setDisabled( true );

                                                 } );

    ac_interpolate = new QAction( "Generate", this );
    connect( ac_interpolate, &QAction::triggered, [=](){ emit sketch_scene.interpolateObject(); } );

    connect( &sketch_scene, &SketchScene::interpolateObject, this, &MainWindow::interpolate );

    ac_screenshot_sketch = new QAction( "Screenshot", this );
    connect( ac_screenshot_sketch, &QAction::triggered, &sketch_scene, &SketchScene::screenshot );


    ac_edit_boundary = new QAction( "Edit Boundary", this );
    connect( ac_edit_boundary,  &QAction::triggered, &sketch_scene, &SketchScene::setModeBoundaryEditing );


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

    connect( mn_pickercolor, &QMenu::aboutToShow, cd_pickercolor, &QColorDialog::show );
    connect( cd_pickercolor, &QColorDialog::rejected, mn_pickercolor, &QMenu::hide );
    connect( cd_pickercolor, &QColorDialog::accepted, mn_pickercolor, &QMenu::hide );






    QToolBar* tb_path1 = new QToolBar( this );

    tb_path1->addAction( ac_discard_sketch );
    tb_path1->addAction( ac_commit_sketch );
    tb_path1->addAction( ac_interpolate );
    tb_path1->addSeparator();
    tb_path1->addAction( ac_edit_boundary );
    tb_path1->addSeparator();
    tb_path1->addWidget( tbt_colorsketch );
    tb_path1->addAction( ac_screenshot_sketch );

    cp_teste = new ColorPicker();;
//    tb_path1->addWidget( cp_teste );


    mw_sketch_window->addToolBar( Qt::BottomToolBarArea, tb_path1 );


    dw_sketch_canvas = new QDockWidget( "Cross-Section" );
    dw_sketch_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_canvas->setWidget( mw_sketch_window );
    dw_sketch_canvas->setVisible( true );
    addDockWidget( Qt::BottomDockWidgetArea, dw_sketch_canvas );



    canvas_path = new CanvasPath();
    canvas_path->setScenePath( &scene_path );

    QMainWindow* mw_canvas_window = new QMainWindow();
    mw_canvas_window->setCentralWidget( canvas_path );

    QAction* ac_discard_path = new QAction( "Discard", mw_canvas_window );
    QAction* ac_commit_path = new QAction( "Commit", mw_canvas_window );
    QAction* ac_generate_in_path = new QAction( "Generate", mw_canvas_window );
    QAction* ac_screenshot_path = new QAction( "Screenshot", mw_canvas_window );



    connect( ac_discard_path, &QAction::triggered, &scene_path,  &PathScene::clearSketch );
    connect( ac_commit_path, &QAction::triggered, [=](){ scene_path.finishSketch(); } );
    connect( ac_generate_in_path, &QAction::triggered, [=](){ emit scene_path.interpolateObject(); } );
    connect( ac_screenshot_path, &QAction::triggered, &scene_path,  &PathScene::screenshot );

    connect( &scene_path, &PathScene::interpolateObject, this, &MainWindow::interpolate );


    QToolBar* tb_path = new QToolBar( this );
    tb_path->addAction( ac_discard_path );
    tb_path->addAction( ac_commit_path );
    tb_path->addAction( ac_generate_in_path );
    tb_path->addSeparator();
    tb_path->addAction( ac_screenshot_path );

    mw_canvas_window->addToolBar( Qt::BottomToolBarArea, tb_path );


    dw_sketch_path_canvas = new QDockWidget( "Trajectory" );
    dw_sketch_path_canvas->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_sketch_path_canvas->setWidget( mw_canvas_window );
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



void MainWindow::createVolumePropertiesSection()
{

    sl_width_volume->setRange( 0, 10000 );
    sl_height_volume->setRange( 0, 10000 );
    sl_depth_volume->setRange( 0, 10000 );

    sp_width_volume->setRange( 0, 10000 );
    sp_height_volume->setRange( 0, 10000 );
    sp_depth_volume->setRange( 0, 10000 );

    tb_properties->removeItem( 0 );
    tb_properties->addItem( gb_volume_properties, "Object Properties" );


}


void MainWindow::createObjectPropertiesSection()
{
    dw_properties = new QDockWidget( "", this );
    dw_properties->setAllowedAreas( Qt::LeftDockWidgetArea );
    dw_properties->setWidget( tb_properties );
    dw_properties->setVisible( false );
    addDockWidget( Qt::LeftDockWidgetArea, dw_properties );


    cd_color_object = new QColorDialog();
    cd_color_object->setWindowFlags( Qt::Widget );
    cd_color_object->setCurrentColor( QColor( 255, 0, 0 ) );

    ac_color_object = new QWidgetAction( this );
    ac_color_object->setDefaultWidget( cd_color_object );
    mn_color_object = new QMenu();
    mn_color_object->addAction( ac_color_object );

    tbt_color_object = new QToolButton;
    tbt_color_object->setPopupMode( QToolButton::MenuButtonPopup );
    tbt_color_object->setMenu( mn_color_object );
    tbt_color_object ->setCheckable( true );

    QPixmap px(20, 20);
    px.fill( QColor( 255, 0, 0 ) );
    tbt_color_object->setIcon( px );

    gl_object_properties->addWidget( tbt_color_object, 0, 2 );

    connect( mn_color_object, &QMenu::aboutToShow, cd_color_object, &QColorDialog::show );
    connect( cd_color_object, &QColorDialog::rejected, mn_color_object, &QMenu::hide );
    connect( cd_color_object, &QColorDialog::accepted, mn_color_object, &QMenu::hide );

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

    ac_object_tree = new QAction( "Object Tree", this );
    ac_object_tree->setCheckable( true );
    ac_object_tree->setChecked( false );
    connect( ac_object_tree, &QAction::toggled, dw_object_tree, &QDockWidget::setVisible );
    connect( ac_object_tree, &QAction::toggled, dw_properties, &QDockWidget::setVisible );


    ac_clear = new QAction( "Clear", this );
    connect( ac_clear, &QAction::triggered, this, &MainWindow::clear );


    ac_save_file = new QAction( "Save", this );
    connect( ac_save_file, &QAction::triggered, this, &MainWindow::saveFile );

    ac_load_file = new QAction( "Load", this );
    connect( ac_load_file, &QAction::triggered, this, &MainWindow::loadFile );


    ac_undo = new QAction( "Undo", this );
    connect( ac_undo, &QAction::triggered, this, &MainWindow::undo );


    ac_redo = new QAction( "Redo", this );
    connect( ac_redo, &QAction::triggered, this, &MainWindow::redo );


    ac_discard_sketch = new QAction( "Discard sketch", this );
    connect( ac_discard_sketch, &QAction::triggered, &sketch_scene, &SketchScene::clearSketch );


    ac_screenshot = new QAction( "Screenshot", this );
    connect( ac_screenshot, &QAction::triggered, this, &MainWindow::screenshot );


    ac_center_axes = new QAction( "Center Axes", this );
    ac_center_axes->setCheckable( true );
    connect( ac_center_axes, &QAction::toggled, &scene3d, &Scene3D::centerAxes );




    tb_general = new QToolBar( this );
    tb_general->addAction( ac_object_tree );
//    tb_general->addAction( ac_center_axes );
    tb_general->addSeparator();
    tb_general->addAction( ac_clear );
    tb_general->addSeparator();
    tb_general->addAction( ac_save );
    tb_general->addAction( ac_load );
    tb_general->addSeparator();
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
    ac_remove_above->setChecked( true );


    tb_sketch = new QToolBar( this );
    tb_sketch->addActions( ag_surface_type->actions() );
    tb_sketch->addSeparator();
    tb_sketch->addAction( ac_sketch_above );
    tb_sketch->addAction( ac_sketch_below );
    tb_sketch->addSeparator();
    tb_sketch->addActions( ag_stratigraphy_rules->actions() );

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
    ac_remove_above->setChecked( true );
    ac_sketch_above->setChecked( false );
    ac_sketch_below->setChecked( false );
    ac_undo->setEnabled( true );
    ac_redo->setEnabled( true );

    sl_depth_volume->setEnabled( true );
    sl_width_volume->setEnabled( true );
    sl_height_volume->setEnabled( true );

    sp_depth_volume->setEnabled( true );
    sp_width_volume->setEnabled( true );
    sp_height_volume->setEnabled( true );

    ac_edit_boundary->setEnabled( true );
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

    QString selectedFilter;
    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
                                                         &selectedFilter );


    if( selectedFilter == "PNG (*.png)" )
    {
        sketch_scene.savetoRasterImage( name_of_file );
        scene_path.savetoRasterImage( name_of_file );
        canvas3d->savetoRasterImage( name_of_file );
    }
    else if ( selectedFilter == "SVG (*.svg)" )
    {
        sketch_scene.savetoVectorImage( name_of_file );
        scene_path.savetoVectorImage( name_of_file );
        canvas3d->savetoVectorImage( name_of_file );
    }

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

         case Qt::Key_E:
        {
//            exportTo();
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




void MainWindow::on_sl_width_volume_sliderMoved( int position )
{
    controller->setInputVolumeWidth( (double) position );
    sp_width_volume->setValue( position );
    emit updateScenes();

}


void MainWindow::on_sl_height_volume_sliderMoved( int position )
{
    controller->setInputVolumeHeight( (double)position );
    sp_height_volume->setValue( position );
    emit updateScenes();
}


void MainWindow::on_sl_depth_volume_sliderMoved( int position )
{
    controller->setInputVolumeDepth( (double)position );
    sp_depth_volume->setValue( position );

    setupCrossSectionsDiscretization();

    emit updateScenes();
}




void MainWindow::editObjectTreeItem( QTreeWidgetItem* item_, int column_ )
{
    ObjectTreeItem* obj_ = (ObjectTreeItem*) item_;


    ObjectTreeItem::TreeItemType type = obj_->getType();
    if( type == ObjectTreeItem::TreeItemType::VOLUME )
        enableVolumeEditionProperty();
    else if( type == ObjectTreeItem::TreeItemType::OBJECT )
        enableObjectEditionProperty( obj_->getId() );

}


void MainWindow::enableVolumeEditionProperty()
{
    sw_properties_objects->setCurrentIndex( 0 );

    double w_, h_, d_;
    controller->getInputVolumeDimensions( w_, h_, d_ );

    sl_width_volume->setValue( (int) w_ );
    sl_height_volume->setValue( (int) h_ );
    sl_depth_volume->setValue( (int) d_ );

    sp_width_volume->setValue( (int) w_ );
    sp_height_volume->setValue( (int) h_ );
    sp_depth_volume->setValue( (int) d_ );
}


void MainWindow::enableObjectEditionProperty( const std::size_t& id_ )
{
    sw_properties_objects->setCurrentIndex( 1 );

    std::string name_;
    controller->getNameofObjectofId( id_, name_ );
    edt_object_name->setText( name_.c_str() );

    int r_ = 255, g_ = 0, b_ = 0;
    controller->getCurrentObjectColor( id_, r_, g_, b_ );
    cd_color_object->setCurrentColor( QColor( r_, g_, b_ ) );

    QPixmap px(20, 20);
    px.fill( QColor( r_, g_, b_ ) );
    tbt_color_object->setIcon( px );

}




void MainWindow::on_btn_save_object_clicked()
{
    QList<QTreeWidgetItem* > list_ = object_tree->selectedItems();
    if( list_.empty() == true ) return;

    ObjectTreeItem* obj_ = (ObjectTreeItem*) list_[0];
    controller->setNameofObjectofId( obj_->getId(), edt_object_name->text().toStdString() );

    QColor c_ = cd_color_object->currentColor();
    controller->setObjectColor( obj_->getId(), c_.red(), c_.green(), c_.blue() );

    QPixmap px(20, 20);
    px.fill( c_);
    tbt_color_object->setIcon( px );

    emit updateScenes();

}


void MainWindow::saveFile()
{
    QString filename = QFileDialog::getSaveFileName( this, tr( "Save File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );

    controller->saveFile( filename.toStdString() );

}

void MainWindow::loadFile()
{
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./saved/",
                                                             "rrm files (*.rrm)", &selected_format );

    controller->loadFile( filename.toStdString() );

}
