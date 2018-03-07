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
