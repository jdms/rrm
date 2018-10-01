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
    sl_vertical_exagg_->setRange( 10, 100 );
    sl_vertical_exagg_->setSingleStep( 1 );
    sl_vertical_exagg_->setValue( 10 );

    lb_exagger_value_ = new QLabel( "Value: " );
    QVBoxLayout* vb_exagg_ = new QVBoxLayout;
    vb_exagg_->addStretch();
    vb_exagg_->addWidget( lb_exagger_value_ );

    QHBoxLayout* hb_exaggerattion_ = new QHBoxLayout;
    hb_exaggerattion_->addWidget( sl_vertical_exagg_ );
    hb_exaggerattion_->addSpacing( 30 );
    hb_exaggerattion_->addLayout( vb_exagg_ );

    QGroupBox* gb_exagger_ = new QGroupBox( "Vert. Exaggeration: " );
    gb_exagger_->setLayout( hb_exaggerattion_ );


    dl_input_angle_ = new QDial();
    dl_input_angle_->setMaximumSize( 40, 40 );
    dl_input_angle_->setRange( 0, 180 );
    dl_input_angle_->setSingleStep( 15 );

    dl_output_angle_ = new QDial();
    dl_output_angle_->setMaximumSize( 40, 40 );
    dl_output_angle_->setRange( 0, 180 );
    dl_output_angle_->setSingleStep( 15 );


    lb_input_angle_ = new QLabel( "Input: " );
    lb_output_angle_ = new QLabel( "Output: " );

    QVBoxLayout* vb_input_angle_ = new QVBoxLayout;
    vb_input_angle_->addWidget( dl_input_angle_ );
    vb_input_angle_->addWidget( lb_input_angle_ );

    QVBoxLayout* vb_output_angle_ = new QVBoxLayout;
    vb_output_angle_->addWidget( dl_output_angle_ );
    vb_output_angle_->addWidget( lb_output_angle_ );


    lb_input_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_output_dpangle = new AnglePicture( QSize( 70, 70 ), 0 );


    vb_angles = new QHBoxLayout();
    vb_angles->addLayout( vb_input_angle_ );
//    vb_angles->addSpacing( 10 );
    vb_angles->addWidget( lb_input_dpangle );
    vb_angles->addSpacing( 10 );
    vb_angles->addWidget( lb_output_dpangle );
//    vb_angles->addLayout( vb_output_angle_ );


    QGroupBox* gb_dip_angle_ = new QGroupBox( "Dip Angle: " );
    gb_dip_angle_->setLayout( vb_angles );

    hb_central = new QVBoxLayout();
    hb_central->addWidget( gb_exagger_ );
    hb_central->addWidget( gb_dip_angle_ );


    bar_ = new QWidget();
    bar_->setMinimumWidth( 170 );
    bar_->setLayout( hb_central );

    connect( sl_vertical_exagg_, &QSlider::sliderMoved, this, &SketchWindow::usingVerticalExaggeration );

    connect( dl_input_angle_ , &QDial::sliderMoved, this, &SketchWindow::setDipAngle );

}



//QPixmap SketchWindow::drawDipAngles( double angle_ )
//{
//    int value = 80;
//    QPixmap pix( value, value );
//    QPainter painter(&pix);
//    QLinearGradient gradient(0,0, 0, value);
//    gradient.setColorAt(0.0, QColor(250, 250, 250));
//    gradient.setColorAt(1.0, QColor(234, 234, 234));
//    QBrush brush(gradient);

//    painter.setPen(QColor(0, 0, 255, 64));
//    qreal xAxis = value/2;
//    qreal yAxis = value/2;
//    painter.drawLine(0, xAxis, value,  xAxis);
//    painter.drawLine(yAxis, 0, yAxis, value);

//    QPointF center = QPointF(yAxis, xAxis);

//    QLineF angleline;
//    angleline.setP1( center);
//    angleline.setAngle( angle_ );
//    angleline.setLength(value);
//    painter.drawLine(angleline);

//    QPainterPath myPath;
//    myPath.moveTo(center);
//    myPath.arcTo( pix.rect() , 0, angle_ );
//    painter.setBrush( QBrush( Qt::yellow ) );
//    painter.drawPath(myPath);

//    painter.setRenderHint(QPainter::Antialiasing, true );
//    return pix;

////    QLabel* teste = new QLabel();
////    teste->setPixmap( pix );

//}

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
//        scene_->setSelectingStratigraphyMode( status_ );
        scene_->setSelectingStratigraphyMode( false );
    }
    if( topviewcanvas != nullptr )
    {
        const std::shared_ptr< SketchScene >& scene_ = topviewcanvas->getScene();
//        scene_->setSelectingStratigraphyMode( status_ );
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
    if( sketchingcanvas == nullptr ) return;

    double v_exagg_db_ = static_cast< double > ( v_exagg_*0.05 );
    sketchingcanvas->setVerticalExaggeration( v_exagg_db_ );

    lb_exagger_value_->setText( QString( "Value: %1" ).arg( v_exagg_db_ ) );


    emit setVerticalExaggeration( v_exagg_db_ );

}


void SketchWindow::setDipAngle( double angle_ )
{
    double v_exag_ = sketchingcanvas->getVerticalExaggeration();
    double param_ = v_exag_*tan( angle_*PI/180 );
    double beta_ = atan(param_) * 180 / PI;

    lb_input_angle_ ->setText( QString( "Input: %1" ).arg( angle_ ) );
    lb_output_angle_->setText( QString( "Output: %2" ).arg( beta_ ) );

    lb_input_dpangle->updateAngle( angle_ );

    dl_output_angle_->setValue( static_cast< int >( beta_ ) );
    lb_output_dpangle->updateAngle( beta_ );

    std::cout << "Beta value: " << beta_ << std::endl << std::flush;
}


void SketchWindow::reset()
{
    ac_select_regions->setChecked( SELECT_REGION_DEFAULT_STATUS );
    ac_select_wells->setChecked( SELECT_WELLS_DEFAULT_STATUS );
    cp_color->setColor( QColor( 255, 0, 0 ) );

    if( sketchingcanvas != nullptr )
        sketchingcanvas->setVerticalExaggeration( 1 );
    emit setVerticalExaggeration( 1.0 );
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
}


