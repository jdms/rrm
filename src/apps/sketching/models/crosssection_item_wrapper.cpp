/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "crosssection_item_wrapper.h"

#include <QPainter>
#include <QPen>

CrossSectionItemWrapper::CrossSectionItemWrapper( double w, double d ):
                                    width( w ), depth( d )
{
    setupPens();
}


void CrossSectionItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                                                QWidget* w )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( current_pen );
    painter->drawLine( 0, depth, width, depth );
}

void CrossSectionItemWrapper::setDimensions( double w, double d )
{
    width = w;
    depth = d;
    update();
}


void CrossSectionItemWrapper::getDimensions( double& w, double& d ) const
{
    w = width;
    d = depth;
}


void CrossSectionItemWrapper::setWidth( double w )
{
    width = w;
    update();
}


void CrossSectionItemWrapper::setDepth( double d )
{
    depth = d;
    update();
}


double CrossSectionItemWrapper::getWidth() const
{
    return width;
}


double CrossSectionItemWrapper::getDepth() const
{
    return depth;
}


void CrossSectionItemWrapper::setupPens()
{
    dynamic_csection_pen.setStyle( Qt::SolidLine );
    dynamic_csection_pen.setColor( Qt::yellow );
    dynamic_csection_pen.setWidth ( 3 );

    fixed_csection_pen.setStyle( Qt::DotLine );
    fixed_csection_pen.setColor( Qt::black );
    fixed_csection_pen.setWidth ( 2 );
}

void CrossSectionItemWrapper::setCurrent( bool status )
{
    if( status == true )
        current_pen = dynamic_csection_pen;
    else
        current_pen = fixed_csection_pen;
}


void CrossSectionItemWrapper::clear()
{
    width = 0.0;
    depth = 0.0;
    update();
}
