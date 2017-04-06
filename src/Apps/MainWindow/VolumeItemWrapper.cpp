#include "VolumeItemWrapper.h"

VolumeItemWrapper::VolumeItemWrapper()
{

}

void VolumeItemWrapper::paint( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
    painter->setRenderHint ( QPainter::Antialiasing );

    QPen pen_color ( QColor ( 0 , 200 , 0 ) );
    pen_color.setWidth ( 1 );

    QBrush brush;
    brush.setColor( QColor( 55, 100, 55, 75 ) );
    brush.setStyle( Qt::SolidPattern );


    painter->setBrush( brush );
    painter->setPen ( pen_color );
    painter->drawPolygon( boundary );
}

QRectF VolumeItemWrapper::boundingRect() const
{
    return boundary.boundingRect();
}


void VolumeItemWrapper::setVolumeRaw( const Volume* vol_ )
{
    boundary.clear();

    double ox_ = 0.0f, oy_ = 0.0f, oz_ = 0.0f;
    double w_ = 0.0f, h_ = 0.0f, d_ = 0.0f;

    vol_->getOrigin( ox_, oy_, oz_ );
    vol_->getDimensions( w_, h_, d_ );

    QRectF rect_( ox_, oy_, w_, h_ );
    boundary = QPolygonF( rect_ );
}
