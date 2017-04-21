#include "CrossSectionItemWrapper.h"

CrossSectionItemWrapper::CrossSectionItemWrapper( double width_, double depth_ )
{
    width = width_;
    depth = depth_;
}

void CrossSectionItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{

    QPen pen_color ( QColor ( 167 , 0 , 200 ) );
    pen_color.setWidth ( 2 );

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( pen_color );
    painter->drawLine( 0, depth, width, depth );
}

//QRectF CrossSectionItemWrapper::boundingRect() const
//{
//    return boundingRect();
//}
