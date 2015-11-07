#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Modeller/Canvas2D.h"
#include "CanvasComputation.h"
//#include "Canvas3D.h"

#include "Sketching2DModule.h"
#include "FlowComputationModule.h"

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
#include <QDialog>
#include <QToolBox>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStatusBar>


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

        void newSection();
        void changeColorLine();






    private:

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


};

#endif // MAINWINDOW_H
