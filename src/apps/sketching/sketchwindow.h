#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H


#include <QMainWindow>

#include "./core/base/models/container.h"


class QGraphicsView;
class QWheelEvent;


class SketchWindow: public QMainWindow
{
    Q_OBJECT


    public:

        SketchWindow( QWidget* parent = 0 );


    protected:

        void createWindow();





};

#endif // SKETCHWINDOW_H
