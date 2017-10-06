#ifndef CANVASSTACK_H
#define CANVASSTACK_H

#include <QGraphicsView>
#include <QHBoxLayout>

#include "container.h"


using CanvasContainer = Container< std::size_t, QGraphicsView* >;


class CanvasStack: public QGraphicsView, public CanvasContainer
{
    public:

        CanvasStack();

        void initialize();

        void addElement( std::size_t id_, QGraphicsView* canvas_ );
        void removeElement( std::size_t id_ );

        void setCurrent( std::size_t id_ );
        QGraphicsView* getCurrent();



    protected:

        std::size_t current;
        QHBoxLayout* hb_mainlayout;

};

#endif // CANVASSTACK_H
