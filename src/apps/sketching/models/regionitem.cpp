/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file regionitem.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class RegionItem
 */


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

    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setFillColor( r_, g_, b_ );

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
