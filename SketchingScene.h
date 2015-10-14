#ifndef SKETCHINGSCENE_H
#define SKETCHINGSCENE_H

#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


#include "InputSketch.h"
#include "HorizonController.h"


class SketchingScene: public QGraphicsScene
{
    Q_OBJECT

    public:

        SketchingScene( QObject *parent = 0 );

        // virtual mouse methods

        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

    private:

        InputSketch *sketch;
        HorizonController *horizonc;

};

#endif // SKETCHINGSCENE_H
