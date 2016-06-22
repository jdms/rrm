#include "FlowWindow.h"

FlowWindow::FlowWindow( QWidget *parent ) : QMainWindow( parent )
{
    createWindow();
    createToolBar();
    createActions();
    getCurrentDirectory();

    file_of_parameters.clear();
    file_of_mesh.clear();
    type_of_file.clear();
    mesh_method = 1;
    read_file = true;

    show_toolbar = false;

}

void FlowWindow::createWindow()
{

    qdockparametersBar = new QDockWidget( this );
    qdockparametersBar->setAllowedAreas( Qt::LeftDockWidgetArea );
    qdockparametersBar->setWidget( &parametersBar );
    qdockparametersBar->setVisible( false );

    addDockWidget( Qt::LeftDockWidgetArea, qdockparametersBar );


    qdockopenfilesBar = new QDockWidget( this );
    qdockopenfilesBar->setAllowedAreas( Qt::BottomDockWidgetArea );
    qdockopenfilesBar->setWidget( &openfilesBar );
    qdockopenfilesBar->setVisible( false );

    addDockWidget( Qt::BottomDockWidgetArea, qdockopenfilesBar );


    qdockresolutionmesh = new QDockWidget( this );
    qdockresolutionmesh->setAllowedAreas( Qt::BottomDockWidgetArea );
    qdockresolutionmesh->setWidget( &resolutionmeshBar );
    qdockresolutionmesh->setVisible( false );

    addDockWidget( Qt::LeftDockWidgetArea, qdockresolutionmesh );

    qdockcrosssectionnormalBar = new QDockWidget( this );
    qdockcrosssectionnormalBar->setAllowedAreas( Qt::LeftDockWidgetArea );
    qdockcrosssectionnormalBar->setWidget( &crosssectionnormalBar );
    qdockcrosssectionnormalBar->setVisible( false );
    addDockWidget( Qt::LeftDockWidgetArea, qdockcrosssectionnormalBar );



    sb_statusbar = new QStatusBar();
    this->setStatusBar( sb_statusbar );

    pb_processprogress = new QProgressBar( this );
    pb_processprogress->setVisible( false );
    sb_statusbar->addPermanentWidget( pb_processprogress, 1 );


    controller = new FlowVisualizationController( this );
    canvas.setController( controller );

    controller->setParent( this );
    controller->setCounterProgressinData();


    canvas.setMinimumSize( 1024, 600 );
    this->setCentralWidget( &canvas );
}


void FlowWindow::createToolBar()
{

    qtoolbarFlow = new QToolBar();


    qreloadSurface = new QAction( "Surface from Skecth", qtoolbarFlow );
    connect( qreloadSurface, &QAction::triggered, this, [=](){ emit getCrossSection(); } );

    qoopenfilesDialog = new QAction( "Surface from a File", qtoolbarFlow );
    connect( qoopenfilesDialog, SIGNAL( triggered(bool) ), qdockopenfilesBar, SLOT( show() ) );


    qflowparametersDialog = new QAction( "Flow Input", qtoolbarFlow );
    connect( qflowparametersDialog, SIGNAL( triggered(bool) ), qdockparametersBar, SLOT( show() ) );


    qmeshresolution  = new QAction( "Mesh Resolution", qtoolbarFlow );
    connect( qmeshresolution, &QAction::triggered, this, [=](){ qdockresolutionmesh->show(); updateVisualizationParameters(); } );



    qbuildvolumetricMesh = new QAction( "Volumetric Mesh", qtoolbarFlow );
    connect( qbuildvolumetricMesh, &QAction::triggered, this, [=](){ if( controller->isMeshOk() == false || controller->isVolumetricOk() == true ) return;
                                                                     controller->computeVolumetricMesh();
                                                                     canvas.updateVolumetricMesh();  }  );

    qcomputeFlowProperties = new QAction( "Compute Properties", qtoolbarFlow );
    connect( qcomputeFlowProperties, &QAction::triggered, this, [=](){  if( controller->isVolumetricOk() == false || controller->arePropertiesComputed() == true ) return;
                                                                        controller->computeFlowProperties(); }  );

    qshowMovingCrossSection = new QAction( "CrossSection", qtoolbarFlow );
    qshowMovingCrossSection->setCheckable( true );
    connect( qshowMovingCrossSection, &QAction::toggled, this, [=]( bool option ){ if( option == true )
                                                                                    qdockcrosssectionnormalBar->show();
                                                                                   else{
                                                                                    canvas.disableCrossSection();
                                                                                    qdockcrosssectionnormalBar->close();
                                                                                    } } );


    qexportsurface  = new QAction( "Export Surface", qtoolbarFlow );
    connect( qexportsurface, SIGNAL( triggered(bool) ), this, SLOT( exportSurfaceFile() ) );


    qexportvolume  = new QAction( "Export Volume", qtoolbarFlow );
    connect( qexportvolume, SIGNAL( triggered(bool) ), this, SLOT( exportVolumeFile() ) );


    qexportcornerpoint  = new QAction( "Export CornerPoint", qtoolbarFlow );
    connect( qexportcornerpoint, SIGNAL( triggered(bool) ), this, SLOT( exportCornerPointFile() ) );


    qclear = new QAction( "Clear", qtoolbarFlow );
    connect( qclear, &QAction::triggered, this, [=](){ controller->clear(); canvas.clear(); qexportcornerpoint->setEnabled( true ); } );


	qtoolbarFlow->addAction( qreloadSurface );
    qtoolbarFlow->addAction( qoopenfilesDialog );
    qtoolbarFlow->addAction( qflowparametersDialog );
    qtoolbarFlow->addAction( qmeshresolution );
    qtoolbarFlow->addAction( qbuildvolumetricMesh );
    qtoolbarFlow->addAction( qcomputeFlowProperties );
    qtoolbarFlow->addAction( qshowMovingCrossSection );

    qtoolbarFlow->addAction( qexportsurface );
    qtoolbarFlow->addAction( qexportvolume );
    qtoolbarFlow->addAction( qexportcornerpoint );

    qtoolbarFlow->addAction( qclear );


    addToolBar( qtoolbarFlow );
    qtoolbarFlow->setVisible( true );

}


void FlowWindow::createActions()
{

    connect( &openfilesBar, SIGNAL( closeBar() ), qdockopenfilesBar, SLOT( close() ) );

    connect( &openfilesBar, &OpenFlowFilesBar::emitParametersandMeshFiles, this, [=]( const int& method, bool read,  const std::string& file_mesh, const std::string& file_type, const std::string& input_file )
    {
        if( controller->isMeshOk() == true ) return;

        read_file = read;

        if( method == 1 )
        {
            controller->readUnstructured( read, file_mesh, file_type, input_file  );
            updateParameterFields();

            file_of_parameters = input_file;
            file_of_mesh = file_mesh;
            type_of_file = file_type;
            mesh_method = method;

            qexportcornerpoint->setEnabled( false );

        }
        else if( method == 2 )
        {
            controller->readCornerPoint( read, file_mesh );

            file_of_mesh = file_mesh;
            type_of_file = ".txt";
            mesh_method = method;

            qexportcornerpoint->setEnabled( true );

        }

        qdockopenfilesBar->close();

    } );

    connect( &parametersBar, &FlowParametersBar::readSurface, controller, [=]()
    {
        controller->reloadMesh( mesh_method, read_file,  file_of_mesh, type_of_file, file_of_parameters  );

    } );


    connect( &openfilesBar, SIGNAL( emitTolerancesValues( float, float ) ), controller, SLOT( setToleranceValues( float, float ) ) );



    connect( &parametersBar, &FlowParametersBar::rebuildTetrahedricalVolume, controller, [=](){ controller->computeVolumetricMesh(); } );

    connect( &parametersBar, SIGNAL( sendToleranceValues( const float& , const float& ) ), controller, SLOT( setToleranceValues( const float& , const float& ) ) );
    connect( &parametersBar, SIGNAL( sendTetgenCommand( std::string& ) ), controller, SLOT( setTetgenCommand( std::string& ) ) );

    connect( &parametersBar, SIGNAL( sendPropertyArea(  const int& , const std::vector< double >& ) ), controller, SLOT( setPropertyArea(  const int&, const std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendBoundariesValues(  int, std::vector< double >& ) ), controller, SLOT( setBoundariesValues(  int, std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendWellsValues(  int, std::vector< double >& ) ), controller, SLOT( setWellsValues(  int, std::vector< double >& ) ) );

    connect( &parametersBar, SIGNAL( sendTOFBoundaryValues( int, std::vector< double >&  ) ), controller, SLOT( setTofBoundaryValues( int, std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendTrBoundaryValues(  int, std::vector< double >& ) ), controller, SLOT( setTrBoundaryValues(  int, std::vector< double >& ) ) );


    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, this, [=]( float X, float Y, float Z ){ canvas.setCrossSectionNormalCoordinates( X, Y, Z ); qdockcrosssectionnormalBar->close(); } );
    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, this, [=](){ qdockcrosssectionnormalBar->close();  qshowMovingCrossSection->setChecked( false ); canvas.disableCrossSection(); } );

    connect( &parametersBar, SIGNAL( closeBar() ), qdockparametersBar, SLOT( close() ) );


    connect( controller, SIGNAL( updateMesh() ), &canvas, SLOT( updateMesh() ) );
    connect( controller, SIGNAL( updateVolumetricMesh() ), &canvas, SLOT( updateVolumetricMesh() ) );
    connect( controller, SIGNAL( updatePolyMesh() ), &canvas, SLOT( updateMeshfromFile() ) );


    connect( controller, &FlowVisualizationController::getSurfaceCrossSection, this, [=](){ emit getSurfaceCrossSection(); } );
    connect( controller, SIGNAL( readFile() ), qdockopenfilesBar, SLOT( show() ) );
    connect( controller, SIGNAL( editParameters() ), qdockparametersBar, SLOT( show() ) );
    connect( controller, SIGNAL( applyCrossSection() ), qdockcrosssectionnormalBar, SLOT( show() ) );

	connect(controller, &FlowVisualizationController::hideToolbar, this, [=](){ show_toolbar = !show_toolbar; qtoolbarFlow->setVisible(show_toolbar);  });

    connect( controller, &FlowVisualizationController::clearAll, this, [=](){
        controller->clear(); canvas.clear(); qexportcornerpoint->setEnabled( true );
    } );

    connect( &resolutionmeshBar, &DialogMeshVisualizationParameters::sendVisualizationParameters, this, [=]( const std::string& cmd, const std::string& method, const float &value ) {

        if( method.compare( "PARTITION" ) == 0 )
            controller->setMeshVisualizationParameters( cmd, 1, value, -1 );
        else if( method.compare( "EDGELENGHT" ) == 0 )
            controller->setMeshVisualizationParameters( cmd, 2, -1, value );

        qdockresolutionmesh->close();


    } );

    connect( &resolutionmeshBar, SIGNAL( closeBar() ), qdockresolutionmesh, SLOT( close() ) );

}



void FlowWindow::getCurrentDirectory()
{

    QDir app_dir = QDir( qApp->applicationDirPath() );

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    QString current_dir ( app_dir.path ()+"\\" );

#elif defined(__linux__)               // Linux Directory Style
    QString current_dir ( app_dir.path ( ) + "/" );

#else
    /* Error, both can't be defined or undefined same time */
    std::cout << "Operate System not supported !"
    halt();

#endif

    canvas.setCurrentDirectory( current_dir.toStdString() );


}


void FlowWindow::updateParameterFields()
{
    std::string cmd;

    int np = 0;
    std::vector< double > values;


    int nb = 0;
    std::vector< double > vbound;
    int nswells = 0;
    std::vector< double > vswells;
    int ntfbound = 0;
    std::vector< int > vtfbound;
    int ntrbound = 0;
    std::vector< int > vtrbound;

    controller->getTetgenParameter( cmd );
    parametersBar.setTetgenCommand( cmd.c_str() );


    controller->getPropertyArea( np, values );
    parametersBar.setPropertyAreaParameters( np, values );


    controller->getBoundariesValues( nb, vbound );
    parametersBar.setSurfaceBoundariesParameter( nb, vbound );

    controller->getWellsParameter( nswells, vswells );
    parametersBar.setWellParameter( nswells, vswells );

    controller->getTofBoundaryParameter( ntfbound, vtfbound );
    parametersBar.setTofBoundaryParameter( ntfbound, vtfbound );


    controller->getTracerBoundaryParameter( ntrbound, vtrbound );
    parametersBar.setTracerBoundaryParameter( ntrbound, vtrbound );


}


void FlowWindow::updateVisualizationParameters()
{


    std::string trianglecmd;
    double meshscale = 0.0;
    int resolutiontype = 1;
    int npartitionedge = 1;
    double lenghtedge = 0.5;

    controller->getMeshVisualizationParameters( trianglecmd, resolutiontype, npartitionedge, lenghtedge );

    if( resolutiontype == 1 )
        resolutionmeshBar.setVisualizationParameters( trianglecmd, "PARTITION", npartitionedge, lenghtedge );
    else if( resolutiontype == 2 )
        resolutionmeshBar.setVisualizationParameters( trianglecmd, "EDGELENGHT", npartitionedge, lenghtedge );



}


void FlowWindow::startProgressBar( const unsigned int& min, const unsigned int& max )
{

    pb_processprogress->setVisible( true );

    pb_processprogress->setMaximum( (int) max );
    pb_processprogress->setMinimum( (int) min );

    pb_processprogress->setValue( min );
}


void FlowWindow::updateProgressBar( const unsigned int& value )
{

    pb_processprogress->setValue( value );

    if( ( int ) value == pb_processprogress->maximum() )
        pb_processprogress->setVisible( false );
}


void FlowWindow::exportSurfaceFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    controller->exportSurfacetoVTK( filename.toStdString() );

}


void FlowWindow::exportVolumeFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    controller->exportVolumetoVTK( filename.toStdString() );

}



void FlowWindow::exportCornerPointFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./exported/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    controller->exportCornerPointtoVTK( filename.toStdString() );

}


void FlowWindow::keyPressEvent( QKeyEvent *event )
{


    switch( event->key() )
    {

        case Qt::Key_H:
        {
            show_toolbar =!show_toolbar;
            qtoolbarFlow->setVisible( show_toolbar );
        } break;
        default:
            break;


    }

    update();
}
