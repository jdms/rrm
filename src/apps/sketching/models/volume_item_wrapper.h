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



#ifndef VOLUMEITEMWRAPPER_H
#define VOLUMEITEMWRAPPER_H

#include <QGraphicsPathItem>
#include <QColor>
#include <QPainter>


#include "./core/definitions/constants.hpp"

class Volume;


class VolumeItemWrapper: public QGraphicsPathItem
{

    public:

        VolumeItemWrapper() = default;
        VolumeItemWrapper( Volume* const& vol_, const Settings::CrossSection::CrossSectionDirections& sec_ = Settings::CrossSection::CrossSectionDirections::Z );


        void setRawVolume( Volume* const& vol );
        Volume* getRawVolume() const ;


        void defineSectionPlane( const Settings::CrossSection::CrossSectionDirections& sec );
        Settings::CrossSection::CrossSectionDirections getSectionPlane() const;


        double getHeight() const;
        double getWidth() const ;


        void startPoint( const QPointF& origin_ );


        void resize( const QPointF& end_ );
        void resize( double width, double height, double ox = 0.0, double oy = 0.0 );
        void updateItem();


        bool isVisible() const ;
        void clear();

        QRectF boundingRect() const;


    protected:

        void setupPen();
        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );



    private:

        QPolygonF section_boundary;
        QPen countor_volume;
        QBrush fill_volume;

        QPointF start;
        QPointF end;

        Volume* raw;
        Settings::CrossSection::CrossSectionDirections section;

};

#endif // VOLUMEITEMWRAPPER_H
