#ifndef BOUNDARYCONTROLLER_H
#define BOUNDARYCONTROLLER_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>


using namespace std;


class BoundaryItem: public QGraphicsPathItem
{

    public:

        BoundaryItem( int width, int height  /*, const QColor& _color =  QColor ( 180 , 255 , 180 , 100 ) */);
        ~BoundaryItem();

        void setNewBoundary ( qreal x, qreal y,qreal width, qreal height );
        void setBackGroundImage( const QString& url );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;


    protected:

        void loadBackGroud();


    protected:


        QRectF boundary;
        QString image_file;
        QImage background_image;


        QPointF image_position;


};

#endif // BOUNDARYCONTROLLER_H
