#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QDockWidget>


class QActionGroup;
class QAction;
class QToolbar;
class QSlider;



class Canvas3d;
class RealFeaturedSlider;
class ObjectTree;
class SketchWindow;
class PagesStack;


#include "./core/base/models/controller.h"


class MainWindow: public QMainWindow
{
    Q_OBJECT

   public:

        explicit MainWindow( QWidget *parent = 0 );



    signals:

        void setUpColor();

        void updateVolume();
        void defineVolumeDimensions(  double width, double height, double depth );
        void addObject( Object* const& obj_ );
        void updateObject( const std::size_t );
        void updateObjects();

        void defineMainCrossSection( double depth_ );
        void addCrossSection( CrossSection* const& cs_ );


    public slots:

        void setupSlider();




    private:

        void createWindow();
        void setupWindowProperties();
        void createSketchingWindow();
        void createMainInterface();
        void createSidebar();
        void createBottombar();
        void createToolbar();


        void createController();
        void setupController();
        void run_app();



    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;


        QHBoxLayout* hb_central_widget;
        QWidget* central_widget;

        Controller* controller;
        Canvas3d* canvas3d;

        RealFeaturedSlider* sl_depth_csection;

        QDockWidget* dw_object_tree;
        ObjectTree* object_tree;
        QDockWidget* dw_object_properties;
        PagesStack* object_properties;

        QDockWidget* dw_sketchwindow;
        SketchWindow* sketch_window;

        QDockWidget* dw_topview_window;
        SketchWindow* sketch_topview_window;


        QAction* ac_sketch_above;
};

#endif // MAINWINDOW_H
