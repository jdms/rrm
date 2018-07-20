#include "view3dinterface.h"

#include "mainwindow1.h"

View3dInterface::View3dInterface( MainWindow1* const& window_ )
{
  window = window_;
}


void View3dInterface::createInterface()
{
  if( window == nullptr ) return;

  createView3dWindow();
  createView3dActions();

}


void View3dInterface::createView3dWindow()
{

  controller3d = new View3dController();
  canvas3d = new Canvas3d();
  canvas3d->show();

  sl_depth_csection = new RealFeaturedSlider( Qt::Vertical );
  sl_depth_csection->setDiscretization( 500 );
  sl_depth_csection->setRange( 0, 500 );
  sl_depth_csection->setValue( 500 );

  QHBoxLayout* hb_central_widget = new QHBoxLayout();
  hb_central_widget->addWidget( canvas3d );
  hb_central_widget->addWidget( sl_depth_csection );

  QWidget* central_widget = new QWidget();
  central_widget->setLayout( hb_central_widget );

  window->setCentralWidget( central_widget );

}


void View3dInterface::createView3dActions()
{

}


void View3dInterface::init()
{
    controller3d->setMainWindow( std::shared_ptr< Canvas3d > ( canvas3d ) );
    controller3d->setController( std::shared_ptr< Controller > ( window->controller ) );
    controller3d->init();
}
