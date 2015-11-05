#ifndef BOUNDARYCONTROLLER_H
#define BOUNDARYCONTROLLER_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>

using namespace std;


class BoundaryController: public QGraphicsPathItem
{
	
    public:

        BoundaryController( int width, int height );
        ~BoundaryController();

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;


    protected:

        vector< QPainterPath > boundary;

};

#endif // BOUNDARYCONTROLLER_H
