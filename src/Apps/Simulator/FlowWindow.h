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

#include "NormalMovableCrossSectionFlow.H"
#include "FlowVisualizationCanvas.h"
#include "ColorBar.h"
#include "PoreVolumeResultsForm.h"

#include "FlowParametersBar.h"

class FlowWindow : public  QMainWindow
{
    Q_OBJECT

    public:

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

        void readUserInputFile( const std::string& input_file );

        void startProgressBar( const unsigned int& min, const unsigned int& max );
        void updateProgressBar( const unsigned int& value );


        void addVertexProperty( std::string name, std::string dimension );
        void addFaceProperty( std::string name, std::string dimension );

        void clearPropertiesMenu();

        void buildUnstructured();
        void buildCornerPoint();

        void acceptUserParameters();
        void clear();

		void regionPoints(const std::map<int, Eigen::Vector3f>& region_points);
		/// Will emit a signal sendNumberOfRegions
		int getNumberOfRegions();

        void setVolumeDimensions( double width_, double height_, double depth_ )
        {
            controller->setVolumeDimensions( width_, height_, depth_ );
        }
		
    signals:

        void getSurfaceCrossSection();
        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
		void sendNumberOfRegions(int _number_of_regions);

    private:

        QToolBar *qtoolbarFlow;
        QAction *qoopenfilesDialog;
        QAction *qflowparametersDialog;
        QAction *qcomputeFlowProperties;
        QAction *qshowMovingCrossSection;
        QAction* qreloadSurface;
        QAction *qclear;

        QAction* qbuildCornerPoint;
        QAction* qbuildUnstructured;

        QAction *qexportsurface;
        QAction *qexportvolume;
        QAction *qexportcornerpointVTK;
        QAction *qexportcornerpointGRDECL;
        QAction* qexportresults;


        QAction* ac_cornerpoint;
        QAction* ac_unstructured;

        QMenu *mn_export;
        QToolButton* tbn_export;


        QDockWidget *qdockparametersBar;
        FlowParametersBar parametersBar;


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
        QMenu* mn_coloring_byvertex;
        QMenu* mn_coloring_byfaces;

        std::vector< QMenu *> mn_vectorsproperties_byvertex;
        std::vector< QRadioButton *> rd_vectormethods_byvertex;
        QWidgetAction* wa_vectormethods_byvertex;
        std::vector<QAction* > ac_vertex_property;


        std::vector< QMenu *> mn_vectorsproperties_byface;
        std::vector< QRadioButton *> rd_vectormethods_byface;
        QWidgetAction* wa_vectormethods_byface;
        std::vector<QAction* > ac_face_property;


        QHBoxLayout *hb_mainwindow;
        ColorBar colorbar;
        QToolButton* tbn_colormaps;
        QMenu* mn_colormaps;

        QAction* ac_constant;
		QAction* ac_cool_to_warm;
        QAction* ac_hot;
        QAction* ac_cool;
        QAction* ac_parula;
        QAction* ac_spring;
        QAction* ac_summer;
        QAction* ac_copper;
        QAction* ac_polar;
        QAction* ac_winter;
		QAction* ac_jet;

        QAction* ac_showregions;

        PoreVolumeResultsForm porevolumeform;
        bool are_regionsdefined;
        bool is_cornerpoint;


};

#endif // FLOWWINDOW_H
