#ifndef CANVASSTACK_H
#define CANVASSTACK_H

#include <QWidget>
#include <QGraphicsView>
#include <QHBoxLayout>

#include "./core/base/models/container.h"


using CanvasContainer = Container< double, QGraphicsView* >;


class CanvasStack: public QWidget, public CanvasContainer
{
    Q_OBJECT

    public:

        CanvasStack();

        void initialize();

        void addElement( double, QGraphicsView* canvas_ );
        QGraphicsView* getElement( double id_ );

        void removeElement( double id_ );

        void setCurrent( double id_ );
        QGraphicsView* getCurrent();



    protected:

        double current;
        QHBoxLayout* hb_mainlayout;

};

#endif // CANVASSTACK_H
