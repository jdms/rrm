#ifndef ITEM_H
#define ITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class Item: public QGraphicsItem
{

    public:

        Item();
        void resizeRectangle( const QPointF& p );


    protected:

        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                        QWidget* widget = 0 );

        void mouseMoveEvent( QGraphicsSceneMouseEvent *event );

        QRectF boundingRect() const;


    private:

        QRectF m_imageRect;
        QImage m_image;


};

#endif // ITEM_H
