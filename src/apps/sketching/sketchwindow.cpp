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
    createToolBar();
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


    ac_select_regions = new QAction( "Select Regions", this );
    ac_select_regions->setToolTip( "Select Regions" );
    ac_select_regions->setCheckable( true );
    ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );

    tb_region = addToolBar( "Region" );
    tb_region->addAction( ac_select_regions );
    tb_region->setVisible( false );

    tb_well = addToolBar( "Well" );
    ac_select_wells = new QAction( "Select Well", this );
    ac_select_wells->setToolTip( "Select Wells" );
    ac_select_wells->setCheckable( true );
    ac_select_wells->setChecked( SELECT_WELLS_DEFAULT_STATUS );
    tb_well->addAction( ac_select_wells );
    tb_well->setVisible( false );

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
    tb_misc->addAction( ac_screenshot );
    tb_misc->addAction( ac_axes );



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


    connect( ac_show_bar, &QAction::toggled, bar_, &QWidget::setVisible );

    connect( ac_screenshot, &QAction::triggered, this, &SketchWindow::screenshot );

    connect( ac_axes, &QAction::triggered, scene_.get(), &SketchScene::setAxesVisible );



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

    sl_vertical_exagg_ = new RealFeaturedSlider( Qt::Vertical );
    sl_vertical_exagg_->setToolTip( "Vertical Exaggeration" );
    sl_vertical_exagg_->setInvertedAppearance( false );
    sl_vertical_exagg_->setDiscretization( 100 );
    sl_vertical_exagg_->setRange( 0, 1 );
    resetVerticalExaggeration();

    btn_reset_exaggeration = new QPushButton( "Reset" );
    btn_reset_exaggeration->setMaximumWidth( 45 );
    connect( btn_reset_exaggeration, SIGNAL( clicked( bool ) ), this, SLOT( resetVerticalExaggeration() ) );

    lb_exagger_value_ = new QLabel( "Value: " );


    steps_exagg = (max_exagg - min_exagg);

    QHBoxLayout* hb_exaggerattion_ = new QHBoxLayout;
    hb_exaggerattion_->addWidget( btn_reset_exaggeration );
    hb_exaggerattion_->addWidget( lb_exagger_value_ );


    QVBoxLayout* vb_layout_exag_ = new QVBoxLayout;
    vb_layout_exag_->addWidget( sl_vertical_exagg_ );
    vb_layout_exag_->addLayout( hb_exaggerattion_ );


    QGroupBox* gb_exagger_ = new QGroupBox( "Vert. Exaggeration: " );
    gb_exagger_->setLayout( vb_layout_exag_ );


    dl_input_angle_ = new QDial();
    dl_input_angle_->setToolTip( "Input Dip Angle" );
    dl_input_angle_->setMaximumSize( 70, 70 );
    dl_input_angle_->setInvertedAppearance( true );
    dl_input_angle_->setNotchesVisible( true );
    dl_input_angle_->setRange( 0, 90 );
    dl_input_angle_->setSingleStep( 10 );

    btn_show_oangle = new QPushButton( "Show" );
    btn_show_oangle->setToolTip( "Show Dip Angle inside Canvas" );
    btn_show_oangle->setCheckable( true );

    btn_move_oangle = new QPushButton( "Move" );
    btn_move_oangle->setToolTip( "Move Dip Angle Picture" );
    btn_move_oangle->setCheckable( true );

    QVBoxLayout* vb_input_angle = new QVBoxLayout();
    vb_input_angle->addWidget( dl_input_angle_ );
    vb_input_angle->addWidget( btn_show_oangle );
    vb_input_angle->addWidget( btn_move_oangle );


    lb_input_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_input_dpangle->setToolTip( "Input Dip Angle" );
    lb_input_angle_ = new QLCDNumber();
    lb_input_angle_->setDecMode();
    lb_input_angle_->setFrameShape( QFrame::NoFrame );
    lb_input_angle_->setSegmentStyle(QLCDNumber::Flat);

    QVBoxLayout* vb_input_angle_ = new QVBoxLayout;
    vb_input_angle_->addWidget( lb_input_dpangle );
    vb_input_angle_->addWidget( lb_input_angle_ );


    lb_output_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_output_dpangle->setToolTip( "Output Dip Angle" );
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

    connect( sl_vertical_exagg_, &RealFeaturedSlider::sliderMoved, this, &SketchWindow::usingVerticalExaggeration );

    connect( dl_input_angle_ , &QDial::sliderMoved, this, &SketchWindow::setDipAngle );

    connect( btn_show_oangle, SIGNAL( toggled( bool ) ), this, SLOT( showDipAngle( bool ) ) );

    connect( btn_move_oangle, SIGNAL( toggled( bool ) ), this, SLOT( setDipAnglePictureMovable( bool ) ) );
}


std::shared_ptr< SketchScene > SketchWindow::createTopViewCanvas()
{

    topviewcanvas = new SketchingCanvas();
    const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
    scene_->invertImage( true );
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

    connect( ac_axes, &QAction::triggered, scene_.get(), &SketchScene::setAxesVisible );

    connect( ac_remove_image, &QAction::triggered, scene_.get(), &SketchScene::removeImageInCrossSectionAndUpdate );

    connect( ac_resize_image, &QAction::triggered, scene_.get(), &SketchScene::setResizingImageMode );


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



void SketchWindow::usingVerticalExaggeration( double v_exagg_ )
{
    std::cout << "slider value: " << v_exagg_ << std::endl << std::flush;

    if( sl_vertical_exagg_ == nullptr ) return;

    double value_ = min_exagg + v_exagg_* (max_exagg - min_exagg);
    double v_exagg_db_ = static_cast< double > ( pow( 10, value_ ) );

    lb_exagger_value_->setText( QString("Value: %1").arg( v_exagg_db_ ) );

    std::cout << "exag: " << v_exagg_db_ << std::endl << std::flush;

    if( sketchingcanvas != nullptr )
        sketchingcanvas->setVerticalExaggeration( v_exagg_db_ );

    emit setVerticalExaggeration( v_exagg_db_ );

}


void SketchWindow::resetVerticalExaggeration()
{

    sl_vertical_exagg_->setValue( 0.2 );
//    double value_ = ( 1 - min_exagg )/(max_exagg - min_exagg);
//    usingVerticalExaggeration( 0.2 );
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


    if( sketchingcanvas == nullptr ) return;
    std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();
    const QPixmap* pix_ = lb_output_dpangle->pixmap();
    if( pix_ == nullptr ) return;

    scene_->updateDipAnglePicture( *pix_ );



}


void SketchWindow::showDipAngle( bool status_ )
{
    if( sketchingcanvas == nullptr ) return;

    std::shared_ptr< SketchScene > scene_ = sketchingcanvas->getScene();

    const QPixmap* pix_ = lb_output_dpangle->pixmap();
    if( pix_ == nullptr ) return;

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
    disableResizeVolume( false );

    if( ac_resize_image != nullptr )
        ac_resize_image->setChecked( false );

    if( ac_select_regions != nullptr )
        ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );

    if( ac_select_wells != nullptr )
        ac_select_wells->setChecked( SELECT_WELLS_DEFAULT_STATUS );


    cp_color->setColor( QColor( 255, 0, 0 ) );


    if( ac_show_bar != nullptr )
        ac_show_bar->setChecked( SHOW_VERTICAL_EXAGGERATION );

    if( btn_show_oangle != nullptr )
        btn_show_oangle->setChecked( false );

    if( btn_move_oangle != nullptr )
        btn_move_oangle->setChecked( false );

    if( dl_input_angle_ != nullptr )
        dl_input_angle_->setValue( 0 );

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

    if( tb_well!= nullptr )
        delete tb_well;
    tb_well = nullptr;

    if( ac_select_wells!= nullptr )
        delete ac_select_wells;
    ac_select_wells = nullptr;


    if( sl_vertical_exagg_!= nullptr )
        delete sl_vertical_exagg_;
    sl_vertical_exagg_ = nullptr;

    if( dl_input_angle_!= nullptr )
        delete dl_input_angle_;
    dl_input_angle_ = nullptr;

    if( vb_angles!= nullptr )
        delete vb_angles;
    vb_angles = nullptr;

    if( hb_central!= nullptr )
        delete hb_central;
    hb_central = nullptr;

    if( hb_central1!= nullptr )
        delete hb_central1;
    hb_central1 = nullptr;

    if( lb_input_angle_!= nullptr )
        delete lb_input_angle_;
    lb_input_angle_  = nullptr;

    if( lb_output_angle_!= nullptr )
        delete lb_output_angle_;
    lb_output_angle_ = nullptr;

    if( lb_input_dpangle!= nullptr )
        delete lb_input_dpangle;
    lb_input_dpangle = nullptr;

    if( lb_output_dpangle!= nullptr )
        delete lb_output_dpangle;
    lb_output_dpangle = nullptr;

    if( btn_show_oangle!= nullptr )
        delete btn_show_oangle;
    btn_show_oangle  = nullptr;

    if( bar_!= nullptr )
        delete bar_;
    bar_ = nullptr;

}


