#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>

#include "./core/base/models/canvasstack.h"

class QGraphicsView;
class QWheelEvent;


class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

        SketchWindow( QWidget* parent = 0 );


    protected:

        void createWindow();



    private:

        CanvasStack* cs;



};

#endif // SKETCHWINDOW_H
