#ifndef _MYMAINWINDOW_
#define _MYMAINWINDOW_

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>

#include "AboutWidget/AboutWidget.hpp"
#include "FluidWidget.hpp"
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
                AboutWidget*        aboutWidget_;

                QDockWidget*        dockContainer_;
                RRM::FluidWidget*   fluid_parameters_;
        private:



};


#endif
