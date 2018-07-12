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



#include <QToolBar>
#include <QFileDialog>

#include "sketchwindow.h"


SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{
//    createWindow();
    createToolBar();
}

///================================================================================

void SketchWindow::createToolBar()
{

    tb_sketch = addToolBar( "Edit Sketch" );

    ac_sketch_color = new QAction( "Color", this );
    ac_cancel_sketch = new QAction( "Cancel", this );
    ac_submit_sketch = new QAction( "Submit", this );
    ac_end_object = new QAction( "End", this );

    tb_sketch->addAction( ac_sketch_color );
    tb_sketch->addAction( ac_cancel_sketch );
    tb_sketch->addAction( ac_submit_sketch );
    tb_sketch->addAction( ac_end_object );

    tb_boundary = addToolBar( "Boundary" );
    ac_resize_boundary = new QAction( "Resize boundary", this );
    ac_resize_boundary->setCheckable( true );
    ac_resize_boundary->setChecked( RESIZE_BOUNDARY_DEFAULT_STATUS );
    tb_boundary->addAction( ac_resize_boundary );

    tb_region = addToolBar( "Region" );
    ac_select_regions = new QAction( "Select Regions", this );
    ac_select_regions->setCheckable( true );
    ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );
    tb_region->addAction( ac_select_regions );

    tb_well = addToolBar( "Well" );
    ac_select_wells = new QAction( "Select Well", this );
    ac_select_wells->setCheckable( true );
    ac_select_wells->setChecked( SELECT_WELLS_DEFAULT_STATUS );
    tb_well->addAction( ac_select_wells );


    ///================================================================================


//    QToolBar *tb_actions = new QToolBar();


//    cp_color = new ColorPicker( this );
//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ ){ emit defineColorCurrent( color_ ); } );

//    ac_discard = new QAction( "Discard", this );
//    ac_discard->setIcon(QIcon(":/images/icons/denyCurve.png"));

//    ac_commit = new QAction( "Commit", this );
//    ac_commit->setIcon(QIcon(":/images/icons/add_curve2.png"));

//    ac_create = new QAction( "Create", this );
//    ac_create->setIcon(QIcon(":/images/icons/accept.png"));
//    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );


//    ac_edit_boundary = new QAction( "Edit Boundary", this );
//    ac_edit_boundary->setIcon(QIcon(":/images/icons/newBoundary.png"));
//    ac_edit_boundary->setCheckable( true );

//    ac_edit_scene = new QAction( "Edit Scene", this );
//    ac_edit_scene->setIcon(QIcon(":/images/icons/select_curve.png"));
//    ac_edit_scene->setCheckable( true );

//    ac_screenshot = new QAction( "Screenshot", this );
//    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));
//    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );


//    ac_axes = new QAction( "Axes", this );
//    ac_axes->setIcon(QIcon(":/images/icons/axes.png"));
//    ac_axes->setCheckable( true );
//    ac_axes->setChecked( true );

//    ac_height_map = new QAction( "Map", this );
//    connect( ac_height_map, &QAction::triggered, [=](){ if( tv_main != nullptr ) std::cout << "tv_main->devicePixelRatio() = " << tv_main->devicePixelRatio() << std::endl << std::flush;
//                                                                                              emit getHeightMap(); } );

//    ac_fixed_csections = new QAction( "Fixed Cross-Sections", this );
//    ac_fixed_csections->setIcon(QIcon(":/images/icons/fixedcsections.png"));
//    ac_fixed_csections->setCheckable( true );
//    connect( ac_fixed_csections, &QAction::toggled, cs, &QDockWidget::setVisible );


//    ac_enable_preview = new QAction( "Enable Preview", this );
//    ac_enable_preview->setIcon(QIcon(":/images/icons/preview.png"));
//    ac_enable_preview->setCheckable( true );
//    ac_enable_preview->setChecked( true );

//    connect( ac_enable_preview, &QAction::toggled, [=]( bool status_ ){ emit enablePreview( status_ ); } );


//    tb_actions->addWidget( cp_color );
//    tb_actions->addSeparator();
//    tb_actions->addAction( ac_discard );
//    tb_actions->addAction( ac_commit );
//    tb_actions->addAction( ac_create );
//    tb_actions->addSeparator();
//    tb_actions->addAction( ac_edit_boundary );
//    tb_actions->addAction( ac_edit_scene );
//    tb_actions->addSeparator();
//    tb_actions->addAction( ac_enable_preview );
//    tb_actions->addAction( ac_fixed_csections );
//    tb_actions->addAction( ac_axes );
////    tb_actions->addAction( ac_height_map );
//    tb_actions->addAction( ac_screenshot );


//    addToolBar( tb_actions );

}


std::shared_ptr< SketchScene > SketchWindow::createMainCanvas()
{
    fixed_csections_canvas = new CanvasStack();
    fixed_csections_canvas->setWindowTitle( "Fixed Cross-Sections" );
    fixed_csections_canvas->setVisible( false );

    sketchingcanvas = new SketchingCanvas();
    const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
    setCentralWidget( sketchingcanvas );


    connect( ac_sketch_color, &QAction::triggered, scene_.get(), &SketchScene::setSketchColor );
    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );
    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );
    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );
//    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit addCurve( curve_, dir_, depth_ ); }  );

    return scene_;
}


std::shared_ptr< SketchScene > SketchWindow::addCanvas( double depth_ )
{
    SketchingCanvas* canvas_ = new SketchingCanvas();
    const std::shared_ptr< SketchScene >&scene_ = canvas_->getScene();
    fixed_csections_canvas->addElement( depth_, canvas_ );
    fixed_csections_canvas->setVisible( true );

    connect( ac_sketch_color, &QAction::triggered, scene_.get(), &SketchScene::setSketchColor );
    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );
    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );
    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );
//    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit addCurve( curve_, dir_, depth_ ); }  );


    return scene_;
}


void SketchWindow::removeCanvas( double depth_ )
{
    if( fixed_csections_canvas->findElement( depth_ ) == false ) return;
    fixed_csections_canvas->removeElement( depth_ );
}


SketchWindow::~SketchWindow()
{
    delete fixed_csections_canvas;
    fixed_csections_canvas = nullptr;

    delete sketchingcanvas;
    sketchingcanvas = nullptr;

    delete ac_sketch_color;
    ac_sketch_color = nullptr;

    delete ac_cancel_sketch;
    ac_cancel_sketch = nullptr;

    delete ac_submit_sketch;
    ac_submit_sketch = nullptr;

    delete ac_end_object;
    ac_end_object = nullptr;

    delete tb_boundary;
    tb_boundary = nullptr;

    delete ac_resize_boundary;
    ac_resize_boundary = nullptr;

    delete tb_region;
    tb_region = nullptr;

    delete ac_select_regions;
    ac_select_regions = nullptr;

    delete tb_well;
    tb_well = nullptr;

    delete ac_select_wells;
    ac_select_wells = nullptr;
}



//void SketchWindow::createWindow()
//{
//    cs = new CanvasStack();
//    cs->setWindowTitle( "Fixed Cross-Sections" );


//    cs->setVisible( false );
//    cs->setMinimumSize( WIDTH_APP, HEIGHT_APP );

//    connect( cs, &CanvasStack::closeSubWindow, this, &SketchWindow::removeFixedCrossSectionCanvas );

//    hb_central_widget = new QHBoxLayout( this );

//    QWidget* central_widget = new QWidget( this );
//    central_widget->setLayout( hb_central_widget );
//    setCentralWidget( central_widget );

//}


//void SketchWindow::addMainCanvas( CrossSection* const& cs_ )
//{

//    if( cs_ == nullptr ) return;

//    main_scene = new SketchScene( cs_ );
//    main_scene->setCurrent( true );

//    main = new QGraphicsView();

//    main->scale( 1, -1 );
//    main->setScene( main_scene );

//    ////// teste

//    QColor color_ = cp_color->currentColor();
//    main_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() );

//    connect( ac_discard, &QAction::triggered, [=](){ if( main_scene == nullptr) return; emit main_scene->discard(); } );

//    connect( ac_commit, &QAction::triggered, [=](){ if( main_scene == nullptr) return; emit main_scene->commit(); } );


//    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( main_scene == nullptr) return; main_scene->edit( status_ ); } );

//    connect( ac_edit_boundary, &QAction::triggered, [=]( bool status_ ){ if( main_scene == nullptr) return;
//                                                    if( status_ == true ) main_scene->setModeEditingBoundary();
//                                                    else main_scene->setModeSketching(); } );

//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
//                                                    { if( main_scene == nullptr) return; main_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

//    connect( ac_axes, &QAction::triggered, main_scene, &SketchScene::setAxesVisible );



//    connect( main_scene, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

//    connect( main_scene, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

//    connect( main_scene, &SketchScene::commitObject, [=](){ emit commitObject(); } );

//    connect( main_scene, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

//    connect( main_scene, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
//                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ ); updateCanvas(); }  );


//    connect( main_scene, &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
//                                                            { emit removeCurveFromObject( depth_, index_ ); } );


//    connect( main_scene, &SketchScene::removeImageFromCrossSection, [=]( double depth_ )
//                                                            { emit removeImageFromCrossSection( depth_ ); } );




//    ////// teste

//    ac_height_map->setVisible( false );
//    hb_central_widget->insertWidget( 0, main );

//}


//void SketchWindow::addTopViewCanvas( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;

//    tv_scene = new SketchScene( cs_ );
//    tv_scene->setCurrent( true );

//    tv_main = new QGraphicsView();
//    tv_main->setScene( tv_scene );

//    ////// teste

//    QColor color_ = cp_color->currentColor();
//    tv_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() );

//    connect( ac_discard, &QAction::triggered, [=](){ if( tv_scene == nullptr) return; emit tv_scene->discard(); } );

//    connect( ac_commit, &QAction::triggered, [=](){ if( tv_scene == nullptr) return; emit tv_scene->commit(); } );

////    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

//    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( tv_scene == nullptr) return; tv_scene->edit( status_ ); } );

//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
//    { if( tv_scene == nullptr) return; tv_scene->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

//    connect( ac_axes, &QAction::triggered, tv_scene, &SketchScene::setAxesVisible );



//    connect( tv_scene, &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

//    connect( tv_scene, &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

//    connect( tv_scene, &SketchScene::commitObject, [=](){ emit commitObject(); } );

//    connect( tv_scene, &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

//    connect( tv_scene, &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
//    { emit setImageToTopView( file_, ox_, oy_, x_, y_ ); } );


//    connect( tv_scene, &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
//    { emit removeCurveFromObject( depth_, index_ ); } );


//    connect( tv_scene, &SketchScene::removeImageFromCrossSection, [=](){ emit removeImageFromTopView(); } );

//    ////// teste

//    ac_fixed_csections->setVisible( false );
//    ac_edit_boundary->setVisible( false );
//    ac_enable_preview->setVisible( false );
//    hb_central_widget->insertWidget( 0, tv_main );

//}



//void SketchWindow::updateCanvas()
//{

//    if( main != nullptr )
//    {

//        if( main_scene != nullptr)
//        {
//            main_scene->updateVolume();
//            main_scene->updateCrossSection();
//        }

//    }

//    if( tv_main != nullptr )
//    {

//        if( tv_scene != nullptr)
//        {
//            tv_scene->updateVolume();
//            tv_scene->updateCrossSection();
//        }

//    }

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateVolume();
//        sc_->updateCrossSection();
//    }

//}




//void SketchWindow::addCrossSection( CrossSection* const& cs_ )
//{
//    if( cs_ == nullptr ) return;

//    if( tv_scene == nullptr) return;
//    tv_scene->addCrossSection( cs_ );

//}




//void SketchWindow::addObject( Object* const& obj_ )
//{

//    if( main == nullptr ) return;
//    if( main_scene == nullptr) return;

//    main_scene->addObject( obj_ );

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->addObject( obj_ );
//    }

//}


//void SketchWindow::updateObject( const std::size_t& index_ )
//{

//    if( main == nullptr ) return;
//    if( main_scene == nullptr) return;

//    main_scene->updateObject( index_ );


//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->updateObject( index_ );
//    }
//}




//void SketchWindow::addTrajectory( Object* const& obj_ )
//{

//    if( tv_scene == nullptr ) return;
//    tv_scene->addTrajectory( obj_ );
//}


//void SketchWindow::updateTrajectory( const std::size_t& index_ )
//{
//    if( tv_scene == nullptr ) return;
//    tv_scene->updateTrajectory( index_ );
//}





//void SketchWindow::setModeSelecting()
//{
//    if( main == nullptr ) return;
//    if( main_scene == nullptr) return;

//    main_scene->setModeSelecting();

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSelecting();
//    }
//}


//void SketchWindow::setModeSketching()
//{
//    if( main == nullptr ) return;
//    if( main_scene == nullptr) return;

//    main_scene->setModeSketching();

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setModeSketching();
//    }

//}



//void SketchWindow::setCurrentColor( int r_, int g_, int b_ )
//{
//    cp_color->setColor( QColor( r_, g_, b_ ) );

//    if( main != nullptr )
//    {
//        if( main_scene != nullptr)
//            main_scene->setCurrentColor( r_, g_, b_ );

//    }
//    if( tv_main != nullptr )
//    {
//        if( tv_scene != nullptr )
//            tv_scene->setCurrentColor( r_, g_, b_ );

//    }

//    for ( CanvasContainer::Iterator it =  cs->begin(); it != cs->end(); ++it )
//    {
//        QGraphicsView* gview_ = cs->getElement( it->first );
//        SketchScene* sc_ = ( SketchScene* )( gview_->scene() );
//        sc_->setCurrentColor( r_, g_, b_ );
//    }

//    updateCanvas();
//}





//void SketchWindow::clear()
//{
//    if( main != nullptr )
//    {
//        if( main_scene != nullptr )
//        {
//            main_scene->clear();
//            delete main_scene;
//            main_scene = nullptr;
//        }
//        delete main;
//        main = nullptr;
//    }


//    if( tv_main != nullptr )
//    {
//        if( tv_scene != nullptr )
//        {
//            tv_scene->clear();
//            delete tv_scene;
//            tv_scene = nullptr;
//        }
//        delete tv_main;
//        tv_main = nullptr;
//    }


//    std::map< double, SketchScene* >::iterator it_ =  scenes.begin();
//    while( it_ != scenes.end() )
//    {
//        if( it_->second == nullptr )
//            scenes.erase( it_->first );
//        else
//            removeFixedCrossSectionCanvas( it_->first );

//        it_ =  scenes.begin();
//    }
//    scenes.clear();

//    if( cs == nullptr ) return;
//    cs->setVisible( false );


//    CanvasContainer::Iterator it =  cs->begin();
//    while( it != cs->end() )
//    {
//        cs->removeElement( it->first );
//        it =  cs->begin();
//    }
//    cs->clear();


//}




//void SketchWindow::addFixedCrossSectionCanvas( CrossSection* const& cs_, QColor c_ )
//{
//    if( cs_ == nullptr ) return;

//    cs->setVisible( true );
//    cs->raise();

//    scenes[ cs_->getDepth() ] = new SketchScene( cs_ );
//    SketchScene* const& scene_ = scenes[ cs_->getDepth() ];


//    QColor color_ = cp_color->currentColor();
//    scene_->addLabel( cs_->getDepth(), c_ );
//    scene_->setCurrentColor( color_.red(), color_.green(), color_.blue() );
//    scene_->setCurrent( true );

//    QGraphicsView* gv_ = new QGraphicsView();
//    gv_->scale( 1, -1 );
//    gv_->setScene( scene_ );
//    gv_->setWindowTitle( QString( "CrossSection %1" ).arg( cs_->getDepth() ) );
//    cs->addElement( cs_->getDepth(), gv_ );


//    connect( ac_discard, &QAction::triggered, [=](){ if( scenes[ cs_->getDepth() ] == nullptr) return; emit scenes[ cs_->getDepth() ]->discard(); } );

//    connect( ac_commit, &QAction::triggered, [=](){ if( scenes[ cs_->getDepth() ] == nullptr) return; emit scenes[ cs_->getDepth() ]->commit(); } );

////    connect( ac_create, &QAction::triggered, [=](){ emit commitObject(); } );

//    connect( ac_edit_scene, &QAction::triggered, [=]( bool status_ ){ if( scenes[ cs_->getDepth() ] == nullptr) return; scenes[ cs_->getDepth() ]->edit( status_ ); } );

//    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor color_ )
//                                                    { if( scenes[ cs_->getDepth() ] == nullptr) return; scenes[ cs_->getDepth() ]->setCurrentColor( color_.red(), color_.green(), color_.blue() ); } );

//    connect( ac_axes, &QAction::triggered, scene_, &SketchScene::setAxesVisible );



//    connect( scenes[ cs_->getDepth() ], &SketchScene::acceptVolumeDimensions, [=]( Settings::CrossSection::CrossSectionDirections dir_, double w, double h ){ emit updateVolume( dir_, w, h ); } );

//    connect( scenes[ cs_->getDepth() ], &SketchScene::acceptCurve, [=]( const PolyCurve& curve_, double depth_ ){ emit acceptCurve( curve_, depth_ ); } );

//    connect( scenes[ cs_->getDepth() ], &SketchScene::commitObject, [=](){ emit commitObject(); } );

//    connect( scenes[ cs_->getDepth() ], &SketchScene::objectSelected, [=]( std::size_t index_ ){ emit objectSelected( index_ ); } );

//    connect( scenes[ cs_->getDepth() ], &SketchScene::setImageCrossSection, [=]( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
//                                                            { emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ ); main_scene->updateCrossSection(); } );


//    connect( scenes[ cs_->getDepth() ], &SketchScene::removeCurveFromObject, [=]( double depth_, std::size_t index_ )
//                                                            { emit removeCurveFromObject( depth_, index_ ); } );


//    connect( scenes[ cs_->getDepth() ], &SketchScene::removeImageFromCrossSection, [=]( double depth_ )
//                                                            { emit removeImageFromCrossSection( depth_ ); } );


//}


//bool SketchWindow::removeFixedCrossSectionCanvas( double depth_ )
//{

//    if( scenes.find( depth_ ) != scenes.end() )
//    {
//        scenes[ depth_ ]->clear();
//        delete scenes[ depth_ ];
//        scenes.erase( depth_ );
//    }

//    if( cs->findElement( depth_  ) == false ) return false;

//    cs->removeElement( depth_ );

//    emit removeFixedCrossSection( depth_ );
//    return true;

//}


//void SketchWindow::setFixedCrossSectionsVisible( bool status_ )
//{
//    dw_canvas_stack->setVisible( true );
//}


//void SketchWindow::setCurrentCrossSection( double depth_ )
//{
//    if( tv_scene != nullptr )
//    {
//        tv_scene->moveCurrentCrossSection( depth_ );
//    }

//    if( cs->findElement( depth_ ) == false ) return;
//    cs->setCurrent( depth_ );
//}



//void SketchWindow::setTopViewImage( const std::string& image_ )
//{
//    if( tv_main == nullptr ) return;

//    if( tv_scene != nullptr )
//    {
//        tv_scene->setImageToCrossSection( QString( image_.c_str() ) );
//    }

//    update();
//}



//void SketchWindow::setCrossSectionImage( double depth_, const QString& file_, double ox_, double oy_, double x_, double y_ )
//{

//    emit setImageCrossSection( depth_, file_, ox_, oy_, x_, y_ );


//    if( main_scene == nullptr ) return;
//    main_scene->updateCrossSection();
//}


//void SketchWindow::screenshot()
//{
//    QString selectedFilter;
//    QString name_of_file = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
//                                                         tr( "PNG (*.png);;SVG (*.svg)" ),
//                                                         &selectedFilter );


//    if( main_scene != nullptr)
//    {
//        if( selectedFilter == "PNG (*.png)" )
//        {
//            main_scene->savetoRasterImage( name_of_file );
//        }
//        else if ( selectedFilter == "SVG (*.svg)" )
//        {
//            main_scene->savetoVectorImage( name_of_file );
//        }
//    }
//    else if( tv_scene != nullptr)
//    {
//        if( selectedFilter == "PNG (*.png)" )
//        {
//            tv_scene->savetoRasterImage( name_of_file );
//        }
//        else if ( selectedFilter == "SVG (*.svg)" )
//        {
//            tv_scene->savetoVectorImage( name_of_file );
//        }
//    }

//}
