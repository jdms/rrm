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

FlowWindow::FlowWindow(QWidget *parent) : QMainWindow(parent)
{
    setFocusPolicy(Qt::StrongFocus);

    createWindow();
    createToolBar();
    createActions();
    getCurrentDirectory();

    reset();
    this->are_regionsdefined = false;
    	/// Initinal stage
	emit fluid_parameters_->setSinglePhase();
	

    /// FIXME June 2017. It tells when a new model is build. Used to keep well position
    this->bounding_box_changed_ = true;
}

void FlowWindow::createWindow()
{
	flowModule_file_ = this->menuBar()->addMenu(tr("&File"));
    flowModule_view_ = this->menuBar()->addMenu(tr("&View"));
    flowModule_diagnostic_  = this->menuBar()->addMenu(tr("&Flow Diagnostic"));


	crosssectionNormalWidget = new NormalMovableCrossSectionFlow();

    dockWidget_CrosssectioNormal = new QDockWidget(this);
	dockWidget_CrosssectioNormal->setAllowedAreas(Qt::LeftDockWidgetArea);
	dockWidget_CrosssectioNormal->setWidget(crosssectionNormalWidget);
	dockWidget_CrosssectioNormal->setVisible(false);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget_CrosssectioNormal);


    statusbar = new QStatusBar();
    this->setStatusBar(statusbar);

    controller = new FlowVisualizationController(this);
    controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);

    canvas = new FlowVisualizationCanvas(this, this->getCurrentDirectory());
    canvas->setController(controller);

    createWellModule();
    createRegionModule();
    createFluidModule();

	save_ = new QAction(tr("&Save"));
	open_ = new QAction(tr("&Open"));

	flowModule_file_->addAction(save_);
	flowModule_file_->addAction(open_);

    flow_tab_widget_ = new QTabWidget(this);
    flow_tab_widget_->addTab(canvas, "3D Visualization");
    //flow_tab_widget_->addTab(this->well_canvas_, "Well Module");

    /// Remove this from contructor prevent the warning menssage
    horizontalLayout_mainwindow = new QHBoxLayout();
    //hb_mainwindow->addWidget( canvas );
	horizontalLayout_mainwindow->addWidget(flow_tab_widget_);
	horizontalLayout_mainwindow->addWidget(&colorbar);

    QWidget * centralWidget = new QWidget(this);
	centralWidget->setLayout(horizontalLayout_mainwindow);

	this->setCentralWidget(centralWidget);

    colorbar.setSize(canvas->height(), 25);
}

void FlowWindow::createToolBar()
{

    toolBar_flowDiagnostic = new QToolBar();

	action_reloadSurface = new QAction(tr("Get Extruded Surface"), toolBar_flowDiagnostic);
//    qreloadSurface->setIcon(QIcon(":/images/icons/surfacesfromsketch.png"));
	action_reloadSurface->setVisible(false);

	action_reloadSurface1 = new QAction(tr("Get General Surface"), toolBar_flowDiagnostic);
//    qreloadSurface1->setIcon(QIcon(":/images/icons/surfacesfromsketch.png"));
	action_reloadSurface1->setVisible(false);

	action_openFilesDialog = new QAction(tr("Surface from File"), toolBar_flowDiagnostic);
	action_openFilesDialog->setIcon(QIcon(":/images/icons/surfacesfromfile.png"));
	action_openFilesDialog->setVisible(false);


	action_flowParametersDialog = new QAction(tr("User Input"), toolBar_flowDiagnostic);
	action_flowParametersDialog->setIcon(QIcon(":/images/icons/userinput.png"));

	action_buildCornerPoint = new QAction(tr("Corner Point"), toolBar_flowDiagnostic);
	action_buildCornerPoint->setIcon(QIcon(":/images/icons/cpgridmesh.png"));
	action_buildCornerPoint->setEnabled(false);
	action_buildCornerPoint->setVisible(false);
    //qbuildCornerPoint->setToolTip("<h5><b><font color='red'>Warnning !</font></b></h5>"
    //							  "<h5><b><font color='black'>Corner Point can be only from simple geometry.</font></b></h5>");

	action_buildUnstructured = new QAction(tr("Unstructured"), toolBar_flowDiagnostic);
	action_buildUnstructured->setIcon(QIcon(":/images/icons/unstructural.png"));
	action_buildUnstructured->setEnabled(false);


	action_computeFlowProperties = new QAction(tr("Compute Properties"), toolBar_flowDiagnostic);
	action_computeFlowProperties->setIcon(QIcon(":/images/icons/computeproperties.png"));
	action_computeFlowProperties->setEnabled(false);



	action_showMovingCrossSection = new QAction(tr("CrossSection"), toolBar_flowDiagnostic);
	action_showMovingCrossSection->setIcon(QIcon(":/images/icons/cross.png"));
	action_showMovingCrossSection->setCheckable(true);


    /// Properties

    toolButton_coloringByVertex = new QToolButton();
	toolButton_coloringByVertex->setIcon(QIcon(":/images/icons/vertex.png"));

    /// OpenVolume Mesh Integration ---------------------------------------------->
    menu_vertex_properties = new QMenu(tr("OVM Vertex Properties"));
	toolButton_coloringByVertex->setMenu(menu_vertex_properties);
	toolButton_coloringByVertex->setPopupMode(QToolButton::InstantPopup);
	toolButton_coloringByVertex->setEnabled(false);


    toolButton_coloringByFace = new QToolButton();
	toolButton_coloringByFace->setIcon(QIcon(":/images/icons/properties.png"));

    /// OpenVolume Mesh Integration ---------------------------------------------->
    menu_cell_properties = new QMenu(tr("OVM Cell Properties"));
	toolButton_coloringByFace->setMenu(menu_cell_properties);
	toolButton_coloringByFace->setPopupMode(QToolButton::InstantPopup);
	toolButton_coloringByFace->setEnabled(false);

	toolButton_colormaps = new QToolButton();
	menu_colormaps = new QMenu(tr("Colormaps"));
	toolButton_colormaps->setIcon(QIcon(":/images/icons/colormap.png"));			
	toolButton_colormaps->setMenu(menu_colormaps);
	toolButton_colormaps->setPopupMode(QToolButton::InstantPopup);

	QActionGroup* actionGroup_colormaps = new QActionGroup(menu_colormaps);
	actionGroup_colormaps->setExclusive(true);

	action_constant = new QAction(tr("Constant"), actionGroup_colormaps);
    action_constant->setCheckable(true);
    action_constant->setChecked(true);
	action_cool_to_warm = new QAction(tr("Cool to Warm"), actionGroup_colormaps);
    action_cool_to_warm->setCheckable(true);
	action_hot = new QAction(tr("Hot"), actionGroup_colormaps);
    action_hot->setCheckable(true);
	action_cool = new QAction(tr("Cool"), actionGroup_colormaps);
    action_cool->setCheckable(true);
    //action_parula = new QAction(tr("Parula"), actionGroup_colormaps);
    //action_parula->setCheckable(true);
	action_spring = new QAction(tr("Spring"), actionGroup_colormaps);
    action_spring->setCheckable(true);
	action_summer = new QAction(tr("Summer"), actionGroup_colormaps);
    action_summer->setCheckable(true);
	action_copper = new QAction(tr("Copper"), actionGroup_colormaps);
    action_copper->setCheckable(true);
	action_polar = new QAction(tr("Polar"), actionGroup_colormaps);
    action_polar->setCheckable(true);
	action_winter = new QAction(tr("Winter"), actionGroup_colormaps);
    action_winter->setCheckable(true);
	action_jet = new QAction(tr("Jet"), actionGroup_colormaps);
    action_jet->setCheckable(true);

	menu_colormaps->addActions(actionGroup_colormaps->actions());

    action_showRegions = new QAction(tr("Show Pore Volumes"), toolBar_flowDiagnostic);
    action_showRegions->setIcon(QIcon(":/images/icons/porus.png"));
    action_showRegions->setCheckable(true);
    //connect( action_showregions, &QAction::toggled, &porevolumeform, &PoreVolumeResultsForm::setVisible );

    action_showRegions->setEnabled(false);

	/// @FiXME January 2018

	action_clear = new QAction(tr("Clear"), toolBar_flowDiagnostic);
	action_clear->setIcon(QIcon(":/images/icons/clear.png"));
	action_clear->setEnabled(true);
	action_clear->setVisible(true);

	action_clearComputedQuantities_ = new QAction(tr("Clear Computed Quantities"), toolBar_flowDiagnostic);
	action_clearComputedQuantities_->setIcon(QIcon(":/images/icons/refresh.png"));
	action_clearComputedQuantities_->setEnabled(false);
	action_clearComputedQuantities_->setVisible(true);

    /// ToolBar setUp

	toolBar_flowDiagnostic->addAction(action_reloadSurface);
	toolBar_flowDiagnostic->addAction(action_reloadSurface1);
	toolBar_flowDiagnostic->addAction(action_openFilesDialog);
	//toolBar_flowDiagnostic_->addAction(action_flowParametersDialog);

	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addAction(action_buildCornerPoint);
	toolBar_flowDiagnostic->addAction(action_buildUnstructured);

	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addAction(action_computeFlowProperties);
	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addAction(action_clearComputedQuantities_);
	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addWidget(toolButton_coloringByVertex);
	toolBar_flowDiagnostic->addWidget(toolButton_coloringByFace);
	toolBar_flowDiagnostic->addAction(action_showRegions);

	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addAction(action_showMovingCrossSection);

	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addWidget(toolButton_colormaps);
    /// FIXME June
	toolBar_flowDiagnostic->addSeparator();
	toolBar_flowDiagnostic->addAction(action_clear);

	/// Menu - Flow Diagnostic

	/// Exporters
	// @FIXME June 2017

	action_upscalledPermeability_ = new QAction(tr("Get Upscaled Permeability"), toolBar_flowDiagnostic);
	action_upscalledPermeability_->setEnabled(false);
	action_oilInPlace_ = new QAction(tr("Get Oil in Place"), toolBar_flowDiagnostic);
	action_oilInPlace_->setEnabled(false);

	action_exportDerivedQuantities_ = new QAction(tr("Flow Diagnostics Quantities"), toolBar_flowDiagnostic);
	action_exportSurface = new QAction(tr("Unstructured Surface Mesh to VTK"), toolBar_flowDiagnostic);
	action_exportVolume = new QAction(tr("Unstructured Volume Mesh to VTK"), toolBar_flowDiagnostic);
	action_exportCornerpointVTK = new QAction(tr("Corner-Point Grid to VTK"), toolBar_flowDiagnostic);
	action_exportCornerpointGRDECL = new QAction(tr("Corner-Point Grid to GRDECL"), toolBar_flowDiagnostic);
	action_exportResults = new QAction(tr("Results *.vtk"), toolBar_flowDiagnostic);
	action_exportMesh = new QAction(tr("Mesh *.msh"), toolBar_flowDiagnostic);

	menu_export = new QMenu(tr("Export"), this);
	//menu_export_->addAction(qexportsurface);
	//menu_export_->addAction(qexportvolume);
	//menu_export_->addAction(qexportcornerpointVTK);
	//menu_export_->addAction(qexportcornerpointGRDECL);
	menu_export->addAction(action_exportResults);
	menu_export->addAction(action_exportDerivedQuantities_);
	menu_export->addAction(action_exportMesh);
	menu_export->setEnabled(false);

    flowModule_diagnostic_->addAction(action_upscalledPermeability_);
	flowModule_diagnostic_->addAction(action_oilInPlace_);
	flowModule_diagnostic_->addMenu(menu_export);



	addToolBar(toolBar_flowDiagnostic);
	toolBar_flowDiagnostic->setVisible(true);
}

void FlowWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    colorbar.setSize(canvas->height(), 25);
}

void FlowWindow::createActions()
{


	connect(action_reloadSurface, &QAction::triggered, this, [=]()
	{
		action_computeFlowProperties->setEnabled(false);
		action_showRegions->setEnabled(false);
		action_upscalledPermeability_->setEnabled(false);
		toolButton_coloringByVertex->setEnabled(false);
		toolButton_coloringByFace->setEnabled(false);

		this->loadSurfacesfromSketch();

		if (are_regionsdefined == true)
		{
			action_buildCornerPoint->setEnabled(true);
			action_buildUnstructured->setEnabled(true);
		}

	});
	
	connect(action_buildUnstructured, &QAction::triggered, this, [=]()
	{
		/// Now we can Compute and see region Identifications
		/// @FIXME catch execption from HWU mesh generator
		this->buildUnstructured();
		action_computeFlowProperties->setEnabled(true);
		action_showRegions->setEnabled(false);
		action_buildCornerPoint->setEnabled(false);
		action_buildUnstructured->setEnabled(false);
		action_upscalledPermeability_->setEnabled(true);
		is_cornerpoint = false;

	});

	connect(action_computeFlowProperties, &QAction::triggered, controller, [=]()
	{

		/// @TODO Fixed Janurary
		setParametersBeforeSimulation();
		/// Now we can visualize the properties and export
		/// @FIXME catch execption from HWU mesh generator
		controller->computeFlowProperties();
		action_computeFlowProperties->setEnabled(false);
		toolButton_coloringByVertex->setEnabled(true);
		toolButton_coloringByFace->setEnabled(true);
		/// Export Properties
		menu_export->setEnabled(true);
		/// Set Default ColorMap
		action_cool_to_warm->setChecked(true);
		controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);
		action_oilInPlace_->setEnabled(true);

			this->updatePropertyAction();
			current_property_ = RRM::PropertyProfile("Pressure (bar)", "Vertex","", "Scalar");
			updateModelColors(current_property_);
			action_showRegions->setEnabled(true);
			controller->getPoreVolume();
			action_clearComputedQuantities_->setEnabled(true);
	});

	connect(action_showRegions, &QAction::toggled, [=](bool is_toggled)
	{
		if (is_toggled)
		{
			//current_property_ = RRM::PropertyProfile("Tetrahedron", "Pore Volume", "CProp", "", "double");
			updateModelColors(current_property_);
			canvas->showRegions();
		}

	});
	/// Exporters
	/// @TODO June 2017
	connect(action_exportDerivedQuantities_, SIGNAL(triggered(bool)), controller, SLOT(exportDerivedQuantities()));

	connect(action_clear, &QAction::triggered, [=]()
	{
		this->region_parameters_->reset();
		this->fluid_parameters_->reset();
		this->well_parameters_->reset();

		loadSurfacesfromSketch();

		action_buildUnstructured->setEnabled(true);
	});


	connect(action_clearComputedQuantities_, &QAction::triggered, [=]()
	{
		controller->clearComputedQuantities();

		// No results to show
		menu_export->setEnabled(false);
		toolButton_coloringByVertex->setEnabled(false);
		toolButton_coloringByFace->setEnabled(false);
		action_showRegions->setEnabled(false);
		action_computeFlowProperties->setEnabled(true);
		canvas->setDefaultColor();
	});
	connect(action_upscalledPermeability_, &QAction::triggered, [=]()
	{
		std::string result;

		/// @TODO Fixed Janurary
		setParametersBeforeSimulation();
		controller->getUpscalledPermeability(result);
		up_scaled_.setText(QString::fromStdString(result));
		up_scaled_.show();
	}
	);



	connect(action_oilInPlace_, &QAction::triggered, [=]()
	{
		std::string result;
        controller->getOilInPlace(result);
		result = result + " stb ";
        up_scaled_.setText(QString::fromStdString(result));
        up_scaled_.show();
    }
    );

    connect(action_exportSurface, SIGNAL(triggered(bool)), controller, SLOT(exportSurfacetoVTK()));
    connect(action_exportVolume, SIGNAL(triggered(bool)), controller, SLOT(exportVolumetoVTK()));
    connect(action_exportCornerpointVTK, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoVTK()));
    connect(action_exportCornerpointGRDECL, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoGRDECL()));
    //connect(qexportcornerpointGRDECL, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoGRDECL()));
	/// January 2018
	connect(action_exportMesh, SIGNAL(triggered(bool)), controller, SLOT(exportMeshtoMSH()));
    connect(action_exportResults, SIGNAL(triggered(bool)), controller, SLOT(exportResultstoVTK()));
    // CrossSection
    connect(action_showMovingCrossSection, &QAction::toggled, dockWidget_CrosssectioNormal, &QDockWidget::setVisible);
    connect(action_showMovingCrossSection, &QAction::toggled, canvas, &FlowVisualizationCanvas::disableCrossSection);
    /// ColorMap
    connect(action_constant, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::CONSTANT);
    });
    connect(action_cool_to_warm, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);
        updateModelColors(current_property_);
    });
    connect(action_hot, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::HOT);
        updateModelColors(current_property_);
    });
    connect(action_cool, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::COOL);
        updateModelColors(current_property_);
    });
    //connect(action_parula, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::PARULA);
    //	updateModelColors(current_property_);
    //});
    connect(action_spring, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::SPRING);
        updateModelColors(current_property_);
    });
    connect(action_summer, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::SUMMER);
        updateModelColors(current_property_);
    });
    connect(action_copper, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::COPPER);
        updateModelColors(current_property_);
    });
    connect(action_polar, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::POLAR);
        updateModelColors(current_property_);
    });
    connect(action_winter, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::WINTER);
        updateModelColors(current_property_);
    });
    connect(action_jet, &QAction::triggered, this, [=](){ controller->setCurrentColormap(ColorMap::COLORMAP::JET);
        updateModelColors(current_property_);
    });


	connect(controller, &FlowVisualizationController::showToolbar, toolBar_flowDiagnostic, &QToolBar::setVisible);
    connect(controller, &FlowVisualizationController::clearAll, this, &FlowWindow::clear);


    connect(controller, SIGNAL(clearPropertiesMenu()), this, SLOT(clearPropertiesMenu()));

    connect(canvas, &FlowVisualizationCanvas::applyCrossSection, dockWidget_CrosssectioNormal, &QDockWidget::show);
    connect(canvas, &FlowVisualizationCanvas::clearAll, this, &FlowWindow::clear);


    connect(crosssectionNormalWidget, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, canvas, &FlowVisualizationCanvas::setCrossSectionNormalCoordinates);
	connect(crosssectionNormalWidget, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, dockWidget_CrosssectioNormal, &QDockWidget::close);
	connect(crosssectionNormalWidget, &NormalMovableCrossSectionFlow::canceled, dockWidget_CrosssectioNormal, &QDockWidget::close);
	connect(crosssectionNormalWidget, &NormalMovableCrossSectionFlow::canceled, this, [=](){ action_showMovingCrossSection->setChecked(false); });

    //Region Signal
    connect(region_parameters_, &RRM::RegionWidget::numberOfRegions, this, [=](int _number_of_regions){ emit sendNumberOfRegions(_number_of_regions); });


    connect( fluid_parameters_, &RRM::FluidWidget::setSinglePhase, this, [=]( )
    {
		controller->setSinglePhase( );
		region_parameters_->setSinglePhase();
	});

	connect(fluid_parameters_, &RRM::FluidWidget::setMultiPhase, this, [=]()
	{
		///  Not used
	});

    connect( fluid_parameters_, &RRM::FluidWidget::setSaturationPerRegion, this, [=]()
    {
        controller->setSaturationMethod( FlowVisualizationController::SaturationMethod::PERREGION );
		region_parameters_->setMultiPhaseByRegionSaturation();
    } );

    connect( fluid_parameters_, &RRM::FluidWidget::setAPIGravity, this, [=]()
    {
        controller->setSaturationMethod( FlowVisualizationController::SaturationMethod::APIGRAVITY );
		region_parameters_->setMultiPhaseByDensity();
    } );

	connect(save_, &QAction::triggered, this, [=]()
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Open JSON file"), "", tr("JSON file (*.txt *.rrm)"));
		
		QFile saveFile(fileName);

		if (!saveFile.open(QIODevice::WriteOnly))
		{
			qWarning("Couldn't open save file.");
			
		}

		QJsonObject flowDiagnosticData;
		/// Save Petrophysic Data
		QJsonObject petrophysicObject;
		region_parameters_->write(petrophysicObject);
		flowDiagnosticData["petrophysicData"] = petrophysicObject;

		/// Save Fluid Data
		QJsonObject fluidObject;
		fluid_parameters_->write(fluidObject);
		flowDiagnosticData["fluidData"] = fluidObject;
		/// Save Well  Data
		QJsonObject wellObject;
		well_parameters_->write(wellObject);
		flowDiagnosticData["wellData"] = wellObject;

		QJsonDocument saveDocument(flowDiagnosticData);
		saveFile.write(saveDocument.toJson());
	});

	connect(open_, &QAction::triggered, this, [=]()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open JSON file"), "", tr("JSON file (*.txt *.rrm)"));

		QFile loadFile(fileName);

		if (!loadFile.open(QIODevice::ReadOnly))
		{
			qWarning("Couldn't open save file.");		
		}

		QByteArray saveData = loadFile.readAll();

		QJsonDocument loadDocument(QJsonDocument::fromJson(saveData));

		/// @see http://erickveil.github.io/2016/04/06/How-To-Manipulate-JSON-With-C++-and-Qt.html
		if (!loadDocument.isObject())
		{
			qDebug() << "JSON is not an object.";
	
		}

		/// Read Petrophysics data
		if (loadDocument.object().contains("petrophysicData"))
		{
			region_parameters_->read(loadDocument.object()["petrophysicData"].toObject());
		}
		else
		{
			qWarning("No Petrophysic Data.");
		}

		/// Read Fluid module
		if (loadDocument.object().contains("fluidData"))
		{
			fluid_parameters_->read(loadDocument.object()["fluidData"].toObject());
		}
		else
		{
			qWarning("No Fluid Data.");
		}
		
		/// Read Well module
		if (loadDocument.object().contains("wellData"))
		{
			well_parameters_->read(loadDocument.object()["wellData"].toObject());
			well_scene_->loadWells(well_parameters_->getNumberOfWells(), well_parameters_->getWellsSign(), well_parameters_->getDimension2D(), well_parameters_->getWells2DPosition());
		}
		else
		{
			qWarning("No Well Data.");
		}
		
		
	});

}

QString FlowWindow::getCurrentDirectory()
{

    QDir app_dir = QDir(qApp->applicationDirPath());

#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
    QString current_dir(app_dir.path() + "\\");

#elif defined(__linux__)               // Linux Directory Style
    QString current_dir(app_dir.path() + "/");

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

	action_computeFlowProperties->setEnabled(false);
	action_showRegions->setEnabled(false);
	action_upscalledPermeability_->setEnabled(false);
	action_oilInPlace_->setEnabled(false);
	toolButton_coloringByVertex->setEnabled(false);
	toolButton_coloringByFace->setEnabled(false);

    controller->clear();
    canvas->clear();

    std::vector< TriangleMesh > triangles_meshes;
    std::vector< CurveMesh > left_curves;
    std::vector< CurveMesh > right_curves;
    std::vector< CurveMesh > front_curves;
    std::vector< CurveMesh > back_curves;

    emit getSurfacesMeshes( triangles_meshes, left_curves, right_curves, front_curves, back_curves );

    double width, height, depth;

    emit getVolumeDimension( width, height, depth);
    this->setVolumeDimensions( width, height, depth );


    controller->setSkeleton( triangles_meshes, left_curves, right_curves, front_curves, back_curves );

    std::vector< double > vertices;
    std::vector< std::size_t > faces;
    int number_of_surfaces = static_cast< int >( triangles_meshes.size() );

    std::size_t offset = 0;
    for( int i = 0; i < number_of_surfaces; ++i )
    {
        const TriangleMesh& t = triangles_meshes[ i ];
        for( std::size_t j = 0; j < t.vertex_list.size(); ++j )
        {
            vertices.push_back( t.vertex_list[ j ] );
        }
        for( std::size_t k = 0; k < t.face_list.size(); ++k )
        {
            faces.push_back( t.face_list[ k ] + offset );
        }

        offset += (t.vertex_list.size()/3);

    }


    canvas->updateTriangleMesh( vertices, faces );

    /////// Changed here -- Clarissa
	
    Tucano::BoundingBox3<double> bbox;
    std::vector<Eigen::Vector3d> v;

    for (std::size_t it = 0; it < vertices.size(); it += 3)
    {
        v.push_back(Eigen::Vector3d(vertices[it + 0], vertices[it + 1], vertices[it + 2]));
    }

    bbox.fromPointCloud(v.begin(), v.end());

    well_parameters_->setBoundingBoxDimension(Eigen::Vector3d(bbox.Min().x(), bbox.Min().y(), bbox.Min().z()), Eigen::Vector3d(bbox.Max().x(), bbox.Max().y(), bbox.Max().z()));

	/// Re arrange the default well positions
	this->bounding_box_changed_ = true;

	well_scene_->setDimension(Eigen::Vector2d((bbox.Max().x()) - (bbox.Min().x()), (bbox.Max().y()) - (bbox.Min().y())), this->bounding_box_changed_);

    this->well_canvas_->fitInView();

    /// Well Module
    canvas->updateWellsPosition(well_parameters_->getNumberOfWells(),well_parameters_->getWellsPosition(), well_parameters_->getWellsSign(), well_parameters_->getWellsRange());


}


void FlowWindow::setParametersBeforeSimulation()
{
	/// Region parameters
	int number_of_regions = 0;
	std::vector< double > saturation_values;
	std::vector< double > pct_values;
	std::vector< double > sort_factor_values;
	std::vector< double > siw_values;

	std::map<int, std::pair<double, double> > x_permeability_values;
	std::map<int, std::pair<double, double> > y_permeability_values;
	std::map<int, std::pair<double, double> > z_permeability_values;
	std::map<int, std::pair<double, double> > porosity_values;

	/// Region Module
	///@FIXME September
	region_parameters_->getRegionData(number_of_regions,
		x_permeability_values,
		y_permeability_values,
		z_permeability_values,
		porosity_values,
		saturation_values,   /// Water Saturation
		pct_values,			 /// Threshold Pressure
		sort_factor_values,  /// Lambda
		siw_values			 /// Connate Water Saturation
		);

	/// Feeding HWU Flow Diagnostic
	controller->setPropertyArea(number_of_regions,
		x_permeability_values,
		y_permeability_values,
		z_permeability_values,
		porosity_values,
		saturation_values,   /// Water Saturation
		pct_values,			 /// Threshold Pressure
		sort_factor_values,  /// Lambda
		siw_values			 /// Connate Water Saturation
		);

	/// Fluid parameters
		double oil_viscosity   = 0.0;
		double oil_density	   = 0.0;
		double bo			   = 0.0;
		double water_viscosity = 0.0;
		double water_density   = 0.0;
		double bw              = 0.0; 
		double fwl             = 0.0; 
		std::pair<int, int> phase_method;

	    /// Fluid Module
		fluid_parameters_->getFluidData(oil_viscosity, oil_density, bo, water_viscosity, water_density, bw, fwl, phase_method);

		controller->setFluidProperty(oil_viscosity, oil_density, bo, water_viscosity, water_density, bw, fwl, phase_method);

}


void FlowWindow::setParametersBeforeMehsing()
{
	///setParametersBeforeSimulation();

	int nw = 0;
	std::vector< unsigned int > type;
	std::vector< double > values;
	std::vector< int > sign;

	/// Well Module
	std::vector< Eigen::Vector4d> wells_position;
	std::vector< Eigen::Vector2d> wells_range;

	well_parameters_->getWellData(nw, type, values, sign, wells_position, wells_range);

	controller->setWellsValues(nw, type, values, sign, wells_position, wells_range);
}

void FlowWindow::buildUnstructured()
{

    controller->setCurrentMethod(FlowVisualizationController::MESHING_METHOD::UNSTRUCTURED);

    setParametersBeforeMehsing();
  

	std::vector< float > vertices, raw_vertices;
    std::vector< unsigned int > edges;
    std::vector< unsigned int > faces, raw_cells;

    controller->generateUnstructured();

	raw_vertices = controller->getVertices();
	raw_cells = controller->getCells();

	//controller->getTetrahedeonMeshGeometry(raw_vertices, raw_cells, vertices, faces);
	

    emit sendSimplifiedMesh( raw_vertices, edges, raw_cells );

}

void FlowWindow::clear()
{
    controller->clear();
    canvas->clear();

    this->region_parameters_->clear();
    this->well_parameters_->clear();
    crosssectionNormalWidget->clear();
    reset();
}

void FlowWindow::boundingBoxChnaged(bool _is_new_model)
{
    this->bounding_box_changed_ = _is_new_model;
    /// The Boundary Has Changed
    std::cout << " The Boundary Has Changed " << std::endl;
}

void FlowWindow::clearPropertiesMenu()
{

}

void FlowWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{

		case Qt::Key_F5:
		{
			canvas->reloadShader();
			break;
		}
		default:
		{
			break;
		}

	}
    
}

void FlowWindow::reset()
{
    //file_of_parameters.clear();
    //file_of_mesh.clear();
    //type_of_file.clear();
}
/// Flow Parameters Widget
//void FlowWindow::regionPoints(const std::map<int, Eigen::Vector3f>& region_points)
//{
//    // Z is fixed
//    //this->parametersBar.setRegionPoints(region_points);
//
//    this->region_parameters_->updateRegionPosition(region_points);
//
//    are_regionsdefined = true;
//
//    qbuildCornerPoint->setEnabled(true);
//    qbuildUnstructured->setEnabled(true);
//}

int FlowWindow::getNumberOfRegions()
{
    //return parametersBar.getNumberOfRegions();
    return this->region_parameters_->getNumberOfRegion();
}
/// OpenVolume Mesh Integration ---------------------------------------------->
void FlowWindow::updatePropertyAction()
{

	std::vector<RRM::PropertyProfile> properties = controller->getListOfComputedProperites();

	clearVertexPropertiesActions();
	clearCellPropertiesActions();

	for (const auto& property : properties)
	{
		if (property.entity() == "Vertex")
		{
			/// Get the action pointer
			QAction * action = menu_vertex_properties->addAction(QString::fromStdString(property.name()));

			/// The connection object in order to proper delete the action later.
			QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
			{
				current_property_ = property;
				updateModelColors(current_property_);
			});

			/// Action Container
			action_vertex_properties.push_back(std::make_tuple(action, connection, property));
			
		}
		else if (property.entity() == "Cell")
		{
			/// Get the action pointer
			QAction * action = menu_cell_properties->addAction(QString::fromStdString(property.name()));

			/// The connection object in order to proper delete the action later.
			QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
			{
				current_property_ = property;
			    updateModelColors(current_property_);
			});

			/// Action Container
			action_cell_properties.push_back(std::make_tuple(action, connection, property));
		}
		else
		{

		}

	}
}

void FlowWindow::clearVertexPropertiesActions()
{
    /// @see https://wiki.qt.io/New_Signal_Slot_Syntax
    for (auto action : action_vertex_properties)
    {
        if (std::get<0>(action))
        {
            menu_vertex_properties->removeAction(std::get<0>(action));
            disconnect(std::get<1>(action));
            delete std::get<0>(action);
        }
    }

    action_vertex_properties.clear();
    menu_vertex_properties->clear();
}

void FlowWindow::clearCellPropertiesActions()
{
    /// @see https://wiki.qt.io/New_Signal_Slot_Syntax
    for (auto action : action_cell_properties)
    {
        if (std::get<0>(action))
        {
            menu_cell_properties->removeAction(std::get<0>(action));
            disconnect(std::get<1>(action));
            delete std::get<0>(action);
        }
    }

    action_cell_properties.clear();
    menu_cell_properties->clear();
}

void FlowWindow::createWellModule()
{
    // Centrall Widget - The Canvas Area
    this->well_canvas_ = new RRM::WellCanvas(this);
    /// Well Scene
    this->well_scene_ = new RRM::WellScene(this->well_canvas_);
    this->well_scene_->setItemIndexMethod(QGraphicsScene::NoIndex);
    this->well_scene_->setTranformationMatrix(this->well_canvas_->getTransformationMatrix());

    this->well_canvas_->setScene(this->well_scene_);

    this->dockWellContainer_ = new QDockWidget(QString("Well Module"), this);
    this->dockWellSceneContainer_ = new QDockWidget(QString("Well Visualization"), this);
    this->well_parameters_ = new RRM::WellWidget(this);

    this->fitView_ = new QPushButton("Fit to View");

    connect(this->fitView_, &QPushButton::pressed, [=]()
    {
        this->well_canvas_->fitInView();
    }
    );

    QVBoxLayout * h = new QVBoxLayout();
    h->addWidget(this->well_canvas_);
    h->addWidget(this->fitView_);

    QWidget * w = new QWidget(this->dockWellContainer_);
    w->setLayout(h);

    this->dockWellContainer_->setWidget(this->well_parameters_);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockWellContainer_);

    this->dockWellSceneContainer_->setWidget(w);
    this->dockWellSceneContainer_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockWellSceneContainer_);

    this->tabifyDockWidget(this->dockWellSceneContainer_, this->dockWellContainer_);

    connect(this->well_parameters_, &RRM::WellWidget::numberOfWells, [=]()
    {
        this->well_scene_->createWells(this->well_parameters_->getNumberOfWells(),this->well_parameters_->getWellsSign());
    }
    );

    connect(this->well_scene_, &RRM::WellScene::updatedWellsPosition, this->well_parameters_, &RRM::WellWidget::updateWellPosition);

    connect(this->well_parameters_, &RRM::WellWidget::wellPositionsHasChanged, [=]()
    {
        canvas->updateWellsPosition(well_parameters_->getNumberOfWells(), well_parameters_->getWellsPosition(), well_parameters_->getWellsSign(), well_parameters_->getWellsRange());
    }
    );

    view_well_module_ = new QAction(tr("View Well Module"),0);
    view_well_module_->setCheckable(true);

    view_well_visualization_ = new QAction(tr("View Well Visualization"), 0);
    view_well_visualization_->setCheckable(true);

    connect(this->dockWellContainer_, &QDockWidget::visibilityChanged, this->view_well_module_, &QAction::setChecked);
    connect(this->dockWellSceneContainer_, &QDockWidget::visibilityChanged, this->view_well_visualization_, &QAction::setChecked);

    connect(this->view_well_module_, &QAction::triggered, this->dockWellContainer_, &QDockWidget::setVisible);
    connect(this->view_well_visualization_, &QAction::triggered, this->dockWellSceneContainer_, &QDockWidget::setVisible);

    flowModule_view_->addAction(view_well_module_);
    flowModule_view_->addAction(view_well_visualization_);
}

void FlowWindow::createRegionModule()
{
    this->dockRegionContainer_ = new QDockWidget(QString("Region Module"), this);
    this->region_parameters_ = new RRM::RegionWidget(this);

    this->dockRegionContainer_->setWidget(this->region_parameters_);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->dockRegionContainer_);


    view_region_module_ = new QAction(tr("View Region Module"), 0);
    view_region_module_->setCheckable(true);

    connect(this->dockRegionContainer_, &QDockWidget::visibilityChanged, this->view_region_module_, &QAction::setChecked);
    connect(this->view_region_module_, &QAction::triggered, this->dockRegionContainer_, &QDockWidget::setVisible);


    flowModule_view_->addAction(view_region_module_);

}

void FlowWindow::createFluidModule()
{
        this->dockFluidContainer_ = new QDockWidget(QString("Fluid Module"), this);
        this->fluid_parameters_ = new RRM::FluidWidget(this);

        this->dockFluidContainer_->setWidget(this->fluid_parameters_);
        this->addDockWidget(Qt::LeftDockWidgetArea, this->dockFluidContainer_);


        view_fluid_module_ = new QAction(tr("View Fluid Module"), 0);
        view_fluid_module_->setCheckable(true);

        connect(this->dockFluidContainer_, &QDockWidget::visibilityChanged, this->view_fluid_module_, &QAction::setChecked);
        connect(this->view_fluid_module_, &QAction::triggered, this->dockFluidContainer_, &QDockWidget::setVisible);

        flowModule_view_->addAction(view_fluid_module_);
}


void FlowWindow::updateModelColors( const RRM::PropertyProfile& _profile)
{
    std::vector<float> colors;
    double min;
    double max;
    unsigned int nc = 0;

	///@ New interface. June 2018
	controller->updateTetrahedronColors(_profile, colors, min, max);
	colorbar.updateColorMap(colormap.getColors(controller->getCurrentColormap(), nc), min, max);
	canvas->setColors(colors);
}

void FlowWindow::setRegions( const std::map< int,  std::vector< int > >& region_colors  /*std::size_t number_of_regions_, std::vector<std::size_t > regions_, std::vector<float> colors_ */)
{
    region_parameters_->setRegionData( region_colors );
	    
    action_buildUnstructured->setEnabled(true);
}


void FlowWindow::setTetrahedronRegions( const std::vector< int >& regions_, std::map< int, std::vector< float > > colors_ )//const std::vector< float >& colors_ )
{

    std::vector<double> values_for_visualization_;
    controller->updateTetrahedonRegions( regions_, values_for_visualization_ );

    //std::vector< float > colors_ov_mesh_;
    //colors_ov_mesh_.resize( values_for_visualization_.size()*3 );
    //int i = 0;
    //for( auto v: values_for_visualization_ )
    //{
    //    int id = static_cast< int >(v);
    //    colors_ov_mesh_[ 3*i ] = colors_[ id ].at(0);
    //    colors_ov_mesh_[ 3*i + 1 ] = colors_[ id ].at(1);
    //    colors_ov_mesh_[ 3*i + 2 ] = colors_[ id ].at(2);
    //    ++i;
    //}



    canvas->updateVolumetricMesh();
    //canvas->setColors( colors_ov_mesh_ );
}


void FlowWindow::updateRegionColor( int index_, int r_, int g_, int b_ )
{
    region_parameters_->updateRegionColor( index_, r_, g_, b_ );
}



/// Old Interface
//void FlowWindow::startProgressBar(const unsigned int& min, const unsigned int& max)
//{
//
//	pb_processprogress->setVisible(true);
//
//	pb_processprogress->setMaximum((int)max);
//	pb_processprogress->setMinimum((int)min);
//
//	pb_processprogress->setValue(min);
//}
//
//void FlowWindow::updateProgressBar(const unsigned int& value)
//{
//
//	pb_processprogress->setValue(value);
//
//	if ((int)value == pb_processprogress->maximum())
//		pb_processprogress->setVisible(false);
//}

/// FlowWindo ToolBar
//connect(qflowparametersDialog, &QAction::triggered, qdockparametersBar, &QDockWidget::show);
/// Load Surface. Clear the scene before. @todo, asks to the user to save current scene section
//connect(qoopenfilesDialog, &QAction::triggered, this, [=]()
//{
//	/// Clear Scene
//	action_clear_->trigger();
//	this->loadSurfacesfromFile();
//	if (are_regionsdefined == true)
//	{
//		qbuildCornerPoint->setEnabled(true);
//		qbuildUnstructured->setEnabled(true);
//	}
//});
///// Load Surface. Clear the scene before. @todo, asks to the user to save current scene section
//connect(qreloadSurface, &QAction::triggered, this, [=]()
//{
//	/// Clear Scene
//	action_clear_->trigger();
//	this->loadSurfacesfromSketch();
//	/// Now we can build the volumetric mesh
//
//	if (are_regionsdefined == true)
//	{
//		qbuildCornerPoint->setEnabled(true);
//		qbuildUnstructured->setEnabled(true);
//	}
//});

//connect(qbuildCornerPoint, &QAction::triggered, this, [=]()
//{
//
//	//QMessageBox::StandardButton reply;
//	//reply = QMessageBox::information(this, "Corner Point Meshing", "Corner-point grids can only be created for simple geometries with non-intersecting surfaces, do you want to continue ?", QMessageBox::Yes | QMessageBox::No);
//
//	///@FIXME June 2017
//	if (true) //if (reply == QMessageBox::Yes)
//	{
//		//				qDebug() << "Yes was clicked";
//		/// Now we can Compute
//		/// @FIXME catch execption from HWU mesh generator
//		this->buildCornerPoint();
//		qcomputeFlowProperties->setEnabled(false);
//		qbuildCornerPoint->setEnabled(false);
//		qbuildUnstructured->setEnabled(false);
//
//	}
//	else {
//		qDebug() << "Yes was *not* clicked";
//	}
//
//	is_cornerpoint = true;
//});


//void FlowWindow::loadSurfacesfromFile()
//{
//
//	controller->clear();
//	canvas->clear();
//
//	QString selected_format = "";
//	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./inputs/",
//		".skt files (*.skt)", &selected_format);
//	if (filename.isEmpty() == true) return;
//
//
//
//	QStringList path = filename.split("\.");
//	QString name_of_file = path[0];
//	QString extension_of_file = path[1];
//
//	type_of_file = extension_of_file.toStdString();
//
//	if (extension_of_file.compare("skt") == 0){
//
//		file_of_mesh = filename.toStdString();
//		controller->readSkeletonFiles(file_of_mesh);
//		canvas->updateMesh();
//
//	}
//
//}
//
//void FlowWindow::readUserInputFile(const std::string& input_file)
//{
//
//	controller->readInputParameters(input_file);
//	updateParameterFields();
//
//	file_of_parameters = input_file;
//
//}
//
//void FlowWindow::updateParameterFields()
//{
//
//	int np = 0;
//	std::vector< double > positions;
//	std::vector< double > perm;
//	std::vector< double > poros;
//	std::vector< double > visc;
//	std::vector< double > saturation;
//
//
//	controller->getPropertyArea(np, positions, perm, poros, visc);
//
//	/// Region Module
//	//    this->region_parameters_->setRegionData(np, positions, perm, poros, saturation);
//
//
//	int nw = 0;
//	std::vector< unsigned int > type;
//	std::vector< double > values;
//	std::vector< int > sign;
//
//	/// Well Module
//	controller->getWellsValues(nw, type, values, sign);
//
//
//
//}

//void FlowWindow::loadSurfacesfromSketch()
//{
//
//	controller->clear();
//	canvas->clear();
//
//	std::vector< size_t > nu;
//	std::vector< size_t > nv;
//	std::vector< double > points;
//	size_t num_extrusion_steps = 1;
//
//	Eigen::Affine3f m2D_to_3D;
//	Eigen::Affine3f m3D_to_2D;
//
//	emit getLegacyMeshes(points, nu, nv, num_extrusion_steps);
//
//
//	double width, height, depth;
//
//	emit getVolumeDimension(width, height, depth);
//	this->setVolumeDimensions(width, height, depth);
//
//	controller->setSkeletonData(points, nu, nv, num_extrusion_steps);
//	canvas->updateMesh();
//
//	/// @TODO
//	/// Used to get the right Region depth in 3D
//	///this->region_parameters_->setRegionDepth(canvas->getDepth());
//
//	std::vector< double > vertices;
//	std::vector< unsigned int > faces;
//
//	controller->buildSurfaceSkeleton(vertices, faces);
//
//	Tucano::BoundingBox3<double> bbox;
//	std::vector<Eigen::Vector3d> v;
//
//	for (std::size_t it = 0; it < vertices.size(); it += 3)
//	{
//		v.push_back(Eigen::Vector3d(vertices[it + 0], vertices[it + 1], vertices[it + 2]));
//	}
//
//	bbox.fromPointCloud(v.begin(), v.end());
//
//	well_parameters_->setBoundingBoxDimension(Eigen::Vector3d(bbox.Min().x(), bbox.Min().y(), bbox.Min().z()), Eigen::Vector3d(bbox.Max().x(), bbox.Max().y(), bbox.Max().z()));
//
//	well_scene_->setDimension(Eigen::Vector2f((bbox.Max().x()) - (bbox.Min().x()), (bbox.Max().y()) - (bbox.Min().y())), this->bounding_box_changed_);
//
//	this->bounding_box_changed_ = false;
//
//	this->well_canvas_->fitInView();
//
//	/// Well Module
//	canvas->updateWellsPosition(well_parameters_->getNumberOfWells(), well_parameters_->getWellsPosition(), well_parameters_->getWellsSign(), well_parameters_->getWellsRange());
//
//}


//void FlowWindow::buildCornerPoint()
//{
//	controller->setCurrentMethod(FlowVisualizationController::MESHING_METHOD::CORNERPOINT);
//
//	if (controller->isUserInputOk() == false)
//	{
//		//acceptUserParameters();
//		// @TODO January 2018
//		setParametersBeforeMehsing();
//	}
//
//	controller->generateCornerPoint();
//
//
//}

//void FlowWindow::acceptUserParameters()
//{
//    /// Region parameters
//    int np = 0;
//    std::vector< double > positions;
//    std::vector< double > permeability;
//    std::vector< double > porosity;
//    std::vector< double > saturation;
//    std::vector< int > permeability_curves;
//
//    std::map<int, std::pair<double, double> > permeability_gradients;
//    std::map<int, std::pair<double, double> > porosity_gradients;
//
//    /// Region Module
//    ///@FIXME September
//	region_parameters_->getRegionData(np, 
//									  positions, 
//									  permeability, 
//									  porosity, 
//									  saturation, 
//									  porosity_gradients, 
//									  permeability_gradients);
//
//    /// Feeding HWU Flow Diagnostic
//    controller->setPropertyArea(np, 
//								positions, 						
//								saturation, 								
//								permeability_gradients, 
//								porosity_gradients);
//
//
//	for (int i = 0; i < np; i++)
//	{
//		std::cout << " permeability " << permeability_gradients.at(i).first << std::endl;
//		std::cout << " permeability " << permeability_gradients.at(i).second << std::endl;
//		std::cout << " porosity     " << porosity_gradients.at(i).first << std::endl;
//		std::cout << " porosity     " << porosity_gradients.at(i).second << std::endl;
//		std::cout << " saturation   " << saturation[i] << std::endl;
//	}
//
//    /// Fluid parameters
//    std::vector< double > bo;
//    std::vector< double > viscosity;
//    std::vector< double > oildensity;
//    std::pair< int, int>  phase_method;
//
//    /// Fluid Module
//    fluid_parameters_->getFluidData(viscosity, bo, oildensity, phase_method);
//	   
//
//    controller->setFluidProperty(viscosity[0], bo[0], oildensity[0], phase_method);
//
//    
//    int nw = 0;
//    std::vector< unsigned int > type;
//    std::vector< double > values;
//    std::vector< int > sign;
//
//    /// Well Module
//    std::vector< Eigen::Vector4d> wells_position;
//    std::vector< Eigen::Vector2d> wells_range;
//
//    well_parameters_->getWellData(nw, type, values, sign, wells_position, wells_range);
//
//    controller->setWellsValues(nw, type, values, sign, wells_position, wells_range);
//
//	
//    /// FIXME Septembe 2017
//    /// Set Fluid Property
//
//}
