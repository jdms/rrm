#ifndef MAINWINDOW_REFACTORED_H
#define MAINWINDOW_REFACTORED_H

#include <QMainWindow>


class QActionGroup;
class QAction;
class QToolbar;
class QSlider;

class Canvas3d_Refactored;
class SketchWindow;
class ObjectTree;
class PagesStack;
class Controller_Refactored;

class MainWindow_Refactored : public QMainWindow
{
    Q_OBJECT

   public:

        explicit MainWindow_Refactored( QWidget *parent = 0 );


    signals:


        void resizedVolume( double w, double h, double d );


    protected slots:

        inline void showSidebar( bool status_ ){}
        inline void showObjectTree( bool status_ ){}
        inline void showObjectProperties( bool status_ ){}

        inline void clear(){}
        inline void resetInterface(){}
        void setDefaultValues();
        void setupInterface();


    private:

        void createWindow();
        void setupWindowProperties();
        void createMainInterface();
        void createCrossSectionInterface();
        void createTopViewInterface();
        void createSidebar();
        void createToolbarActions();

        void createController();
        void setupController();
        inline void initController(){}


    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;


        QAction* ac_show_sidebar;
        QAction* ac_show_topview;


        Controller_Refactored* controller;
        Canvas3d_Refactored* canvas3d;
        SketchWindow* sketch_window;
        SketchWindow* topview_window;
        ObjectTree* object_tree;
        PagesStack* pages_sidebar;
        QSlider* sl_depth_csection;

        QDockWidget* dw_csection;
        QDockWidget* dw_topview;
        QDockWidget* dw_object_tree;
        QDockWidget* dw_info_objects;


        const bool SIDEBAR_VISIBLE = true;
        const bool TOPVIEW_VISIBLE = false;
        const bool CSECTION_VISIBLE = true;


};

#endif // MAINWINDOW_REFACTORED_H
