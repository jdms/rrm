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


#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QStatusBar>
#include <QScreen>

#include <QKeyEvent>
#include <QKeySequence>
#include <QShortcut>


#include "Help/HelpDialog.h"
#include "Object_Tree/ObjectTree.h"

#include "Controller.hpp"
#include "Sketching/SketchCanvas.h"
#include "Sketching/SketchScene.h"
#include "Sketching/CanvasPath.h"
#include "Sketching/PathScene.h"

#include "3dView/Canvas3D.h"
#include "3dView/Scene3d.h"

/**
 * @brief Main GUI and Controller of the Project
 * GUI representing the Main Window of the Project.
 * It also responsible make a bridge between Sketch Module and Extrusion Module
 */


namespace Ui {
class MainWindow;
}

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
    {
        Q_OBJECT


    public:


        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

        void run_app();

        inline void randomColor( bool status, QColor c_ = QColor( 255, 0, 0 ) )
        {
            QColor color = c_;

            if( status == true ){
                int r_ = 0, g_ = 0, b_ = 0;
                Model3DUtils::randomColor( r_, g_, b_ );
                color = QColor( r_, g_, b_ );
            }

            controller->setCurrentColor( color.red(), color.green(), color.blue() );
            sketch_scene.setCurrentColor( color );
            scene3d.setCurrentColor( color );
        }


    protected:


        void init();

        void getCurrentDirectory();

        void createWindow();
        void setupWindowProperties();
        void startController();

        void create3dSection();
        void createObjectTreeSection();
        void createSketchSection();


        void createActions();
        void createMenuBar();
        void createMainWindowActions();

        void createGeneralActions();
        void createAppRelatedActions();


        void keyPressEvent( QKeyEvent *event );


    signals:

        void saveAsCPS3( const std::string& filename );
        void saveAsIrapGrid( const std::string& filename );

        bool enableUndo( bool );
        bool enableRedo( bool );

        void updateScenes();



    protected slots:

        void clear();
        void undo();
        void redo();
        void screenshot();



    public slots:


        void exportTo();


    protected:

        Controller* controller;


private:

//        Ui::MainWindow *ui;

        int app_height;
        int app_width;

        int app_orig_x;
        int app_orig_y;

        QSlider* sl_depth_csection;

        QDockWidget* dw_object_tree;
        ObjectTree* object_tree;

        Scene3D scene3d;
        Canvas3D* canvas3d;

        QDockWidget* dw_sketch_path_canvas;
        PathScene scene_path;
        CanvasPath* canvas_path;

        QDockWidget* dw_sketch_canvas;
        SketchCanvas *canvas2d;
        SketchScene sketch_scene;

        QAction* ac_undo;
        QAction* ac_redo;

        QAction* ac_screenshot;
        QAction* ac_clear;


        QAction* ac_stratigraphy;
        QAction* ac_channel;
        QActionGroup* ag_surface_type;

        QAction* ac_sketch_above;
        QAction* ac_sketch_below;

        QAction* ac_remove_above;
        QAction* ac_remove_above_int;
        QAction* ac_remove_below;
        QAction* ac_remove_below_int;
        QActionGroup* ag_stratigraphy_rules;

        QAction* ac_discard_sketch;
        QAction* ac_commit_sketch;
        QAction* ac_interpolate;


        QWidgetAction *ac_sketchcolor;
        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

        QToolBar* tb_general;
        QToolBar* tb_sketch;


};



#endif // MAINWINDOW_H
