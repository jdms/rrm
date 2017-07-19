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
