#include "pages_stack.h"

PagesStack::PagesStack()
{
    createVolumePropertiesPage();
    createVolumeActions();
    changeRangeSize( MAX_WIDTH, MAX_HEIGHT, MAX_DEPTH );
}


void PagesStack::createVolumePropertiesPage()
{
    QWidget* wd_volume_page = new QWidget( this );
    wd_volume_resize.setupUi( wd_volume_page );
    addWidget( wd_volume_page );
}


void PagesStack::createVolumeActions()
{


    connect( wd_volume_resize.hs_width_volume, SIGNAL( sliderMoved(int) ),
             wd_volume_resize.sb_width_volume, SLOT( setValue(int) ) );

    connect( wd_volume_resize.sb_width_volume, SIGNAL( valueChanged(int) ),
             wd_volume_resize.hs_width_volume, SLOT( setValue(int) ) );

    connect( wd_volume_resize.hs_height_volume, SIGNAL( sliderMoved(int) ),
             wd_volume_resize.sb_height_volume, SLOT( setValue(int) ) );

    connect( wd_volume_resize.sb_height_volume, SIGNAL( valueChanged(int) ),
             wd_volume_resize.hs_height_volume, SLOT( setValue(int) ) );


    connect( wd_volume_resize.hs_depth_volume, SIGNAL( sliderMoved(int) ),
             wd_volume_resize.sb_depth_volume, SLOT( setValue(int) ) );

    connect( wd_volume_resize.sb_depth_volume, SIGNAL( valueChanged(int) ),
             wd_volume_resize.hs_depth_volume, SLOT( setValue(int) ) );


    connect( wd_volume_resize.hs_width_volume, &QSlider::valueChanged, [=]( int value ){
                                                        emit widthVolumeChanged( value ); } );

    connect( wd_volume_resize.hs_height_volume, &QSlider::valueChanged, [=]( int value ){
                                                         emit heightVolumeChanged( value ); } );

    connect( wd_volume_resize.hs_depth_volume, &QSlider::valueChanged, [=]( int value ){
                                                         emit depthVolumeChanged( value ); } );




}



void PagesStack::changeRangeSize( double width, double height, double depth )
{

    int w = static_cast< int > (width);
    int h = static_cast< int > (height);
    int d = static_cast< int > (depth);

    wd_volume_resize.hs_width_volume->setRange( 0, w );
    wd_volume_resize.hs_height_volume->setRange( 0, h );
    wd_volume_resize.hs_depth_volume->setRange( 0, d );

    wd_volume_resize.sb_width_volume->setRange( 0, w );
    wd_volume_resize.sb_height_volume->setRange( 0, h );
    wd_volume_resize.sb_depth_volume->setRange( 0, d );


    wd_volume_resize.hs_width_volume->setSingleStep( SINGLE_STEP );
    wd_volume_resize.hs_height_volume->setSingleStep( SINGLE_STEP );
    wd_volume_resize.hs_depth_volume->setSingleStep( SINGLE_STEP );

    wd_volume_resize.sb_width_volume->setSingleStep( SINGLE_STEP );
    wd_volume_resize.sb_height_volume->setSingleStep( SINGLE_STEP );
    wd_volume_resize.sb_depth_volume->setSingleStep( SINGLE_STEP );
}



void PagesStack::changeVolumeSize( double width, double height, double depth )
{
    int w = static_cast< int > (width);
    int h = static_cast< int > (height);
    int d = static_cast< int > (depth);

    wd_volume_resize.sb_width_volume->setValue( w );
    wd_volume_resize.sb_height_volume->setValue( h );
    wd_volume_resize.sb_depth_volume->setValue( d );

}


void PagesStack::setEnabledVolumeResize( bool status )
{
    wd_volume_resize.hs_width_volume->setEnabled( status );
    wd_volume_resize.hs_height_volume->setEnabled( status );
    wd_volume_resize.hs_depth_volume->setEnabled( status );

    wd_volume_resize.sb_width_volume->setEnabled( status );
    wd_volume_resize.sb_height_volume->setEnabled( status );
    wd_volume_resize.sb_depth_volume->setEnabled( status );
}



void PagesStack::setVolumeWidth( double w )
{
    wd_volume_resize.sb_width_volume->setValue( static_cast< int > (w) );
}

void PagesStack::setVolumeHeight( double h )
{
    wd_volume_resize.sb_height_volume->setValue( static_cast< int > (h) );
}

void PagesStack::setVolumeDepth( double d )
{
    wd_volume_resize.sb_depth_volume->setValue( static_cast< int > (d) );
}



void PagesStack::createRulesHistoryPage()
{
//    addWidget( gl_volume_resizing );
}


void PagesStack::clear()
{
    changeRangeSize( MAX_WIDTH, MAX_HEIGHT, MAX_DEPTH );
    changeVolumeSize( MAX_WIDTH*0.5, MAX_HEIGHT*0.5, MAX_DEPTH*0.5 );
    setEnabledVolumeResize( VOLUME_RESIZING );
}
