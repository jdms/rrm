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
