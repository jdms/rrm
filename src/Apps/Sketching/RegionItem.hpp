/*
 * RegionItem.hpp
 *
 *  Created on: Dec 13, 2016
 *      Author: felipe
 */

#ifndef _SKETCHING_REGIONITEM_HPP_
#define _SKETCHING_REGIONITEM_HPP_

#include <QtWidgets/QGraphicsItem>
#include <QtCore/QList>

namespace RRM
{

    class RegionItem : public QGraphicsItem
    {
        public:
            RegionItem( );

            enum { Type = UserType + 1 };
            int type() const Q_DECL_OVERRIDE { return Type; }


            QRectF boundingRect() const Q_DECL_OVERRIDE;
            QPainterPath shape() const Q_DECL_OVERRIDE;
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

        protected:
            QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

            void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
            void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    };

} /* namespace RRM */

#endif /* _SKETCHING_REGIONITEM_HPP_ */
