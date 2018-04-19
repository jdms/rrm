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



#include "MainWindow.hpp"

#include <iostream>

MainWindow::MainWindow ( QMainWindow *parent ) : QMainWindow ( parent )
{
        QIcon icon;
        icon.addFile ( ":/icons/rrm.png" , QSize ( ) , QIcon::Normal , QIcon::Off );
        setWindowIcon ( icon );

        this->ui_ = new Ui::MainWindow;
        this->ui_->setupUi ( this );

        this->aboutWidget_ = new AboutWidget ( this );

        connect ( this->ui_->action_About_ , &QAction::triggered , aboutWidget_ , &AboutWidget::show );

        this->createWidgets();
        this->createWidgetsConnections();


}

void MainWindow::keyPressEvent ( QKeyEvent *e )
{

}

void MainWindow::createWidgets()
{
        this->dockContainer_ = new QDockWidget ( QString ( "Fluid Module" ) , this );
        this->fluid_parameters_ = new RRM::FluidWidget ( this );

        this->dockContainer_->setWidget ( this->fluid_parameters_ );
        this->addDockWidget ( Qt::LeftDockWidgetArea , this->dockContainer_ );

}

void MainWindow::createWidgetsConnections()
{

}
