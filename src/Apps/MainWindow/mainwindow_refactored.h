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
class RealSlider;

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

        void clear();

        void saveFile();
        void loadFile();

        void undo();
        void redo();


        inline void resetInterface(){}
        void setDefaultValues();
        void loadVolumeDimensions();
        void loadDefaultValues();

        void resizingVolumeWidth( double w );
        void resizingVolumeHeight( double h );
        void resizingVolumeDepth( double d );



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

        void setVolumeResizingEnabled( bool status );
        void loadDefaultRule();

        void checkUndoRedo();
        void checkSketchStatus();

        void clearInterface();
        void clearMenu();
        void clearWindows();

        void run_app();



    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;


        QAction* ac_sketch_above;
        QAction* ac_sketch_below;

        QAction* ac_remove_above;
        QAction* ac_ra_intersection;
        QAction* ac_remove_below;
        QAction* ac_rb_intersection;

        QAction* ac_show_sidebar;
        QAction* ac_show_topview;

        QAction* ac_undo;
        QAction* ac_redo;


        Controller_Refactored* controller;
        Canvas3d_Refactored* canvas3d;
        SketchWindow* sketch_window;
        SketchWindow* topview_window;
        ObjectTree* object_tree;
        PagesStack* pages_sidebar;
        RealSlider* sl_depth_csection;

        QDockWidget* dw_csection;
        QDockWidget* dw_topview;
        QDockWidget* dw_object_tree;
        QDockWidget* dw_info_objects;


        const bool SIDEBAR_VISIBLE = true;
        const bool TOPVIEW_VISIBLE = false;
        const bool CSECTION_VISIBLE = true;

        const bool CREATE_REGIONS_ALLOWED = true;
        const bool CREATE_REGIONS_DEFINED = false;

        const bool NO_RULE_CHECKED = false;
};

#endif // MAINWINDOW_REFACTORED_H
