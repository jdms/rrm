/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */


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
