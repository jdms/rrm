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



#include <random>
#include <QHBoxLayout>

#include "color_picker.h"


ColorPicker::ColorPicker( QWidget* parent ): QToolButton ( parent )
{
    createWidget();
    createActions();
}

void ColorPicker::createWidget()
{
    cd_picker_color = new QColorDialog( this );
    cd_picker_color->setWindowFlags( Qt::Widget );
    cd_picker_color->setCurrentColor( QColor( 255, 0, 0 ) );

    wa_picker_color = new QWidgetAction( this );
    wa_picker_color->setDefaultWidget( cd_picker_color );

    mn_picker_color = new QMenu( this );
    mn_picker_color->addAction( wa_picker_color );

    setPopupMode( QToolButton::MenuButtonPopup );
    setMenu( mn_picker_color );

    //the toolbutton is checkable, since it is checked the color should be fixed, and
    // random otherwise
    setCheckable( true );

    colorChanged( currentColor() );

    setMaximumSize( 35, 30 );

}

void ColorPicker::createActions()
{
    connect( mn_picker_color, &QMenu::aboutToShow, cd_picker_color, &QColorDialog::show );
    connect( cd_picker_color, &QColorDialog::rejected, mn_picker_color, &QMenu::hide );
    connect( cd_picker_color, &QColorDialog::accepted, mn_picker_color, &QMenu::hide );

    connect( cd_picker_color, &QColorDialog::currentColorChanged, this, &ColorPicker::colorChanged );
    connect( cd_picker_color, &QColorDialog::colorSelected, [=]( const QColor& color_ )
                                                            { emit colorSelected( color_ ); } );

    connect( this, &ColorPicker::toggled, [=]( bool status_ ){
                                          if( status_ == true ) setColor( cd_picker_color->currentColor() );
                                          else defineRandomColor(); } );


}


void ColorPicker::setColor( const QColor& c )
{
    cd_picker_color->setCurrentColor( c );
    colorChanged( c );
}

QColor ColorPicker::currentColor() const
{
    return cd_picker_color->currentColor();
}


void ColorPicker::colorChanged( const QColor& color_ )
{

    // this create a small icon with the color on the toolbutton
    QPixmap px( 15, 15 );
    px.fill( QColor( color_.red(), color_.green(), color_.blue() ) );
    setIcon( px );
}


void ColorPicker::defineRandomColor()
{
    QColor c;

    bool status = this->isChecked();

    if( status == false )
        c = randomColor();
    else
        c = cd_picker_color->currentColor();

    setColor( c );

    emit colorSelected( c );

}


QColor ColorPicker::randomColor()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int r = static_cast< int >( distr( eng ) );
    int b = static_cast< int >( distr( eng ) );
    int g = static_cast< int >( distr( eng ) );

    return QColor( r, g, b );
}

