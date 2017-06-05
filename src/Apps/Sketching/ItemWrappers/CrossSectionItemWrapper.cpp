#include "CrossSectionItemWrapper.h"

CrossSectionItemWrapper::CrossSectionItemWrapper( double width_, double depth_ )
{
    width = width_;
    depth = depth_;

    pen_color = QPen( QColor ( 0, 0 , 0 ) );
    pen_color.setWidth ( 2 );
}

void CrossSectionItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( pen_color );
    painter->drawLine( 0, depth, width, depth );
}
