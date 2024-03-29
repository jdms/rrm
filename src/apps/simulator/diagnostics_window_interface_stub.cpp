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

#include "diagnostics_window_interface.h"

#include "stratmod/smodeller.hpp"

struct DiagnosticsWindowInterface::DiagnosticsWindowInterfaceImpl {
};

DiagnosticsWindowInterface::DiagnosticsWindowInterface(QWidget *parent)
{
    this->setParent(parent);
    pimpl_ = std::make_unique<DiagnosticsWindowInterfaceImpl>();
    pparent_ = parent;
}

DiagnosticsWindowInterface::~DiagnosticsWindowInterface() = default;

bool DiagnosticsWindowInterface::isImplemented() const
{
    return false;
}

bool DiagnosticsWindowInterface::isActive() const
{
    return false;
}

void DiagnosticsWindowInterface::setModel(stratmod::SModeller& model)
{
    pmodel_ = &model;
}

void DiagnosticsWindowInterface::setProjectPath(std::filesystem::path path)
{
    return;
}

bool DiagnosticsWindowInterface::init()
{
    return true;
}

void DiagnosticsWindowInterface::clear()
{
    pimpl_ = std::make_unique<DiagnosticsWindowInterfaceImpl>();
    pmodel_ = nullptr;
}

bool DiagnosticsWindowInterface::create()
{
    return true;
}

void DiagnosticsWindowInterface::close()
{
    return;
}

bool DiagnosticsWindowInterface::update()
{
    return true;
}

bool DiagnosticsWindowInterface::preferDockedWindow() const
{
    return false;
}
