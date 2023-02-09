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

#include <QDir>
#include <QFileInfo>

#include "diagnostics_window_interface.h"
#include "stratmod/smodeller.hpp"

#include "diagnostics/fd_interface.hpp"
#include "diagnostics/fd_definitions.h"
#include "diagnostics/model/metadata_access.h"


struct DiagnosticsWindowInterface::DiagnosticsWindowInterfaceImpl {
    FlowDiagnosticsInterface window;
};

DiagnosticsWindowInterface::DiagnosticsWindowInterface(QWidget *parent)
{
    this->setParent(parent);
    pimpl_ = std::make_unique<DiagnosticsWindowInterfaceImpl>();
    pimpl_->window.setParent(this);
    pparent_ = parent;
}

DiagnosticsWindowInterface::~DiagnosticsWindowInterface() = default;

bool DiagnosticsWindowInterface::isImplemented() const
{
    return true;
}

bool DiagnosticsWindowInterface::isActive() const
{
    return pimpl_->window.isActive();
}

void DiagnosticsWindowInterface::setModel(stratmod::SModeller& model)
{
    pmodel_ = &model;
    pimpl_->window.setModel(pmodel_);
    model::MetadataAccess::pModel(pmodel_);
    FlowDiagnosticsDefinitions::pModel(pmodel_);
}

void DiagnosticsWindowInterface::setProjectPath(std::filesystem::path path)
{
    pimpl_->window.setProjectPath(path);
}

bool DiagnosticsWindowInterface::init()
{
    return true;
}

bool DiagnosticsWindowInterface::create()
{
    return pimpl_->window.createWindow();
}

void DiagnosticsWindowInterface::close()
{
    pimpl_->window.closeWindow();
    if(pmodel_)
    {
        pmodel_->useOpenGLCoordinateSystem();
    }
}

bool DiagnosticsWindowInterface::update()
{
    /* pimpl_->window.closeWindow(); */

    bool status = pimpl_->window.createWindow();

    return status;
}

void DiagnosticsWindowInterface::clear()
{
    pimpl_ = std::make_unique<DiagnosticsWindowInterfaceImpl>();
    if(pmodel_)
    {
        pmodel_->useOpenGLCoordinateSystem();
    }
    pmodel_ = nullptr;
    model::MetadataAccess::pModel(pmodel_);
    FlowDiagnosticsDefinitions::pModel(pmodel_);
}

bool DiagnosticsWindowInterface::preferDockedWindow() const
{
    return false;
}
