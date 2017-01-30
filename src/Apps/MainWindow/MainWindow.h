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

/**
 * @file MainWindow.h
 * @author Felipe Moura de Carvalho
 * @date Sep, 2015
 * @brief RRM's Main GUI.
 */


#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDialog>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QStyle>
#include <QKeyEvent>

#include <QtGui/QDesktopServices>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

#include "MainWindow/About/AboutWidget.hpp"

#include "3dView/View3DWindow.hpp"
#include "Sketching/SketchingWindow.h"
#include "Simulator/FlowWindow.h"




#include "Help/HelpDialog.h"
#include "Controller.hpp"


/**
 * @brief Main GUI and Controller of the Project
 * GUI representing the Main Window of the Project.
 * It also responsible make a bridge between Sketch Module and Extrusion Module
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:


        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();


    protected:


        void init();

        void createWindow();
        void createActions();
        void createMenuBar();
        void createMainWindowActions();

        void create3DViewModule();
        void create3DWindowMenuBar();
        void create3DWindowActions();

        void createSketchingModule();
        void createSketchingMenuBar();
        void createSketchingActions();

        void createFlowDiagnosticsModule();
        void createFlowDiagnosticsActions();
        void createFlowDiagnosticsMenuBar();




        void keyPressEvent( QKeyEvent *event );


    signals:

        void saveAsCPS3( const std::string& filename );
        void saveAsIrapGrid( const std::string& filename );



    protected slots:

        void clear();


    public slots:


        void exportTo();
        void initScene();


    protected:


        bool scene_initialized;

        Scene* scene;
        Controller* controller;

        HelpDialog help;
        AboutWidget * aboutRRM;



        QMenu *mn_file;
        QAction *ac_exit;

        QMenu *mn_help;
        QAction *ac_rrmGuide;
        QAction *ac_about;

        QMenu *mn_windows;
        QAction *ac_wdwsketching;
        QAction *ac_wdwseismic;
        QAction *ac_3dview;


        // Sketching Module
        QDockWidget* dw_sketching;
        SketchingWindow *sketching_window;


        // 3D View Module
        QDockWidget* dw_3dview;
        View3DWindow *view3d_window;


        //Flow Diagostics Module

        QDockWidget* dw_flowdiagnostics;
        FlowWindow *flow_window;
        QAction *ac_flowwindow;







};



#endif // MAINWINDOW_H
