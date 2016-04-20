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
#include "Modeller/SketchBoardWidget/SketchBoard.hpp"
#include "Modeller/Sketching2DModule.h"

#include "SBIM/View/SeismicWindow.hpp"
// Simulator Volume
#include "Simulator/CanvasComputation.h"
#include "Simulator/FlowComputationModule.h"

/**
 * @brief Main GUI and Controller of the Project
 * GUI representing the Main Window of the Project.
 * It also responsible make a bridge between Sketch Module and Extrusion Module
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

    protected:

        void createWindow();
        void createMenuBar();
        void createMainWindowMenuBar();
        void createMainWindowToolbar();
        void createMainWindowActions();

        void create3DModule();
        void create3DWindowMenuBar();
        void create3DWindowActions();


        void create2DModule();
        void createSketchingMenuBar();
        void createSketchingToolbar();
        void createSketchingActions();


        void createFlowComputationModule();
        void createFlowComputationMenuBar();
        void createFlowComputationActions();



    protected slots:

//        void newSection();
        void changeColorLine();

        // CrossSection
        void keyPressEvent ( QKeyEvent *e );
        void mousePressEvent(QMouseEvent* event);

    public slots:


	void on_horizontalSlider_curve_valueChanged();
	void on_horizontalSlider_surface_valueChanged();
	void on_horizontalSlider_extrusion_valueChanged();

    private:

	AboutWidget * aboutRRM;

        Sketching2DModule *dc_2DModule;
        QDockWidget *dc_3DModule;
        FlowComputationModule *dc_computation;

        QAction *ac_new;
        QAction *ac_open;
        QAction *ac_save;
        QAction *ac_export;
        QAction *ac_exit;
        QAction *ac_contents;
        QAction *ac_about;


        QToolBar *tlb_section;
        QToolBar *tlb_rules;
        QToolBar *tlb_interaction;
        QToolBar *tlb_customization;

        QMenu *mn_file;
        QMenu *mn_sketching;
        QMenu *mn_windows;
        QMenu *mn_help;

        QAction *ac_removeabove;
        QAction *ac_removebelow;
        QAction *ac_select;
        QAction *ac_wdwsketching;
        QWidgetAction *ac_sketchcolor;
        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;


        QAction *ac_compute;
        QAction *ac_flowcomputation;
        QToolBar *tlb_computation;


        QAction *ac_window3d;


        // CrossSection ////////////////////////////////////////////////////////////////////

        RRM::CrossSection<qreal>  	cross_section__;

        QStatusBar			*status_bar_;
        	QLabel			*status_text;

        QAction *ac_newBoundary;
        // Sketching Surface and Extrusion
        GLWidget *glWidget;
        Eigen::Vector3f min_;
        Eigen::Vector3f max_;

	// Sketch Views
	SketchBoard  		*sketch_board_;

	// SketchController
	SketchController  	*sketch_controller_;

	    QGroupBox *groupBox;
	    QFormLayout *formLayout;
	    QSlider *horizontalSlider_curve;
	    QSlider *horizontalSlider_surface;
	    QSlider *horizontalSlider_extrusion;
	    QLabel *label;
	    QLabel *label_2;
	    QLabel *label_3;

	RRM::SeismicWindow * seismic_view_;
};

#endif // MAINWINDOW_H
