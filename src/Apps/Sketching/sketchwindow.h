#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class QGraphicsView;
class QWheelEvent;

class SketchScene_Refactored;
class CSectionScene;
class TopViewScene;


class SketchWindow: public QMainWindow
{
    Q_OBJECT

    public:

        SketchWindow( QWidget* parent = 0, bool customizable = true );

        SketchScene_Refactored* getScene() const;

    protected slots:

        bool discardSketch(){ return false; }
        bool commitSketch(){ return false; }
        bool createSurface(){ return false; }

    protected:

        void createWindow();
        void createScene();
        void createToolbarActions();

        void wheelEvent( QWheelEvent *event );

    private:

        bool is_customizable;

        QGraphicsView* gv_view;
        SketchScene_Refactored* scene;

        const double ZOOM_SCALE = 1.1;

};

#endif // SKETCHWINDOW_H
