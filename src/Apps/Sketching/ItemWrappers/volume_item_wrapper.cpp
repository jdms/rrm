#include "volume_item_wrapper.h"
#include "Core/Geology/Models/volume.h"


VolumeItemWrapper::VolumeItemWrapper( const Section& sec ): section( sec )
{
    raw = nullptr;
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
//    if( raw != nullptr )
//        raw->clear();
    raw = nullptr;

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




void VolumeItemWrapper::setRawVolume( Volume* const& vol )
{
    raw = vol;
    updateItem();
}


Volume* VolumeItemWrapper::getRawVolume() const
{
    return raw;
}




void VolumeItemWrapper::defineSectionPlane( const Section& sec )
{
    section = sec;
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
        return raw->getDepth();
    return raw->getHeight();
}




bool VolumeItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->getVisibility();
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


