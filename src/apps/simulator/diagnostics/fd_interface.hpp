/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2021
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with RRM.  If not, see <http://www.gnu.org/licenses/>,
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301  USA.
 */

/**
 * @file fd_interface.h
 * @author Julio Daniel Machado Silva
 * @brief Interface between sketching and flow diagnostics gui
 */

#ifndef RRMQMLVTK_SRC_FD_INTERFACE_HPP
#define RRMQMLVTK_SRC_FD_INTERFACE_HPP

#include <memory>

class FlowDiagnosticsInterface {
    public:
        /// Constructor
        FlowDiagnosticsInterface();

        /// Default destructor
        virtual ~FlowDiagnosticsInterface();

        /// Copy constructor is deleted
        FlowDiagnosticsInterface(const FlowDiagnosticsInterface&) = delete;

        /// Copy assigment is deleted
        FlowDiagnosticsInterface& operator=(const FlowDiagnosticsInterface&) = delete;

        /// Default move constructor
        FlowDiagnosticsInterface(FlowDiagnosticsInterface&&);

        /// Default move assigment
        FlowDiagnosticsInterface& operator=(FlowDiagnosticsInterface&&);

        /// Return true if FD is available -- thread compatible
        //
        // This method queries the current number of regions in the model to
        // determine if FD should run (it returns true if there is at least 1
        // region in the model).
        //
        // Concurrent changes to the model might produce incorrect results.
        //
        bool isAvailable();

        /// Return true if FD gui is running -- thread safe
        bool isActive();

        /// Create new FD window -- thread safe
        bool createWindow();

        /// Close FD window -- thread safe
        void closeWindow();

        /// DEPRECATED: Create new FD window
        [[deprecated("Use FlowDiagnosticsInterface::createWindow()")]]
        bool createFlowDiagnosticsWindow() { return createWindow(); }

        /// DEPRECATED: Close FD window
        [[deprecated("Use FlowDiagnosticsInterface::closeWindow()")]]
        void closeFlowDiagnosticsWindow() { closeWindow(); }

        void setModel(stratmod::SModeller& model);

    private:
        struct Impl;                    /// FD gui implementation
        std::unique_ptr<Impl> pimpl_;   /// Pointer to FD gui implementation
};

#endif
