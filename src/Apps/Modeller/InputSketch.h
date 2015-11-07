#ifndef INPUTSKETCH_H
#define INPUTSKETCH_H

#include <QPainter>
#include <QGraphicsPathItem>
#include <iostream>

using namespace std;

class InputSketch : public QGraphicsPathItem
{
	
    public:

        InputSketch( QColor color );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *w = 0 );
        QRectF boundingRect() const;

        void create( const QPointF& p );
        void add( const QPointF& p );
        QRectF clear();


        QPainterPath getSketch();
        void setSketch( const QPainterPath& p);
        void setDone( bool option );

        bool isVisible() const;
        bool isInside() const;
        void isVisible( bool option );
        void isInside( bool option );

        QPainterPath shape() const;

        void setPen( const QPen& pen );

    protected:

        QPen pen_color;

        QPainterPath curve;
        bool done;
        bool is_visible;
        bool is_inside;

};

#endif // INPUTSKETCH_H
