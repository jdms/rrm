#include "FlowWindow.h"

FlowWindow::FlowWindow( QWidget *parent )
{
	setFocusPolicy(Qt::StrongFocus);

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

	canvas = new FlowVisualizationCanvas(this, this->getCurrentDirectory());
    canvas->setController( controller );
    canvas->setColorBar( &colorbar );


    hb_mainwindow = new QHBoxLayout( this );
    hb_mainwindow->addWidget( canvas );
    hb_mainwindow->addWidget( &colorbar );


    QWidget *wd_main = new QWidget( this );
    wd_main->setLayout( hb_mainwindow );


    this->setCentralWidget( wd_main );

    colorbar.setSize( canvas->height() , 25 );

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
    qbuildCornerPoint->setIcon(QIcon(":/images/icons/cornerpoint.png"));
    connect( qbuildCornerPoint, &QAction::triggered, this, &FlowWindow::buildCornerPoint );

    qbuildUnstructured = new QAction( "Unstructured", qtoolbarFlow );
    qbuildUnstructured->setIcon(QIcon(":/images/icons/unstructured.png"));
    connect( qbuildUnstructured, &QAction::triggered, this, &FlowWindow::buildUnstructured );


    qcomputeFlowProperties = new QAction( "Compute Properties", qtoolbarFlow );
    qcomputeFlowProperties->setIcon(QIcon(":/images/icons/computeproperties.png"));
    connect( qcomputeFlowProperties, &QAction::triggered, controller, &FlowVisualizationController::computeFlowProperties  );



    qshowMovingCrossSection = new QAction( "CrossSection", qtoolbarFlow );
    qshowMovingCrossSection->setIcon(QIcon(":/images/icons/clippingplane.png"));
    qshowMovingCrossSection->setCheckable( true );
    connect( qshowMovingCrossSection, &QAction::toggled, qdockcrosssectionnormalBar, &QDockWidget::setVisible );
	connect( qshowMovingCrossSection, &QAction::toggled, canvas, &FlowVisualizationCanvas::disableCrossSection );


    qexportsurface  = new QAction( "Unstructured Surface Mesh to VTK", qtoolbarFlow );
    connect( qexportsurface, SIGNAL( triggered(bool) ), controller, SLOT( exportSurfacetoVTK() ) );

    qexportvolume  = new QAction( "Unstructured Volume Mesh to VTK", qtoolbarFlow );
    connect( qexportvolume, SIGNAL( triggered(bool) ), controller, SLOT( exportVolumetoVTK() ) );

    qexportcornerpointVTK  = new QAction( "Corner-Point Grid to VTK", qtoolbarFlow );
    connect( qexportcornerpointVTK, SIGNAL( triggered(bool) ), controller, SLOT( exportCornerPointtoVTK() ) );

    qexportcornerpointGRDECL  = new QAction( "Corner-Point Grid to GRDECL", qtoolbarFlow );
    connect( qexportcornerpointGRDECL,SIGNAL( triggered(bool) ), controller, SLOT( exportCornerPointtoGRDECL() ) );

    qexportresults  = new QAction( "Results to VTK", qtoolbarFlow );
    connect( qexportresults, SIGNAL( triggered(bool) ), controller, SLOT( exportResultstoVTK() ) );


    mn_export = new QMenu( "Export", this );
    mn_export->addAction(qexportsurface);
    mn_export->addAction(qexportvolume);
    mn_export->addAction(qexportcornerpointVTK);
    mn_export->addAction(qexportcornerpointGRDECL);
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

    mn_colormaps = new QMenu ( tr ( "Colormaps" ) );
    tbn_colormaps = new QToolButton();
    tbn_colormaps->setIcon(QIcon(":/images/icons/colormap.png"));
    tbn_colormaps->setMenu( mn_colormaps );
    tbn_colormaps->setPopupMode( QToolButton::InstantPopup );


    ac_constant = new QAction( "Constant", qtoolbarFlow );
    ac_jet = new QAction( "Jet", qtoolbarFlow );
    ac_hot = new QAction( "Hot", qtoolbarFlow );
    ac_cool = new QAction( "Cool", qtoolbarFlow );
    ac_parula = new QAction( "Parula", qtoolbarFlow );
    ac_spring = new QAction( "Spring", qtoolbarFlow );
    ac_summer = new QAction( "Summer", qtoolbarFlow );
    ac_copper = new QAction( "Copper", qtoolbarFlow );
    ac_polar = new QAction( "Polar", qtoolbarFlow );
    ac_winter = new QAction( "Winter", qtoolbarFlow );


    QActionGroup* ag_colormaps = new QActionGroup( this );
    ag_colormaps->setExclusive( true );

    ag_colormaps->addAction( ac_constant );
    ag_colormaps->addAction( ac_jet );
    ag_colormaps->addAction( ac_hot );
    ag_colormaps->addAction( ac_cool );
    ag_colormaps->addAction( ac_parula );
    ag_colormaps->addAction( ac_spring );
    ag_colormaps->addAction( ac_summer );
    ag_colormaps->addAction( ac_copper );
    ag_colormaps->addAction( ac_polar );
    ag_colormaps->addAction( ac_winter );



    connect( ac_constant, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::CONSTANT ); } );
    connect( ac_jet, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::JET ); } );
    connect( ac_hot, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::HOT ); } );
    connect( ac_cool, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::COOL ); } );
    connect( ac_parula, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::PARULA ); } );

    connect( ac_spring, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::SPRING ); } );
    connect( ac_summer, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::SUMMER ); } );
    connect( ac_copper, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::COPPER ); } );
    connect( ac_polar, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::POLAR ); } );
    connect( ac_winter, &QAction::toggled, this, [=](){ canvas->setCurrentColormap( ColorMap::COLORMAP::WINTER ); } );


    mn_colormaps->addActions( ag_colormaps->actions() );


    ac_showregions = new QAction( "Show Pore Volumes", qtoolbarFlow );
    ac_showregions->setIcon(QIcon(":/images/icons/porevolume4.png"));
    ac_showregions->setCheckable( true );
    connect( ac_showregions, &QAction::toggled, &porevolumeform, &PoreVolumeResultsForm::setVisible );
	connect( ac_showregions, &QAction::toggled, canvas, &FlowVisualizationCanvas::showRegions );

    qclear = new QAction( "Clear", qtoolbarFlow );
    qclear->setIcon(QIcon(":/images/icons/clear.png"));
    connect( qclear, &QAction::triggered, this, &FlowWindow::clear );


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
    qtoolbarFlow->addAction( ac_showregions );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction( qshowMovingCrossSection );

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addWidget( tbn_colormaps );
    qtoolbarFlow->addWidget( tbn_export );
    qtoolbarFlow->addAction( qclear );

    qtoolbarFlow->addSeparator();

    addToolBar( qtoolbarFlow );
    qtoolbarFlow->setVisible( true );

}


void FlowWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    colorbar.setSize( canvas->height(), 25 );
}


void FlowWindow::createActions()
{


    connect( &parametersBar, SIGNAL( readParameterFile( const std::string& ) ), this, SLOT( readUserInputFile( const std::string& ) ) );
	// New Gui
    //connect( &parametersBar, &FlowParametersBar::closeBarandAccept, this, &FlowWindow::acceptUserParameters );
    connect( &parametersBar, SIGNAL( closeBar() ), qdockparametersBar, SLOT( close() ) );



    connect( controller, &FlowVisualizationController::showToolbar, qtoolbarFlow, &QToolBar::setVisible );
    connect( controller, &FlowVisualizationController::clearAll, this, &FlowWindow::clear );


    connect( controller, SIGNAL( clearPropertiesMenu() ), this, SLOT( clearPropertiesMenu() ) );
    connect( controller, SIGNAL( propertybyVertexComputed( std::string, std::string ) ), this, SLOT( addVertexProperty( std::string, std::string ) ) );
    connect( controller, SIGNAL( propertybyFaceComputed( std::string, std::string ) ), this, SLOT( addFaceProperty( std::string, std::string ) ) );



    connect( canvas, &FlowVisualizationCanvas::getSurfaceCrossSection, this, &FlowWindow::loadSurfacesfromSketch );
    connect( canvas, &FlowVisualizationCanvas::readSurfacefromFile, this, &FlowWindow::loadSurfacesfromFile ); 
    connect( canvas, &FlowVisualizationCanvas::buildcornerpoint, this, &FlowWindow::buildCornerPoint );
	connect(canvas, &FlowVisualizationCanvas::buildunstructured, this, [=]{ acceptUserParameters(), buildCornerPoint(); });
//	connect( canvas, &FlowVisualizationCanvas::computeFlowProperties, this, &FlowWindow::computeFlowProperties );


    qcomputeFlowProperties = new QAction( "Compute Properties", qtoolbarFlow );
    qcomputeFlowProperties->setIcon(QIcon(":/images/icons/computeproperties.png"));
    connect( qcomputeFlowProperties, &QAction::triggered, controller, &FlowVisualizationController::computeFlowProperties  );

	
	

    connect( canvas, &FlowVisualizationCanvas::editParameters, qdockparametersBar, &QDockWidget::show );
    connect( canvas, &FlowVisualizationCanvas::applyCrossSection, qdockcrosssectionnormalBar, &QDockWidget::show );
    connect( canvas, &FlowVisualizationCanvas::clearAll, this, &FlowWindow::clear );


    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, canvas, &FlowVisualizationCanvas::setCrossSectionNormalCoordinates ); 
	connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, qdockcrosssectionnormalBar, &QDockWidget::close );
    connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, qdockcrosssectionnormalBar, &QDockWidget::close );
	connect( &crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, this, [=](){ qshowMovingCrossSection->setChecked( false ); } );




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



QString FlowWindow::getCurrentDirectory()
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

    //canvas->setCurrentDirectory( current_dir.toStdString() );

	return current_dir;
}



void FlowWindow::loadSurfacesfromSketch()
{

    controller->clear();
    canvas->clear();

    std::vector< size_t > nu;
    std::vector< size_t > nv;
    std::vector< double > points;
    size_t num_extrusion_steps = 1;

	Eigen::Affine3f m2D_to_3D;
	Eigen::Affine3f m3D_to_2D;

    emit getLegacyMeshes( points, nu, nv, num_extrusion_steps );

//	emit get2Dto3DMatrix(m2D_to_3D);
//	emit get3Dto2DMatrix(m3D_to_2D);

//	controller->setScene2Dto3D(m2D_to_3D);
//	controller->setScene3Dto2D(m3D_to_2D);

    controller->setSkeletonData( points, nu, nv, num_extrusion_steps );
    canvas->updateMesh();


	std::cout << "-- FlowWindow --" << std::endl;
	std::cout << m2D_to_3D.matrix() << std::endl;
	std::cout << m3D_to_2D.matrix() << std::endl;

}


void FlowWindow::loadSurfacesfromFile()
{

    controller->clear();
    canvas->clear();

    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName( this, tr( "Open File" ), "./inputs/",
                                                     ".skt files (*.skt)", &selected_format );
    if( filename.isEmpty() == true ) return;



    QStringList path = filename.split( "\." );
    QString name_of_file = path[ 0 ];
    QString extension_of_file = path[ 1 ];

    type_of_file = extension_of_file.toStdString();

    if( extension_of_file.compare( "skt" ) == 0 ){

        file_of_mesh = filename.toStdString();
        controller->readSkeletonFiles( file_of_mesh );
        canvas->updateMesh();

    }

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
    std::vector< double > positions;
    std::vector< double > perm;
    std::vector< double > poros;
    std::vector< double > visc;


    controller->getPropertyArea( np, positions, perm, poros, visc );
    parametersBar.setPropertyAreaParameters( np, positions, perm, poros, visc );


    int nw = 0;
    std::vector< unsigned int > type;
    std::vector< double > values;
    std::vector< int > sign;


    controller->getWellsValues( nw, type, values, sign );
    parametersBar.setWellParameter( nw, type, values, sign );


}



void FlowWindow::acceptUserParameters()
{

    int np = 0;
    std::vector< double > positions;
    std::vector< double > perm;
    std::vector< double > poros;
    std::vector< double > visc;

    parametersBar.getPropertyAreaParameters(  np, positions , perm, poros, visc );
    controller->setPropertyArea( np, positions , perm, poros, visc );


    int nw = 0;
    std::vector< unsigned int > type;
    std::vector< double > values;
    std::vector< int > sign;

    parametersBar.getWellParameter( nw, type, values, sign  );
    controller->setWellsValues( nw, type, values, sign );

    qdockparametersBar->close();

}



void FlowWindow::buildCornerPoint()
{
    std::cout << "FlowWindow buildCornerPoint" << std::endl;

	controller->setCurrentMethod(FlowVisualizationController::MESHING_METHOD::CORNERPOINT);

	if (controller->isUserInputOk() == false)
	{
		acceptUserParameters();
	}

    controller->generateCornerPoint();
    canvas->updateCornerPoint();

}


void FlowWindow::buildUnstructured()
{

   std::cout << "FlowWindow buildUnstructured" << std::endl;

   controller->setCurrentMethod(FlowVisualizationController::MESHING_METHOD::UNSTRUCTURED);
  
   if (controller->isUserInputOk() == false)
   {
	   acceptUserParameters();
   }

   controller->generateUnstructured();
   canvas->updateVolumetricMesh();
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





void FlowWindow::clear()
{
    controller->clear();
    canvas->clear();
	clearPropertiesMenu();
    qexportcornerpointVTK->setEnabled( true );
    qexportcornerpointGRDECL->setEnabled( true );
    parametersBar.clear();
    crosssectionnormalBar.clear();
    qcomputeFlowProperties->setEnabled( true );
	reset();
}


void FlowWindow::clearPropertiesMenu()
{
	mn_vectorsproperties_byvertex.clear();
    rd_vectormethods_byvertex.clear();
    ac_vertex_property.clear();
	
    mn_coloring_byvertex->clear();
	
	
    mn_vectorsproperties_byface.clear();
    rd_vectormethods_byface.clear();
	
    ac_face_property.clear();
	
    mn_coloring_byfaces->clear();
}




void FlowWindow::keyPressEvent( QKeyEvent *event )
{


    switch( event->key() )
    {

        case Qt::Key_H:
        {
            qtoolbarFlow->setVisible( false );
        }
        break;
        case Qt::Key_T:
        {
            qtoolbarFlow->setVisible( true );
		}
		break;
		case Qt::Key_F5:
		{
			canvas->reloadShader();
			std::cout << "Reload Shader " << std::endl;
		}
        break;

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

}
