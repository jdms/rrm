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



#include "volume_item_wrapper.h"
#include "core/models/volume.h"


VolumeItemWrapper::VolumeItemWrapper( Volume* const& vol_, const Settings::CrossSection::CrossSectionDirections& sec_ )
{
    setRawVolume( vol_ );
    defineSectionPlane( sec_ );
    setupPen();
}



void VolumeItemWrapper::setupPen()
{
    countor_volume.setColor( QColor ( 0 , 200 , 0 ) );
    countor_volume.setWidth( 1 );

    fill_volume.setColor( QColor( 55, 100, 55, 75 ) );
    fill_volume.setStyle( Qt::SolidPattern );
}


void VolumeItemWrapper::clear()
{
    raw = nullptr;
    section = Settings::CrossSection::CrossSectionDirections::Y;
    section_boundary.clear();

    start.setX( 0 );
    start.setY( 0 );

    end.setX( 0 );
    end.setY( 0 );

    setupPen();
}




void VolumeItemWrapper::paint( QPainter *painter, const QStyleOptionGraphicsItem *option,
                                          QWidget *w )
{
    if( isVisible() == false ) return;

    painter->setRenderHint ( QPainter::Antialiasing );
    painter->setBrush( fill_volume );
    painter->setPen ( countor_volume );
    painter->drawPolygon( section_boundary );
}


QRectF VolumeItemWrapper::boundingRect() const
{
    return section_boundary.boundingRect();
}




void VolumeItemWrapper::setRawVolume( Volume* const& vol_ )
{
    raw = vol_;
    updateItem();
}


Volume* VolumeItemWrapper::getRawVolume() const
{
    return raw;
}




void VolumeItemWrapper::defineSectionPlane( const Settings::CrossSection::CrossSectionDirections& sec_ )
{
    section = sec_;
}


Settings::CrossSection::CrossSectionDirections VolumeItemWrapper::getSectionPlane() const
{
    return section;
}




double VolumeItemWrapper::getWidth() const
{
    return raw->getWidth();
}


double VolumeItemWrapper::getHeight() const
{
    if( section == Settings::CrossSection::CrossSectionDirections::Y )
        return raw->getLenght();
    return raw->getHeight();
}




bool VolumeItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}



void VolumeItemWrapper::startPoint( const QPointF& origin_ )
{
    start = origin_;
}



void VolumeItemWrapper::resize( const QPointF& end_ )
{
    int w = static_cast< int > ( end_.x() - start.x() );
    int h = static_cast< int > ( end_.y() - start.y() );

    resize( start.x(), start.y(), w, h ) ;

}



void VolumeItemWrapper::resize( double ox, double oy, double width, double height )
{
    section_boundary.clear();
    section_boundary = QPolygonF( QRectF( QPointF( ox, oy ), QSizeF( width, height ) ) );
    update();
}


void VolumeItemWrapper::updateItem()
{
    QPointF origin = section_boundary.boundingRect().bottomLeft();
    resize( 0.0, 0.0, getWidth(), getHeight() ) ;
}


