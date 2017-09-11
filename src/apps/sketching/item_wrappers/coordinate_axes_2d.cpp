#include <cmath>
#include <iostream>

#include <QPen>
#include <QGraphicsScene>

#include "coordinate_axes_2d.h"

#define PI 3.14159265

CoordinateAxes2d::CoordinateAxes2d( const Plane& pl )
{
    setPlane( pl );
    axisy_length = 500;
    axisx_length = 500;
}


void CoordinateAxes2d::setPlane( const Plane& pl )
{

    plane = pl;
    if( pl == Plane::XY )
    {
        current_y = height_color;
        scale = -1;
    }
    else
    {
        scale = 1;
        current_y = depth_color;
    }
}

void CoordinateAxes2d::setAxisXLenght( double l )
{
    axisx_length = l;
    update();
}

void CoordinateAxes2d::setAxisYLenght( double l )
{
    axisy_length = l;
    update();
}


QRectF CoordinateAxes2d::boundingRect() const
{
    return QRectF( -20, 20, axisx_length + 20, axisy_length + 20 );
}


 void CoordinateAxes2d::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     drawAxisX( painter );
     drawAxisY( painter );
 }


 void CoordinateAxes2d::drawAxisX( QPainter* p )
 {


     QPointF tail = QPointF( -15, -15 );

     int discX = 10;
     double stepX = static_cast< double >( axisx_length/ discX );


     QPen pen;
     pen.setWidth( 1 );
     pen.setColor( width_color );

     p->setPen( pen );
     p->drawLine( tail.x(), tail.y(), axisx_length, tail.y() );


     for( int i = 0; i < discX + 1; ++i )
     {
         double value = i*stepX;
         double dez = 0;

         if( value > 0 )
            dez = log10( value );

         p->drawLine( value , tail.y() - 3, value , tail.y() + 3 );
         p->scale( 1, scale );
         p->drawText( QPointF( value - dez*0.1*stepX, scale*tail.y() - scale*18 ), QString( "%1" ).arg( value ) );
         p->scale( 1, scale );
    }


 }

 void CoordinateAxes2d::drawAxisY( QPainter* p )
 {

     QPointF tail = QPointF( -15, -15 );

     int discY = 10;
     double stepY = static_cast< double >( axisy_length/ discY );


     QPen pen;
     pen.setWidth( 1 );
     pen.setColor( current_y );
     p->setPen( pen );
     p->drawLine( tail.x(), tail.y(), tail.x(), axisy_length );

     for( int i = 0; i < discY + 1; ++i )
     {
         double value = i*stepY;
         double dez = 0;

         if( value > 0 )
            dez = log10( value );
         p->drawLine( tail.x() - 3 , value, tail.x() + 3 , value );


         p->rotate( -scale*90 );
         p->scale( 1, scale );
         p->drawText( QPointF( -scale*value - dez*0.1*stepY, tail.x() - 10 ), QString( "%1" ).arg( value ) );
         p->scale( 1, scale );
         p->rotate( scale*90 );
     }

 }
