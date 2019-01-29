#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "lateralbar.h"

#define PI 3.14159265


LateralBar::LateralBar()
{
    createInterface();
    createActions();
}


void LateralBar::createInterface()
{

    sl_vertical_exagg = new QSlider( Qt::Vertical );
    sl_vertical_exagg->setToolTip( "Vertical Exaggeration" );
    sl_vertical_exagg->setInvertedAppearance( false );
    sl_vertical_exagg->setRange( 0, 100 );
    sl_vertical_exagg->setSingleStep( 1 );

    btn_reset_exaggeration = new QPushButton( "Reset" );
    btn_reset_exaggeration->setMaximumWidth( 45 );

    sp_exagger_value = new QDoubleSpinBox();
    sp_exagger_value->setRange( 0.1, 10000. );
    sp_exagger_value->setSingleStep( 0.1 );

    QHBoxLayout* hb_exaggerattion_ = new QHBoxLayout;
    hb_exaggerattion_->addWidget( btn_reset_exaggeration );
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

    connect( sl_vertical_exagg, &QSlider::sliderMoved, this, &LateralBar::changeVerticalExaggeration );

    QObject::connect<void(QDoubleSpinBox::*)(double)>( sp_exagger_value, &QDoubleSpinBox::valueChanged,
                                                      this,  &LateralBar::changeVerticalExaggerationBySpinbox );

    connect( dl_input_dipangle , &QDial::sliderMoved, this, &LateralBar::changeDipAngle );

    connect( btn_reset_exaggeration, SIGNAL( clicked( bool ) ), this, SLOT( resetVerticalExaggeration() ) );

    connect( btn_reset_exaggeration, &QPushButton::clicked, [=](){ emit sgn_resetVerticalExaggeration(); } );

    QObject::connect<void(QPushButton::*)(bool)>( btn_show_output_dipangle, &QPushButton::toggled, [=]( bool status_ ){ emit sgn_sendDipAnglePicture( status_); } );

    QObject::connect<void(QPushButton::*)(bool)>( btn_move_output_dipangle, &QPushButton::toggled, [=]( bool status_ ){ emit sgn_setDipAnglePictureMovable( status_ ); } );
}


void LateralBar::changeVerticalExaggeration( int v_exagg_ )
{
    count++;

    double value_ = min_exagg + v_exagg_*0.01* (max_exagg - min_exagg);
    double v_exagg_db_ = static_cast< double > ( pow( 10, value_ ) );

    if( count < 2 )
        sp_exagger_value->setValue( v_exagg_db_ );
    else
        count = 0;

    emit sgn_updateVerticalExaggeration( v_exagg_db_ );

    updateDipAngle();
}


void LateralBar::changeVerticalExaggerationBySpinbox( double v_exagg_ )
{
    count++;

    double lg = log10( v_exagg_ );
    double value = 100*(lg - min_exagg)/(max_exagg - min_exagg);


    if( count < 2 )
        sl_vertical_exagg->setValue( static_cast< int >( value ) );
    else
        count = 0;

}


void LateralBar::resetVerticalExaggeration()
{
    count = 0;
    sl_vertical_exagg->setValue( 20 );
    changeVerticalExaggeration( 20 );

}


void LateralBar::changeDipAngle( double angle_ )
{
    double v_exag_ = sp_exagger_value->value();
    double param_ = v_exag_*tan( angle_*PI / 180 );
    double beta_ = atan(param_) * 180 / PI;

    QString arg_ = QString::number( angle_, 'f', 1 );
    lcd_input_dipangle ->display( QObject::tr( arg_.append("'" ).toStdString().c_str() ) );
    lb_input_dipangle->updateAngle( angle_ );

    QString arg1_ = QString::number( beta_, 'f', 1 );
    lcd_output_dipangle->display( QObject::tr( arg1_.append("'" ).toStdString().c_str() ) );
    lb_output_dipangle->updateAngle( beta_ );

    // show the picture updated
    if( btn_show_output_dipangle->isChecked() == false ) return;
    btn_show_output_dipangle->toggled( true );

}


void LateralBar::updateDipAngle()
{
    double angle_ = static_cast< double >( dl_input_dipangle->value() );
    changeDipAngle( angle_ );
}


const QPixmap* LateralBar::getDipAnglePicture() const
{
    const QPixmap* pix_ = lb_output_dipangle->pixmap();
    return pix_;
}


void LateralBar::clear()
{

    if( btn_show_output_dipangle!= nullptr )
        btn_show_output_dipangle->setChecked( false );

    if( btn_move_output_dipangle != nullptr )
        btn_move_output_dipangle->setChecked( false );

    if( dl_input_dipangle != nullptr )
        dl_input_dipangle->setValue( 0 );

    changeDipAngle( 0 );
    resetVerticalExaggeration();
}


LateralBar::~LateralBar()
{
    if( sl_vertical_exagg != nullptr )
        delete sl_vertical_exagg;
    sl_vertical_exagg = nullptr;

    if( btn_reset_exaggeration != nullptr )
        delete btn_reset_exaggeration;
    btn_reset_exaggeration = nullptr;

    if( sp_exagger_value != nullptr )
        delete sp_exagger_value;
    sp_exagger_value = nullptr;

    if( dl_input_dipangle != nullptr )
        delete dl_input_dipangle;
    dl_input_dipangle = nullptr;

    if( lb_input_dipangle != nullptr )
        delete lb_input_dipangle;
    lb_input_dipangle = nullptr;

    if( lcd_input_dipangle != nullptr )
        delete lcd_input_dipangle;
    lcd_input_dipangle = nullptr;

    if( lb_output_dipangle != nullptr )
        delete lb_output_dipangle;
    lb_output_dipangle = nullptr;

    if( lcd_output_dipangle != nullptr )
        delete lcd_output_dipangle;
    lcd_output_dipangle = nullptr;

    if( btn_show_output_dipangle != nullptr )
        delete btn_show_output_dipangle;
    btn_show_output_dipangle = nullptr;

    if( btn_move_output_dipangle != nullptr )
        delete btn_move_output_dipangle;
    btn_move_output_dipangle = nullptr;


    min_exagg = -1;
    max_exagg = 4;

    count = 0;
}
