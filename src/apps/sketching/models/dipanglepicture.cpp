#include "dipanglepicture.h"

DipAnglePicture::DipAnglePicture()
{
    setAcceptedMouseButtons( Qt::LeftButton|Qt::RightButton ) ;

    setFlag( QGraphicsItem::ItemIgnoresTransformations, true );
}

void DipAnglePicture::setImage( const QPixmap &im )
{
    setPixmap( im );
    update();
}

void DipAnglePicture::setMovable( bool status_ )
{
    setFlag( QGraphicsItem::ItemIsSelectable, status_ );
    setFlag( QGraphicsItem::ItemIsMovable, status_ );
    update();
}
