#include "volumeitem.h"

VolumeItem::VolumeItem( QGraphicsItem *parent_ ): PlaneItem( parent_ )
{

}

void VolumeItem::setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    PlaneItem::resize( raw_->getHeight(), raw_->getWidth() );
    update();
}

const std::shared_ptr< Volume >& VolumeItem::getRawVolume() const
{
    return raw;
}


void VolumeItem::update()
{
}


void VolumeItem::clear()
{
    raw.reset();
    PlaneItem::clear();
}


VolumeItem::~VolumeItem()
{
    clear();
}
