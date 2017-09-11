#ifndef _MYMAINWINDOW_
#define _MYMAINWINDOW_

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>

#include "AboutWidget/AboutWidget.hpp"
#include "WellCanvas.hpp"
#include "WellScene.hpp"
#include "WellWidget.hpp"

#include "ui_Wells.h"

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
                RRM::WellCanvas*         well_canvas_;
                RRM::WellScene*          well_scene_;

                AboutWidget*        aboutWidget_;

                QDockWidget*        dockContainer_;
                RRM::WelWidget*   well_parameters_;

};


#endif
