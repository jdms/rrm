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



#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QString>

class ImageItemWrapper: public QGraphicsPixmapItem
{

    enum class Interaction{ MOVING, RESIZING, NONE };

    public:

        ImageItemWrapper();

        void setImage( const QPixmap& im, QPointF origin_, QPointF top_right_ );

        void setImagePath( const QString& file_ );
        QString getImagePath() const;

        QPointF getOrigin() const;
        QPointF getTopRight() const;

        void moveRectangle( QPointF origin_ );
        void resizeRectangle(const QPointF &p );


        QRectF boundingRect() const;


    protected:

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget = 0 );





    private:

        QRectF m_imageRect;
        QPixmap m_image;
        QString path;

        Interaction default_action = Interaction::NONE;

};

#endif // ITEM_H
