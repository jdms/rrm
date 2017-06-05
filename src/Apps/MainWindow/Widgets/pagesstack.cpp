#include "pagesstack.h"

PagesStack::PagesStack()
{
    createVolumePropertiesPage();
    createVolumeActions();
}


void PagesStack::createVolumePropertiesPage()
{
    QWidget* wd_volume_page = new QWidget( this );
    wd_volume_resize.setupUi( wd_volume_page );
    addWidget( wd_volume_page );
}


void PagesStack::createVolumeActions()
{
    connect( wd_volume_resize.hs_width_volume, &QSlider::sliderMoved, [=]( int value ){
                                                         emit widthVolumeChanged( value ); } );

    connect( wd_volume_resize.hs_height_volume, &QSlider::sliderMoved, [=]( int value ){
                                                         emit heightVolumeChanged( value ); } );

    connect( wd_volume_resize.hs_depth_volume, &QSlider::sliderMoved, [=]( int value ){
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


void PagesStack::createRulesHistoryPage()
{
//    addWidget( gl_volume_resizing );
}
