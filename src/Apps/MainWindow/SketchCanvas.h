#ifndef SKETCHCANVAS_H
#define SKETCHCANVAS_H

#include <QtWidgets/QGraphicsView>
#include <QWheelEvent>

#include "SketchScene.h"


class SketchCanvas: public QGraphicsView
{
    Q_OBJECT

    public:

        SketchCanvas( QWidget* parent = 0 );
        void wheelEvent( QWheelEvent *event );


};

#endif // SKETCHCANVAS_H
