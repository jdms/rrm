#ifndef _MYMAINWINDOW_
#define _MYMAINWINDOW_

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>

#include "AboutWidget/AboutWidget.hpp"
#include "RegionCanvas.hpp"
#include "RegionScene.hpp"
#include "RegionWidget.hpp"
#include "ui_MainWindowForm.h"

class MainWindow: public QMainWindow
{
        Q_OBJECT

        public:
                MainWindow ( QMainWindow *parent = 0 );
                virtual ~MainWindow ( ) = default;
                void keyPressEvent ( QKeyEvent *e );

        public slots:

        private:

                void createWidgets();
                void createWidgetsConnections();

                Ui::MainWindow*     ui_;
                RRM::RegionCanvas*         region_canvas_;
                RRM::RegionScene*          region_scene_;

                AboutWidget*        aboutWidget_;

                QDockWidget*        dockContainer_;
                RRM::RegionWidget*   region_parameters_;
        private:

                QLabel* label_scene_mouse_coordinates_;

};


#endif
