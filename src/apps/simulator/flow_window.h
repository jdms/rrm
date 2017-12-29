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


#ifndef FLOWWINDOW_H
#define FLOWWINDOW_H

#include <map>

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QProgressBar>
#include <QKeyEvent>
#include <QToolButton>
#include <QResizeEvent>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QMenuBar>

#include "normal_movable_crosssection_flow.h"
#include "flow_visualization_canvas.h"
#include "colorbar.h"


#include "well_module/well_scene.hpp"
#include "well_module/well_canvas.hpp"
#include "well_module/well_widget.hpp"

#include "region_module/region_widget.hpp"
#include "fluid_module/fluid_widget.hpp"
#include "flow_model/property_profile.hpp"

class FlowWindow : public  QMainWindow
{
    Q_OBJECT

    public:


        using TriangleMesh = FlowVisualizationController::TriangleMesh;
        using CurveMesh = FlowVisualizationController::CurveMesh;



        explicit FlowWindow( QWidget *parent = 0 );

        void createWindow();
        void createToolBar();
        void createActions();

        void updateParameterFields();
        QString getCurrentDirectory();

        void keyPressEvent( QKeyEvent *event );
        void resizeEvent(QResizeEvent *event);

    protected:
        void reset();

    public slots:


        void loadSurfacesfromFile();
        void loadSurfacesfromSketch();
        void loadSurfacesfromSketch1();

        void readUserInputFile( const std::string& input_file );

        void startProgressBar( const unsigned int& min, const unsigned int& max );
        void updateProgressBar( const unsigned int& value );

        void clearPropertiesMenu();

        void buildUnstructured();
        void buildCornerPoint();

        void acceptUserParameters();
        void clear();

        void setRegions( std::size_t number_of_regions_, std::vector<std::size_t > regions_, std::vector<float> colors_ );
        void regionPoints(const std::map<int, Eigen::Vector3f>& region_points);
        /// Will emit a signal sendNumberOfRegions
        int getNumberOfRegions();

        void setVolumeDimensions( double width_, double height_, double depth_ )
        {
            controller->setVolumeDimensions( width_, height_, depth_ );
        }

        /// @FIXME
        void boundingBoxChnaged(bool _is_new_model);

        void setTetrahedronRegions( const std::vector< int >& regions_, std::map< int, std::vector< float > > colors_ );//const std::vector< float >& colors_ );


    signals:

        void getSurfaceCrossSection();
        void getSurfacesMeshes( std::vector< TriangleMesh>& triangles_meshes, std::vector< CurveMesh>& left_curves, std::vector< CurveMesh >& right_curves,
                                std::vector< CurveMesh> & front_curves, std::vector< CurveMesh >& back_curves );
        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
        void sendNumberOfRegions(int _number_of_regions);

        void getVolumeDimension(double& width, double& heigth, double&  depth);

        void requestRegionsPosition( std::map<int, Eigen::Vector3f>& region_point );
        void sendSimplifiedMesh( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces );

    private:

        QToolBar *qtoolbarFlow;
        QAction *qoopenfilesDialog;
        QAction *qflowparametersDialog;
        QAction *qcomputeFlowProperties;
        QAction *qshowMovingCrossSection;
        QAction* qreloadSurface;
        QAction* qreloadSurface1;
        QAction *qclear;

        QAction* qbuildCornerPoint;
        QAction* qbuildUnstructured;

        QAction *qexportsurface;
        QAction *qexportvolume;
        QAction *qexportcornerpointVTK;
        QAction *qexportcornerpointGRDECL;
        QAction* qexportresults;


        QAction* action_cornerpoint;
        QAction* action_unstructured;

        QMenu *mn_export;
        QToolButton* tbn_export;

        FlowVisualizationCanvas *canvas;
        FlowVisualizationController *controller;

        QDockWidget * qdockcrosssectionnormalBar;
        NormalMovableCrossSectionFlow crosssectionnormalBar;


        QStatusBar* sb_statusbar;
        QProgressBar* pb_processprogress;

        std::string file_of_parameters;
        std::string file_of_mesh;
        std::string type_of_file;


        QToolButton* tbn_coloringbyvertex;
        QToolButton* tbn_coloringbyface;


        QHBoxLayout *hb_mainwindow;
        ColorBar colorbar;
        ColorMap colormap;
        QToolButton* tbn_colormaps;
        QMenu* mn_colormaps;

        QAction* action_constant;
        QAction* action_cool_to_warm;
        QAction* action_hot;
        QAction* action_cool;
        QAction* action_parula;
        QAction* action_spring;
        QAction* action_summer;
        QAction* action_copper;
        QAction* action_polar;
        QAction* action_winter;
        QAction* action_jet;

        QAction* action_showregions;

        bool are_regionsdefined;
        bool is_cornerpoint;
        bool bounding_box_changed_;

        /// New Interface for June

        QAction* action_exportDerivedQuantities_;
        QAction* action_clearComputedQuantities_;
        QAction* action_upscalledPermeability_;
        QAction* action_oilinPlace_;

        /// OpenVolume Mesh Integration ---------------------------------------------->
        void FlowWindow::updatePropertyAction(std::shared_ptr<OpenVolumeMesh::HexahedralMesh3d> _ptr_mesh);
        void FlowWindow::updatePropertyAction(std::shared_ptr<OpenVolumeMesh::TetrahedralMeshV3d> _ptr_mesh);

        std::vector<std::tuple<QAction*, QMetaObject::Connection, RRM::PropertyProfile> >  action_vertex_properties;
        std::vector<std::tuple<QAction*, QMetaObject::Connection, RRM::PropertyProfile> >  action_cell_properties;
        RRM::PropertyProfile current_property_;

        void updateModelColors(const RRM::PropertyProfile& _profile);

        void clearVertexPropertiesActions();
        void clearCellPropertiesActions();

        QMenu * menu_vertex_properties;
        QMenu * menu_cell_properties;

        QTabWidget * flow_tab_widget_;

        ///@FIXME 2017
        QMenu * flowModule_view_;
        QMenu * flowModule_diagnostic_;

        void createWellModule();
        RRM::WellCanvas*         well_canvas_;
        RRM::WellScene*          well_scene_;
        RRM::WellWidget*		 well_parameters_;
        QDockWidget*			 dockWellContainer_;
        QDockWidget*			 dockWellSceneContainer_;
        QAction * view_well_module_;
        QAction * view_well_visualization_;
        QPushButton * fitView_;

        void createRegionModule();
        RRM::RegionWidget*   region_parameters_;
        QDockWidget*         dockRegionContainer_;
        QAction * view_region_module_;

        void createFluidModule();
        RRM::FluidWidget*   fluid_parameters_;
        QDockWidget*         dockFluidContainer_;
        QAction * view_fluid_module_;


        QTextEdit up_scaled_;





};

#endif // FLOWWINDOW_H
