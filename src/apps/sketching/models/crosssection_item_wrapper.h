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


#ifndef CROSSSECTIONITEMWRAPPER_H
#define CROSSSECTIONITEMWRAPPER_H

#include <QGraphicsLineItem>
#include <QPen>

class QPainter;


class CrossSectionItemWrapper: public QGraphicsLineItem
{
    public:

        CrossSectionItemWrapper( double w, double d );

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void setDimensions( double w, double d );
        void getDimensions( double& w, double& d ) const;

        void setWidth( double w );
        void setDepth( double d );

        double getWidth() const;
        double getDepth() const;

        void setupPens();

        void setCurrent( bool status );

        void clear();

    protected:

        double width;
        double depth;

        QPen dynamic_csection_pen;
        QPen fixed_csection_pen;
        QPen current_pen;
};

#endif // CROSSSECTIONITEMWRAPPER_H
