#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "lateralbar.h"

LateralBar::LateralBar()
{
    createInterface();
}

void LateralBar::createInterface()
{

    sl_vertical_exagg = new RealFeaturedSlider( Qt::Vertical );
    sl_vertical_exagg->setToolTip( "Vertical Exaggeration" );
    sl_vertical_exagg->setInvertedAppearance( false );
    sl_vertical_exagg->setRange( 0, 100 );
    sl_vertical_exagg->setSingleStep( 1 );

    btn_reset_exaggeration = new QPushButton( "Reset" );
    btn_reset_exaggeration->setMaximumWidth( 45 );

    lb_exagger_value = new QLabel( "Value: " );
    sp_exagger_value = new QDoubleSpinBox();
    sp_exagger_value->setRange( 0.1, 10000. );
    sp_exagger_value->setSingleStep( 0.1 );

    QHBoxLayout* hb_exaggerattion_ = new QHBoxLayout;
    hb_exaggerattion_->addWidget( btn_reset_exaggeration );
    hb_exaggerattion_->addWidget( lb_exagger_value );
    hb_exaggerattion_->addWidget( sp_exagger_value );


    QVBoxLayout* vb_layout_exag_ = new QVBoxLayout;
    vb_layout_exag_->addWidget( sl_vertical_exagg );
    vb_layout_exag_->addLayout( hb_exaggerattion_ );


    QGroupBox* gb_exagger_ = new QGroupBox( "Vert. Exaggeration: " );
    gb_exagger_->setLayout( vb_layout_exag_ );


    dl_input_dipangle = new QDial();
    dl_input_dipangle->setToolTip( "Input Dip Angle" );
    dl_input_dipangle->setMaximumSize( 70, 70 );
    dl_input_dipangle->setInvertedAppearance( true );
    dl_input_dipangle->setNotchesVisible( true );
    dl_input_dipangle->setRange( 0, 90 );
    dl_input_dipangle->setSingleStep( 10 );

    btn_show_output_dipangle = new QPushButton( "Show" );
    btn_show_output_dipangle->setToolTip( "Show Dip Angle inside Canvas" );
    btn_show_output_dipangle->setCheckable( true );

    btn_move_output_dipangle = new QPushButton( "Move" );
    btn_move_output_dipangle->setToolTip( "Move Dip Angle Picture" );
    btn_move_output_dipangle->setCheckable( true );


    QVBoxLayout* vb_input_angle = new QVBoxLayout();
    vb_input_angle->addWidget( dl_input_dipangle );
    vb_input_angle->addWidget( btn_show_output_dipangle );
    vb_input_angle->addWidget( btn_move_output_dipangle );


    lb_input_dipangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_input_dipangle->setToolTip( "Input Dip Angle" );

    lcd_input_dipangle = new QLCDNumber();
    lcd_input_dipangle->setDecMode();
    lcd_input_dipangle->setFrameShape( QFrame::NoFrame );
    lcd_input_dipangle->setSegmentStyle(QLCDNumber::Flat);

    QVBoxLayout* vb_input_angle_ = new QVBoxLayout;
    vb_input_angle_->addWidget( lb_input_dipangle );
    vb_input_angle_->addWidget( lcd_input_dipangle );


    lb_output_dipangle = new AnglePicture( QSize( 70, 70 ), 0 );
    lb_output_dipangle->setToolTip( "Output Dip Angle" );

    lcd_output_dipangle = new QLCDNumber();
    lcd_output_dipangle->setDecMode();
    lcd_output_dipangle->setFrameShape( QFrame::NoFrame );
    lcd_output_dipangle->setSegmentStyle( QLCDNumber::Flat );


    QVBoxLayout* vb_output_angle_ = new QVBoxLayout;
    vb_output_angle_->addWidget( lb_output_dipangle );
    vb_output_angle_->addWidget( lcd_output_dipangle );


    QHBoxLayout* vb_angles = new QHBoxLayout();
    vb_angles->addLayout( vb_input_angle );
    vb_angles->addSpacing( 10 );
    vb_angles->addLayout( vb_input_angle_ );
    vb_angles->addSpacing( 10 );
    vb_angles->addLayout( vb_output_angle_ );


    QGroupBox* gb_dip_angle_ = new QGroupBox( "Dip Angle: " );
    gb_dip_angle_->setLayout( vb_angles );

    QVBoxLayout* hb_lateral_bar = new QVBoxLayout();
    hb_lateral_bar->addWidget( gb_exagger_ );
    hb_lateral_bar->addWidget( gb_dip_angle_ );


    setLayout( hb_lateral_bar );

}

void LateralBar::createActions()
{
//    connect( btn_reset_exaggeration, SIGNAL( clicked( bool ) ), this, SLOT( resetVerticalExaggeration() ) );

//    connect( sl_vertical_exagg_, &QSlider::sliderMoved, this, &SketchWindow::usingVerticalExaggeration );

//    QObject::connect<void(QDoubleSpinBox::*)(double)>(sp_exagger_value, &QDoubleSpinBox::valueChanged,
//                                                      this,  &SketchWindow::usingVerticalExaggerationSpinBox);

//    connect( dl_input_dipangle , &QDial::sliderMoved, this, &SketchWindow::setDipAngle );

//    connect( btn_show_output_dipangle, SIGNAL( toggled( bool ) ), this, SLOT( showDipAngle( bool ) ) );

//    connect( btn_move_oangle, SIGNAL( toggled( bool ) ), this, SLOT( setDipAnglePictureMovable( bool ) ) );
}
