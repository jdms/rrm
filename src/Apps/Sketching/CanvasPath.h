#ifndef CANVASPATH_H
#define CANVASPATH_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QKeyEvent>

#include "Sketching/PathScene.h"

class CanvasPath: public QGraphicsView
{
    Q_OBJECT

    public:

        CanvasPath( QWidget* parent = 0 );
        inline void setScenePath( PathScene* const& sc_ ){ scene = sc_; setScene( scene );}


    signals:

        void decreaseSlider();
        void increaseSlider();

    protected:

        void wheelEvent( QWheelEvent *event );
        void keyPressEvent( QKeyEvent *event );

        bool draw_cross_sections = true;
        PathScene* scene;
};

#endif // CANVASPATH_H
