#include "MainWindow.h"

void MainWindow::createFlowComputationModule ( )
{

	 dc_computation = new QDockWidget ( );
	
	 flowwindow = new FlowWindow();
	 dc_computation->setWidget(flowwindow);
	 dc_computation->setAllowedAreas(Qt::AllDockWidgetAreas);

     dc_computation->setWindowTitle( "Flow Diagnostic View" );
     dc_computation->setVisible( false );
	 addDockWidget ( Qt::RightDockWidgetArea , dc_computation );
}

void MainWindow::createFlowComputationMenuBar ( )
{
	mn_windows->addAction ( ac_flowcomputation );
	ac_flowcomputation->setCheckable ( true );
}

void MainWindow::createFlowComputationActions ( )
{
	// flow computation
	ac_flowcomputation = new QAction ( tr ( "Window Flow Computation" ) , this );
	ac_flowcomputation->setCheckable ( true );

	connect ( ac_flowcomputation , SIGNAL( toggled(bool) ) , dc_computation , SLOT( setVisible(bool) ) );
	connect(dc_computation, &QDockWidget::visibilityChanged, ac_flowcomputation, &QAction::setChecked);
	connect(flowwindow, &FlowWindow::getCrossSection, this, [=](){  flowwindow->setCrossSection(dc_2DModule->sketch_board_->sketch_controller->getCrossSection());  });
}


