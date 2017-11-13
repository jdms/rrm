#include <iostream>
#include <algorithm>

#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionSlider>
#include <QRect>
#include <QPainter>

#include "slider.h"


Slider::Slider( QWidget* parent ): QSlider( parent ), markers( std::map<int, int >() )
{
    setTickPosition( QSlider::TicksLeft );
    setRange( 1, 100 );
    setTickInterval( 1 );
    setInvertedControls( true );
}



void Slider::addMarker( int value )
{
    if( isValidMarker( value ) == true ) return;

    int sliderLength, sliderMin, sliderMax;
    getSubControlDimensions( sliderLength, sliderMin, sliderMax );
    saveMarkerPosition( value, sliderLength, sliderMin, sliderMax );

    emit markValue( value );

}


void Slider::removeMarker( int value )
{
    if( isValidMarker( value ) == false ) return;

    markers.erase( value );
    emit unmarkValue( value );

    update();
}


bool Slider::isValidMarker( int value )
{
    auto search = markers.find( value );
    if( search == markers.end() )
        return false;
    return true;
}


void Slider::getMarkers( std::map< int, int >& markers_ ) const
{
    markers_ = markers;
}


int Slider::getNumberofMarkers() const
{
    return static_cast< int > ( markers.size() );
}



void Slider::updateMarkerPositions()
{

    int sliderLength;
    int sliderMin;
    int sliderMax;

    getSubControlDimensions( sliderLength, sliderMin, sliderMax );

    for( auto s: markers )
    {
        saveMarkerPosition( s.first, sliderLength, sliderMin, sliderMax );
    }
}


void Slider::saveMarkerPosition( int value, int slider_length, int slider_min, int slider_max )
{

    int pos = QStyle::sliderPositionFromValue( minimum(), maximum(), maximum() - value, slider_max - slider_min,
                                                    true ) + slider_length/2.0;

    markers[ value ] = pos;

}


void Slider::getSubControlDimensions( int& slider_length, int& slider_min, int& slider_max ) const
{

    QStyleOptionSlider opt;
    initStyleOption( &opt );

    QRect sr = style()->subControlRect( QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this );
    QRect gr = style()->subControlRect( QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this );

    slider_length = sr.height();
    slider_min = gr.y();
    slider_max = gr.bottom() - slider_length + 1;
}


void Slider::hightlightMarker( int pos_ )
{

    if( markers.empty() == true ) return;

    for( auto it: markers )
    {
        if( it.second != pos_ ) continue;
        hightlightValue( it.first );
    }

}



void Slider::paintEvent( QPaintEvent *ev )
{
    QPen pen_;
    pen_.setColor( Qt::darkRed );
    pen_.setWidth( 2.0 );

    QPainter painter( this );
    painter.setPen( pen_ );
    for( auto s: markers )
        painter.drawLine( 0, s.second, 10, s.second );
    QSlider::paintEvent(ev);
}


void Slider::resizeEvent( QResizeEvent *event )
{
    updateMarkerPositions();
    QSlider::resizeEvent( event );
}


void Slider::mousePressEvent( QMouseEvent *event )
{

    QStyleOptionSlider opt;
    initStyleOption( &opt );

    QRect sr = style()->subControlRect( QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this );
    bool status_ = sr.contains( event->pos() );

    if ( status_ && event->button() == Qt::RightButton )
        addMarker( value() );

    else if ( status_ && event->button() == Qt::MiddleButton )
        removeMarker( value() );

    else if ( event->button() == Qt::LeftButton )
        hightlightMarker( event->pos().y() );


    QSlider::mousePressEvent( event );

 }
