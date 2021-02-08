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

#include "apps/simulator/flow_window.h"
#include "mainwindow.h"

#include "stratmod/smodeller.hpp"

DiagnosticsInterface::DiagnosticsInterface( MainWindow* const& window_ )
{
    window = window_;
    flow_window = std::make_unique<FlowWindow>();

    if ( isImplemented() && (window != nullptr))
    {
        flow_window->setModel(window->controller->getRulesProcessorModel());
    }
}

DiagnosticsInterface::~DiagnosticsInterface() = default;

void DiagnosticsInterface::createInterface()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    if (flow_window->preferDockedWindow())
    {
        createDockedDiagnosticsWindow();
    }
    else
    {
        flow_window->createFlowDiagnosticsWindow();
    }

    createDiagnosticsActions();
}

bool DiagnosticsInterface::isImplemented()
{
    if (flow_window == nullptr)
    {
        return false;
    }

    return flow_window->isImplemented();
}

void DiagnosticsInterface::createDockedDiagnosticsWindow()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    dw_flow_window = std::make_unique<QDockWidget>( "Flow Diagnostics" );
    dw_flow_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_flow_window->setWidget( flow_window.get() );
    dw_flow_window->setVisible( false );
    window->addDockWidget( Qt::BottomDockWidgetArea, dw_flow_window.get() );
}


void DiagnosticsInterface::createDiagnosticsActions()
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }

    // init/update flow diagnostics
    connect( window, &MainWindow::runDiagnostics, this, &DiagnosticsInterface::update );

    /* *********************************************************************************************************** */
    /* Obsolete: flow window now gets information directly from model */
    /* *********************************************************************************************************** */

/*     // send the mesh and the curves of the boundary of each surface to flow diagnostics */
/*     connect( flow_window, &FlowWindow::getSurfacesMeshes, this, [=]( std::vector< FlowWindow::TriangleMesh >& triangles_meshes, */
/*              std::vector< FlowWindow::CurveMesh>& left_curves, */
/*              std::vector< FlowWindow::CurveMesh >& right_curves, */
/*              std::vector< FlowWindow::CurveMesh > & front_curves, */
/*              std::vector< FlowWindow::CurveMesh >& back_curves ) { */
/*         window->app->getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves ); } ); */

/*     // send the mesh and the color of the regions to flow diagnostics */
/*     connect( flow_window, &FlowWindow::sendSimplifiedMesh, [=]( const std::vector< float >& vertices, const std::vector< unsigned int >& faces ) */
/*     { */
/*         std::vector< int > regions_; */
/*         std::map< int, std::vector< float > > colors_; */
/*         window->app->getTetrahedronsRegions( vertices, faces, regions_, colors_ ); */
/*         flow_window->setTetrahedronRegions( regions_, colors_ ); } ); */

    /* *********************************************************************************************************** */

    // restart flow diagnostics
    connect( window->app, &RRMApplication::resetApplication, [=]()
    {
        flow_window->clear(); 
        flow_window->setModel(window->controller->getRulesProcessorModel());
        this->update( false );
    } );
}


void DiagnosticsInterface::update( bool status_ )
{
    if (!isImplemented() || (window == nullptr))
    {
        return;
    }
    /* flow_window->setModel(window->controller->getRulesProcessorModel()); */

    if (flow_window->preferDockedWindow() && (dw_flow_window != nullptr))
    {
        dw_flow_window->setVisible( status_ );
    }

    if (status_ == false)
    {
        return;
    }

    /* Call methods to update `flow_window` */
    flow_window->update();
}
