#include "MainWindow.h"

// Skecthing
void MainWindow::create2DModule ( )
{

	// Sketch Module

	glWidget = new GLWidget ( this );

	sketch_board_ = new SketchBoard ();
	// Default Sketch Window
	sketch_board_->newSession(0.0,0.0,700,400);

	dc_2DModule = new Sketching2DModule( this );
	dc_2DModule->setWindowTitle ( "Sketch View" );
	dc_2DModule->setCentralWidget ( sketch_board_ );
	dc_2DModule->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->addDockWidget( Qt::TopDockWidgetArea, dc_2DModule );

	// Seismic Module
	dc_Seismic_Module_ = new Sketching2DModule(this);
	seismic_view_ = new RRM::SeismicWindow(this);
	dc_Seismic_Module_->setWindowTitle("Seismic View");
	dc_Seismic_Module_->setWidget(this->seismic_view_);
	dc_Seismic_Module_->setAllowedAreas(Qt::AllDockWidgetAreas);
	this->addDockWidget(Qt::BottomDockWidgetArea, dc_Seismic_Module_);

	status_bar_ = new QStatusBar ( this );
	status_bar_->addWidget ( sketch_board_->status_text );
	this->setStatusBar ( this->status_bar_ );

	/// Black Screen Module
	connect ( this->sketch_board_ , SIGNAL( currentCrossSection( const CrossSection& ) ) , this->glWidget , SLOT( updateBlackScreen(const CrossSection&) ) );
	/// Seismic Module
	/// Equip the Sketch Board with the current slice CrossSection
	connect ( this->seismic_view_ , SIGNAL( currentCrossSection( const CrossSection&) ) , this->sketch_board_ , SLOT( setCrossSection( const CrossSection&) ) );
	/// Notify the Seismic Module with the new CrossSection configuration
	connect ( this->sketch_board_ , SIGNAL( currentCrossSection( const CrossSection& ) ) , this->seismic_view_ , SLOT( updateCrossSection( const CrossSection& ) ) );
	/// Notify the 3D the current slice position
	connect ( this->seismic_view_->ui->seismic_slices_verticalSlider_ , SIGNAL( valueChanged(int)) , this->glWidget , SLOT( setPlanePosition(int) ) );
	/// Notify the 3D View to update the surfaces
	connect ( this->seismic_view_->ui->seismic_pushButton_interpolate , SIGNAL( clicked(bool) ) , this , SLOT( interpolate() ) );
	glWidget->updateSeismicSlices(this->seismic_view_->getSeimicSlices());

	/// Notify the 3D View to update the surfaces
	connect ( this->seismic_view_->ui->seismic_pushButton_clear_ , SIGNAL( pressed() ) , this , SLOT( clear() ) );

	glWidget->updateSeismicSlices(this->seismic_view_->getSeimicSlices());

}

void MainWindow::interpolate( )
{
	std::cout << this->seismic_view_->getSeimicSlices().size() << std::endl;

	this->seismic_view_->updateCrossSection(this->sketch_board_->sketch_controller->getCrossSection());

	glWidget->updateSeismicSlices(this->seismic_view_->getSeimicSlices());
	glWidget->updateRendering();
}

void MainWindow::clear()
{
	glWidget->clear();
}

void MainWindow::create3DModule ( )
{
	dc_3DModule = new View3DModule ( this );
	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
	dc_3DModule->setWindowTitle ( "3D View" );

	addDockWidget ( Qt::BottomDockWidgetArea, dc_3DModule );

    dc_3DModule->hb_canvas3d->addWidget ( glWidget );
	dc_3DModule->setAllowedAreas(Qt::AllDockWidgetAreas);

    connect ( this->dc_3DModule->horizontalSlider_curve, SIGNAL( valueChanged(int) ) , this->glWidget , SLOT( black_screen_stepx(int) ) );
    connect ( this->dc_3DModule->horizontalSlider_surface, SIGNAL( valueChanged(int) ) , this->glWidget , SLOT( black_screen_stepz(int) ) );
    connect ( this->dc_3DModule->horizontalSlider_extrusion, SIGNAL( valueChanged(int) ) , this->glWidget , SLOT( black_screen_volumeWidth(int) ) );

}

void MainWindow::keyPressEvent ( QKeyEvent *event )
{
	event->ignore();
}

void MainWindow::mousePressEvent ( QMouseEvent* event )
{
	event->ignore();
}
