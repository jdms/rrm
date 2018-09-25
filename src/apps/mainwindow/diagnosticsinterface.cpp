#include "diagnosticsinterface.h"

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

  connect( flow_window, &FlowWindow::getLegacyMeshes, this, [=]( std::vector<double> &points, std::vector<size_t> &nu,
           std::vector<size_t> &nv, size_t num_extrusion_steps ){
    window->app->getLegacyMeshes( points, nu, nv, num_extrusion_steps ) ; } );


  connect( flow_window, &FlowWindow::getSurfacesMeshes, this, [=]( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
           std::vector< FlowWindow::CurveMesh>& left_curves,
           std::vector< FlowWindow::CurveMesh >& right_curves,
           std::vector< FlowWindow::CurveMesh > & front_curves,
           std::vector< FlowWindow::CurveMesh >& back_curves ) {
    window->app->getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves ); } );

  connect( flow_window, &FlowWindow::sendSimplifiedMesh, [=]( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces )
  {
    std::map< int, std::vector< float > > colors_ = window->app->getTetrahedronsRegions( vertices, edges, faces );

    std::vector< int > regions_;

    for( auto it: colors_ )
    {
        regions_.push_back( it.first );
    }

    flow_window->setTetrahedronRegions( regions_, colors_ ); } );


}


void DiagnosticsInterface::init()
{
    window->lockUndoRedo( true );
    dw_flow_window->setVisible( true );

    std::vector< std::size_t > domain_indexes_ = window->app->getDomains();
    flow_window->loadSurfacesfromSketch();
//    flow_window->setDomainIndexes( domain_indexes_ );

}
