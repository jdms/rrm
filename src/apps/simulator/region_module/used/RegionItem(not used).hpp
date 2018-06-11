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



#ifndef _WELLITEM_HPP_
#define _WELLITEM_HPP_

#include <iostream>

#include <QtWidgets/QGraphicsItem>
#include <QtCore/QList>

namespace RRM
{

    class RegionItem : public QGraphicsItem
    {		
        public:
            RegionItem( );
			RegionItem(int _id);

			void setup();

            enum { Type = UserType + 1 };
            int type() const override { return Type; }

            QRectF boundingRect() const override;
            QPainterPath shape() const override;
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

			void setID(const int _id);
			int getID() const;

        protected:
            QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

            void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
            void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
            void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		private:
			int id_;
			QPainterPath ellipse_;

    };

} /* namespace RRM */

#endif /* _WELLITEM_HPP_ */
