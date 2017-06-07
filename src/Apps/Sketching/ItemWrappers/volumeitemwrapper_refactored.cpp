#include "volumeitemwrapper_refactored.h"
#include "Core/Geology/Models/Volume.h"


VolumeItemWrapper_Refactored::VolumeItemWrapper_Refactored( const Section& sec ): section( sec )
{
    raw = nullptr;
    setupPen();
}



void VolumeItemWrapper_Refactored::setupPen()
{
    countor_volume.setColor( QColor ( 0 , 200 , 0 ) );
    countor_volume.setWidth( 1 );

    fill_volume.setColor( QColor( 55, 100, 55, 75 ) );
    fill_volume.setStyle( Qt::SolidPattern );
}


void VolumeItemWrapper_Refactored::clear()
{
    if( raw != nullptr )
        raw->clear();
    raw = nullptr;

    section_boundary.clear();
    setupPen();
}




void VolumeItemWrapper_Refactored::paint( QPainter *painter, const QStyleOptionGraphicsItem *option,
                                          QWidget *w )
{
    if( isVisible() == false ) return;

    painter->setRenderHint ( QPainter::Antialiasing );
    painter->setBrush( fill_volume );
    painter->setPen ( countor_volume );
    painter->drawPolygon( section_boundary );

}


QRectF VolumeItemWrapper_Refactored::boundingRect() const
{
    return section_boundary.boundingRect();
}




void VolumeItemWrapper_Refactored::setRawVolume( Volume* const& vol )
{
    raw = vol;
    updateItem();
}


Volume* VolumeItemWrapper_Refactored::getRawVolume() const
{
    return raw;
}




void VolumeItemWrapper_Refactored::defineSectionPlane( const Section& sec )
{
    section = sec;
}


VolumeItemWrapper_Refactored::Section VolumeItemWrapper_Refactored::getSectionPlane() const
{
    return section;
}




double VolumeItemWrapper_Refactored::getWidth() const
{
    return raw->getWidth();
}


double VolumeItemWrapper_Refactored::getHeight() const
{
    if( section == Section::XZ )
        return raw->getDepth();
    return raw->getHeight();
}




bool VolumeItemWrapper_Refactored::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->getVisibility();
}




void VolumeItemWrapper_Refactored::resize( double ox, double oy, double width, double height )
{
    section_boundary.clear();
    section_boundary = QPolygonF( QRectF( QPointF( ox, oy ), QSizeF( width, height ) ) );
    update();
}


void VolumeItemWrapper_Refactored::updateItem()
{
    QPointF origin = section_boundary.boundingRect().bottomLeft();
    resize( 0.0, 0.0, getWidth(), getHeight() ) ;
}


