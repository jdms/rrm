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
 * @file sketchinterface.h
 * @author Clarissa C. Marques
 * @brief File containing the class SketchInterface
 */


#ifndef SKETCHINTERFACE_H
#define SKETCHINTERFACE_H

#include <QDockWidget>

#include "sketching/sketchwindow.h"
#include "sketching/sketchingcontroller.h"


class MainWindow;


/**
 * \brief Class responsible for initializing the 3d application
 */


class SketchInterface: public QObject
{
    Q_OBJECT

    public:


        /**
        * Constructor.
        * @param window a const reference to the MainWindow
        * @see MainWindow
        */
        SketchInterface( MainWindow* const& window_ );


        /**
        * \brief Method to creates the interface of the Sketch application
        * @return void.
        */
        void createInterface();


    public slots:


        /**
        * \brief Method to initialize the controller and set initial setup.
        * @return void.
        */
        void init();


        /**
        * \brief Method to indicate if the user can see both canvas ( main and top-view canvas ) or not.
        * @param status if true both windows will be hidded. If false, both windows will be visible to the user.
        * @return void.
        */
        void showOnlyMainCanvas( bool status_ );


    protected:


        /**
        * \brief Method to create the Sketch window interface
        * @return void.
        */
        void createSketchingWindow();


        /**
        * \brief Method to initialize and create the connects related to the actions of the sketch application
        * @return void.
        */
        void createSketchingActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the sketch window application
        * @see sketch_window
        * @return void.
        */
        void createSketchWindowActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the top view sketch window application
        * @see sketch_topview_window
        * @return void.
        */
        void createSketchTopViewWindowActions();


        /**
        * \brief Method to initialize and create the connects related to the actions of the mainwindow application
        * @return void.
        */
        void createMainWindowActions();


    protected:

        MainWindow* window = nullptr;                                               /**< Main Window */

        SketchingController* scontroller = nullptr;                                 /**< Sketch Controller */

        SketchWindow* sketch_window = nullptr;                                      /**< Main cross-section canvas */

        SketchWindow* sketch_topview_window = nullptr;                              /**< Top View cross-section canvas */

        QDockWidget* dw_sketchwindow = nullptr;                                     /**< DockWidget to hold the main cross-section canvas */

        QDockWidget* dw_topview_window = nullptr;                                   /**< DockWidget to hold the top view cross-section canvas */

        QAction* ac_csection = nullptr;                                             /**< Action to turn the dw_sketchwindow visible/invisible */

        QAction* ac_topview = nullptr;                                              /**< Action to turn the dw_topview_window visible/invisible */


};

#endif // SKETCHINTERFACE_H
