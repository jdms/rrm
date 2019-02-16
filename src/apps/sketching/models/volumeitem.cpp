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
 * @file volumeitem.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class VolumeItem
 */

#include "volumeitem.h"

VolumeItem::VolumeItem( QGraphicsItem *parent_ ): PlaneItem( parent_ )
{

}

void VolumeItem::setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    update( dir_ );
}

const std::shared_ptr< Volume >& VolumeItem::getRawVolume() const
{
    return raw;
}


void VolumeItem::updateDimensions( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    // the plane dimensions depends on the direction of the cross-section

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

}


VolumeItem::~VolumeItem()
{
    clear();
}
