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
