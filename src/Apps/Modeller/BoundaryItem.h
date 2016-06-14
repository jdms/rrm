#ifndef BOUNDARYCONTROLLER_H
#define BOUNDARYCONTROLLER_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>

using namespace std;


class BoundaryItem: public QGraphicsPathItem
{

    public:

	BoundaryItem( int width, int height, const QColor& _color =  QColor ( 180 , 255 , 180 , 100 ) );
        ~BoundaryItem();

        void setNewBoundary (qreal x, qreal y,qreal width, qreal height);

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;


    protected:
        QColor color;
        vector< QPainterPath > boundary;

        QRectF bb2D_;

};

#endif // BOUNDARYCONTROLLER_H
