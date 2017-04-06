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

        void setVolumeRaw( const Volume* vol_ );

    protected:

        QPolygonF boundary;


};

#endif // VOLUMEITEMWRAPPER_H
