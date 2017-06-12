#include <QPen>

#include "coordinateaxes2d.h"

#define PI 3.14159265

CoordinateAxes2d::CoordinateAxes2d()
{
    axisy_length = 10;
    axisx_length = 10;
}


void CoordinateAxes2d::setAxisXLenght( double l ){}
void CoordinateAxes2d::setAxisYLenght( double l ){}


 void CoordinateAxes2d::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
 {

     QPen pen;
     pen.setColor( Qt::black );
     pen.setWidth( 3 );

     QPointF tail = QPointF( -2, -2 );


     QPointF head_axisx = QPointF( 10, -2 );

     QPointF upper_arrow_x = QPointF( head_axisx.x() - 0.1*axisx_length, head_axisx.y() + 0.1*axisx_length*
                               std::sin( 30*PI/180 ) );
     QPointF bottom_arrow_x = QPointF( head_axisx.x() - 0.1*axisx_length, head_axisx.y() - 0.1*axisx_length*
                                std::sin( 30*PI/180 ) );

     painter->drawLine( tail.x(), tail.y(), tail.x() + axisx_length, tail.y() );
     painter->drawLine( upper_arrow_x.x(), upper_arrow_x.y(), head_axisx.x(), head_axisx.y() );
     painter->drawLine( bottom_arrow_x.x(), bottom_arrow_x.y(), head_axisx.x(), head_axisx.y() );




     QPointF head_axisy = QPointF( -2, 10 );

     QPointF upper_arrow_y = QPointF( head_axisy.x() - 0.1*axisy_length*std::sin( 30*PI/180 ),
                               head_axisy.y() - 0.1*axisy_length );
     QPointF bottom_arrow_y = QPointF( head_axisy.x() + 0.1*axisy_length*std::sin( 30*PI/180 ),
                                head_axisy.y() - 0.1*axisy_length );


     painter->drawLine( tail.x(), tail.y(), tail.x(), tail.y()+ axisy_length );
     painter->drawLine( upper_arrow_y.x(), upper_arrow_y.y(), head_axisy.x(), head_axisy.y() );
     painter->drawLine( bottom_arrow_y.x(), bottom_arrow_y.y(), head_axisy.x(), head_axisy.y() );


 }
