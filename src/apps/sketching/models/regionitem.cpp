#include <iostream>

#include "regionitem.h"

RegionItem::RegionItem( QGraphicsItem *parent_ ): PolygonItem( parent_ )
{
}

void RegionItem::setRawRegion( const std::shared_ptr< Regions >& raw_ )
{
    raw = raw_;

    const PolyCurve& upper_ = raw->getUpperBound();
    const PolyCurve& lower_ = raw->getLowerBound();

    QPolygonF pol_upper_ = SketchLibraryWrapper::fromCurve2DToQt( upper_.getSubcurve( 0 ) );
    QPolygonF pol_lower_ = SketchLibraryWrapper::fromCurve2DToQt( lower_.getSubcurve( 0 ) );

    QPolygonF pol_ = pol_upper_.intersected( pol_lower_ );
    setPolygon( pol_ );
}

const std::shared_ptr< Regions >& RegionItem::getRawRegion() const
{
    return raw;
}


bool RegionItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
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
