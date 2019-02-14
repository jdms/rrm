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
 * @file stratigraphyitem.cpp
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class StratigraphyItem
 */


#include "stratigraphyitem.h"

StratigraphyItem::StratigraphyItem( QGraphicsItem *parent_): CurveItem( parent_ )
{
    setStyle( Qt::PenStyle::DotLine );
}


void StratigraphyItem::setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    csection_direction = dir_;
    csection_depth = depth_;
    
    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    
    setColor( r_, g_, b_ );

    // initially the curve is rendered as a dot line curve, if it is done, the style turns into full
    setStyle( Qt::PenStyle::DotLine );
    updateCurve();
    
}


const std::shared_ptr< Stratigraphy >& StratigraphyItem::getRawStratigraphy() const
{
    return raw;
}


void StratigraphyItem::setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    csection_depth = depth_;
    csection_direction = dir_;
}


bool StratigraphyItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}


bool StratigraphyItem::isSelected() const
{
    if( raw == nullptr ) return false;
    return raw->isSelected();
}



void StratigraphyItem::update()
{
    updateCurve();
}


void StratigraphyItem::updateCurve()
{
    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();
    
    if( raw->isSelectable() == true )
        setColor( 255, 255, 0 );
    else
    {
        int r_, g_, b_;
        raw->getColor( r_, g_, b_ );
        setColor( r_, g_, b_ );
    }

    prepareGeometryChange();
    curve = QPainterPath();

    // if the curve is on direction Y and it is not active dont update it
    if( raw->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Y )
        if( raw->isActive() == false ) return;

    setCurve( raw->getCurve( csection_depth ) );

    QGraphicsPathItem::update();
}


std::size_t StratigraphyItem::getIndex() const
{
    return raw->getIndex();
}

void StratigraphyItem::updateLevelCurves()
{
    
    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();
    
    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setColor( r_, g_, b_ );
    
    
    // this method is only to curves on the Y direction
    prepareGeometryChange();
    if( raw->getCrossSectionDirection() != Settings::CrossSection::CrossSectionDirections::Y ) return;


    if( csection_direction == Settings::CrossSection::CrossSectionDirections::X )
        setCurves( raw->getCurves(), true );
    else
        setCurves( raw->getCurves() );
    
    
}


void StratigraphyItem::updateTrajectory()
{
    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();
    
    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setColor( r_, g_, b_ );
    
    // this method is only to curves on the X and Z direction, since the curves on Y direction has no trajectory
    if( raw->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Y ) return;
    prepareGeometryChange();
    
    setCurve( raw->getTrajectory() );
}


void StratigraphyItem::clear()
{
    csection_direction = Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;
    csection_depth = 0.0;
    
    raw.reset();
    CurveItem::clear();
}


StratigraphyItem::~StratigraphyItem()
{
    clear();
}
