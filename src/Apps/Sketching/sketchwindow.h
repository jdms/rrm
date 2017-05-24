#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class SketchWindow: public QMainWindow
{
    Q_OBJECT

    public:

        SketchWindow( QWidget* parent = 0, bool customizable = true );

    protected slots:

        bool discardSketch(){ return false; }
        bool commitSketch(){ return false; }
        bool createSurface(){ return false; }

    protected:

        void createWindow();
        void createToolbarActions();

    private:

        bool is_customizable;


};

#endif // SKETCHWINDOW_H
