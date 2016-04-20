/*
 * SeismicView.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicWindow.hpp"

namespace RRM
{
	// The parent destructor are responsible to delete all children
	// @see http://stackoverflow.com/questions/2491707/memory-management-in-qt

	SeismicWindow::SeismicWindow ( QWidget* parent ) : QWidget(parent)
	{
		Qt::WindowFlags flags = Qt::Window;

		this->setWindowFlags(flags);

		this->seismic_viewer_ = new SeismicView(this);

		this->ui = new Ui::SeismicWindowWidget();
		this->ui->setupUi ( this );


		ui->seismic_viewer_frame_->insertWidget(0,this->seismic_viewer_);

		/// Added new slice
		connect(this->ui->pushButton_AddSlice, SIGNAL (pressed()),this, SLOT(addSeismicSlice()) );
	}

	SeismicWindow::~SeismicWindow ( )
	{
		delete ui;
	}

	void SeismicWindow::wheelEvent ( QWheelEvent* event )
	{

	}

	void SeismicWindow::keyPressEvent ( QKeyEvent* event )
	{

	}

	void SeismicWindow::fitInView( )
	{
		this->seismic_viewer_->fitInView(this->seismic_viewer_->sceneRect(), Qt::KeepAspectRatio);
	}

	bool SeismicWindow::addSeismicSlice (  )
	{
		QLabel* l = new QLabel("Number 1",this);

		this->ui->layout_slice_image_preview_->insertWidget(this->ui->verticalSlider_seismic_slices_->value() ,l);

		this->sketch_seismic_controller_.addSeismicSlice( this->ui->verticalSlider_seismic_slices_->value() );

		std::cout << " Cross Sections  "  <<  this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_.size() << std::endl;

		return false;

	}


} /* namespace RRM */





