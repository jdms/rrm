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
	dc_2DModule->setWidget ( sketch_board_ );
	this->addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

	// Seismic Module
	seismic_Module_ = new Sketching2DModule( this );
	seismic_view_ = new RRM::SeismicWindow(this);
	seismic_Module_->setWindowTitle ( "Seismic View" );
	seismic_Module_->setWidget(this->seismic_view_);
	this->addDockWidget ( Qt::LeftDockWidgetArea , seismic_Module_ );

	status_bar_ = new QStatusBar ( this );
	status_text = new QLabel ( "Sketch" , this );
	status_bar_->addWidget ( status_text );
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

	//connect ( this->seismic_view_ , SIGNAL( currentSeismicSlices( const SeismicSlices&) ) , this->glWidget , SLOT( updateSeismicSlices( const SeismicSlices&) ) );

	// Sketching
//	connect ( this->sketchSession_ , SIGNAL( curve2DSignal(QPolygonF) ) , this , SLOT( curve2DSlot(QPolygonF) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( smoothSketchSignal(QPolygonF) ) , this , SLOT( smoothCurveSlot(QPolygonF) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(QPixmap) ) , this , SLOT( newSessionSlot(QPixmap) ) );
//
//	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(qreal,qreal,qreal,qreal) ) , this , SLOT( newSessionSlot(qreal,qreal,qreal,qreal) ) );
}

void MainWindow::interpolate( )
{
	std::cout << this->seismic_view_->getSeimicSlices().size() << std::endl;

	this->seismic_view_->updateCrossSection(this->sketch_board_->sketch_controller->getCrossSection());

	glWidget->updateSeismicSlices(this->seismic_view_->getSeimicSlices());
	glWidget->updateRendering();
}

void MainWindow::create3DModule ( )
{
	dc_3DModule = new View3DModule ( this );
	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
	dc_3DModule->setWindowTitle ( "3D View" );

	addDockWidget ( Qt::RightDockWidgetArea , dc_3DModule );

        dc_3DModule->hb_canvas3d->addWidget ( glWidget );

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

void MainWindow::on_horizontalSlider_curve_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_surface_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_extrusion_valueChanged()
{
	//update3DExtrusion();
}

