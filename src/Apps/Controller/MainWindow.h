#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Modeller/Canvas2D.h"
#include "Simulator/canvasComputation.h"

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
#include <QtWidgets/QStatusBar>

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
        void createActionsComputation();
        void createToolbar();



        void create2DModule();
        //void create3DModule();
        void createComputationModule();

        void clearCanvas2D();
//        void clearCanvas3D();
        void clearComputation();
//        void resetToolbar();


        void callComputationElements();
        void createToolbarComputation();

        void createInputDialog();
        void createWidgetInputFiles();
        void createWidgetInputTolerance();


    protected slots:

        void newSection();
        void applyRemoveAbove();
        void applyRemoveBelow();

        void pointerSelection();
        void changeColorLine();

        void doComputation();
        void openUserInputFile();
        void createMeshVolumetric();
        void selectProperty( int id );
        void computePressure();
        void computeVelocity();
        void computeTOF();

        void findUserFile();
        void findSurfaceFile();

        void acceptInputUser();
        void rejectInputUser();

        void selectColorVectorOption( int idc );

    public slots:

        void updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells );





    signals:

        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );

        void selectMode();
        void sketchingMode();

        void sendSurfaceFile( std::string filename );
        void sendInputUserFile( std::string filename );
        void computeVolume();


        void computePressureProperty();
        void computeVelocityProperty();
        void computeTOFProperty();

        void selectFlowProperty( int, bool& );


        void sendInputUser( std::string input_user, std::string surface_file, float tol1, float tol2 );



    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;
        QDockWidget *dc_computation;

        Canvas2D *canvas2D;
        CanvasComputation *canvas_computation;

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

};

#endif // MAINWINDOW_H
