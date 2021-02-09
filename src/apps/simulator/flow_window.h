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


#ifndef RRM_APPS_SIMULATOR_FLOWWINDOW_H
#define RRM_APPS_SIMULATOR_FLOWWINDOW_H

// Forward declaration of modeller class
/* namespace stratmod */ 
/* { */
    class SModeller;
/* } */

#include <functional>
#include <memory>
#include <vector>

#include <QMainWindow>


class FlowWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /**
        * Constructor.
        * @param Optional pointer to parent widget
        */
        FlowWindow(QWidget* parent = nullptr);

        /**
        * Default Destructor.
        */
        virtual ~FlowWindow();

        /**
        * Deleted copy constructor.
        */
        FlowWindow(const FlowWindow&) = delete;

        /**
        * Deleted copy assigment operator.
        */
        FlowWindow& operator=(const FlowWindow&) = delete;

        /**
        * Deleted move constructor.
        */
        FlowWindow(FlowWindow&&) = delete;

        /**
        * Deleted move assigment operators
        */
        FlowWindow& operator=(FlowWindow&&) = delete;

        /**
        * Method to check if the flow diagnostics application is implemented
        * @return true is flow diagnostics works, false otherwise (returned value must not change during execution).
        */
        bool isImplemented() const;

        /**
        * Set model for flow diagnostics application
        * param model Reference to modeller class object
        */
        void setModel(SModeller& model);

        /**
        * Method to clear the flow diagnostics application window
        */
        void clear();

        /**
        * Method to update the flow diagnostics application window
        */
        void update();

        /**
        * Method to create an independent flow diagnostics application window
        * @return true if successful, false otherwise.
        */
        bool createFlowDiagnosticsWindow();

        /**
        * Method to tell main application if Flow Diagnostics window should be docked in main GUI
        * @return true if FlowWindow should be docked, false otherwise (returned value must not change during execution).
        */
        bool preferDockedWindow() const;

        /** Deprecated data structure */
        struct TriangleMesh
        {
            std::vector<double> vertex_list;
            std::vector<std::size_t> face_list;
        };

        /** Deprecated data structure */
        struct CurveMesh
        {
            std::vector<double> vertex_list;
            std::vector<std::size_t> edge_list;
        };

    protected:
        SModeller* pmodel_ = nullptr;                              /**< Pointer to model */
        QWidget* pparent_ = nullptr;                              /**< Pointer to parent Widget */

    private:
        struct FlowWindowImpl;
        std::unique_ptr<FlowWindowImpl> pimpl_;
};

#endif
