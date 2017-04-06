#ifndef SKETCHCANVAS_H
#define SKETCHCANVAS_H

#include <QtWidgets/QGraphicsView>

#include "SketchScene.h"


class SketchCanvas: public QGraphicsView
{
    Q_OBJECT

    public:

        SketchCanvas( QWidget* parent = 0 );

};

#endif // SKETCHCANVAS_H
