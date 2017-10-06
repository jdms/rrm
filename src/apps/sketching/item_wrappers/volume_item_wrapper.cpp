#include "volume_item_wrapper.h"
#include "core/base/models/volume.h"


VolumeItemWrapper::VolumeItemWrapper( Volume* const& vol_, const Section& sec_ )
{
    setRawVolume( vol_ );
    defineSectionPlane( sec_ );
    setupPen();
}



void VolumeItemWrapper::setupPen()
{
    countor_volume.setColor( QColor ( 0 , 200 , 0 ) );
    countor_volume.setWidth( 1 );

    fill_volume.setColor( QColor( 55, 100, 55, 75 ) );
    fill_volume.setStyle( Qt::SolidPattern );
}


void VolumeItemWrapper::clear()
{
    raw = nullptr;
    section = Section::XZ;
    section_boundary.clear();
    setupPen();
}




void VolumeItemWrapper::paint( QPainter *painter, const QStyleOptionGraphicsItem *option,
                                          QWidget *w )
{
    if( isVisible() == false ) return;

    painter->setRenderHint ( QPainter::Antialiasing );
    painter->setBrush( fill_volume );
    painter->setPen ( countor_volume );
    painter->drawPolygon( section_boundary );
}


QRectF VolumeItemWrapper::boundingRect() const
{
    return section_boundary.boundingRect();
}




void VolumeItemWrapper::setRawVolume( Volume* const& vol_ )
{
    raw = vol_;
    updateItem();
}


Volume* VolumeItemWrapper::getRawVolume() const
{
    return raw;
}




void VolumeItemWrapper::defineSectionPlane( const Section& sec_ )
{
    section = sec_;
}


VolumeItemWrapper::Section VolumeItemWrapper::getSectionPlane() const
{
    return section;
}




double VolumeItemWrapper::getWidth() const
{
    return raw->getWidth();
}


double VolumeItemWrapper::getHeight() const
{
    if( section == Section::XZ )
        return raw->getLenght();
    return raw->getHeight();
}




bool VolumeItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}



void VolumeItemWrapper::startPoint( const QPointF& origin_ )
{
    start = origin_;
}



void VolumeItemWrapper::resize( const QPointF& end_ )
{
    int w = static_cast< int > ( end_.x() - start.x() );
    int h = static_cast< int > ( end_.y() - start.y() );

    resize( start.x(), start.y(), w, h ) ;

}



void VolumeItemWrapper::resize( double ox, double oy, double width, double height )
{
    section_boundary.clear();
    section_boundary = QPolygonF( QRectF( QPointF( ox, oy ), QSizeF( width, height ) ) );
    update();
}


void VolumeItemWrapper::updateItem()
{
    QPointF origin = section_boundary.boundingRect().bottomLeft();
    resize( 0.0, 0.0, getWidth(), getHeight() ) ;
}


