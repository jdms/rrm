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



#include <QTextDocument>
#include <QString>

#include <iostream>

#include "pages_stack.h"

PagesStack::PagesStack()
{
    createVolumePropertiesPage();
    createVolumeActions();
    createResolutionActions();
    createObjectPropertiesPage();
    changeRangeSize( MAX_WIDTH, MAX_HEIGHT, MAX_DEPTH );

}


void PagesStack::selectObjectPage( const Settings::Objects::ObjectType& type_ )
{
    switch ( type_ ) {

        case Settings::Objects::ObjectType::VOLUME:
            setCurrentIndex( 0 );
            break;
        case Settings::Objects::ObjectType::STRATIGRAPHY:
            setCurrentIndex( 1 );
            break;
        case Settings::Objects::ObjectType::STRUCTURAL:
            setCurrentIndex( 1 );
            break;
        default:
            break;
    }



}


void PagesStack::createVolumePropertiesPage()
{
    QWidget* wd_volume_page = new QWidget( this );
    wd_volume_resize.setupUi( wd_volume_page );
    wd_volume_page->setToolTip( "Volume Dimensions" );
    addWidget( wd_volume_page );

    wd_volume_resize.groupBox->setVisible( false );
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


void PagesStack::createResolutionActions()
{
    connect( wd_volume_resize.rb_lowresolution, &QRadioButton::pressed,
             [=](){ emit setLowResolution(); } );

    connect( wd_volume_resize.rb_mdresolution, &QRadioButton::pressed,
             [=](){ emit setMediumResolution(); } );

    connect( wd_volume_resize.rb_hgresolution, &QRadioButton::pressed,
             [=](){ emit setHighResolution(); } );

}

void PagesStack::checkLowResolution()
{
    wd_volume_resize.rb_lowresolution->setChecked( true );
}

void PagesStack::checkMediumResolution()
{
    wd_volume_resize.rb_mdresolution->setChecked( true );
}

void PagesStack::checkHighResolution()
{
    wd_volume_resize.rb_hgresolution->setChecked( true );
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


//    wd_volume_resize.hs_width_volume->setSingleStep( SINGLE_STEP );
//    wd_volume_resize.hs_height_volume->setSingleStep( SINGLE_STEP );
//    wd_volume_resize.hs_depth_volume->setSingleStep( SINGLE_STEP );

    wd_volume_resize.hs_width_volume->setTickInterval( SINGLE_STEP );
    wd_volume_resize.hs_height_volume->setTickInterval( SINGLE_STEP );
    wd_volume_resize.hs_depth_volume->setTickInterval( SINGLE_STEP );

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


    wd_volume_resize.rb_lowresolution->setEnabled( status );
    wd_volume_resize.rb_mdresolution->setEnabled( status );
    wd_volume_resize.rb_hgresolution->setEnabled( status );

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
    wd_object_properties.te_object_properties->clear();

    setCurrentIndex( 0 );

}
