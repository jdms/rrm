/**
*  @file    RegionItem.hpp
*  @author  Felipe Moura de Carvalho (fmourade)
*  @date    13/12/2016
*
*  @brief Region Item description and Geometry Information
*
*  @section DESCRIPTION
*
*  This class is intended to provide a convenient interface
*  to collect geometry information to the flowdiagnostic
*  module.  
*  The user can move the item on the Scene, and
*  place it on the desired region, created by the sketch. 
*  The item has a diamond's shape and has a number to
*  refer to the regions its belong. 
*/
#ifndef _SKETCHING_REGIONITEM_HPP_
#define _SKETCHING_REGIONITEM_HPP_

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
            int type() const Q_DECL_OVERRIDE { return Type; }

            QRectF boundingRect() const Q_DECL_OVERRIDE;
            QPainterPath shape() const Q_DECL_OVERRIDE;
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

			void setID(const int _id);
			int getID() const;

        protected:
            QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

            void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
            void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
		private:
			int id_;
    };

} /* namespace RRM */

#endif /* _SKETCHING_REGIONITEM_HPP_ */
