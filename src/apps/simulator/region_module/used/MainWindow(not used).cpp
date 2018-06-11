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
        this->region_canvas_ = new RRM::RegionCanvas ( this );
        this->setCentralWidget ( this->region_canvas_ );

        /// Region Scene
        this->region_scene_ = new RRM::RegionScene ( this->region_canvas_->canvasArea(),this->region_canvas_ );
        this->region_scene_->setItemIndexMethod ( QGraphicsScene::NoIndex );
        this->region_scene_->setTranformationMatrix ( this->region_canvas_->getTransformationMatrix() );

        this->region_canvas_->setScene ( this->region_scene_ );

        this->dockContainer_ = new QDockWidget ( QString ( "Region Module" ) , this );
        this->region_parameters_ = new RRM::RegionWidget ( this );

        this->dockContainer_->setWidget ( this->region_parameters_ );
        this->addDockWidget ( Qt::LeftDockWidgetArea , this->dockContainer_ );

        this->label_scene_mouse_coordinates_ = new QLabel("(?,?)");

        this->statusBar()->addPermanentWidget(this->label_scene_mouse_coordinates_);


}

void MainWindow::createWidgetsConnections()
{
        connect ( this->region_parameters_ , &RRM::RegionWidget::numberOfRegions , this->region_scene_ , &RRM::RegionScene::createRegions );
        connect ( this->region_scene_ , &RRM::RegionScene::updatedRegionsPosition , this->region_parameters_ , &RRM::RegionWidget::updateRegionPosition );

        connect ( this->region_scene_ , &RRM::RegionScene::mousePosition , [this](const QPointF &_coordiantes)
                  {
                        this->label_scene_mouse_coordinates_->setText("("+QString::number(_coordiantes.x())+","+QString::number(_coordiantes.y())+")");

                  } );
}
