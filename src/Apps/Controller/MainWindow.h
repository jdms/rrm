#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @file MainWindow.h
 * @author Felipe Moura de Carvalho
 * @date Sep, 2015
 * @brief RRM's Main GUI.
 */


#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDialog>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QStyle>

// About
#include "Controller/About/AboutWidget.hpp"
// Sketching Surface
#include "Model/CrossSection.hpp"

//#include "Modeller/ExtrusionWidget/OpenGLWidget.hpp"
#include "Modeller/ExtrusionWidget/View3DModule.hpp"
#include "Modeller/Sketching2DModule.h"

#include "SBIM/View/SeismicWindow.hpp"

// Flow Simulator
#include "Simulator/FlowWindow.h"
#include "Simulator/HelpDialog.h"


/**
 * @brief Main GUI and Controller of the Project
 * GUI representing the Main Window of the Project.
 * It also responsible make a bridge between Sketch Module and Extrusion Module
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

    	typedef  RRM::CrossSection<qreal>  CrossSection;

        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

    protected:

        void createWindow();
        void createActions();
        void createMenuBar();
        void createMainWindowActions();

        void createSeismicModule();
        void createSeismicgActions();

        void createFlowDiagnosticsModule();
        void createFlowDiagnosticsMenuBar();
        void createFlowDiagnosticsActions();


        void create3DViewModule();
        void create3DWindowMenuBar();
        void create3DWindowActions();

        void createSketchingModule();
        void createSketchingMenuBar();
        void createSketchingActions();



    protected slots:

    	// CrossSection
    	void clear();

        // Seismic
        void interpolate();

        void keyPressEvent ( QKeyEvent *e );
        void mousePressEvent(QMouseEvent* event);


    private:

        // Information about the Software
		AboutWidget * aboutRRM;

        QMenu *mn_file;
        QAction *ac_exit;

        QMenu *mn_help;
        QAction *ac_contents;
        QAction *ac_about;

        QMenu *mn_windows;
        QAction *ac_wdwsketching;
        QAction *ac_wdwseismic;
        QAction *ac_3dview;
        QAction *ac_flowdiagnostics;


        // Sketching Module
        QDockWidget* dw_sketching;
        Sketching2DModule *dc_Sketching_Module;

        // Seismic Module
        Sketching2DModule *dc_Seismic_Module;
        RRM::SeismicWindow * seismic_window;

        // 3D View Module
        QDockWidget* dw_3dview;
        View3DModule *dc_3DView_Module;

        // Flow Diagnostics Module
        QDockWidget* dw_flowdiagnostics;
        FlowWindow *flowdiagnostics_window;


        HelpDialog help;





};



#endif // MAINWINDOW_H
