#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QToolBar>
#include <QColorDialog>
#include <QWidgetAction>
#include <QToolButton>

#include "Canvas2D.h"
#include "Canvas3D.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow( QWidget *parent = 0 );


    protected:

        void createWindow();
        void createMenuBar();
        void createActions();
        void createToolbar();

        void create2DModule();
        void create3DModule();

        void clearCanvas2D();

    protected slots:

        void newSection();

//        void clearCanvas3D();
//        void clearComputation();



    private:

        QDockWidget *dc_2DModule;
        QDockWidget *dc_3DModule;

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

        QAction *ac_select;
        QAction *ac_select1;
        QAction *ac_grab;
        QWidgetAction *ac_sketchcolor;


        QToolBar *tlb_section;
        QToolBar *tlb_computation;
        QToolBar *tlb_rules;
        QToolBar *tlb_interaction;
        QToolBar *tlb_customization;

        QMenu *mn_pickercolor;
        QColorDialog *cd_pickercolor;
        QToolButton *tbt_colorsketch;

};

#endif // MAINWINDOW_H
