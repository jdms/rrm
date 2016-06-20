#ifndef FLOWWINDOW_H
#define FLOWWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QProgressBar>
#include <QKeyEvent>

#include "FlowParametersBar.h"
#include "OpenFlowFilesBar.h"
#include "NormalMovableCrossSectionFlow.H"
#include "FlowVisualizationCanvas.h"
#include "DialogMeshVisualizationParameters.h"

#include "Model/CrossSection.hpp"

class FlowWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit FlowWindow( QWidget *parent = 0 );

        void createWindow();
        void createToolBar();
        void createActions();

        void updateParameterFields();
        void getCurrentDirectory();

        inline void setCrossSection( const RRM::CrossSection<qreal>& c){ controller->getSurfaceFromCrossSection( c ); }

        void keyPressEvent( QKeyEvent *event );


    public slots:

        void startProgressBar( const unsigned int& min, const unsigned int& max );
        void updateProgressBar( const unsigned int& value );

        void exportSurfaceFile();
        void exportVolumeFile();
        void exportCornerPointFile();

        void updateVisualizationParameters();


    signals:

        void getSurfaceCrossSection();
		void getCrossSection();

    private:

        QToolBar *qtoolbarFlow;
        QAction *qoopenfilesDialog;
        QAction *qflowparametersDialog;
        QAction *qbuildvolumetricMesh;
        QAction *qcomputeFlowProperties;
        QAction *qshowMovingCrossSection;
        QAction* qmeshresolution;
        QAction* qreloadSurface;
        QAction *qclear;

        QAction *qexportsurface;
        QAction *qexportvolume;
        QAction *qexportcornerpoint;


        QDockWidget *qdockparametersBar;
        FlowParametersBar parametersBar;


        QDockWidget *qdockopenfilesBar;
        OpenFlowFilesBar openfilesBar;

        FlowVisualizationCanvas canvas;
        FlowVisualizationController *controller;


        QDockWidget * qdockcrosssectionnormalBar;
        NormalMovableCrossSectionFlow crosssectionnormalBar;

        QDockWidget* qdockresolutionmesh;
        DialogMeshVisualizationParameters resolutionmeshBar;

        QStatusBar* sb_statusbar;
        QProgressBar* pb_processprogress;

        std::string file_of_parameters;
        std::string file_of_mesh;
        std::string type_of_file;
        bool read_file;
        int mesh_method;

        bool show_toolbar;
};

#endif // FLOWWINDOW_H
