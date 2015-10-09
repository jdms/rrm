#ifndef CANVAS2D_H
#define CANVAS2D_H


#include <QGraphicsView>

class Canvas2D: public QGraphicsView
{
    Q_OBJECT

    public:

        Canvas2D( QWidget *parent = 0 );
};

#endif // CANVAS2D_H
