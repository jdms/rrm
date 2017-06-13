#include <QPen>

#include "coordinateaxes2d.h"

#define PI 3.14159265

CoordinateAxes2d::CoordinateAxes2d()
{
    axisy_length = 50;
    axisx_length = 50;
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


 void CoordinateAxes2d::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {
     drawAxisX( painter);
     drawAxisY( painter);
 }


 void CoordinateAxes2d::drawAxisX( QPainter* p )
 {


     QPointF tail = QPointF( -15, -15 );

     int discX = 10;
     double stepX = static_cast< double >( axisx_length/ discX );


     QPen pen;
     pen.setWidth( 1 );
     pen.setColor( Qt::red );
     p->setPen( pen );
     p->drawLine( tail.x(), tail.y(), tail.x() + axisx_length, tail.y() );

     for( int i = 0; i < discX; ++i )
     {
         p->drawLine( tail.x() + i*stepX , tail.y() - 3, tail.x() + i*stepX , tail.y() + 3 );
     }

 }

 void CoordinateAxes2d::drawAxisY( QPainter* p )
 {

     QPointF tail = QPointF( -15, -15 );

     int discY = 10;
     double stepY = static_cast< double >( axisy_length/ discY );


     QPen pen;
     pen.setWidth( 1 );
     pen.setColor( Qt::blue );
     p->setPen( pen );
     p->drawLine( tail.x(), tail.y(), tail.x(), tail.y()+ axisy_length );

     for( int i = 0; i < discY; ++i )
     {
         p->drawLine( tail.x() - 3 , tail.y() + i*stepY, tail.x() + 3 , tail.y() + i*stepY );
     }

 }
