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
    are_regionsdefined = false;
    is_cornerpoint = false;


    /// FIXME June 2017. It tells when a new model is build. Used to keep well position
    bounding_box_changed_ = true;
}

void FlowWindow::createWindow()
{
    flowModule_view_ = this->menuBar()->addMenu(tr("&View"));
    flowModule_diagnostic_  = this->menuBar()->addMenu(tr("&Flow Diagnostic"));

    qdockcrosssectionnormalBar = new QDockWidget(this);
    qdockcrosssectionnormalBar->setAllowedAreas(Qt::LeftDockWidgetArea);
    qdockcrosssectionnormalBar->setWidget(&crosssectionnormalBar);
    qdockcrosssectionnormalBar->setVisible(false);
    addDockWidget(Qt::LeftDockWidgetArea, qdockcrosssectionnormalBar);


    sb_statusbar = new QStatusBar();
    this->setStatusBar(sb_statusbar);

    pb_processprogress = new QProgressBar(this);
    pb_processprogress->setVisible(false);
    sb_statusbar->addPermanentWidget(pb_processprogress, 1);


    controller = new FlowVisualizationController(this);
    controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);


    canvas = new FlowVisualizationCanvas(this, this->getCurrentDirectory());
    canvas->setController(controller);

    createWellModule();
    createRegionModule();
    createFluidModule();

    flow_tab_widget_ = new QTabWidget(this);
    flow_tab_widget_->addTab(canvas, "3D Visualization");
    //flow_tab_widget_->addTab(this->well_canvas_, "Well Module");

    /// Remove this from contructor prevent the warning menssage
    hb_mainwindow = new QHBoxLayout();
    //hb_mainwindow->addWidget( canvas );
    hb_mainwindow->addWidget(flow_tab_widget_);
    hb_mainwindow->addWidget(&colorbar);

    QWidget *wd_main = new QWidget(this);
    wd_main->setLayout(hb_mainwindow);

    this->setCentralWidget(wd_main);

    colorbar.setSize(canvas->height(), 25);
}

void FlowWindow::createToolBar()
{

    qtoolbarFlow = new QToolBar();


    qreloadSurface = new QAction(tr("Get Extruded Surface"), qtoolbarFlow);
//    qreloadSurface->setIcon(QIcon(":/images/icons/surfacesfromsketch.png"));

    qreloadSurface1 = new QAction(tr("Get General Surface"), qtoolbarFlow);
//    qreloadSurface1->setIcon(QIcon(":/images/icons/surfacesfromsketch.png"));

    qoopenfilesDialog = new QAction(tr("Surface from File"), qtoolbarFlow);
    qoopenfilesDialog->setIcon(QIcon(":/images/icons/surfacesfromfile.png"));


    qflowparametersDialog = new QAction(tr("User Input"), qtoolbarFlow);
    qflowparametersDialog->setIcon(QIcon(":/images/icons/userinput.png"));



    qbuildCornerPoint = new QAction(tr("Corner Point"), qtoolbarFlow);
    qbuildCornerPoint->setIcon(QIcon(":/images/icons/cpgridmesh.png"));
    qbuildCornerPoint->setEnabled(false);
    //qbuildCornerPoint->setToolTip("<h5><b><font color='red'>Warnning !</font></b></h5>"
    //							  "<h5><b><font color='black'>Corner Point can be only from simple geometry.</font></b></h5>");

    qbuildUnstructured = new QAction(tr("Unstructured"), qtoolbarFlow);
    qbuildUnstructured->setIcon(QIcon(":/images/icons/unstructural.png"));
    qbuildUnstructured->setEnabled(false);


    qcomputeFlowProperties = new QAction(tr("Compute Properties"), qtoolbarFlow);
    qcomputeFlowProperties->setIcon(QIcon(":/images/icons/computeproperties.png"));
    qcomputeFlowProperties->setEnabled(false);



    qshowMovingCrossSection = new QAction(tr("CrossSection"), qtoolbarFlow);
    qshowMovingCrossSection->setIcon(QIcon(":/images/icons/cross.png"));
    qshowMovingCrossSection->setCheckable(true);

    /// Exporters
    // @FIXME June 2017
    action_exportDerivedQuantities_ = new QAction(tr("Export Derived Quantities"), qtoolbarFlow);

    qexportsurface = new QAction(tr("Unstructured Surface Mesh to VTK"), qtoolbarFlow);
    qexportvolume = new QAction(tr("Unstructured Volume Mesh to VTK"), qtoolbarFlow);
    qexportcornerpointVTK = new QAction(tr("Corner-Point Grid to VTK"), qtoolbarFlow);
    qexportcornerpointGRDECL = new QAction(tr("Corner-Point Grid to GRDECL"), qtoolbarFlow);
    qexportresults = new QAction(tr("Results to VTK"), qtoolbarFlow);

    mn_export = new QMenu(tr("Export"), this);
    mn_export->addAction(qexportsurface);
    mn_export->addAction(qexportvolume);
    mn_export->addAction(qexportcornerpointVTK);
    mn_export->addAction(qexportcornerpointGRDECL);
    mn_export->addAction(qexportresults);
    mn_export->addAction(action_exportDerivedQuantities_);

    tbn_export = new QToolButton();
    tbn_export->setIcon(QIcon(":/images/icons/document_export.png"));
    tbn_export->setMenu(mn_export);
    tbn_export->setPopupMode(QToolButton::InstantPopup);
    tbn_export->setEnabled(false);

    /// Properties

    tbn_coloringbyvertex = new QToolButton();
    tbn_coloringbyvertex->setIcon(QIcon(":/images/icons/vertex.png"));

    /// OpenVolume Mesh Integration ---------------------------------------------->
    menu_vertex_properties = new QMenu(tr("OVM Vertex Properties"));
    tbn_coloringbyvertex->setMenu(menu_vertex_properties);
    tbn_coloringbyvertex->setPopupMode(QToolButton::InstantPopup);
    tbn_coloringbyvertex->setEnabled(false);


    tbn_coloringbyface = new QToolButton();
    tbn_coloringbyface->setIcon(QIcon(":/images/icons/properties.png"));

    /// OpenVolume Mesh Integration ---------------------------------------------->
    menu_cell_properties = new QMenu(tr("OVM Cell Properties"));
    tbn_coloringbyface->setMenu(menu_cell_properties);
    tbn_coloringbyface->setPopupMode(QToolButton::InstantPopup);
    tbn_coloringbyface->setEnabled(false);

    mn_colormaps = new QMenu(tr("Colormaps"));
    tbn_colormaps = new QToolButton();
    tbn_colormaps->setIcon(QIcon(":/images/icons/colormap.png"));
    tbn_colormaps->setMenu(mn_colormaps);
    tbn_colormaps->setPopupMode(QToolButton::InstantPopup);

    QActionGroup* ag_colormaps = new QActionGroup(mn_colormaps);
    ag_colormaps->setExclusive(true);

    action_constant = new QAction(tr("Constant"), ag_colormaps);
    action_constant->setCheckable(true);
    action_constant->setChecked(true);
    action_cool_to_warm = new QAction(tr("Cool to Warm"), ag_colormaps);
    action_cool_to_warm->setCheckable(true);
    action_hot = new QAction(tr("Hot"), ag_colormaps);
    action_hot->setCheckable(true);
    action_cool = new QAction(tr("Cool"), ag_colormaps);
    action_cool->setCheckable(true);
    //action_parula = new QAction(tr("Parula"), ag_colormaps);
    //action_parula->setCheckable(true);
    action_spring = new QAction(tr("Spring"), ag_colormaps);
    action_spring->setCheckable(true);
    action_summer = new QAction(tr("Summer"), ag_colormaps);
    action_summer->setCheckable(true);
    action_copper = new QAction(tr("Copper"), ag_colormaps);
    action_copper->setCheckable(true);
    action_polar = new QAction(tr("Polar"), ag_colormaps);
    action_polar->setCheckable(true);
    action_winter = new QAction(tr("Winter"), ag_colormaps);
    action_winter->setCheckable(true);
    action_jet = new QAction(tr("Jet"), ag_colormaps);
    action_jet->setCheckable(true);

    mn_colormaps->addActions(ag_colormaps->actions());

    action_showregions = new QAction(tr("Show Pore Volumes"), qtoolbarFlow);
    action_showregions->setIcon(QIcon(":/images/icons/porus.png"));
    action_showregions->setCheckable(true);
    //connect( action_showregions, &QAction::toggled, &porevolumeform, &PoreVolumeResultsForm::setVisible );

    action_showregions->setEnabled(false);

    qclear = new QAction(tr("Clear"), qtoolbarFlow);
    qclear->setIcon(QIcon(":/images/icons/clear.png"));

    /// ToolBar setUp

    qtoolbarFlow->addAction(qreloadSurface);
    qtoolbarFlow->addAction(qreloadSurface1);
    qtoolbarFlow->addAction(qoopenfilesDialog);
    qtoolbarFlow->addAction(qflowparametersDialog);

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction(qbuildCornerPoint);
    qtoolbarFlow->addAction(qbuildUnstructured);

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction(qcomputeFlowProperties);
    qtoolbarFlow->addWidget(tbn_coloringbyvertex);
    qtoolbarFlow->addWidget(tbn_coloringbyface);
    qtoolbarFlow->addAction(action_showregions);

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addAction(qshowMovingCrossSection);

    qtoolbarFlow->addSeparator();
    qtoolbarFlow->addWidget(tbn_colormaps);
    qtoolbarFlow->addWidget(tbn_export);
    /// FIXME June
    //qtoolbarFlow->addAction(qclear);

    qtoolbarFlow->addSeparator();

    action_clearComputedQuantities_ = new QAction(tr("Clear Computed Quantities"), qtoolbarFlow);
    action_clearComputedQuantities_->setIcon(QIcon(":/images/icons/refresh.png"));
    action_clearComputedQuantities_->setEnabled(false);
    action_upscalledPermeability_ = new QAction(tr("Get Upscaled Permeability"), qtoolbarFlow);
    action_upscalledPermeability_->setEnabled(false);
    action_oilinPlace_ = new QAction(tr("Get Oil in Place"), qtoolbarFlow);
    action_oilinPlace_->setEnabled(false);


//	action_oil_in_place_ = new QAction(tr("Get Oil in Place"), qtoolbarFlow);
//	action_oil_in_place_->setEnabled(false);

    flowModule_diagnostic_->addAction(action_upscalledPermeability_);

    flowModule_diagnostic_->addAction(action_oilinPlace_);

    //qtoolbarFlow->addAction(action_exportDerivedQuantities_);
    qtoolbarFlow->addAction(action_clearComputedQuantities_);
    //qtoolbarFlow->addAction(action_upscalledPermeability_);

    addToolBar(qtoolbarFlow);
    qtoolbarFlow->setVisible(true);
}

void FlowWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = event->size();
    colorbar.setSize(canvas->height(), 25);
}

void FlowWindow::createActions()
{

    /// FlowWindo ToolBar
    //connect(qflowparametersDialog, &QAction::triggered, qdockparametersBar, &QDockWidget::show);
    /// Load Surface. Clear the scene before. @todo, asks to the user to save current scene section
    connect(qoopenfilesDialog, &QAction::triggered, this, [=]()
    {
        /// Clear Scene
        qclear->trigger();
        this->loadSurfacesfromFile();
        if (are_regionsdefined == true)
        {
            qbuildCornerPoint->setEnabled(true);
            qbuildUnstructured->setEnabled(true);
        }
    });
    /// Load Surface. Clear the scene before. @todo, asks to the user to save current scene section
    connect(qreloadSurface, &QAction::triggered, this, [=]()
    {
        /// Clear Scene
        qclear->trigger();
        this->loadSurfacesfromSketch();
        /// Now we can build the volumetric mesh

        if (are_regionsdefined == true)
        {
            qbuildCornerPoint->setEnabled(true);
            qbuildUnstructured->setEnabled(true);
        }
    });


    connect( qreloadSurface1, &QAction::triggered, this, [=](){

        qclear->trigger();
        this->loadSurfacesfromSketch1();
        if (are_regionsdefined == true)
        {
            qbuildCornerPoint->setEnabled(true);
            qbuildUnstructured->setEnabled(true);
        }

    });

    connect(qbuildCornerPoint, &QAction::triggered, this, [=]()
    {

        //QMessageBox::StandardButton reply;
        //reply = QMessageBox::information(this, "Corner Point Meshing", "Corner-point grids can only be created for simple geometries with non-intersecting surfaces, do you want to continue ?", QMessageBox::Yes | QMessageBox::No);

        ///@FIXME June 2017
        if (true) //if (reply == QMessageBox::Yes)
        {
            //				qDebug() << "Yes was clicked";
            /// Now we can Compute
            /// @FIXME catch execption from HWU mesh generator
            this->buildCornerPoint();
            qcomputeFlowProperties->setEnabled(false);
            qbuildCornerPoint->setEnabled(false);
            qbuildUnstructured->setEnabled(false);

        }
        else {
            qDebug() << "Yes was *not* clicked";
        }

        is_cornerpoint = true;
    });

    connect(qbuildUnstructured, &QAction::triggered, this, [=]()
    {
        /// Now we can Compute and see region Identifications
        /// @FIXME catch execption from HWU mesh generator
        this->buildUnstructured();
        qcomputeFlowProperties->setEnabled(true);
        action_showregions->setEnabled(false);
        qbuildCornerPoint->setEnabled(false);
        qbuildUnstructured->setEnabled(false);
        action_upscalledPermeability_->setEnabled(true);
        is_cornerpoint = false;

    });

    connect(qcomputeFlowProperties, &QAction::triggered, controller, [=]()
    {
        /// Now we can visualize the properties and export
        /// @FIXME catch execption from HWU mesh generator
        controller->computeFlowProperties();
        qcomputeFlowProperties->setEnabled(false);
        tbn_export->setEnabled(true);
        tbn_coloringbyvertex->setEnabled(true);
        tbn_coloringbyface->setEnabled(true);
        /// Set Default ColorMap
        action_cool_to_warm->setChecked(true);
        controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);
        action_upscalledPermeability_->setEnabled(false);
        action_oilinPlace_->setEnabled( true );

        if (is_cornerpoint == false)
        {
            controller->loadPropertiesTetrahedron();
            this->updatePropertyAction(controller->getPtrTetrahedralMesh());
            current_property_ = RRM::PropertyProfile("Tetrahedron", "Pressure", "VProp", "", "double");
            updateModelColors(current_property_);
            action_showregions->setEnabled(true);
            controller->getPoreVolume();
            action_clearComputedQuantities_->setEnabled(true);

        }
        else
        {
            controller->loadPropertiesHexahedron();
            this->updatePropertyAction(controller->getPtrHexahedralMesh());
            current_property_ = RRM::PropertyProfile("Hexahedron","Pressure","CProp","","double");
            updateModelColors(current_property_);
            action_showregions->setEnabled(false);
        }


    });
    /// FIXME JUNE 2017
    /// Clear Reload
    connect(action_clearComputedQuantities_, &QAction::triggered, this, [=]()
    {
        qclear->trigger();
        this->loadSurfacesfromSketch();
        this->buildUnstructured();
        //controller->computeFlowProperties();


            //controller->loadPropertiesTetrahedron();
            this->updatePropertyAction(controller->getPtrTetrahedralMesh());
            //current_property_ = RRM::PropertyProfile("Tetrahedron", "Pressure", "VProp", "", "double");
            //updateModelColors(current_property_);
            action_showregions->setEnabled(false);
            //controller->getPoreVolume();
            action_clearComputedQuantities_->setEnabled(true);
            action_upscalledPermeability_->setEnabled(false);

        /// Set Default ColorMap
        action_cool_to_warm->setChecked(true);
        controller->setCurrentColormap(ColorMap::COLORMAP::COOL_TO_WARM);

        qcomputeFlowProperties->setEnabled(true);
        tbn_export->setEnabled(false);
        tbn_coloringbyvertex->setEnabled(false);
        tbn_coloringbyface->setEnabled(false);
        action_clearComputedQuantities_->setEnabled(true);


    });
    /// Region ID
    connect(action_showregions, &QAction::toggled, [=](bool is_toggled)
    {
        if (is_toggled)
        {
            current_property_ = RRM::PropertyProfile("Tetrahedron", "Pore Volume", "CProp", "", "double");
            updateModelColors(current_property_);
            canvas->showRegions();
        }

    });
    /// Exporters
    /// @TODO June 2017
    connect(action_exportDerivedQuantities_, SIGNAL(triggered(bool)), controller, SLOT(exportDerivedQuantities()));
    connect(action_clearComputedQuantities_, &QAction::triggered, [=]()
    {
        controller->clearComputedQuantities();
        canvas->setDefaultColor();
    });
    connect(action_upscalledPermeability_, &QAction::triggered, [=]()
    {
        std::string result;
        action_upscalledPermeability_->setEnabled(false);
        qcomputeFlowProperties->setEnabled(false);
        controller->getUpscalledPermeability(result);
        up_scaled_.setText(QString::fromStdString(result));
        up_scaled_.show();
    }
    );



    connect(action_oilinPlace_, &QAction::triggered, [=]()
    {
        std::string result;
        controller->getOilInPlace(result);
        up_scaled_.setText(QString::fromStdString(result));
        up_scaled_.show();
    }
    );

    connect(qexportsurface, SIGNAL(triggered(bool)), controller, SLOT(exportSurfacetoVTK()));
    connect(qexportvolume, SIGNAL(triggered(bool)), controller, SLOT(exportVolumetoVTK()));
    connect(qexportcornerpointVTK, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoVTK()));
    connect(qexportcornerpointGRDECL, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoGRDECL()));
    connect(qexportcornerpointGRDECL, SIGNAL(triggered(bool)), controller, SLOT(exportCornerPointtoGRDECL()));
    connect(qexportresults, SIGNAL(triggered(bool)), controller, SLOT(exportResultstoVTK()));
    // CrossSection
    connect(qshowMovingCrossSection, &QAction::toggled, qdockcrosssectionnormalBar, &QDockWidget::setVisible);
    connect(qshowMovingCrossSection, &QAction::toggled, canvas, &FlowVisualizationCanvas::disableCrossSection);
    /// ColorMap
    connect(action_constant, &QAction::triggered, this, [=](){
        controller->setCurrentColormap(ColorMap::COLORMAP::CONSTANT);
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


    connect(controller, &FlowVisualizationController::showToolbar, qtoolbarFlow, &QToolBar::setVisible);
    connect(controller, &FlowVisualizationController::clearAll, this, &FlowWindow::clear);


    connect(controller, SIGNAL(clearPropertiesMenu()), this, SLOT(clearPropertiesMenu()));

    connect(canvas, &FlowVisualizationCanvas::applyCrossSection, qdockcrosssectionnormalBar, &QDockWidget::show);
    connect(canvas, &FlowVisualizationCanvas::clearAll, this, &FlowWindow::clear);


    connect(&crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, canvas, &FlowVisualizationCanvas::setCrossSectionNormalCoordinates);
    connect(&crosssectionnormalBar, &NormalMovableCrossSectionFlow::sendCrossSectionNormalCoordinates, qdockcrosssectionnormalBar, &QDockWidget::close);
    connect(&crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, qdockcrosssectionnormalBar, &QDockWidget::close);
    connect(&crosssectionnormalBar, &NormalMovableCrossSectionFlow::canceled, this, [=](){ qshowMovingCrossSection->setChecked(false); });

    //Region Signal
    connect(region_parameters_, &RRM::RegionWidget::numberOfRegions, this, [=](int _number_of_regions){ emit sendNumberOfRegions(_number_of_regions); });



    connect( fluid_parameters_, &RRM::FluidWidget::setSinglePhase, this, [=]( )
    {
		controller->setSinglePhase( );
        region_parameters_->setByRegionSaturation( false );
    } );


	connect(fluid_parameters_, &RRM::FluidWidget::setMultiPhase, this, [=]()
	{
		//controller->setMultiPhase(FlowVisualizationController::SaturationMethod::PERREGION);
		//region_parameters_->setByRegionSaturation(true);
	});

    connect( fluid_parameters_, &RRM::FluidWidget::setSaturationPerRegion, this, [=]()
    {
        controller->setSaturationMethod( FlowVisualizationController::SaturationMethod::PERREGION );
        region_parameters_->setByRegionSaturation( true );
    } );

    connect( fluid_parameters_, &RRM::FluidWidget::setAPIGravity, this, [=]()
    {
        controller->setSaturationMethod( FlowVisualizationController::SaturationMethod::APIGRAVITY );
        region_parameters_->setByRegionSaturation( false );
    } );


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

    controller->clear();
    canvas->clear();

    std::vector< size_t > nu;
    std::vector< size_t > nv;
    std::vector< double > points;
    size_t num_extrusion_steps = 1;

    Eigen::Affine3f m2D_to_3D;
    Eigen::Affine3f m3D_to_2D;

    emit getLegacyMeshes( points, nu, nv, num_extrusion_steps );


    double width, height, depth;

    emit getVolumeDimension( width, height, depth);
    this->setVolumeDimensions( width, height, depth );

    controller->setSkeletonData( points, nu, nv, num_extrusion_steps );
    canvas->updateMesh();

    /// @TODO
    /// Used to get the right Region depth in 3D
    this->region_parameters_->setRegionDepth(canvas->getDepth());

    std::vector< double > vertices;
    std::vector< unsigned int > faces;

    controller->buildSurfaceSkeleton(vertices, faces);

    Tucano::BoundingBox3<double> bbox;
    std::vector<Eigen::Vector3d> v;

    for (std::size_t it = 0; it < vertices.size(); it += 3)
    {
        v.push_back(Eigen::Vector3d(vertices[it + 0], vertices[it + 1], vertices[it + 2]));
    }

    bbox.fromPointCloud(v.begin(), v.end());

    well_parameters_->setBoundingBoxDimension(Eigen::Vector3d(bbox.Min().x(), bbox.Min().y(), bbox.Min().z()), Eigen::Vector3d(bbox.Max().x(), bbox.Max().y(), bbox.Max().z()));

    well_scene_->setDimension(Eigen::Vector2f((bbox.Max().x()) - (bbox.Min().x()), (bbox.Max().y()) - (bbox.Min().y())), this->bounding_box_changed_);

    this->bounding_box_changed_ = false;

    this->well_canvas_->fitInView();

    /// Well Module
    canvas->updateWellsPosition(well_parameters_->getNumberOfWells(),well_parameters_->getWellsPosition(), well_parameters_->getWellsSign(), well_parameters_->getWellsRange());

}



void FlowWindow::loadSurfacesfromSketch1()
{
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

    well_scene_->setDimension(Eigen::Vector2f((bbox.Max().x()) - (bbox.Min().x()), (bbox.Max().y()) - (bbox.Min().y())), this->bounding_box_changed_);

    this->bounding_box_changed_ = false;

    this->well_canvas_->fitInView();

    /// Well Module
    canvas->updateWellsPosition(well_parameters_->getNumberOfWells(),well_parameters_->getWellsPosition(), well_parameters_->getWellsSign(), well_parameters_->getWellsRange());


}



void FlowWindow::loadSurfacesfromFile()
{

    controller->clear();
    canvas->clear();

    QString selected_format = "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "./inputs/",
        ".skt files (*.skt)", &selected_format);
    if (filename.isEmpty() == true) return;



    QStringList path = filename.split("\.");
    QString name_of_file = path[0];
    QString extension_of_file = path[1];

    type_of_file = extension_of_file.toStdString();

    if (extension_of_file.compare("skt") == 0){

        file_of_mesh = filename.toStdString();
        controller->readSkeletonFiles(file_of_mesh);
        canvas->updateMesh();

    }

}

void FlowWindow::readUserInputFile(const std::string& input_file)
{

    controller->readInputParameters(input_file);
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
    std::vector< double > saturation;


    controller->getPropertyArea(np, positions, perm, poros, visc);

    /// Region Module
    this->region_parameters_->setRegionData(np, positions, perm, poros, saturation);


    int nw = 0;
    std::vector< unsigned int > type;
    std::vector< double > values;
    std::vector< int > sign;

    /// Well Module
    controller->getWellsValues(nw, type, values, sign);



}

void FlowWindow::acceptUserParameters()
{
    /// Region parameters
    int np = 0;
    std::vector< double > positions;
    std::vector< double > permeability;
    std::vector< double > porosity;
    std::vector< double > saturation;
    std::vector< int > permeability_curves;

    std::map<int, std::pair<double, double> > permeability_gradients;
    std::map<int, std::pair<double, double> > porosity_gradients;

    /// Region Module
    ///@FIXME September
	region_parameters_->getRegionData(np, 
									  positions, 
									  permeability, 
									  porosity, 
									  saturation, 
									  porosity_gradients, 
									  permeability_gradients);

    /// Feeding HWU Flow Diagnostic
    controller->setPropertyArea(np, 
								positions, 						
								saturation, 								
								permeability_gradients, 
								porosity_gradients);


	for (int i = 0; i < np; i++)
	{
		std::cout << " permeability " << permeability_gradients.at(i).first << std::endl;
		std::cout << " permeability " << permeability_gradients.at(i).second << std::endl;
		std::cout << " porosity     " << porosity_gradients.at(i).first << std::endl;
		std::cout << " porosity     " << porosity_gradients.at(i).second << std::endl;
		std::cout << " saturation   " << saturation[i] << std::endl;
	}

    /// Fluid parameters
    std::vector< double > bo;
    std::vector< double > viscosity;
    std::vector< double > oildensity;
    std::pair< int, int>  phase_method;

    /// Fluid Module
    fluid_parameters_->getFluidData(viscosity, bo, oildensity, phase_method);
	   

    controller->setFluidProperty(viscosity[0], bo[0], oildensity[0], phase_method);

    
    int nw = 0;
    std::vector< unsigned int > type;
    std::vector< double > values;
    std::vector< int > sign;

    /// Well Module
    std::vector< Eigen::Vector4d> wells_position;
    std::vector< Eigen::Vector2d> wells_range;

    well_parameters_->getWellData(nw, type, values, sign, wells_position, wells_range);

    controller->setWellsValues(nw, type, values, sign, wells_position, wells_range);

	
    /// FIXME Septembe 2017
    /// Set Fluid Property

}

void FlowWindow::buildCornerPoint()
{
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

    controller->setCurrentMethod(FlowVisualizationController::MESHING_METHOD::UNSTRUCTURED);

    if (controller->isUserInputOk() == false)
    {
        acceptUserParameters();
    }

    controller->generateUnstructured();
    canvas->updateVolumetricMesh();
}

void FlowWindow::startProgressBar(const unsigned int& min, const unsigned int& max)
{

    pb_processprogress->setVisible(true);

    pb_processprogress->setMaximum((int)max);
    pb_processprogress->setMinimum((int)min);

    pb_processprogress->setValue(min);
}

void FlowWindow::updateProgressBar(const unsigned int& value)
{

    pb_processprogress->setValue(value);

    if ((int)value == pb_processprogress->maximum())
        pb_processprogress->setVisible(false);
}

void FlowWindow::clear()
{
    controller->clear();
    canvas->clear();

    this->region_parameters_->clear();
    this->well_parameters_->clear();
    crosssectionnormalBar.clear();
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

    case Qt::Key_H:
    {
        qtoolbarFlow->setVisible(false);
    }
    break;
    case Qt::Key_T:
    {
        qtoolbarFlow->setVisible(true);
    }
    break;
    case Qt::Key_F5:
    {
        canvas->reloadShader();
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
/// Flow Parameters Widget
void FlowWindow::regionPoints(const std::map<int, Eigen::Vector3f>& region_points)
{
    // Z is fixed
    //this->parametersBar.setRegionPoints(region_points);

    this->region_parameters_->updateRegionPosition(region_points);

    are_regionsdefined = true;

    qbuildCornerPoint->setEnabled(true);
    qbuildUnstructured->setEnabled(true);
}

int FlowWindow::getNumberOfRegions()
{
    //return parametersBar.getNumberOfRegions();
    return this->region_parameters_->getNumberOfRegion();
}
/// OpenVolume Mesh Integration ---------------------------------------------->
void FlowWindow::updatePropertyAction(std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> _ptr_mesh)
{


    std::vector<RRM::PropertyProfile> vertex_properties;
    std::vector<RRM::PropertyProfile> cell_properties;

    if (_ptr_mesh)
    {
        /// Entity Vertex

        for (OpenVolumeMesh::ResourceManager::Properties::const_iterator vp_it = _ptr_mesh->vertex_props_begin(); vp_it != _ptr_mesh->vertex_props_end(); vp_it++)
        {
            std::cout << (*vp_it)->name() << " Type = " << (*vp_it)->entityType() << " Data name = " << (*vp_it)->typeNameWrapper() << std::endl;

            vertex_properties.push_back(RRM::PropertyProfile("Tetrahedron",(*vp_it)->name(), (*vp_it)->entityType(), "", (*vp_it)->typeNameWrapper()));
        }

        /// Entity Cells

        for (OpenVolumeMesh::ResourceManager::Properties::const_iterator cp_it = _ptr_mesh->cell_props_begin(); cp_it != _ptr_mesh->cell_props_end(); cp_it++)
        {
            std::cout << (*cp_it)->name() << " Type = " << (*cp_it)->entityType() << " Data name = " << (*cp_it)->typeNameWrapper() << std::endl;

            cell_properties.push_back(RRM::PropertyProfile("Tetrahedron",(*cp_it)->name(), (*cp_it)->entityType(), "", (*cp_it)->typeNameWrapper()));
        }

    }

    clearVertexPropertiesActions();

    for (const auto vp : vertex_properties)
    {
        /// Get the action pointer
        QAction * action = menu_vertex_properties->addAction(QString::fromStdString(vp.name()));

        /// The connection object in order to proper delete the action later.
        QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
        {
            current_property_ = vp;
            updateModelColors(current_property_);
        });

        /// Action Container
        action_vertex_properties.push_back(std::make_tuple(action, connection, vp));
    }


    clearCellPropertiesActions();

    for (const auto cp : cell_properties)
    {
        /// Get the action pointer
        QAction * action = menu_cell_properties->addAction(QString::fromStdString(cp.name()));

        /// The connection object in order to proper delete the action later.
        QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
        {
            current_property_ = cp;
            updateModelColors(current_property_);
        });

        /// Action Container
        action_cell_properties.push_back(std::make_tuple(action, connection, cp));
    }



}

void FlowWindow::updatePropertyAction(std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> _ptr_mesh)
{
    std::vector<RRM::PropertyProfile> vertex_properties;
    std::vector<RRM::PropertyProfile> cell_properties;

    if (_ptr_mesh)
    {
        /// Entity Vertex

        for (OpenVolumeMesh::ResourceManager::Properties::const_iterator vp_it = _ptr_mesh->vertex_props_begin(); vp_it != _ptr_mesh->vertex_props_end(); vp_it++)
        {
            std::cout << (*vp_it)->name() << " Type = " << (*vp_it)->entityType() << " Data name = " << (*vp_it)->typeNameWrapper() << std::endl;

            vertex_properties.push_back(RRM::PropertyProfile("Hexahedron",(*vp_it)->name(), (*vp_it)->entityType(), "", (*vp_it)->typeNameWrapper()));
        }

        /// Entity Cells

        for (OpenVolumeMesh::ResourceManager::Properties::const_iterator cp_it = _ptr_mesh->cell_props_begin(); cp_it != _ptr_mesh->cell_props_end(); cp_it++)
        {
            std::cout << (*cp_it)->name() << " Type = " << (*cp_it)->entityType() << " Data name = " << (*cp_it)->typeNameWrapper() << std::endl;

            cell_properties.push_back(RRM::PropertyProfile("Hexahedron",(*cp_it)->name(), (*cp_it)->entityType(), "", (*cp_it)->typeNameWrapper()));
        }

    }

    clearVertexPropertiesActions();

    for (const auto vp : vertex_properties)
    {
        /// Get the action pointer
        QAction * action = menu_vertex_properties->addAction(QString::fromStdString(vp.name()));

        /// The connection object in order to proper delete the action later.
        QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
        {
            current_property_ = vp;
            updateModelColors(current_property_);
        });

        /// Action Container
        action_vertex_properties.push_back(std::make_tuple(action, connection, vp));
    }


    clearCellPropertiesActions();

    for (const auto cp : cell_properties)
    {
        /// Get the action pointer
        QAction * action = menu_cell_properties->addAction(QString::fromStdString(cp.name()));

        /// The connection object in order to proper delete the action later.
        QMetaObject::Connection connection = connect(action, &QAction::triggered, [=]()
        {
            current_property_ = cp;
            updateModelColors(current_property_);
        });

        /// Action Container
        action_cell_properties.push_back(std::make_tuple(action, connection, cp));
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
    connect( this->region_parameters_, &RRM::RegionWidget::getRegions, [=](){
        std::map<int, Eigen::Vector3f> region_points;
        emit requestRegionsPosition( region_points );
        regionPoints( region_points );
    } );

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

    if (_profile.elementType() == "Tetrahedron")
    {
        controller->updateTetrahedronColors(_profile.name(), _profile.entity(), _profile.dimension(), colors, min, max);
        colorbar.updateColorMap(colormap.getColors(controller->getCurrentColormap(), nc), min, max);
        canvas->setColors(colors);
    }
    else if (_profile.elementType() == "Hexahedron")
    {
        controller->updateHexahedronColors(_profile.name(), _profile.entity(), _profile.dimension(), colors, min, max);
        colorbar.updateColorMap(colormap.getColors(controller->getCurrentColormap(), nc), min, max);
        canvas->setColors(colors);
    }else
    {
        std::cout << "You shall never reach this point" << std::endl;
    }


}
