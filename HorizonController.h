#ifndef HORIZONCONTROLLER_H
#define HORIZONCONTROLLER_H

#include <vector>

#include <QPainter>

#include "InputSketch.h"

using namespace std;

class HorizonController: public QGraphicsPathItem
{


    public:

        HorizonController();

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void setSketching( QPainterPath* curve );

        bool isSelected() const;
        void isSelected( bool option );
        bool showIntersection() const;
        void showIntersection( bool option );


        bool isValid( QGraphicsScene *scene );


    protected:

        bool selected;
        bool show_intersection;


        QPen color_inside;
        QPen color_outside;
        Qt::PenStyle style_invisible;

        QPainterPath sketch;
        vector< QPainterPath > curves;
        vector< QPointF > intersections;
        vector< bool > are_inside;
        vector< bool > are_visible;

};

#endif // HORIZONCONTROLLER_H
