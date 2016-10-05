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


#include "MainWindow/About/AboutWidget.hpp"

#include "3dView/View3DWindow.hpp"
#include "Sketching/SketchingWindow.h"

#include "HelpDialog.h"
#include "Controller.hpp"


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
        void createActions();
        void createMenuBar();
        void createMainWindowActions();

        void create3DViewModule();
        void create3DWindowMenuBar();
        void create3DWindowActions();

        void createSketchingModule();
        void createSketchingMenuBar();
        void createSketchingActions();



    protected slots:

        void clear();


    public slots:

        void initProgram();


    protected:


        Scene* scene;
        Controller* controller;

        bool scene_initialized;



        QMenu *mn_file;
        QAction *ac_exit;

        QMenu *mn_help;
        QAction *ac_contents;
        QAction *ac_about;

        QMenu *mn_windows;
        QAction *ac_wdwsketching;
        QAction *ac_wdwseismic;
        QAction *ac_3dview;


        // Sketching Module
        QDockWidget* dw_sketching;
        SketchingWindow *sketching_window;


        // 3D View Module
        QDockWidget* dw_3dview;
        View3DWindow *view3d_window;


        HelpDialog help;
        AboutWidget * aboutRRM;





};



#endif // MAINWINDOW_H
