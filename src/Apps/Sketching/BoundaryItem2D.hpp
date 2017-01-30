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


#ifndef BOUNDARYITEM2D_H
#define BOUNDARYITEM2D_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>

#include "./src/Core/Geology/Models/BoundaryRenderable.h"


#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"

using namespace std;


class BoundaryItem2D: public QGraphicsPathItem, public BoundaryRenderable
{


    public:

        BoundaryItem2D();
        ~BoundaryItem2D();

        void edit( const qreal& x, const qreal& y, const qreal& width, const qreal& height );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void clear();
        void update( const Eigen::Affine3f &m );


        int getOriginX() const { return boundary.x(); }
        int getOriginY() const { return boundary.y(); }

        int getWidth() const { return boundary.width(); }
        int getHeight() const { return boundary.height(); }



    protected:


        QRectF boundary;
        QPointF image_position;

};

#endif // BOUNDARYCONTROLLER_H
