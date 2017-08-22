#include <iostream>

#include "item.h"



Item::Item()
{
    m_imageRect = QRectF( 0, 0, 100, 100 );
    m_image.load( "./screenshots/complex3_csection.png" );
    if( m_image.isNull()  == true )
        std::cout << "Empty image\n" << std::flush;

}


QRectF Item::boundingRect() const
{
    return m_imageRect;
}




void Item::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget )
{
//    painter->drawRect( m_imageRect );
    painter->drawImage( m_imageRect, m_image );
}


void Item::resizeRectangle( const QPointF& p )
{
    m_imageRect.setBottomRight( p );
    update();
}


void Item::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{

    QGraphicsItem::mouseMoveEvent( event );
    update();
}
