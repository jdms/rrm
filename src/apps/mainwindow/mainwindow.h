#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QHBoxLayout>
#include <QWidget>
#include <QDockWidget>


class QActionGroup;
class QAction;
class QToolbar;
class QSlider;


//#include "3dview/canvas3d.h"
//#include "sketching/sketchwindow.h"
//#include "simulator/flow_window.h"
//#include "./core/widgets/realfeaturedslider.h"
//#include "./core/widgets/objecttree.h"
//#include "./core/widgets/pages_stack.h"


#include "./core/models/controller.h"
#include "rrmapplication.h"



class MainWindow: public QMainWindow
{
    Q_OBJECT


    friend class RRMApplication;




   public:

        explicit MainWindow( QWidget *parent = 0 );
        void updateSketchingWindowGeometry();



    signals:

        void setUpColor();

        void updateVolume();
        void defineVolumeDimensions(  double width, double height, double depth );
        void addObject( Object* const& obj_ );
        void updateObject( const std::size_t );
        void updateObjects();

        void defineMainCrossSection( double depth_ );
        void addCrossSection( CrossSection* const& cs_ );

        void resetMenus();
        void resetWindows();


    public slots:

        void save();
        void load();


    protected:

        void resizeEvent(QResizeEvent *event);


    private:

        void createWindow();
        void setupWindowProperties();
        void createMainInterface();
        void createSidebar();
        void createMenuBar();
        void createToolbar();


        void createActions();
        void createMainWindowActions();
        void createSidebarActions();


        void createSketchingWindow();
        void createSketchingActions();


        void createFlowWindow();

        void run_app();



    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;



        Controller* controller;
        Canvas3d* canvas3d;
        RealFeaturedSlider* sl_depth_csection;

        QWidget* central_widget;
        QHBoxLayout* hb_central_widget;


        ObjectTree* object_tree;
        QDockWidget* dw_object_tree;


        PagesStack* object_properties;
        QDockWidget* dw_object_properties;


        SketchWindow* sketch_window;
        QDockWidget* dw_sketchwindow;


        SketchWindow* sketch_topview_window;
        QDockWidget* dw_topview_window;

        FlowWindow* flow_window;
        QDockWidget* dw_flow_window;


        QToolBar* tb_mainwindow;



        QAction* ac_save;
        QAction* ac_load;
        QAction* ac_clear;

        QAction* ac_sketch_above;
        QAction* ac_sketch_below;

        QAction* ac_undo;
        QAction* ac_redo;

        QAction* ac_remove_above;
        QAction* ac_remove_above_int;
        QAction* ac_remove_below;
        QAction* ac_remove_below_int;
        QAction* ac_truncate;

        QAction* ac_screenshot;
        QAction* ac_output_volume;
        QAction* ac_topview;

        QMenu *mn_file;
        QMenu *mn_help;
        QMenu *mn_windows;

        RRMApplication* app;


};

#endif // MAINWINDOW_H
