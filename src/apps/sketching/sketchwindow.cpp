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

#include "sketchwindow.h"
#define PI 3.14159265

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
    tb_well->setVisible( false );

    tb_lateral_bar = addToolBar( "Lateral Bar" );
    ac_show_bar = new QAction( "Vertical Exaggeration", this );
    ac_show_bar->setCheckable( true);
    ac_show_bar->setChecked( SHOW_VERTICAL_EXAGGERATION );
    tb_lateral_bar->addAction( ac_show_bar );


    tb_trajectory = addToolBar( "Trajectory" );
    ac_use_last_trajectory = new QAction( "Last trajectory" );

    tb_trajectory->addAction( ac_use_last_trajectory );



}


std::shared_ptr< SketchScene > SketchWindow::createMainCanvas()
{
    fixed_csections_canvas = new CanvasStack();
    fixed_csections_canvas->setWindowTitle( "Fixed Cross-Sections" );
    fixed_csections_canvas->setVisible( false );

    sketchingcanvas = new SketchingCanvas();
    sketchingcanvas->scale( 1, -1 );
    sketchingcanvas->setVerticalExaggeration( 1 );
    const std::shared_ptr< SketchScene >& scene_ = sketchingcanvas->getScene();

    createLateralBar();

    hb_central1 = new QHBoxLayout( this );
    hb_central1->addWidget( sketchingcanvas );
    hb_central1->addWidget( bar_ );


    QWidget* central_ = new QWidget();
    central_->setLayout( hb_central1 );
    setCentralWidget( central_ );

    tb_trajectory->setVisible( false );


    connect( cp_color, &ColorPicker::colorSelected, [=]( const QColor& color_ )
    {
        scene_->setSketchColor( color_ );
        emit defineColorCurrent( color_.red(), color_.green(), color_.blue() );
    } );


    connect( ac_cancel_sketch, &QAction::triggered, scene_.get(), &SketchScene::cancelSketch );

    connect( scene_.get(), &SketchScene::removeLastCurve, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){ emit removeLastCurve( dir_, depth_ );  } );

    connect( ac_submit_sketch, &QAction::triggered, scene_.get(), &SketchScene::submitSketch );
    connect( ac_end_object, &QAction::triggered, scene_.get(), &SketchScene::endObject );

    connect( ac_remove_image, &QAction::triggered, scene_.get(), &SketchScene::removeImageInCrossSectionAndUpdate );

    connect( ac_resize_image, &QAction::triggered, scene_.get(), &SketchScene::setResizingImageMode );


    connect( ac_resize_boundary, &QAction::toggled, scene_.get(), &SketchScene::setResizingBoundaryMode );

    connect( ac_select_regions, &QAction::triggered, scene_.get(), &SketchScene::setSelectingRegionsMode );


    connect( ac_show_bar, &QAction::triggered, bar_, &QWidget::setVisible );

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


    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::objectSelected, [=]( const std::size_t& id_ ) { emit objectSelected( id_ ); } );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ) { emit sendSketchOfSelection( curve_, dir_, depth_ ); } );

    connect( scene_.get(), &SketchScene::regionSelected, [=]( const std::size_t& id_, bool status_ ) { emit regionSelected( id_, status_ ); } );

    connect( scene_.get(), &SketchScene::sendPointGuidedExtrusion, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_  ) { emit sendPointGuidedExtrusion( px_, py_, depth_, dir_ ); } );

    connect( scene_.get(), &SketchScene::setAreaChoosed, [=]() { emit setAreaChoosed();  } );




    return scene_;
}


void SketchWindow::createLateralBar()
{

    sl_vertical_exagg_ = new QSlider( Qt::Vertical );
    sl_vertical_exagg_->setRange( 1, 100 );
    sl_vertical_exagg_->setSingleStep( 1 );
    sl_vertical_exagg_->setValue( 10 );

    steps_exagg = (max_exagg - min_exagg)/nsteps_exagg;

    QHBoxLayout* hb_exaggerattion_ = new QHBoxLayout;
    hb_exaggerattion_->addWidget( sl_vertical_exagg_ );

    QGroupBox* gb_exagger_ = new QGroupBox( "Vert. Exaggeration: " );
    gb_exagger_->setLayout( hb_exaggerattion_ );


    dl_input_angle_ = new QDial();
    dl_input_angle_->setMaximumSize( 70, 70 );
    dl_input_angle_->setInvertedAppearance( true );
    dl_input_angle_->setNotchesVisible( true );
    dl_input_angle_->setRange( 0, 90 );
    dl_input_angle_->setSingleStep( 10 );

    btn_show_oangle = new QPushButton( "Show" );
    QVBoxLayout* vb_input_angle = new QVBoxLayout();
    vb_input_angle->addWidget( dl_input_angle_ );
    vb_input_angle->addWidget( btn_show_oangle );


    lb_input_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_input_angle_ = new QLCDNumber();
    lb_input_angle_->setDecMode();
    lb_input_angle_->setFrameShape( QFrame::NoFrame );
    lb_input_angle_->setSegmentStyle(QLCDNumber::Flat);

    QVBoxLayout* vb_input_angle_ = new QVBoxLayout;
    vb_input_angle_->addWidget( lb_input_dpangle );
    vb_input_angle_->addWidget( lb_input_angle_ );


    lb_output_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_output_angle_ = new QLCDNumber();
    lb_output_angle_->setDecMode();
    lb_output_angle_->setFrameShape( QFrame::NoFrame );
    lb_output_angle_->setSegmentStyle(QLCDNumber::Flat);
    lb_output_angle_->setSmallDecimalPoint( true );
    lb_output_angle_->setDigitCount( 3 );


    QVBoxLayout* vb_output_angle_ = new QVBoxLayout;
    vb_output_angle_->addWidget( lb_output_dpangle );
    vb_output_angle_->addWidget( lb_output_angle_ );



    vb_angles = new QHBoxLayout();
    vb_angles->addLayout( vb_input_angle );
    vb_angles->addSpacing( 10 );

    vb_angles->addLayout( vb_input_angle_ );
    vb_angles->addSpacing( 10 );
    vb_angles->addLayout( vb_output_angle_ );


    QGroupBox* gb_dip_angle_ = new QGroupBox( "Dip Angle: " );
    gb_dip_angle_->setLayout( vb_angles );

    hb_central = new QVBoxLayout();
    hb_central->addWidget( gb_exagger_ );
    hb_central->addWidget( gb_dip_angle_ );


    bar_ = new QWidget();
    bar_->setMinimumWidth( 170 );
    bar_->setLayout( hb_central );
    bar_->setVisible( SHOW_VERTICAL_EXAGGERATION );

    connect( sl_vertical_exagg_, &QSlider::sliderMoved, this, &SketchWindow::usingVerticalExaggeration );

    connect( dl_input_angle_ , &QDial::sliderMoved, this, &SketchWindow::setDipAngle );

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

    connect( ac_use_last_trajectory, &QAction::triggered, [=]()
    { emit useLastTrajectory(); } );


    connect( scene_.get(), &SketchScene::resizeVolumeDimensions, [=]( const Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
    { emit updateVolumeDimensions( dir_, width_, height_ ); ac_resize_boundary->setChecked( false ); } );

    connect( scene_.get(), &SketchScene::sketchDone, [=]( const PolyCurve& curve_ ){ emit addTrajectory( curve_ ); }  );

    connect( scene_.get(), &SketchScene::createObject, [=]() { emit createObject(); } );

    connect( scene_.get(), &SketchScene::setImageToCrossSection, [=]( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ ){ emit setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_); }  );

    connect( scene_.get(), &SketchScene::getRegionByPoint, [=]( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ ){ emit getRegionByPoint( px_, py_, depth_, dir_ ); }  );

    connect( scene_.get(), &SketchScene::sendSketchOfSelection, [=]( const PolyCurve& curve_ , const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ) { emit sendSketchOfSelection( curve_, dir_, depth_ ); } );

    connect( scene_.get(), &SketchScene::stopSketchesOfSelection, [=]() { emit stopSketchesOfSelection(); } );

    connect( scene_.get(), &SketchScene::sketchDoneGuidedExtrusion, [=]( const PolyCurve& curve_ ) { emit sketchDoneGuidedExtrusion( curve_ ); } );

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



void SketchWindow::usingVerticalExaggeration( int v_exagg_ )
{

//    double value_ = min_exagg + v_exagg_* steps_exagg;
//    double v_exagg_db_ = static_cast< double > ( log10( value_ ) );

//    std::cout << "exag: " << v_exagg_db_ << std::endl << std::flush;

    double v_exagg_db_ = static_cast< double > ( v_exagg_*0.1 );
    if( sketchingcanvas != nullptr )
        sketchingcanvas->setVerticalExaggeration( v_exagg_db_ );

    emit setVerticalExaggeration( v_exagg_db_ );

}


void SketchWindow::setDipAngle( double angle_ )
{
    double v_exag_ = 1.0;

    if( sketchingcanvas != nullptr )
        v_exag_ = sketchingcanvas->getVerticalExaggeration();

    double param_ = v_exag_*tan( angle_*PI / 180 );
    double beta_ = atan(param_) * 180 / PI;

    lb_input_angle_ ->display( QObject::tr( "%1'" ).arg( angle_ ) );
    lb_input_dpangle->updateAngle( angle_ );

    lb_output_angle_->display( QObject::tr( "%2'" ).arg( beta_ ) );
    lb_output_dpangle->updateAngle( beta_ );

    std::cout << "Beta value: " << beta_ << std::endl << std::flush;

}


void SketchWindow::reset()
{
    ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );
    ac_select_wells->setChecked( SELECT_WELLS_DEFAULT_STATUS );
    cp_color->setColor( QColor( 255, 0, 0 ) );

    if( dl_input_angle_ != nullptr )
        dl_input_angle_->setValue( 0 );
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


//        case Qt::Key_S:
//            for( auto it: regions )
//            {
//                (it.second)->setFlag( QGraphicsItem::ItemIsSelectable, true );
//            }
//        break;


    case Qt::Key_1:
//        usingVerticalExaggeration();
//        setVerticalExaggeration( 10 );
        default:
            break;
    };
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

    delete bar_;
    bar_ = nullptr;

    delete sl_vertical_exagg_;
    sl_vertical_exagg_ = nullptr;

    delete dl_input_angle_;
    dl_input_angle_ = nullptr;

    delete vb_angles;
    vb_angles = nullptr;

    delete hb_central;
    hb_central = nullptr;

    delete hb_central1;
    hb_central1 = nullptr;

    delete lb_input_angle_;
    lb_input_angle_  = nullptr;

    delete lb_output_angle_;
    lb_output_angle_ = nullptr;

    delete lb_input_dpangle;
    lb_input_dpangle = nullptr;

    delete lb_output_dpangle;
    lb_output_dpangle = nullptr;

    delete btn_show_oangle;
    btn_show_oangle  = nullptr;
}


