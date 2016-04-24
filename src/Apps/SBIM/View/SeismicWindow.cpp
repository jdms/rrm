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

		ui->seismic_viewer_frame_->addWidget(this->seismic_viewer_);

		for (int it = 0; it < 10; it++ )
		{
			QLabel* l = new QLabel(this);
			this->sketch_images_[it] 	= l;
			this->lines_checkpoint_[it] 	= new QGraphicsLineItem(0.0,48*it,640,48*it);
			this->lines_checkpoint_[it]->setVisible(false);
			this->seismic_viewer_->scene()->addItem(this->lines_checkpoint_[it]);

			QPixmap pix = QPixmap("/home/felipe/lastrevision-1.png");
			QIcon icon;

			icon.addPixmap(pix);
			QListWidgetItem * item = new QListWidgetItem(icon,"Slice");
			this->ui->listView->addItem(item);
		}

		this->seismic_viewer_->scale(1, -1);

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
		int index = this->ui->verticalSlider_seismic_slices_->value();

		QLabel* l;

		//l = qobject_cast<QLabel *>(this->ui->layout_slice_image_preview_->itemAt(index-1)->widget());

//		Q_ASSERT( l != 0 );
//
//		l->setText("Number "+QString::number(index));

		this->sketch_seismic_controller_.addSeismicSlice(index-1);

		this->lines_checkpoint_[index-1]->setVisible(true);

		std::cout << " Cross Sections  "  <<  this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_.size() << std::endl;

		return false;

	}


} /* namespace RRM */





