#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class QGraphicsView;

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

    private:

        bool is_customizable;

        QGraphicsView* gv_view;
        SketchScene_Refactored* scene;

};

#endif // SKETCHWINDOW_H
