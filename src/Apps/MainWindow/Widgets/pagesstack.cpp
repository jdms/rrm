#include "pagesstack.h"

PagesStack::PagesStack()
{
    createVolumePropertiesPage();
}


void PagesStack::createVolumePropertiesPage()
{
    QWidget* wd_volume_page = new QWidget( this );
    wd_volume_resize.setupUi( wd_volume_page );
    addWidget( wd_volume_page );
}


void PagesStack::changeVolumeSize( int width, int height, int depth )
{
    wd_volume_resize.hs_width_volume->setRange( 0, width );
    wd_volume_resize.hs_height_volume->setRange( 0, height );
    wd_volume_resize.hs_depth_volume->setRange( 0, depth );

    wd_volume_resize.sb_width_volume->setRange( 0, width );
    wd_volume_resize.sb_height_volume->setRange( 0, height );
    wd_volume_resize.sb_depth_volume->setRange( 0, depth );
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
