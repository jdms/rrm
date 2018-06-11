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



#include <iostream>
#include "MainWindow.hpp"

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
        // Centrall Widget - The Canvas Area
        this->well_canvas_ = new RRM::WellCanvas ( this );
        this->setCentralWidget ( this->well_canvas_ );

        /// Well Scene
        this->well_scene_ = new RRM::WellScene ( this->well_canvas_->canvasArea(),this->well_canvas_ );
        this->well_scene_->setItemIndexMethod ( QGraphicsScene::NoIndex );
        this->well_scene_->setTranformationMatrix ( this->well_canvas_->getTransformationMatrix() );

        this->well_canvas_->setScene ( this->well_scene_ );

        this->dockContainer_ = new QDockWidget ( QString ( "Well Module" ) , this );
        this->well_parameters_ = new RRM::WidgetWells ( this );

        this->dockContainer_->setWidget ( this->well_parameters_ );
        this->addDockWidget ( Qt::LeftDockWidgetArea , this->dockContainer_ );

        this->well_scene_->createWells(2);
}

void MainWindow::createWidgetsConnections()
{
        connect ( this->well_parameters_ , &RRM::WidgetWells::numberOfWells , this->well_scene_ , &RRM::WellScene::createWells );
        connect ( this->well_scene_ , &RRM::WellScene::updatedWellsPosition , this->well_parameters_ , &RRM::WidgetWells::updateWellPosition );
}
