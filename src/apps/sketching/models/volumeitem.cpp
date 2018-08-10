#include "volumeitem.h"

VolumeItem::VolumeItem( QGraphicsItem *parent_ ): PlaneItem( parent_ )
{

}

void VolumeItem::setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
//    PlaneItem::resize( raw_->getHeight(), raw_->getWidth() );
//    updateDimensions( dir_ );
    update( dir_ );
}

const std::shared_ptr< Volume >& VolumeItem::getRawVolume() const
{
    return raw;
}


void VolumeItem::updateDimensions( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        PlaneItem::resize( raw->getLenght(), raw->getHeight() );
    }

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        PlaneItem::resize( raw->getWidth(), raw->getLenght() );
    }

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        PlaneItem::resize( raw->getWidth(), raw->getHeight() );
    }
}

void VolumeItem::update( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    updateDimensions( dir_ );
    QGraphicsRectItem::update();
}


void VolumeItem::clear()
{
    if( raw != nullptr )
        raw.reset();
//    PlaneItem::clear();
}


VolumeItem::~VolumeItem()
{
    clear();
}
