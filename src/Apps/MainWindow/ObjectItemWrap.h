#ifndef OBJECTITEMWRAP_H
#define OBJECTITEMWRAP_H

#include <set>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPen>
#include <QPolygonF>
#include <QPointF>

#include "Object.h"
#include "./Sketching/PolyQtUtils.hpp"


class ObjectItemWrap: public QGraphicsPathItem
{
    public:

        ObjectItemWrap();

        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void clear();
        void setObjectRaw( Object* obj_, const double depth_ );

    private:

        void setupPens();

    protected:

        QPainterPath curve;
        std::set<QPointF> intersection_points;
        std::vector<bool> hidden_subpaths;

        QColor current_color;
        QPen hidden_subpaths_pen;
        QPen visible_subpaths_pen;


};

#endif // OBJECTITEMWRAP_H
