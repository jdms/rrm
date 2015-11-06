#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QHBoxLayout>
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

/// CrossSection
#include "Model/CrossSection.hpp"

#include "Modeller/ExtrusionWidget/OpenGLWidget.hpp"

#include "Modeller/Canvas2D.h"

 /// ZHAO -----// ZHAO
#include "Simulator/canvasComputation.h"
 /// ZHAO -----// ZHAO

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );
        ~MainWindow();

    protected:

        void createWindow();
        void createMenuBar();
        void createActions();
        void createToolbar();

        void create2DModule();
        void create3DModule();
        void createComputationModule();

        void clearCanvas2D();

        void createToolbarComputation();


    protected slots:

        void newSection();
        void applyRemoveAbove();
        void applyRemoveBelow();

        void pointerSelection();
        void changeColorLine();

    public slots:

        //void addCurve( QPolygonF& curve);

        //void updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells );

    signals:


    	/// CrossSection
        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );

        void selectMode();
        void sketchingMode();

    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;
        QDockWidget *dc_computation;

        Canvas2D *canvas2D;

        QAction *ac_new;
        QAction *ac_open;
        QAction *ac_save;
        QAction *ac_export;
        QAction *ac_exit;

        QAction *ac_compute;

        QAction *ac_contents;
        QAction *ac_about;

        QAction *ac_removeabove;
        QAction *ac_removebelow;

        QAction *ac_compute_pressure;
        QAction *ac_compute_velocity;
        QAction *ac_compute_tof;

        QAction *ac_select;
        QWidgetAction *ac_sketchcolor;

        QToolBar *tlb_section;
        QToolBar *tlb_computation;
        QToolBar *tlb_rules;
        QToolBar *tlb_interaction;
        QToolBar *tlb_customization;

        QMainWindow *mw_canvas_computation;
        QToolBar *tlb_workflow_flow;
        QAction *ac_open_surface;
        QAction *ac_compute_volumetric;
        QAction *ac_open_userinput;
        QComboBox *cb_compute_property;
        QComboBox *cb_coloroption_vector;

        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

        QDialog* dg_inputuser;
        QToolBox* tb_inputuser;

        QWidget *wd_inputfiles;
        QWidget *wd_inputtolerance;

        QLineEdit *edt_userfile;
        QPushButton *btn_finduserfile;
        QLineEdit *edt_surfacefile;
        QPushButton *btn_findsurfacefile;
        QLineEdit *edt_tolerance1;
        QLineEdit *edt_tolerance2;

        QDialogButtonBox *btns_inputdialog;

        QAction *ac_wdwsketching;
        QAction *ac_window3d;
        QAction *ac_flowcomputation;

        /// CrossSection
        RRM::CrossSection<qreal> cross_section_;

        /// Sketching Surface and Extrusion
        GLWidget *glWidget;

};

#endif // MAINWINDOW_H
