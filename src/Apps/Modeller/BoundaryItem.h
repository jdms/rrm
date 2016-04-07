#ifndef BOUNDARYCONTROLLER_H
#define BOUNDARYCONTROLLER_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>

using namespace std;


class BoundaryItem: public QGraphicsPathItem
{

    public:

	BoundaryItem( int width, int height );
        ~BoundaryItem();

        void setNewBoundary (qreal x, qreal y,qreal width, qreal height);

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;


    protected:

        vector< QPainterPath > boundary;

};

#endif // BOUNDARYCONTROLLER_H
