#ifndef SKETCHWINDOW_H
#define SKETCHWINDOW_H

#include <QMainWindow>

class QGraphicsView;
class QWheelEvent;

class SketchScene;
class CSectionScene;
class TopViewScene;


class SketchWindow: public QMainWindow
{
    Q_OBJECT

    public:

        SketchWindow( QWidget* parent = 0, bool customizable = true );
        SketchScene* getScene() const;


        void setEnabledVolumeResize( bool status );
        void clear();



    public slots:

        void screenshot();


    signals:

        void createSurface();
        void defineColorCurrent( const QColor& c );
        void increaseSlider();
        void decreaseSlider();



    protected:

        void createWindow();
        void createScene();
        void createToolbarActions();

        void wheelEvent( QWheelEvent *event );

    private:

        bool is_customizable;

        QAction* ac_edit_boundary;
        QGraphicsView* gv_view;
        SketchScene* scene;

        const double ZOOM_SCALE = 1.1;

};

#endif // SKETCHWINDOW_H
