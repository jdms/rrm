#ifndef CANVASSTACK_H
#define CANVASSTACK_H

#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>

#include "./core/models/container.h"


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

        void clear();



    protected:

        double current;
        QVBoxLayout* hb_mainlayout;

};

#endif // CANVASSTACK_H
