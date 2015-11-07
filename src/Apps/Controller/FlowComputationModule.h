#ifndef FLOWCOMPUTATIONMODULE_H
#define FLOWCOMPUTATIONMODULE_H

#include "CanvasComputation.h"
#include "DialogInputFlowComputation.h"

#include <QDockWidget>
#include <QFrame>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QDialog>
#include <QToolBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QToolBar>
#include <QString>
#include <QStringList>

class FlowComputationModule: public QDockWidget
{
    Q_OBJECT

    public:

        FlowComputationModule();

    protected:

        void createWindow();
        void createActions();
        void createToolbar();


    public slots:

        void acceptInputUser();
        void updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells );


    protected slots:

        void createMeshVolumetric();
        void computePressure();
        void computeVelocity();
        void computeTOF();
        void selectProperty( int id );
        void selectColorVectorOption( int idc );


    signals:

        void computeVolume();
        void computePressureProperty();
        void computeVelocityProperty();
        void computeTOFProperty();
        void selectFlowProperty( int, bool& );
        void sendInputUser( std::string input_user, std::string surface_file, float tol1, float tol2 );
        void sendSurfaceFile( std::string );


    private:

        CanvasComputation *canvas_computation;
        QMainWindow* mw_canvas_computation;


        QAction *ac_compute;
        QAction *ac_compute_pressure;
        QAction *ac_compute_velocity;
        QAction *ac_compute_tof;

        QToolBar *tlb_workflow_flow;
        QAction *ac_open_surface;
        QAction *ac_compute_volumetric;
        QAction *ac_open_userinput;
        QComboBox *cb_compute_property;
        QComboBox *cb_coloroption_vector;

        DialogInputFlowComputation* dg_inputuser;
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
        QAction *ac_flowcomputation;

};

#endif // FLOWCOMPUTATIONMODULE_H
