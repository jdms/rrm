#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QColorDialog>
#include <QWidgetAction>
#include <QToolButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>

#include "Canvas2D.h"
#include "Canvas3D.h"
#include "canvasComputation.h"


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
//        void clearCanvas3D();
        void clearComputation();
//        void resetToolbar();

        void createToolbarComputation();


    protected slots:

        void newSection();
        void applyRemoveAbove();
        void applyRemoveBelow();

        void pointerSelection();
        void changeColorLine();

        void doComputation();
        void openSurfaceFile();
        void createMeshVolumetric();
        void selectProperty( int id );
        void computePressure();
        void computeVelocity();
        void computeTOF();


    public slots:

        void updateComboBox( std::vector< std::string > ppoints, std::vector< std::string > pcells );





    signals:
        void applyremoveabove();
        void applyremovebelow();
        void setColor( int R, int G, int B );

        void selectMode();
        void sketchingMode();

        void sendSurfaceFile( std::string filename );
        void computeVolume();


        void computePressureProperty();
        void computeVelocityProperty();
        void computeTOFProperty();

        void selectFlowProperty( int, bool& );



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
        QComboBox *cb_compute_property;
        QComboBox *cb_coloroption_vector;

        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

};

#endif // MAINWINDOW_H
