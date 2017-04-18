#ifndef VOLUMEITEMWRAPPER_H
#define VOLUMEITEMWRAPPER_H


#include <QPainter>
#include <QGraphicsPathItem>

#include "Volume.h"

class VolumeItemWrapper: public QGraphicsPathItem
{

    public:

        VolumeItemWrapper();

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void setVolumeRaw( Volume* const &vol_ );
        void editGeometry( int ox_, int oy_, int w_, int h_ );

        inline void update()
        {
            boundary.clear();
            boundary = QPolygonF( QRectF( boundary.boundingRect().bottomLeft(),
                                          QSizeF( volume->getWidth(), volume->getHeight() ) ) );
        }

    protected:

        QPolygonF boundary;
        Volume* volume;


};

#endif // VOLUMEITEMWRAPPER_H
