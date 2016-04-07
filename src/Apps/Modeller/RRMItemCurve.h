/*
 * RRMItemCurve.h
 *
 *  Created on: Apr 6, 2016
 *      Author: felipe
 */

#ifndef _RRMITEMCURVE_H_
#define _RRMITEMCURVE_H_


#include <QPainter>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QVariant>

class RRMItemCurve : public QGraphicsPathItem
{
	public:
		RRMItemCurve ( );
		~RRMItemCurve ( );

	        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
	        QRectF boundingRect() const;


	    protected:

	        QPainterPath shape() const;
};

#endif /* _RRMITEMCURVE_H_ */




