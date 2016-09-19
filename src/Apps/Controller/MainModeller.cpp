#include "MainWindow.h"

// Skecthing
void MainWindow::create2DModule ( )
{
	// Sketch Module
//	glWidget = new GLWidget ( this );


    dc_2DModule = new Sketching2DModule( this );
	
	// Seismic Module
//	dc_Seismic_Module_ = new Sketching2DModule(this);
//	seismic_view_ = new RRM::SeismicWindow(this);
//	dc_Seismic_Module_->setWindowTitle("Seismic View");
//	dc_Seismic_Module_->setWidget(this->seismic_view_);
//	dc_Seismic_Module_->setAllowedAreas(Qt::AllDockWidgetAreas);
//	this->addDockWidget(Qt::BottomDockWidgetArea, dc_Seismic_Module_);
//	dc_Seismic_Module_->setVisible(false);







//	glWidget->updateSeismicSlices(this->seismic_view_->getSeimicSlices());

}


void MainWindow::create3DModule ( )
{
	dc_3DModule = new View3DModule ( this );
//	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
    dc_3DModule->setWindowTitle ( "3D View" );

//	addDockWidget ( Qt::BottomDockWidgetArea, dc_3DModule );

//    dc_3DModule->hb_canvas3d->addWidget ( glWidget );



}


