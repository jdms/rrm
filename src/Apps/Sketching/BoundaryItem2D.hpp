#ifndef BOUNDARYITEM2D_H
#define BOUNDARYITEM2D_H

#include <vector>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPixmap>
#include <QString>

#include "model_new/BoundaryRenderable.h"


using namespace std;


class BoundaryItem2D: public QGraphicsPathItem, public BoundaryRenderable
{


    public:

        BoundaryItem2D();
        ~BoundaryItem2D();

        void edit ( qreal x, qreal y,qreal width, qreal height );
        void setBackGroundImage( const QString& url );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void update();
        void load();


    protected:

        void loadBackGroud();


    protected:

        QRectF boundary;
        QString image_file;
        QImage background_image;
        QPointF image_position;

};

#endif // BOUNDARYCONTROLLER_H
