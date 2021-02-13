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
 * @file diagnosticsinterface.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class DiagnosticsInterface
 */

#include "diagnosticsinterface.h"

#include <memory>

#include "apps/simulator/diagnostics_window_interface.h"
#include "mainwindow.h"

#include "stratmod/smodeller.hpp"

DiagnosticsInterface::DiagnosticsInterface( MainWindow* const& window_ )
{
    window = window_;
    fd_window_interface = new DiagnosticsWindowInterface(window);

    if ( isImplemented() && (window != nullptr))
    {
        fd_window_interface->setModel(window->controller->getRulesProcessor().getSModeller());
    }
}

DiagnosticsInterface::~DiagnosticsInterface() = default;

void DiagnosticsInterface::createInterface()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    if (fd_window_interface->preferDockedWindow())
    {
        createDockedDiagnosticsWindow();
    }
    else
    {
        fd_window_interface->createFlowDiagnosticsWindow();
    }

    createDiagnosticsActions();
}

bool DiagnosticsInterface::isImplemented()
{
    if (fd_window_interface == nullptr)
    {
        return false;
    }

    return fd_window_interface->isImplemented();
}

bool DiagnosticsInterface::preferDockedWindow()
{
    if (isImplemented())
    {
        return fd_window_interface->preferDockedWindow();
    }

    return false;
}

void DiagnosticsInterface::createDockedDiagnosticsWindow()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    dw_flow_window = new QDockWidget( "Flow Diagnostics" );
    dw_flow_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_flow_window->setWidget( fd_window_interface );
    dw_flow_window->setVisible( false );
    window->addDockWidget( Qt::BottomDockWidgetArea, dw_flow_window );
}


void DiagnosticsInterface::createDiagnosticsActions()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    // init/update flow diagnostics
    connect( window, &MainWindow::runDiagnostics, this, &DiagnosticsInterface::updateWindow );

    /* *********************************************************************************************************** */
    /* Obsolete: flow window now gets information directly from model */
    /* *********************************************************************************************************** */

/*     // send the mesh and the curves of the boundary of each surface to flow diagnostics */
/*     connect( fd_window_interface, &DiagnosticsWindowInterface::getSurfacesMeshes, this, [=]( std::vector< DiagnosticsWindowInterface::TriangleMesh >& triangles_meshes, */
/*              std::vector< DiagnosticsWindowInterface::CurveMesh>& left_curves, */
/*              std::vector< DiagnosticsWindowInterface::CurveMesh >& right_curves, */
/*              std::vector< DiagnosticsWindowInterface::CurveMesh > & front_curves, */
/*              std::vector< DiagnosticsWindowInterface::CurveMesh >& back_curves ) { */
/*         window->app->getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves ); } ); */

/*     // send the mesh and the color of the regions to flow diagnostics */
/*     connect( fd_window_interface, &DiagnosticsWindowInterface::sendSimplifiedMesh, [=]( const std::vector< float >& vertices, const std::vector< unsigned int >& faces ) */
/*     { */
/*         std::vector< int > regions_; */
/*         std::map< int, std::vector< float > > colors_; */
/*         window->app->getTetrahedronsRegions( vertices, faces, regions_, colors_ ); */
/*         fd_window_interface->setTetrahedronRegions( regions_, colors_ ); } ); */

    /* *********************************************************************************************************** */

    // restart flow diagnostics
    connect( window->app, &RRMApplication::resetApplication, [=]()
    {
        fd_window_interface->clear(); 
        fd_window_interface->setModel(window->controller->getRulesProcessor().getSModeller());
        if (!fd_window_interface->preferDockedWindow())
        {
            fd_window_interface->createFlowDiagnosticsWindow();
        }
        this->updateWindow( false );
    } );
}


void DiagnosticsInterface::updateWindow( bool window_is_active )
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }
        /* fd_window_interface->setModel(window->controller->getRulesProcessor().getSModeller()); */

    if (fd_window_interface->preferDockedWindow() && (dw_flow_window != nullptr))
    {
        dw_flow_window->setVisible( window_is_active );
    }

    if (window_is_active == false)
    {
        return;
    }

    /* Call methods to update `fd_window_interface` */
    fd_window_interface->update();
}
