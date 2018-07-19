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
    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    return scene_;
}


std::shared_ptr< SketchScene > SketchWindow::createTopViewCanvas()
{

    topviewcanvas = new SketchingCanvas();
    const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
    setCentralWidget( topviewcanvas );


    connect( ac_sketch_color, &QAction::triggered, scene_.get(), &SketchScene::setSketchColor );
    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );
    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );
    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );
//    connect( ac_select_wells, &QAction::triggered, scene_.get(), &SketchScene::setSelectingWellsMode );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_ ){ emit addTrajectory( curve_ ); }  );
    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

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


