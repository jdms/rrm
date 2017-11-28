#include <QTextDocument>
#include <QString>

#include <iostream>

#include "pages_stack.h"

PagesStack::PagesStack()
{
    createVolumePropertiesPage();
    createVolumeActions();
    createObjectPropertiesPage();
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

    wd_volume_resize.hs_width_volume->setRange( 1, w );
    wd_volume_resize.hs_height_volume->setRange( 1, h );
    wd_volume_resize.hs_depth_volume->setRange( 1, d );

    wd_volume_resize.sb_width_volume->setRange( 1, w );
    wd_volume_resize.sb_height_volume->setRange( 1, h );
    wd_volume_resize.sb_depth_volume->setRange( 1, d );


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



double PagesStack::getWidth()
{
    return static_cast<double>( wd_volume_resize.sb_width_volume->value() );
}


double PagesStack::getHeight()
{
    return static_cast<double>( wd_volume_resize.sb_height_volume->value() );
}


double PagesStack::getLenght()
{
    return static_cast<double>( wd_volume_resize.sb_depth_volume->value() );
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



void PagesStack::createObjectPropertiesPage()
{
    QWidget* wd_object_comments = new QWidget( this );
    wd_object_properties.setupUi( wd_object_comments );
    addWidget( wd_object_comments );

    wd_object_properties.groupBox->setTitle( "Object Log" );

    connect( wd_object_properties.te_object_properties, &QTextEdit::textChanged, [=]() {
            QString text_ = wd_object_properties.te_object_properties->toPlainText();
            emit saveText( text_ );
    } );
}


void PagesStack::loadObjectInformation( const std::string& name_, const std::string& text_ )
{
    QString obj_text_( text_.c_str() );
    wd_object_properties.groupBox->setTitle( QString( "Object Log: " ).append( name_.c_str() ) );
    wd_object_properties.te_object_properties->clear();
    wd_object_properties.te_object_properties->setPlainText( obj_text_ );

}

void PagesStack::clear()
{
    changeRangeSize( MAX_WIDTH, MAX_HEIGHT, MAX_DEPTH );
    changeVolumeSize( MAX_WIDTH*0.5, MAX_HEIGHT*0.5, MAX_DEPTH*0.5 );
    setEnabledVolumeResize( VOLUME_RESIZING );

    wd_object_properties.groupBox->setTitle( "Object Log" );
}
