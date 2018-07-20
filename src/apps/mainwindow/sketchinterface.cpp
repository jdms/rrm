#include "sketchinterface.h"

#include "mainwindow1.h"

SketchInterface::SketchInterface( MainWindow1* const& window_ )
{
  window = window_;
}


void SketchInterface::createInterface()
{
  if( window == nullptr ) return;

  createSketchingWindow();
  createSketchingActions();
}



void SketchInterface::createSketchingWindow()
{

  scontroller = new SketchingController();

  sketch_window = new SketchWindow();
  dw_sketchwindow = new QDockWidget( "Cross-Section" );
  dw_sketchwindow->setAllowedAreas( Qt::AllDockWidgetAreas );
  dw_sketchwindow->setWidget( sketch_window );

  window->addDockWidget( Qt::BottomDockWidgetArea, dw_sketchwindow );

  sketch_topview_window = new SketchWindow();
  dw_topview_window = new QDockWidget( "Top-View" );
  dw_topview_window->setAllowedAreas( Qt::AllDockWidgetAreas );
  dw_topview_window->setWidget( sketch_topview_window );

  window->addDockWidget( Qt::BottomDockWidgetArea, dw_topview_window );

}

void SketchInterface::createSketchingActions()
{

  ac_csection = new QAction ( "Cross-Section Window", window );
  ac_csection->setCheckable ( true );
  ac_csection->setChecked ( Settings::Application::DEFAULT_CSECTION_VISIBILITY );

  ac_topview = new QAction ( "Top-View Window", window );
  ac_topview->setCheckable ( true );
  ac_topview->setChecked ( Settings::Application::DEFAULT_TOPVIEW_VISIBILITY );

  window->mn_windows->addAction( ac_csection );
  window->mn_windows->addAction( ac_topview );


}



void SketchInterface::init()
{
    scontroller->setMainWindow( std::shared_ptr< SketchWindow > ( sketch_window ) );
    scontroller->setTopViewWindow( std::shared_ptr< SketchWindow > ( sketch_topview_window ) );
    scontroller->setController( std::shared_ptr< Controller > ( window->controller ) );
    scontroller->init();
}
