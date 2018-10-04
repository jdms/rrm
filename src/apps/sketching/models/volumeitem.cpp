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
//        PlaneItem::resize( raw->getWidth(), raw->getLenght() );
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


bool VolumeItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}


void VolumeItem::clear()
{
    if( raw != nullptr )
        raw.reset();

    csection_direction = Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;
    csection_depth = 0.0;

//    PlaneItem::clear();
}


VolumeItem::~VolumeItem()
{
    clear();
}
