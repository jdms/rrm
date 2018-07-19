/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



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
#include "3dview/view3dcontroller.h"

#include "sketching/sketchingcontroller.h"
#include "sketching/sketchwindow.h"



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
        void updateSketchingWindowGeometry( int width, int height );



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
		/// Create By Felipe on March 26th using the following solution. @see https://forum.qt.io/topic/18776/solved-open-a-file-with-the-default-application-in-qt/2
		void showHelp();


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
        void run_app_sketch();
        void run_app_3d();



    private:

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;



        Controller* controller;


        View3dController* controller3d;
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
        SketchingController* scontroller;


        SketchWindow* sketch_topview_window;
        QDockWidget* dw_topview_window;

//        FlowWindow* flow_window;
//        QDockWidget* dw_flow_window;


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

        QMenu *mn_file;
        QMenu *mn_help;
        QMenu *mn_windows;

        RRMApplication* app;


};

#endif // MAINWINDOW_H
