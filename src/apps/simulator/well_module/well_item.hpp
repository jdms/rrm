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



/**
*  @file    WellItem.hpp
*  @author  Felipe Moura de Carvalho (fmourade)
*  @date    13/12/2016
*
*  @brief Region Item description and Geometry Information
*
*  @section DESCRIPTION
*
*  This class is intended to provide a convenient interface
*  to collect well position information to the flowdiagnostic
*  module.  
*  A top view of the reservoir is provided as a guide for
*  well placement. The just need to drag and drop the correspondent
*  well on the scene.
*/
#ifndef _SKETCHING_WELLITEM_HPP_
#define _SKETCHING_WELLITEM_HPP_

#include <iostream>

#include <QtWidgets/QGraphicsItem>
#include <QtCore/QList>

namespace RRM
{

    class WellItem : public QGraphicsItem
    {		
        public:
			WellItem(QGraphicsItem *parent = nullptr);
			WellItem(QGraphicsItem *parent, int _id = -1);

			void setup();

            enum { Type = UserType + 1 };
            int type() const override { return Type; }

            QRectF boundingRect() const override;
            QPainterPath shape() const override;
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

			void setID(const int _id);
			int getID() const;

			void setColor(const QColor _color);
			QColor getColor() const;

        protected:
            QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

            void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
            void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
            void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		private:
			int id_;
			QPainterPath ellipse_;
			QColor color_;

    };

} /* namespace RRM */

#endif /* _SKETCHING_WELLITEM_HPP_ */
