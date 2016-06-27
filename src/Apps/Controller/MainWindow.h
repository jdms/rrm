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

#include "Modeller/ExtrusionWidget/OpenGLWidget.hpp"
#include "Modeller/ExtrusionWidget/View3DModule.hpp"
#include "Modeller/Sketching2DModule.h"

#include "SBIM/View/SeismicWindow.hpp"

// Flow Simulator
#include "Simulator/FlowWindow.h"


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

		// MainWindow Releated Widgets
        void createWindow();
        void createMenuBar();
        void createMainWindowMenuBar();
        void createMainWindowToolbar();
        void createMainWindowActions();

		// Windows 3D Module Releated Widgets
        void create3DModule();
        void create3DWindowMenuBar();
        void create3DWindowActions();

		// SBIM Releated Module Widgets
        void create2DModule();
        void createSketchingMenuBar();
        void createSketchingToolbar();
        void createSketchingActions();
		
		// FLow Module Releated Widgets
        void createFlowComputationModule();
        void createFlowComputationMenuBar();
        void createFlowComputationActions();

    protected slots:
    	// CrossSection
    	void clear();
        void changeColorLine();

        // Seismic
        void interpolate();

        void keyPressEvent ( QKeyEvent *e );
        void mousePressEvent(QMouseEvent* event);


    private:
		// Information about the Software
		AboutWidget * aboutRRM;

		//QStatusBar			*status_bar_;

		QMenu *mn_file;
	        QAction *ac_exit;

		QMenu *mn_help;
			QAction *ac_about;
			                		
        QMenu *mn_windows;
	        QAction *ac_wdwsketching;
			QAction *ac_wdwseismic;
			QAction *ac_window3d;
			QAction *ac_flowcomputation;
	
					
        // Sketch Module CrossSection
        Sketching2DModule *dc_2DModule;
                
		// Seismic Module
		Sketching2DModule *dc_Seismic_Module_;
			RRM::SeismicWindow * seismic_view_;

		// 3D Extrusion
        View3DModule *dc_3DModule;
			GLWidget *glWidget;
		
		// Flow Module
		QDockWidget *dc_computation;
			FlowWindow *flowwindow;
};

#endif // MAINWINDOW_H
