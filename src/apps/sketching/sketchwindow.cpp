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

#include <cmath>

#include <QToolBar>
#include <QFileDialog>
#include <QGroupBox>
#include <QPushButton>

#include "sketchwindow.h"
#define PI 3.14159265

SketchWindow::SketchWindow( QWidget* parent ): QMainWindow( parent )
{

    createInterface();
}


void SketchWindow::createToolBar()
{
    cp_color = new ColorPicker( this );
    cp_color->setToolTip( "Sketch Color" );


    ac_cancel_sketch = new QAction( "Cancel", this );
    ac_cancel_sketch->setToolTip( "Cancel Sketch" );

    ac_submit_sketch = new QAction( "Submit", this );
    ac_submit_sketch->setToolTip( "Submit Sketch" );

    ac_end_object = new QAction( "End", this );
    ac_end_object->setToolTip( "Create Surface" );

    tb_sketch = addToolBar( "Edit Sketch" );
    tb_sketch->addWidget( cp_color );
    tb_sketch->addAction( ac_cancel_sketch );
    tb_sketch->addAction( ac_submit_sketch );
    tb_sketch->addAction( ac_end_object );

    ac_resize_boundary = new QAction( "Resize boundary", this );
    ac_resize_boundary->setToolTip( "Resize Boundary" );
    ac_resize_boundary->setCheckable( true );
    ac_resize_boundary->setChecked( RESIZE_BOUNDARY_DEFAULT_STATUS );
    tb_boundary = addToolBar( "Boundary" );
    tb_boundary->addAction( ac_resize_boundary );

    ac_resize_image = new QAction( "Resize image", this );
    ac_resize_image->setToolTip( "Resize Image" );
    ac_resize_image->setCheckable( true );

    ac_remove_image = new QAction( "Remove image", this );
    ac_remove_image->setToolTip( "Remove Image" );

    tb_image = addToolBar( "Image" );
    tb_image->addAction( ac_resize_image );
    tb_image->addAction( ac_remove_image );

    ac_fixed_csections = new QAction( "Fixed Cross-Sections", this );
    ac_fixed_csections->setIcon(QIcon(":/images/icons/fixedcsections.png"));
    ac_fixed_csections->setCheckable( true );

    ac_select_regions = new QAction( "Select Regions", this );
    ac_select_regions->setToolTip( "Select Regions" );
    ac_select_regions->setCheckable( true );
    ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );

    tb_region = addToolBar( "Region" );
    tb_region->addAction( ac_select_regions );
    tb_region->setVisible( false );

    tb_lateral_bar = addToolBar( "Lateral Bar" );
    ac_show_bar = new QAction( "Vertical Exaggeration", this );
    ac_show_bar->setToolTip( "Show Vertical Exaggeration and Dip Angle Bar" );
    ac_show_bar->setCheckable( true);
    ac_show_bar->setChecked( SHOW_VERTICAL_EXAGGERATION );
    tb_lateral_bar->addAction( ac_show_bar );


    tb_trajectory = addToolBar( "Trajectory" );
    ac_use_last_trajectory = new QAction( "Last trajectory" );
    ac_use_last_trajectory->setToolTip( "Reuse Last Trajectory" );

    tb_trajectory->addAction( ac_use_last_trajectory );

    ac_axes = new QAction( "Axes", this );
    ac_axes->setToolTip( "Show axes" );
    ac_axes->setIcon(QIcon(":/images/icons/axes.png"));
    ac_axes->setCheckable( true );
    ac_axes->setChecked( true );

    ac_screenshot = new QAction( "Screenshot", this );
    ac_screenshot->setToolTip( "Screenshot" );
    ac_screenshot->setIcon(QIcon(":/images/icons/Camera.png"));
    tb_misc = addToolBar( "Misc" );
    tb_misc->addAction( ac_fixed_csections );
    tb_misc->addAction( ac_screenshot );
    tb_misc->addAction( ac_axes );



}


void SketchWindow::createInterface()
{
    createToolBar();

    fixed_csections_canvas = new CanvasStack();
    fixed_csections_canvas->setWindowTitle( "Fixed Cross-Sections" );
    fixed_csections_canvas->setVisible( false );
}

std::shared_ptr< SketchScene > SketchWindow::createMainCanvas()
{

    // hidding actions not associated to the main cross-sections
    tb_trajectory->setVisible( false );

    sketchingcanvas = new SketchingCanvas();
    //TODO: create a enum to identify the type of the cross-section, so that we can move the scale to inside the SketchingCanvas class
    sketchingcanvas->scale( 1, -1 );
    const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();

    createLateralBar();

    QHBoxLayout* hb_central = new QHBoxLayout( this );
    hb_central->addWidget( sketchingcanvas );
    hb_central->addWidget( bar_ );


    QWidget* central_ = new QWidget();
    central_->setLayout( hb_central );
    setCentralWidget( central_ );

    //TODO: create method to encapsulated these actions

    // connects related to the fixed cross-sections widget

    connect( fixed_csections_canvas, &CanvasStack::closeSubWindow, [=]( double id_ )
    {
        emit removeMarkerFromSlider( id_ );
    } );

    connect( ac_fixed_csections, &QAction::toggled, fixed_csections_canvas, &CanvasStack::setVisible );

    connect( fixed_csections_canvas, &CanvasStack::canvasClosed, [=](){ ac_fixed_csections->setChecked( false ); } );



    // connects related to scene: from actions

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

    connect( ac_axes, &QAction::triggered, scene_.get(), &SketchScene::setAxesVisible );


    // connects related to scene: from scene

    connect( scene_.get(), &SketchScene::removeLastCurve, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit removeLastCurve( dir_, depth_ );  } );

    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ ); applyVerticalExaggeration();
        ac_resize_boundary->setChecked( false ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    { emit addCurve( curve_, dir_, depth_ ); }  );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::removeImageFromCrossSection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    {
        emit removeImageFromCrossSection( dir_, depth_ );
    } );


    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::objectSelected, [=]( const std::size_t& id_ ) { emit objectSelected( id_ ); } );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ) { emit sendSketchOfSelection( curve_, dir_, depth_ ); } );

    connect( scene_.get(), &SketchScene::regionSelected, [=]( const std::size_t& id_, bool status_ ) { emit regionSelected( id_, status_ ); } );

    connect( scene_.get(), &SketchScene::sendPointGuidedExtrusion, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_  ) { emit sendPointGuidedExtrusion( px_, py_, depth_, dir_ ); } );

    connect( scene_.get(), &SketchScene::stopSketchesOfSelection, [=]() { emit stopSketchesOfSelection(); } );



    // connects related to lateral bar

    connect( ac_show_bar, &QAction::toggled, bar_, &QWidget::setVisible );


    // connects related to the own window

    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );



    return scene_;
}


void SketchWindow::createLateralBar()
{

    latBar = new LateralBar();

    QVBoxLayout* hb_lateral_bar = new QVBoxLayout();
    hb_lateral_bar->addWidget( latBar );

    connect( latBar, &LateralBar::sgn_updateVerticalExaggeration, [=]( double v_ )
    {
        std::cout << "exag: " << v_ << std::endl << std::flush;
        const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
        Settings::CrossSection::CrossSectionDirections dir_;
        double depth_;
        scene_->getCrossSectionInformation( dir_, depth_ );
        if( dir_ == Settings::CrossSection::CrossSectionDirections::Y ) return;
        sketchingcanvas->setVerticalExaggeration( v_ );
        emit setVerticalExaggeration( v_ );
    } );


    connect( latBar, &LateralBar::sgn_resetVerticalExaggeration, [=]()
    {
        sketchingcanvas->stopVerticalExaggeration();
    } );

    connect( latBar, &LateralBar::sgn_sendDipAnglePicture, this, &SketchWindow::showDipAngle );

    connect( latBar, &LateralBar::sgn_setDipAnglePictureMovable, this, &SketchWindow::setDipAnglePictureMovable );


    bar_ = new QWidget();
    bar_->setMinimumWidth( 170 );
    bar_->setLayout( hb_lateral_bar );
    bar_->setVisible( SHOW_VERTICAL_EXAGGERATION );

}


std::shared_ptr< SketchScene > SketchWindow::createTopViewCanvas()
{

    // hidding actions not associated to the top view cross-sections
    tb_trajectory->setVisible( true );
    tb_lateral_bar->setVisible( false );

    topviewcanvas = new SketchingCanvas();
    const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
    setCentralWidget( topviewcanvas );


    //TODO: create method to encapsulated these actions

    // connects related to scene: from actions

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    {   scene_->setSketchColor( color_ );
        emit defineColorCurrent( color_.red(), color_.green(), color_.blue() );
    } );


    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );

    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );

    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );

    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );


    connect( ac_axes, &QAction::triggered, scene_.get(), &SketchScene::setAxesVisible );

    connect( ac_remove_image, &QAction::triggered, scene_.get(), &SketchScene::removeImageInCrossSectionAndUpdate );

    connect( ac_resize_image, &QAction::triggered, scene_.get(), &SketchScene::setResizingImageMode );


    // connects related to scene: from scene

    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    {
        emit updateVolumeDimensions( dir_, width_, height_ );
        applyVerticalExaggeration();
        ac_resize_boundary->setChecked( false );
    } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_ ){ emit addTrajectory( curve_ ); }  );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_ , const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ) { emit sendSketchOfSelection( curve_, dir_, depth_ ); } );

    connect( scene_.get(), &SketchScene::stopSketchesOfSelection, [=]() { emit stopSketchesOfSelection(); } );

    connect( scene_.get(), &SketchScene::sketchDoneGuidedExtrusion, [=]( const PolyCurve& curve_ ) { emit sketchDoneGuidedExtrusion( curve_ ); } );


    // connects related to the own window

    connect( ac_use_last_trajectory, &QAction::triggered, [=](){ emit useLastTrajectory(); } );


    return scene_;
}


std::shared_ptr< SketchScene > SketchWindow::addCanvas( double depth_, const Settings::CrossSection::CrossSectionDirections& dir_, QColor color_ )
{
    SketchingCanvas* canvas_ = new SketchingCanvas();
    const std::shared_ptr< SketchScene >&scene_ = canvas_->getScene();
    fixed_csections_canvas->addElement( depth_, canvas_ );
    fixed_csections_canvas->setVisible( true );


    //TODO: create a enum to identify the type of the cross-section, so that we can move the scale to inside the SketchingCanvas class
    if( dir_ != Settings::CrossSection::CrossSectionDirections::Y )
        canvas_->scale( 1, -1 );


    //TODO: create method to encapsulated these actions

    // connects related to scene: from actions

    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color1_ )
    {
        scene_->setSketchColor( color1_ );
        emit defineColorCurrent( color1_.red(), color1_.green(), color1_.blue() );
    } );


    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );

    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );

    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );

    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );

    connect( ac_remove_image, &QAction::triggered, scene_.get(), &SketchScene::removeImageInCrossSectionAndUpdate );

    connect( ac_resize_image, &QAction::triggered, scene_.get(), &SketchScene::setResizingImageMode );

    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );

    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );

    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );

    connect( ac_axes, &QAction::triggered, scene_.get(), &SketchScene::setAxesVisible );


    // connects related to scene: from scene


    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ ); applyVerticalExaggeration();
        ac_resize_boundary->setChecked( false ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit addCurve( curve_, dir_, depth_ ); }  );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::removeLastCurve, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit removeLastCurve( dir_, depth_ );  } );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::removeImageFromCrossSection, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
    {
        emit removeImageFromCrossSection( dir_, depth_ );
    } );


    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::objectSelected, [=]( const std::size_t& id_ ) { emit objectSelected( id_ ); } );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ) { emit sendSketchOfSelection( curve_, dir_, depth_ ); } );

    connect( scene_.get(), &SketchScene::regionSelected, [=]( const std::size_t& id_, bool status_ ) { emit regionSelected( id_, status_ ); } );

    connect( scene_.get(), &SketchScene::sendPointGuidedExtrusion, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_  ) { emit sendPointGuidedExtrusion( px_, py_, depth_, dir_ ); } );

    connect( scene_.get(), &SketchScene::stopSketchesOfSelection, [=]() { emit stopSketchesOfSelection(); } );


    return scene_;
}


void SketchWindow::removeCanvas( double depth_ )
{
    if( fixed_csections_canvas->findElement( depth_ ) == false ) return;
    fixed_csections_canvas->removeElement( depth_ );
}


void SketchWindow::removeAllCanvas()
{

    if( fixed_csections_canvas == nullptr ) return;

    CanvasContainer::Iterator it =  fixed_csections_canvas->begin();
    while( it != fixed_csections_canvas->end() )
    {
        fixed_csections_canvas->removeElement( it->first );
        it =  fixed_csections_canvas->begin();
    }
    fixed_csections_canvas->clear();
    fixed_csections_canvas->close();

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

    if( fixed_csections_canvas == nullptr ) return;
    CanvasContainer::Iterator it =  fixed_csections_canvas->begin();
    while( it != fixed_csections_canvas->end() )
    {
        SketchingCanvas* canvas_ = static_cast< SketchingCanvas* >( fixed_csections_canvas->getElement( it->first ) );
        const std::shared_ptr< SketchScene >&scene_ = canvas_->getScene();
        scene_->setSketchColor( QColor( red_, green_, blue_ ) );
        ++it;
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
        scene_->setSelectingStratigraphyMode( false );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
        scene_->setSelectingStratigraphyMode( false );
    }
}


void SketchWindow::setModeSelectingStratigraphies( bool status_ )
{
    if( sketchingcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();
        scene_->setSelectingStratigraphyMode( status_ );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
        scene_->setSelectingStratigraphyMode( status_ );
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


void SketchWindow::resetVerticalExaggeration()
{


    if( sketchingcanvas == nullptr ) return;
//    sl_vertical_exagg_->setValue( 20 );

    if( latBar == nullptr ) return;
    latBar->resetVerticalExaggeration();
    sketchingcanvas->stopVerticalExaggeration();
}


void SketchWindow::applyVerticalExaggeration()
{
    if( sketchingcanvas == nullptr ) return;
    std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();

    double v_exag_ = sketchingcanvas->getVerticalExaggeration();
    if( v_exag_ == 1 ) return;

    sketchingcanvas->stopVerticalExaggeration();
    sketchingcanvas->setVerticalExaggeration( v_exag_ );


}


void SketchWindow::showDipAngle( bool status_ )
{
    if( ( sketchingcanvas == nullptr ) || (latBar == nullptr) ) return;

    const QPixmap* pix_ =  latBar->getDipAnglePicture(); //*/lb_output_dpangle->pixmap();
    if( pix_ == nullptr ) return;

    std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();
    scene_->showDipAnglePicture( status_, *pix_ );

}


void SketchWindow::setDipAnglePictureMovable( bool status_ )
{
    if( sketchingcanvas == nullptr ) return;
    std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();
    scene_->setDipAnglePictureMovable( status_ );
}


void SketchWindow::screenshot()
{
    QString selectedFilter;
    QString name_of_file_ = QFileDialog::getSaveFileName( nullptr, tr( "Save Image" ), "./screenshots/",
                                                          tr( "PNG (*.png);;SVG (*.svg)" ),
                                                          &selectedFilter );
    if( sketchingcanvas != nullptr )
    {
        std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();
        if( selectedFilter == "PNG (*.png)" )
        {
            scene_->savetoRasterImage( name_of_file_ );
        }
        else if ( selectedFilter == "SVG (*.svg)" )
        {
            scene_->savetoVectorImage( name_of_file_ );
        }
    }

    if( topviewcanvas != nullptr )
    {
        std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();
        if( selectedFilter == "PNG (*.png)" )
        {
            scene_->savetoRasterImage( name_of_file_ );
        }
        else if ( selectedFilter == "SVG (*.svg)" )
        {
            scene_->savetoVectorImage( name_of_file_ );
        }
    }

}


void SketchWindow::reset()
{


    resetVerticalExaggeration();
    removeAllCanvas();
    disableResizeVolume( false );

    if( latBar != nullptr )
        latBar->clear();

    if( ac_resize_image != nullptr )
        ac_resize_image->setChecked( false );

    if( ac_select_regions != nullptr )
        ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );

    cp_color->setColor( QColor( 255, 0, 0 ) );


    if( ac_show_bar != nullptr )
        ac_show_bar->setChecked( SHOW_VERTICAL_EXAGGERATION );

    if( ac_axes != nullptr )
        ac_axes->setChecked( true );


}


void SketchWindow::keyPressEvent( QKeyEvent *event )
{
    switch( event->key() )
    {
    case Qt::Key_G:
    {
        if( sketchingcanvas == nullptr ) return;
        const std::shared_ptr< SketchScene >&scene_ = sketchingcanvas->getScene();
        scene_->setGuidedExtrusionMode( true );

    }
        break;

    case Qt::Key_D:
    {
        if( sketchingcanvas == nullptr ) return;
        const std::shared_ptr< SketchScene >&scene_ = sketchingcanvas->getScene();
        scene_->setGuidedExtrusionMode( false );

    }
        break;

    default:
        break;
    };
}


SketchWindow::~SketchWindow()
{

    if( cp_color!= nullptr )
        delete cp_color;
    cp_color = nullptr;

    if( fixed_csections_canvas!= nullptr )
        delete fixed_csections_canvas;
    fixed_csections_canvas = nullptr;

    if( sketchingcanvas!= nullptr )
        delete sketchingcanvas;
    sketchingcanvas = nullptr;

    if( ac_cancel_sketch!= nullptr )
        delete ac_cancel_sketch;
    ac_cancel_sketch = nullptr;

    if( ac_submit_sketch!= nullptr )
        delete ac_submit_sketch;
    ac_submit_sketch = nullptr;

    if( ac_end_object!= nullptr )
        delete ac_end_object;
    ac_end_object = nullptr;

    if( tb_boundary!= nullptr )
        delete tb_boundary;
    tb_boundary = nullptr;

    if( ac_resize_boundary!= nullptr )
        delete ac_resize_boundary;
    ac_resize_boundary = nullptr;

    if( tb_region!= nullptr )
        delete tb_region;
    tb_region = nullptr;

    if( ac_select_regions!= nullptr )
        delete ac_select_regions;
    ac_select_regions = nullptr;

    if( bar_!= nullptr )
        delete bar_;
    bar_ = nullptr;

}


