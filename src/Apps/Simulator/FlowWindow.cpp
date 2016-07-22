#include "FlowWindow.h"

FlowWindow::FlowWindow( QWidget *parent ) //: QDockWidget/*QMainWindow( parent )*/
{
    createWindow();
    createToolBar();
    createActions();
    getCurrentDirectory();

    reset();
}

void FlowWindow::createWindow()
{

    qdockparametersBar = new QDockWidget( this );
    qdockparametersBar->setAllowedAreas( Qt::LeftDockWidgetArea );
    qdockparametersBar->setWidget( &parametersBar );
    qdockparametersBar->setVisible( false );

    addDockWidget( Qt::LeftDockWidgetArea, qdockparametersBar );


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
    canvas = new FlowVisualizationCanvas( this );
    canvas->setController( controller );

    controller->setParent( this );
    controller->setCounterProgressinData();


    canvas->show();//setMinimumSize( 300, 300 );
    this->setCentralWidget( canvas );
}


void FlowWindow::createToolBar()
{


    qtoolbarFlow = new QToolBar();


    qreloadSurface = new QAction( "Surface from Sketch", qtoolbarFlow );
    qreloadSurface->setIcon(QIcon(":/images/icons/surfacesfromsketch.png"));
    connect( qreloadSurface, SIGNAL( triggered() ), this, SLOT( loadSurfacesfromSketch() ) );

    qoopenfilesDialog = new QAction( "Surface from File", qtoolbarFlow );
    qoopenfilesDialog->setIcon(QIcon(":/images/icons/surfacesfromfile.png"));
    connect( qoopenfilesDialog, SIGNAL( triggered(bool) ), this, SLOT( loadSurfacesfromFile() ) );

    qflowparametersDialog = new QAction( "User Input", qtoolbarFlow );
    qflowparametersDialog->setIcon(QIcon(":/images/icons/userinput.png"));
    connect( qflowparametersDialog, SIGNAL( triggered(bool) ), qdockparametersBar, SLOT( show() ) );


    qbuildCornerPoint = new QAction( "Corner Point", qtoolbarFlow );
    qbuildCornerPoint->setIcon(QIcon(":/images/icons/cornerpointgrid.png"));
    connect( qbuildCornerPoint, &QAction::triggered, this, [=](){ controller->generateCornerPoint(); qcomputeFlowProperties->setEnabled( false ); } );

    qbuildUnstructured = new QAction( "Unstructured", qtoolbarFlow );
    qbuildUnstructured->setIcon(QIcon(":/images/icons/unstructuredgrid.png"));
    connect( qbuildUnstructured, &QAction::triggered, this, [=](){ controller->generateUnstructured(); qcomputeFlowProperties->setEnabled( true ); } );


    qcomputeFlowProperties = new QAction( "Compute Properties", qtoolbarFlow );
    qcomputeFlowProperties->setIcon(QIcon(":/images/icons/computeproperties.png"));
    connect( qcomputeFlowProperties, &QAction::triggered, this, [=](){  //if( controller->isVolumetricOk() == false || controller->arePropertiesComputed() == true ) return;
                                                                        controller->computeFlowProperties(); }  );

    qshowMovingCrossSection = new QAction( "CrossSection", qtoolbarFlow );
    qshowMovingCrossSection->setIcon(QIcon(":/images/icons/clippingplane.png"));
    qshowMovingCrossSection->setCheckable( true );
    connect( qshowMovingCrossSection, &QAction::toggled, this, [=]( bool option ){ if( option == true )
                                                                                    qdockcrosssectionnormalBar->show();
                                                                                   else{
                                                                                    canvas->disableCrossSection();
                                                                                    qdockcrosssectionnormalBar->close();
                                                                                    } } );



    qexportsurface  = new QAction( "Surface to VTK", qtoolbarFlow );
    connect( qexportsurface, SIGNAL( triggered(bool) ), this, SLOT( exportSurfaceFile() ) );


    qexportvolume  = new QAction( "Volume to VTK", qtoolbarFlow );
    connect( qexportvolume, SIGNAL( triggered(bool) ), this, SLOT( exportVolumeFile() ) );


    qexportcornerpoint  = new QAction( "CornerPoint to VTK", qtoolbarFlow );
    connect( qexportcornerpoint, SIGNAL( triggered(bool) ), this, SLOT( exportCornerPointFile() ) );

    qexportresults  = new QAction( "Results to VTK", qtoolbarFlow );
    connect( qexportresults, SIGNAL( triggered(bool) ), this, SLOT( exportResultstoFile() ) );


    mn_export = new QMenu( "Export", this );
    mn_export->addAction(qexportsurface);
    mn_export->addAction(qexportvolume);
    mn_export->addAction(qexportcornerpoint);
    mn_export->addAction(qexportresults);

    tbn_export = new QToolButton();
    tbn_export->setIcon(QIcon(":/images/icons/document_export.png"));
    tbn_export->setMenu( mn_export );
    tbn_export->setPopupMode( QToolButton::InstantPopup );



    mn_coloring_byvertex = new QMenu ( tr ( "Vertex Properties" ) );
    tbn_coloringbyvertex = new QToolButton();
    tbn_coloringbyvertex->setIcon(QIcon(":/images/icons/byvertex.png"));
    tbn_coloringbyvertex->setMenu( mn_coloring_byvertex );
    tbn_coloringbyvertex->setPopupMode( QToolButton::InstantPopup );


    mn_coloring_byfaces = new QMenu ( tr ( "Faces Properties" ) );
    tbn_coloringbyface = new QToolButton();
    tbn_coloringbyface->setIcon(QIcon(":/images/icons/byface.png"));
    tbn_coloringbyface->setMenu( mn_coloring_byfaces );
    tbn_coloringbyface->setPopupMode( QToolButton::InstantPopup );




    qclear = new QAction( "Clear", qtoolbarFlow );
    qclear->setIcon(QIcon(":/images/icons/clear.png"));
    connect( qclear, &QAction::triggered, this, [=](){ controller->clear(); canvas->clear(); qexportcornerpoint->setEnabled( true ); parametersBar.clear(); } );


    qhelp = new QAction( "Help", qtoolbarFlow );
    qhelp->setIcon(QIcon(":/images/icons/information.png"));
    connect( qhelp, &QAction::triggered, this, [=](){ help.show(); } );

    qtoolbarFlow->addAction( qreloadSurface );
    qtoolbarFlow->addAction( qoopenfilesDialog );
    qtoolbarFlow->addAction( qflowparametersDialog );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qbuildCornerPoint );
    qtoolbarFlow->addAction( qbuildUnstructured );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qcomputeFlowProperties );
    qtoolbarFlow->addWidget( tbn_coloringbyvertex );
    qtoolbarFlow->addWidget( tbn_coloringbyface );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qshowMovingCrossSection );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addWidget( tbn_export );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qclear );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qhelp );

    addToolBar( qtoolbarFlow );
    qtoolbarFlow->setVisible( true );



}


void FlowWindow::createActions()
{


    connect( &parametersBar, SIGNAL(readParameterFile( const std::string& ) ), this, SLOT( readUserInputFile( const std::string& ) ) );


    connect( &parametersBar, SIGNAL( sendPropertyArea(  const int& , const std::vector< double >& ) ), controller, SLOT( setPropertyArea(  const int&, const std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendBoundariesValues(  int, std::vector< double >& ) ), controller, SLOT( setBoundariesValues(  int, std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendWellsValues(  int, std::vector< double >& ) ), controller, SLOT( setWellsValues(  int, std::vector< double >& ) ) );

    connect( &parametersBar, SIGNAL( sendTOFBoundaryValues( int, std::vector< double >&  ) ), controller, SLOT( setTofBoundaryValues( int, std::vector< double >& ) ) );
    connect( &parametersBar, SIGNAL( sendTrBoundaryValues(  int, std::vector< double >& ) ), controller, SLOT( setTrBoundaryValues(  int, std::vector< double >& ) ) );


    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, this, [=]( float X, float Y, float Z ){ canvas->setCrossSectionNormalCoordinates( X, Y, Z ); qdockcrosssectionnormalBar->close(); } );
    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, this, [=](){ qdockcrosssectionnormalBar->close();  qshowMovingCrossSection->setChecked( false ); canvas->disableCrossSection(); } );

    connect( &parametersBar, SIGNAL( closeBar() ), qdockparametersBar, SLOT( close() ) );


    connect( controller, SIGNAL( updateMesh() ), canvas, SLOT( updateMesh() ) );
    connect( controller, SIGNAL( updateVolumetricMesh() ), canvas, SLOT( updateVolumetricMesh() ) );
    connect( controller, SIGNAL( updatePolyMesh() ), canvas, SLOT( updateMeshfromFile() ) );
    connect( controller, SIGNAL( updateCornerPoint() ), canvas, SLOT( updateCornerPoint() ) );


    connect( canvas, &FlowVisualizationCanvas::getSurfaceCrossSection, this, [=](){ emit getCrossSection(); } );
    connect( canvas, &FlowVisualizationCanvas::readSurfacefromFile, this, [=](){ loadSurfacesfromFile(); } );
    connect( canvas, &FlowVisualizationCanvas::buildcornerpoint, this, [=](){ controller->generateCornerPoint(); qcomputeFlowProperties->setEnabled( false ); } );
    connect( canvas, &FlowVisualizationCanvas::buildunstructured, this, [=](){ controller->generateUnstructured(); qcomputeFlowProperties->setEnabled( true ); } );



    connect( canvas, SIGNAL( editParameters() ), qdockparametersBar, SLOT( show() ) );
    connect( canvas, SIGNAL( applyCrossSection() ), qdockcrosssectionnormalBar, SLOT( show() ) );


	connect(controller, &FlowVisualizationController::hideToolbar, this, [=](){ show_toolbar = !show_toolbar; qtoolbarFlow->setVisible(show_toolbar);  });

    connect( controller, &FlowVisualizationController::clearAll, this, [=](){
        controller->clear(); canvas->clear(); qexportcornerpoint->setEnabled( true );
    } );


    connect( controller, SIGNAL( updateMesh(  const Mesh::TYPE&, const std::vector< double >&, const std::vector< unsigned int >&  ) ), canvas, SLOT( updateMesh(  const Mesh::TYPE&, const std::vector< double >&, const std::vector< unsigned int >&  ) ) );

    connect( &resolutionmeshBar, &DialogMeshVisualizationParameters::sendVisualizationParameters, this, [=]( const std::string& cmd, const std::string& method, const float &value ) {

        if( method.compare( "PARTITION" ) == 0 )
            controller->setMeshVisualizationParameters( cmd, 1, value, -1 );
        else if( method.compare( "EDGELENGHT" ) == 0 )
            controller->setMeshVisualizationParameters( cmd, 2, -1, value );

        qdockresolutionmesh->close();


    } );

    connect( &resolutionmeshBar, SIGNAL( closeBar() ), qdockresolutionmesh, SLOT( close() ) );


    connect( controller, SIGNAL( propertybyVertexComputed( std::string, std::string ) ), this, SLOT( addVertexProperty( std::string, std::string ) ) );
    connect( controller, SIGNAL( propertybyFaceComputed( std::string, std::string ) ), this, SLOT( addFaceProperty( std::string, std::string ) ) );

    connect( controller, SIGNAL(updateColors( const std::vector< float >& ) ), canvas, SLOT( setColors( const std::vector< float >& ) ) );

}


void FlowWindow::addVertexProperty( std::string name, std::string dimension )
{

    if( dimension.compare( "VECTOR" ) == 0 )
    {
        int id = mn_vectorsproperties_byvertex.size();

        mn_vectorsproperties_byvertex.push_back( new QMenu( tr( name.c_str() ) ) );

        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "X" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Y" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Z" ) ) );
        rd_vectormethods_byvertex.push_back ( new QRadioButton ( tr ( "Magnitude" ) ) );

        QVBoxLayout *vb_layout = new QVBoxLayout ( this );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 0 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 1 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 2 ] );
        vb_layout->addWidget ( rd_vectormethods_byvertex[ 4*id + 3 ] );

        QGroupBox *gb_vectormethods = new QGroupBox ();
        gb_vectormethods->setFlat ( true );
        gb_vectormethods->setLayout ( vb_layout );

        wa_vectormethods_byvertex = new QWidgetAction ( this );
        wa_vectormethods_byvertex->setDefaultWidget ( gb_vectormethods );


        mn_vectorsproperties_byvertex[ id ]->addAction( wa_vectormethods_byvertex );


        connect ( rd_vectormethods_byvertex[ 4*id + 0 ] , &QRadioButton::clicked, [=](){ canvas->setVerticesColorbyProperty( name.c_str(), "COORDX" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 1 ] , &QRadioButton::clicked, [=](){ canvas->setVerticesColorbyProperty( name.c_str(), "COORDY" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 2 ] , &QRadioButton::clicked, [=](){ canvas->setVerticesColorbyProperty( name.c_str(), "COORDZ" ); } );
        connect ( rd_vectormethods_byvertex[ 4*id + 3 ] , &QRadioButton::clicked, [=](){ canvas->setVerticesColorbyProperty( name.c_str(), "LENGTH" ); } );

        mn_coloring_byvertex->addMenu( mn_vectorsproperties_byvertex[ id ] );

    }
    else if( dimension.compare( "SCALAR" ) == 0 )
    {
        ac_vertex_property.push_back( new QAction( tr( name.c_str() ) , this ) );
        mn_coloring_byvertex->addAction( ac_vertex_property.back() );

        connect ( ac_vertex_property.back() , &QAction::triggered ,[=](){ canvas->setVerticesColorbyProperty( name.c_str() ); } );


    }



}


void FlowWindow::addFaceProperty( std::string name, std::string dimension )
{

    if( dimension.compare( "VECTOR" ) == 0 )
    {
        int id = mn_vectorsproperties_byface.size();

        mn_vectorsproperties_byface.push_back( new QMenu( tr( name.c_str() ) ) );

        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "X" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Y" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Z" ) ) );
        rd_vectormethods_byface.push_back ( new QRadioButton ( tr ( "Magnitude" ) ) );

        QVBoxLayout *vb_layout = new QVBoxLayout ( this );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 0 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 1 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 2 ] );
        vb_layout->addWidget ( rd_vectormethods_byface[ 4*id + 3 ] );

        QGroupBox *gb_vectormethods = new QGroupBox ();
        gb_vectormethods->setFlat ( true );
        gb_vectormethods->setLayout ( vb_layout );

        wa_vectormethods_byface = new QWidgetAction ( this );
        wa_vectormethods_byface->setDefaultWidget ( gb_vectormethods );


        mn_vectorsproperties_byface[ id ]->addAction( wa_vectormethods_byface );


        connect ( rd_vectormethods_byface[ 4*id + 0 ] , &QRadioButton::clicked, [=](){ canvas->setFacesColorbyProperty( name.c_str(), "COORDX" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 1 ] , &QRadioButton::clicked, [=](){ canvas->setFacesColorbyProperty( name.c_str(), "COORDY" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 2 ] , &QRadioButton::clicked, [=](){ canvas->setFacesColorbyProperty( name.c_str(), "COORDZ" ); } );
        connect ( rd_vectormethods_byface[ 4*id + 3 ] , &QRadioButton::clicked, [=](){ canvas->setFacesColorbyProperty( name.c_str(), "LENGTH" ); } );

        mn_coloring_byfaces->addMenu( mn_vectorsproperties_byface[ id ] );

    }
    else if( dimension.compare( "SCALAR" ) == 0 )
    {
        ac_face_property.push_back( new QAction( tr( name.c_str() ) , this ) );
        mn_coloring_byfaces->addAction( ac_face_property.back() );

        connect ( ac_face_property.back() , &QAction::triggered ,[=](){ canvas->setFacesColorbyProperty( name.c_str() ); } );


    }


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

    canvas->setCurrentDirectory( current_dir.toStdString() );


}



void FlowWindow::loadSurfacesfromSketch()
{

//    controller->clear();
    emit getCrossSection();

}


void FlowWindow::loadSurfacesfromFile()
{

    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".skt files (*.skt)", &selected_format );
    if( filename.isEmpty() == true ) return;




    QStringList listofnames = filename.split( "\." );
    QString name_of_file = listofnames[ 0 ];
    QString extension_of_file = listofnames[ 1 ];

    type_of_file = extension_of_file.toStdString();

    if( extension_of_file.compare( "poly" ) == 0 )
    {


        file_of_mesh = name_of_file.toStdString();
        controller->readPolyFiles( file_of_mesh );


    }
    else if( extension_of_file.compare( "skt" ) == 0 ){

        file_of_mesh = filename.toStdString();
        controller->readSkeletonFiles( file_of_mesh );

    }

}



void FlowWindow::setCrossSection( const RRM::CrossSection<qreal>& c ){

    controller->getSurfacesFromCrossSection( c );

}


void FlowWindow::readUserInputFile( const std::string& input_file )
{


    controller->readInputParameters( input_file  );
    updateParameterFields();

    file_of_parameters = input_file;

}


void FlowWindow::updateParameterFields()
{

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



void FlowWindow::exportResultstoFile()
{
    QString selected_format = "";
    QString filename = QFileDialog::getSaveFileName( this, tr( "Export File" ), "./results/",
                                                     ".vtk files (*.vtk)", &selected_format );
    if( filename.isEmpty() == true ) return;

    controller->exportResultstoVTK( filename.toStdString() );
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

void FlowWindow::reset()
{
    file_of_parameters.clear();
    file_of_mesh.clear();
    type_of_file.clear();

    show_toolbar = false;
}
