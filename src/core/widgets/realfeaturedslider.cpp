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

//    setValue( maximum );
}

void RealFeaturedSlider::clear()
{
    markers.clear();

//    setDefaultValues();
}
