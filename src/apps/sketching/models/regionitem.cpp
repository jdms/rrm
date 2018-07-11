#include <iostream>

#include "regionitem.h"

RegionItem::RegionItem( QGraphicsItem *parent_ ): PolygonItem( parent_ )
{
}

void RegionItem::setRawRegion( const std::shared_ptr< Regions >& raw_ )
{
    raw = raw_;
}

const std::shared_ptr< Regions >& RegionItem::getRawRegion() const
{
    return raw;
}

void RegionItem::clear()
{
    raw.reset();
    PolygonItem::clear();
}

RegionItem::~RegionItem()
{
    clear();
}
