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
 * @file diagnosticsinterface.h
 * @author Clarissa C. Marques
 * @brief File containing the class DiagnosticsInterface
 */


#ifndef DIAGNOSTICSINTERFACE_H
#define DIAGNOSTICSINTERFACE_H

#include <memory>

#include <QDockWidget>

class MainWindow;
class DiagnosticsWindowInterface;


/**
 *   Class responsible for initializing the exernal flow diagnostics
 */

class DiagnosticsInterface: public QObject
{
    Q_OBJECT

    public:

        /**
        * Constructor.
        * @param window a const reference to the MainWindow
        * @see MainWindow
        */
        DiagnosticsInterface( MainWindow* const& window_ );

        /**
        * Default Destructor.
        */
        ~DiagnosticsInterface();


        /**
        * Deleted copy constructor.
        */
        DiagnosticsInterface(const DiagnosticsInterface&) = delete;

        /**
        * Deleted copy assigment operator.
        */
        DiagnosticsInterface& operator=(const DiagnosticsInterface&) = delete;

        /**
        * Deleted move constructor.
        */
        DiagnosticsInterface(DiagnosticsInterface&&) = delete;

        /**
        * Deleted move assigment operator.
        */
        DiagnosticsInterface& operator=(DiagnosticsInterface&&) = delete;

        /**
        * Method to creates the interface of the diagnostics application
        * @return void.
        */
        void createInterface();

        /**
        * Method to check if the flow diagnostics application is implemented
        * @return true is flow diagnostics works, false otherwise.
        */
        bool isImplemented();

        /**
        * Method to check if the flow diagnostics application window is active
        * @return true is flow diagnostics is active, false otherwise.
        */
        bool isActive();

        /**
        * Method to tell main application if Flow Diagnostics window should be docked in main GUI
        * @return true if DiagnosticsWindowInterface should be docked, false otherwise.
        */
        bool preferDockedWindow();

    public slots:


        /**
        * Method to update the Flow Diagnostics app window.
        * @param window_is_active boolean to indicate if Flow Diagnostics window is active or not.
        * @return void.
        */
        void updateWindow( bool window_is_active );

    protected:


        /**
        * Method to create a docked Flow Diagnostics interface
        * @return void.
        */
        void createDockedDiagnosticsWindow();


        /**
        * Method to initialize and create the connects related to the actions of the Flow Diagnostics application
        * @return void.
        */
        void createDiagnosticsActions();


    protected:


        MainWindow* window = nullptr;                                       /**< Main Window */

        QDockWidget* dw_flow_window = nullptr;                              /**< DockWidget to hold the Flow Diagnostics window */

        DiagnosticsWindowInterface* fd_window_interface = nullptr;          /**< Flow Diagnostics window */


};

#endif // DIAGNOSTICSINTERFACE_H
