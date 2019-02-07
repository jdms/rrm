#include "diagnosticsinterface.h"

#include "simulator/flow_window.h"
#include "simulator/flow_visualization_controller.h"

#include "mainwindow.h"


DiagnosticsInterface::DiagnosticsInterface( MainWindow* const& window_ )
{
    window = window_;
}


void DiagnosticsInterface::createInterface()
{
    if( window == nullptr ) return;

    createDiagnosticsWindow();
    createDiagnosticsActions();
}


void DiagnosticsInterface::createDiagnosticsWindow()
{
    flow_window = new FlowWindow();
    dw_flow_window = new QDockWidget( "Flow Diagnostics" );
    dw_flow_window->setAllowedAreas( Qt::AllDockWidgetAreas );
    dw_flow_window->setWidget( flow_window );
    dw_flow_window->setVisible( false );
    window->addDockWidget( Qt::BottomDockWidgetArea, dw_flow_window );

}


void DiagnosticsInterface::createDiagnosticsActions()
{

    connect( window, &MainWindow::runDiagnostics, this, &DiagnosticsInterface::init );

    // send the legacy mesh to flow diagnostics
    connect( flow_window, &FlowWindow::getLegacyMeshes, this, [=]( std::vector<double> &points, std::vector<size_t> &nu,
             std::vector<size_t> &nv, size_t num_extrusion_steps ){
        window->app->getLegacyMeshes( points, nu, nv, num_extrusion_steps ) ; } );


    // send the mesh and the curves of the boundary of each surface to flow diagnostics
    connect( flow_window, &FlowWindow::getSurfacesMeshes, this, [=]( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
             std::vector< FlowWindow::CurveMesh>& left_curves,
             std::vector< FlowWindow::CurveMesh >& right_curves,
             std::vector< FlowWindow::CurveMesh > & front_curves,
             std::vector< FlowWindow::CurveMesh >& back_curves ) {
        window->app->getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves ); } );

    // send the mesh and the color of the regions to the flow diagnostics
    connect( flow_window, &FlowWindow::sendSimplifiedMesh, [=]( const std::vector< float >& vertices, const std::vector< unsigned int >& faces )
    {
        std::vector< int > regions_;
        std::map< int, std::vector< float > > colors_;
        window->app->getTetrahedronsRegions( vertices, faces, regions_, colors_ );
        flow_window->setTetrahedronRegions( regions_, colors_ ); } );


    //restart the flow diagnostics
    connect( window->app, &RRMApplication::resetApplication, [=]()
    {
        flow_window->clear(); this->init( false );

    } );

}


void DiagnosticsInterface::init( bool status_ )
{
    window->lockUndoRedo( status_ );
    dw_flow_window->setVisible( status_ );

    if( status_  == false ) return;

    std::vector< std::size_t > domain_indexes_ = window->app->getDomainsToFlowDiagnostics();
    std::vector< int > domains_;
    for( auto it: domain_indexes_ )
        domains_.push_back( static_cast< int >( it ) );

    flow_window->loadSurfacesfromSketch();
    flow_window->setDomains( domains_ );

}
