#include "realslider.h"

RealSlider::RealSlider( Qt::Orientation o ):QSlider( o )
{
    maximum = 10;
    minimum = 0;
    discretization = 10;

    connect( this, &QSlider::sliderMoved, this, &RealSlider::moveInDouble );

}


RealSlider::RealSlider( Qt::Orientation o , int disc ):QSlider( o )
{
    maximum = 10;
    minimum = 0;
    discretization = disc;
}


void RealSlider::setRange( double min, double max )
{
    maximum = max;
    minimum = min;

    updateStep();

    QSlider::setRange( getIndex( min ), getIndex( max ) );
    setValue( max );
}


void RealSlider::setDiscretization( int disc )
{
    discretization = disc;
    updateStep();
}


void RealSlider::setValue( double value )
{
    int value_int = getIndex( value );
    QSlider::setValue( value_int );
}


double RealSlider::getValue() const 
{
    int value_int = QSlider::value();
    return getDoubleValue( value_int );
}


int RealSlider::getIndex( double value ) const
{
    return static_cast< int > ( value/step );
}


double RealSlider::getDoubleValue( int index ) const
{
    return static_cast< int > ( index*step );
}


void RealSlider::updateStep()
{
    step = ( maximum - minimum )/discretization;
}


void RealSlider::moveInDouble( int index )
{
    double value = getDoubleValue( index );
    emit sliderMoved( value );
}
