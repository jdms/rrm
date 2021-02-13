/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file mainwindow.h
 * @author Clarissa C. Marques
 * @brief File containing the class MainWindow
 */


#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <memory>

#include <QMainWindow>
#include <QToolBar>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

#include "rrmapplication.h"
#include "sketchinterface.h"
#include "view3dinterface.h"
#include "diagnosticsinterface.h"

#include "./core/models/controller.h"
#include "./core/widgets/objecttree.h"
#include "./core/widgets/pages_stack.h"

#include "./about/about_widget.hpp"



/**
 *   Main window application
 */

class MainWindow: public QMainWindow
{
    Q_OBJECT


    public:


        /**
        * Constructor. parent is passed to QMainWindow's constructor.
        * @param parent the parent item
        * @see MainWindow
        */
        MainWindow( QWidget* parent_ = 0 );


        /**
        * Destructor.
        */
        ~MainWindow();


        /**
        * Initialize the application.
        */
        void run();



    public slots:


        /**
        * Method (slots) to save the current session
        * @return Void.
        */
        void save();


        /**
        * Method (slots) to open a saved session
        * @return Void.
        */
        void load();


        /**
        * Method (slots) to export model's surfaces as IRAP surfaces
        * @return Void.
        */
        void exportToIRAP();


        /**
        * Method (slots) to disable the cross-section directions differents from dir_
        * This method was created because a surface only can be made in one direction per time
        * @param dir current cross-section diretion
        * @see Settings::CrossSection::CrossSectionDirections
        * @return Void.
        */
        void lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slots) to disable the others options of preserve different from the option_
        * The others options will be available again after the user submit the current option
        * @param option current option of preserve
        * @return Void.
        */
        void lockPreserve( const std::string& option_ );


        /**
        * Method (slots) to disable/enable the actions undo/redo
        * This is necessary mainly when the modelling is currently desactivate.
        * @param status boolean to indicate if the the actions should be enabled or not. If its value is true the action undo/redo should be disabled, and enable otherwise.
        * @return Void.
        */
        void lockUndoRedo( bool status_ );


        /**
        * Method (slots) to initialize all the actions to its default value.
        * @return Void.
        */
        void initializeInterface();


        /**
        * Method (slots) to disable/enable the action 'Preserve Above'
        * @param status boolean to indicate if the the action should be enabled or not. If its value is true the action 'Preserve Above' be enabled, and disabled otherwise.
        * @return Void.
        */
        void activatePreserveAbove( bool status_ );


        /**
        * Method (slots) to disable/enable the action 'Preserve Below'
        * @param status boolean to indicate if the the action should be enabled or not. If its value is true the action 'Preserve Below' be enabled, and disabled otherwise.
        * @return Void.
        */
        void activatePreserveBelow( bool status_ );


        /**
        * Method (slots) to disable/enable the action 'Preserve Region'
        * @param status boolean to indicate if the the action should be enabled or not. If its value is true the action 'Preserve Region' be enabled, and disabled otherwise.
        * @return Void.
        */
        void activatePreserveRegion( bool status_ );


        /**
        * Method (slots) to check if the action 'Regions' is enabled
        * @return boolean if its value is true the action Regions is enabled and false otherwise.
        */
        bool isRegionEnabled() const;


    signals:


        /**
        * Signal emitted to run the Flow Diagnostics application or not
        * @param status if status_ is true starts Flow Diagnostics and stop Flow Diagnostics if false.
        * @return void.
        */
        void runDiagnostics( bool status_ );


        /**
        * Signal emitted to take a screenshot of the 3d canvas
        * @return void.
        */
        void takeScreenshot();


    protected:


        /**
        * Method to set default values and properties to the window
        * @return void.
        */
        void setWindowProperties();


        /**
        * Method to call all the elements creation of the interface
        * @return void.
        */
        void createWindow();


        /**
        * Method to create all the actions and connects related to the toolbar
        * @return void.
        */
        void createActions();


        /**
        * Method to create the menubar of the window
        * @return void.
        */
        void createMenuBar();


        /**
        * Method to create the toolbar of the window
        * @return void.
        */
        void createToolbar();


        /**
        * Method to create the side bar.
        * It contais widgets related to the volume dimensions
        * @return void.
        */
        void createSideBar();


        /**
        * Method to create the controller and the application, as well as their actions and connects
        * @return void.
        */
        void createController();


        /**
        * Method to setup the object tree.
        * @return void.
        */
        void createObjectTree();


        /**
        * Method to setup the sketch application and plug it into the main window.
        * @return void.
        */
        void plugSketchInterface();


        /**
        * Method to setup the 3d application and plug it into the main window.
        * @return void.
        */
        void plug3dInterface();


        /**
        * Method to setup the Flow Diagnostics application and plug it into the main window.
        * @return void.
        */
        void plug3dDiagnosticsInterface();


        /**
        * Method to show the Help widget
        * @return void.
        */
        void showHelp();



    protected:


        friend class SketchInterface;
        friend class View3dInterface;
        friend class DiagnosticsInterface;
        friend class RRMApplication;

        std::shared_ptr< SketchInterface > sketchapp = nullptr;
        std::shared_ptr< View3dInterface > view3dapp = nullptr;
        std::shared_ptr< DiagnosticsInterface > diagapp = nullptr;

        ObjectTree* object_tree = nullptr;
        QDockWidget* dw_object_tree = nullptr;

        Controller* controller = nullptr;
        RRMApplication* app = nullptr;

        QAction* ac_save = nullptr;
        QAction* ac_load = nullptr;
        QAction* ac_clear = nullptr;
        QAction *ac_export = nullptr;
        QAction* ac_exit = nullptr;
        QAction* ac_manual = nullptr;
        QAction* ac_about = nullptr;

        QAction* ac_undo = nullptr;
        QAction* ac_redo = nullptr;
        QAction* ac_sketch_above = nullptr;
        QAction* ac_sketch_region = nullptr;
        QAction* ac_sketch_below = nullptr;
        QAction* ac_remove_above = nullptr;
        QAction* ac_remove_above_int = nullptr;
        QAction* ac_remove_below = nullptr;
        QAction* ac_remove_below_int = nullptr;
        QAction* ac_screenshot = nullptr;

        QAction* ac_direction_x = nullptr;
        QAction* ac_direction_y = nullptr;
        QAction* ac_direction_z = nullptr;

        QAction* ac_stratigraphy = nullptr;
        QAction* ac_structural = nullptr;

        QAction* ac_regions = nullptr;
        QAction* ac_diagnostics = nullptr;

        QMenu* mn_file = nullptr;
        QMenu* mn_windows = nullptr;
        QMenu* mn_help = nullptr;

        QToolBar* tb_mainwindow = nullptr;

        PagesStack* ps_objectdata = nullptr;
        QDockWidget* dw_object_properties = nullptr;

        AboutWidget* about_rrm = nullptr;

        QDir current_path = QDir::homePath();


};

#endif // MAINWINDOW1_H
