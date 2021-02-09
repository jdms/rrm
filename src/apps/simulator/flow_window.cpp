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

#include "flow_window.h"

#include "stratmod/smodeller.hpp"

struct FlowWindow::FlowWindowImpl {
    QWidget window;
};

FlowWindow::FlowWindow(QWidget *parent) : QMainWindow(parent)
{
    pimpl_ = std::make_unique<FlowWindowImpl>();
    pparent_ = parent;
}

FlowWindow::~FlowWindow() = default;

bool FlowWindow::isImplemented() const
{
    return true;
}

void FlowWindow::setModel(SModeller& model)
{
    pmodel_ = &model;
}

bool FlowWindow::createFlowDiagnosticsWindow()
{
    pimpl_->window.resize(320, 240);
    return false;
}

void FlowWindow::update()
{
    pimpl_->window.setVisible(true);
    pimpl_->window.show();
}

void FlowWindow::clear()
{
    pimpl_ = std::make_unique<FlowWindowImpl>();
    pmodel_ = nullptr;
}

bool FlowWindow::preferDockedWindow() const
{
    return false;
}
