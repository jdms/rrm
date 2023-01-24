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
        FlowDiagnosticsInterface();
        virtual ~FlowDiagnosticsInterface();
        FlowDiagnosticsInterface(const FlowDiagnosticsInterface&) = delete;
        FlowDiagnosticsInterface& operator=(const FlowDiagnosticsInterface&) = delete;
        FlowDiagnosticsInterface(FlowDiagnosticsInterface&&);
        FlowDiagnosticsInterface& operator=(FlowDiagnosticsInterface&&);

        bool isAvailable();

        bool isActive();

        bool createWindow();

        void closeWindow();

        [[deprecated("Use createWindow()")]]
        bool createFlowDiagnosticsWindow() { return createWindow(); }

        [[deprecated("Use closeWindow()")]]
        void closeFlowDiagnosticsWindow() { closeWindow(); }

    private:
        struct Impl;
        std::unique_ptr<Impl> pimpl_;
};

#endif
