#ifndef SKETCHCANVAS_H
#define SKETCHCANVAS_H


#include <QGraphicsView>


class SketchCanvas: public QGraphicsView
{

    public:


        SketchCanvas();

        void getScene();



    protected:

        void createScene();







};

#endif // SKETCHCANVAS_H
