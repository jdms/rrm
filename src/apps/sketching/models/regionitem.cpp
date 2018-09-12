#include <iostream>
#include <QPainter>
#include <QPen>
#include <QBrush>
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

std::size_t RegionItem::getIndex() const
{
    return raw->getIndex();
}


bool RegionItem::isSelected() const
{
    if( raw == nullptr ) return false;
    return raw->isSelected();
}



bool RegionItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}

void RegionItem::updateBoundary()
{
    if( raw == nullptr ) return;

    const PolyCurve& upper_ = raw->getUpperBound();
    const PolyCurve& lower_ = raw->getLowerBound();

    QPolygonF pol_upper_ = SketchLibraryWrapper::fromCurve2DToQt( upper_.getSubcurve( 0 ) );
    QPolygonF pol_lower_ = SketchLibraryWrapper::fromCurve2DToQt( lower_.getSubcurve( 0 ) );

    QPolygonF pol_ = pol_upper_.intersected( pol_lower_ );
    setPolygon( pol_ );

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
