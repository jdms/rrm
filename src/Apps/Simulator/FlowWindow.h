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

#include "NormalMovableCrossSectionFlow.H"
#include "FlowVisualizationCanvas.h"
#include "ColorBar.h"
#include "PoreVolumeResultsForm.h"

#include "FlowParametersBar_new.h"

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
		
    signals:

        void getSurfaceCrossSection();
        size_t getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
//		void   get2Dto3DMatrix(Eigen::Affine3f&);
//		void   get3Dto2DMatrix(Eigen::Affine3f&);
		void getNumberOfRegions(int _number_of_regions);

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
        FlowParametersBar_new parametersBar;


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


};

#endif // FLOWWINDOW_H
