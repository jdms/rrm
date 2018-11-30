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



#include <iostream>

#include "realfeaturedslider.h"

RealFeaturedSlider::RealFeaturedSlider( Qt::Orientation o_ )
{
   setDefaultValues();
}


RealFeaturedSlider::RealFeaturedSlider( Qt::Orientation o_, int disc_ )
{

    setDefaultValues();
    RealFeaturedSlider::setDiscretization( disc_ );

}



void RealFeaturedSlider::setRange( double min_, double max_ )
{
    maximum = max_;
    minimum = min_;

    updateStep();

    QSlider::setRange( getIndex( min_ ), getIndex( max_ ) );
    RealFeaturedSlider::setValue( min_ );
}




int RealFeaturedSlider::getIndex( double value_ ) const
{
    return static_cast< int > ( value_/step );
}



double RealFeaturedSlider::getDoubleValue( int index_ ) const
{
    return static_cast< double > ( index_*step );
}


double RealFeaturedSlider::getValue() const
{
    int value_int = QSlider::value();
    return getDoubleValue( value_int );
}



void RealFeaturedSlider::setValue( double value_ )
{
    int value_int = getIndex( value_ );
    QSlider::setValue( value_int );
    emit sliderMoved( value_ );
//    emit valueChanged( value_ );
}



void RealFeaturedSlider::addMarker( double value )
{
    Slider::addMarker( getIndex( value ) );
}
void RealFeaturedSlider::removeMarker( double value )
{
    Slider::removeMarker( getIndex( value ) );
}

void RealFeaturedSlider::setDiscretization( int disc_ )
{
    discretization = disc_;
    updateStep();
}



void RealFeaturedSlider::increaseValue()
{
    int int_d = QSlider::value() + 1;
    double d = getDoubleValue( int_d );

    QSlider::setValue( int_d );
    emit sliderMoved( d );
}

void RealFeaturedSlider::decreaseValue()
{
    int int_d = QSlider::value() - 1;
    double d = getDoubleValue( int_d );

    QSlider::setValue( int_d );
    emit sliderMoved( d );
}



void RealFeaturedSlider::moveInDouble( int index_ )
{
    double value = getDoubleValue( index_ );
    emit sliderMoved( value );
}



double RealFeaturedSlider::maximumValue() const
{
    return maximum;
}



double RealFeaturedSlider::minimumValue() const
{
    return minimum;
}



void RealFeaturedSlider::updateStep()
{
    step = ( maximum - minimum )/discretization;
}



void RealFeaturedSlider::getDoubleMarkers( std::map< double, int >& markers_ )
{

    for( auto it: markers )
    {
        double value_ = getDoubleValue( it.first );
        markers_[ value_ ] = it.second;
    }

}


void RealFeaturedSlider::setDefaultValues()
{
    RealFeaturedSlider::setDiscretization( 10 );
    RealFeaturedSlider::setRange( 0, 1 );
    RealFeaturedSlider::setInvertedAppearance( true );
    RealFeaturedSlider::setInvertedControls( true );

    setTickPosition( QSlider::TicksLeft );

    connect( this, &Slider::sliderMoved, this, &RealFeaturedSlider::moveInDouble );
    connect( this, &Slider::markValue, [=]( int v, QColor c_ ){ emit markValue( getDoubleValue( v ), c_ ); } );
    connect( this, &Slider::unmarkValue, [=]( int v ){ emit unmarkValue( getDoubleValue( v ) ); } );
    connect( this, &Slider::hightlightValue, [=]( int v ){ emit hightlightValue( getDoubleValue( v ) ); } );

    connect( this, &QSlider::valueChanged, this, &Slider::sliderMoved );
//    setValue( maximum );
}

void RealFeaturedSlider::clear()
{
    clearMarkers();

//    setDefaultValues();
}
