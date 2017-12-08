#include <iostream>

#include "image_item_wrapper.h"



ImageItemWrapper::ImageItemWrapper()
{
    setAcceptedMouseButtons( Qt::LeftButton|Qt::RightButton ) ;
    update();

}


QRectF ImageItemWrapper::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}




void ImageItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget )
{
    painter->drawPixmap( m_imageRect.toRect(), m_image );
}


void ImageItemWrapper::resizeRectangle( const QPointF& p )
{
    m_imageRect.setBottomRight( p );
    update();
}


void ImageItemWrapper::moveRectangle( QPointF origin_ )
{
    m_imageRect.moveTo( origin_.x(), origin_.y() );
    update();

}


void ImageItemWrapper::setImage( const QPixmap &im, QPointF origin_, QPointF top_right_ )
{
    m_image = im;
    moveRectangle( origin_ );
    resizeRectangle( top_right_ );
    setPixmap( m_image );
    update();

}



void ImageItemWrapper::setImagePath( const QString& file_ )
{
    path = file_;
}


QString ImageItemWrapper::getImagePath() const
{
    return path;
}

\
QPointF ImageItemWrapper::getOrigin() const
{
    return m_imageRect.topLeft();
}


QPointF ImageItemWrapper::getTopRight() const
{
    return m_imageRect.bottomRight();
}

