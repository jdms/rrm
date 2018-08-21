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
    createToolBar();
}


void SketchWindow::createToolBar()
{
    cp_color = new ColorPicker( this );


    tb_sketch = addToolBar( "Edit Sketch" );

    ac_sketch_color = new QAction( "Color", this );
    ac_cancel_sketch = new QAction( "Cancel", this );
    ac_submit_sketch = new QAction( "Submit", this );
    ac_end_object = new QAction( "End", this );

    tb_sketch->addWidget( cp_color );
//    tb_sketch->addAction( ac_sketch_color );
    tb_sketch->addAction( ac_cancel_sketch );
    tb_sketch->addAction( ac_submit_sketch );
    tb_sketch->addAction( ac_end_object );

    tb_boundary = addToolBar( "Boundary" );
    ac_resize_boundary = new QAction( "Resize boundary", this );
    ac_resize_boundary->setCheckable( true );
    ac_resize_boundary->setChecked( RESIZE_BOUNDARY_DEFAULT_STATUS );
    tb_boundary->addAction( ac_resize_boundary );

    tb_image = addToolBar( "Image" );
    ac_resize_image = new QAction( "Resize image", this );
    ac_resize_image->setCheckable( true );
    ac_remove_image = new QAction( "Remove image", this );
    tb_image->addAction( ac_resize_image );
    tb_image->addAction( ac_remove_image );


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

    tb_trajectory = addToolBar( "Trajectory" );
    ac_use_last_trajectory = new QAction( "Last trajectory" );
    ac_use_last_trajectory->setCheckable( true );
    ac_use_last_trajectory->setChecked( USE_TRAJECTORY_DEFAULT_STATUS );
    tb_trajectory->addAction( ac_use_last_trajectory );



}


std::shared_ptr< SketchScene > SketchWindow::createMainCanvas()
{
    fixed_csections_canvas = new CanvasStack();
    fixed_csections_canvas->setWindowTitle( "Fixed Cross-Sections" );
    fixed_csections_canvas->setVisible( false );

    sketchingcanvas = new SketchingCanvas();
    sketchingcanvas->scale( 1, -1 );
    const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
    setCentralWidget( sketchingcanvas );

    tb_trajectory->setVisible( false );


    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    {
        scene_->setSketchColor( color_ );
        emit defineColorCurrent( color_.red(), color_.green(), color_.blue() );
    } );


    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );
    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_remove_image, &QAction::triggered, scene_.get(), &SketchScene::removeImageInCrossSectionAndUpdate );

    connect( ac_resize_image, &QAction::triggered, scene_.get(), &SketchScene::setResizingImageMode );


    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );
    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );

//    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );

    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ );
    ac_resize_boundary->setChecked( false ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    { emit addCurve( curve_, dir_, depth_ ); }  );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::removeImageFromCrossSection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    {
      emit removeImageFromCrossSection( dir_, depth_ );
    } );

    connect( scene_.get(), &SketchScene::objectSelected, [=]( const std::size_t& id_ ) { emit objectSelected( id_ ); } );

        connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_ ) { emit sendSketchOfSelection( curve_ ); } );


    return scene_;
}


std::shared_ptr< SketchScene > SketchWindow::createTopViewCanvas()
{

    topviewcanvas = new SketchingCanvas();
    const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
    setCentralWidget( topviewcanvas );

    tb_trajectory->setVisible( true );


    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    {   scene_->setSketchColor( color_ );
        emit defineColorCurrent( color_.red(), color_.green(), color_.blue() );
    } );


    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );

    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );

    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );

    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );

    //    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );

    connect( ac_use_last_trajectory, &QAction::toggled, [=]()
    { emit useLastTrajectory(); } );


    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ ); ac_resize_boundary->setChecked( false ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_ ){ emit addTrajectory( curve_ ); }  );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_ ) { emit sendSketchOfSelection( curve_ ); } );

    connect( scene_.get(), &SketchScene::stopSketchesOfSelection, [=]() { emit stopSketchesOfSelection(); } );

    return scene_;
}


std::shared_ptr< SketchScene > SketchWindow::addCanvas( double depth_ )
{
    SketchingCanvas* canvas_ = new SketchingCanvas();
    const std::shared_ptr< SketchScene >&scene_ = canvas_->getScene();
    fixed_csections_canvas->addElement( depth_, canvas_ );
    fixed_csections_canvas->setVisible( true );


    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    {   scene_->setSketchColor( color_ );
        emit defineColorCurrent( color_.red(), color_.green(), color_.blue() ); } );



//    connect( cp_color, &ColorPicker::colorSelected, scene_.get(), &SketchScene::setSketchColor );
////    connect( ac_sketch_color, &QAction::triggered, scene_.get(), &SketchScene::setSketchColor );

    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );
    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );
    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );
//    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );


    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit addCurve( curve_, dir_, depth_ ); }  );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );


    return scene_;
}


void SketchWindow::removeCanvas( double depth_ )
{
    if( fixed_csections_canvas->findElement( depth_ ) == false ) return;
    fixed_csections_canvas->removeElement( depth_ );
}


void SketchWindow::updateColorWidget(int red_, int green_, int blue_)
{
    if( cp_color == nullptr ) return;
    cp_color->setColor( QColor( red_, green_, blue_ ) );

    if( sketchingcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
        scene_->setSketchColor( QColor( red_, green_, blue_ ) );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
       scene_->setSketchColor( QColor( red_, green_, blue_ ) );
    }
}


void SketchWindow::disableResizeVolume( bool status_ )
{
    if( ac_resize_boundary == nullptr ) return;
    ac_resize_boundary->setDisabled( status_ );
}


void SketchWindow::setModeSelecting( bool status_ )
{
    if( sketchingcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
        scene_->setSelectingStratigraphyMode( status_ );
//        scene_->setOldSelectingStratigraphyMode( status_ );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
        scene_->setSelectingStratigraphyMode( status_ );
//        scene_->setOldSelectingStratigraphyMode( status_ );
    }
}



void SketchWindow::setModeRegionSelecting( bool status_ )
{
    if( sketchingcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
        scene_->setSelectingRegionMode( status_ );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
        scene_->setSelectingRegionMode( status_ );
    }
}


SketchWindow::~SketchWindow()
{

    delete cp_color;
    cp_color = nullptr;

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


