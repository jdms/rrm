/*
 * SeismicView.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicWindow.hpp"

#include <iostream>

namespace RRM
{
	// The parent destructor are responsible to delete all children
	// @see http://stackoverflow.com/questions/2491707/memory-management-in-qt
	SeismicWindow::SeismicWindow ( QWidget* parent ) : QWidget(parent)
	{
		Qt::WindowFlags flags = Qt::Window;

		this->setWindowFlags(flags);

		this->ui = new Ui::SeismicWindowWidget();
		this->ui->setupUi ( this );

		this->seismic_viewer_ = new SeismicView(ui->frame);

		ui->seismic_viewer_frame_->addWidget(this->seismic_viewer_);


		//! Debug Version: to load the update shaders
		qDebug ( ) << "Load by Resources ";
		QDir shadersDir = QDir ( qApp->applicationDirPath ( ) );

		#if defined(_WIN32) || defined(_WIN64) // Windows Directory Style
			/* Do windows stuff */
			QString shaderDirectory (shadersDir.path ()+"\\");
		#elif defined(__linux__)               // Linux Directory Style
			/* Do linux stuff */
			QString shaderDirectory ( shadersDir.path ( ) + "/" );
		#else
			/* Error, both can't be defined or undefined same time */
			std::cout << "Operate System not supported !"
			halt();
		#endif

			
		//testing seismic
		this->sketch_seismic_controller_.readSeismic((shaderDirectory+"Seismic/vol3comp_ushort_seismic-dist-id_596x291x297.raw").toStdString());

		for ( int h = 0; h < sketch_seismic_controller_.seismic_data_.height; h++ )
		{
			QImage image = QImage(sketch_seismic_controller_.seismic_data_.width, sketch_seismic_controller_.seismic_data_.depth, QImage::Format_RGB32);

			for ( int w = 0; w < sketch_seismic_controller_.seismic_data_.width; w++ )
			{
				for ( int d = 0; d < sketch_seismic_controller_.seismic_data_.depth; d++ )
				{
					int b = sketch_seismic_controller_.seismic_data_.images_slices_[h][d * sketch_seismic_controller_.seismic_data_.width + w];
					QColor c ( b , b , b , 255 );
					image.setPixel ( w , d , c.rgb ( ) );
				}
			}

			sketch_images_[h+1] = QPixmap::fromImage(image);
		}
		
		this->seismic_viewer_->overlayImage_->setPixmap( sketch_images_[1] );
		this->seismic_viewer_->overlayImage_->setPos((640-this->seismic_viewer_->overlayImage_->boundingRect().width())*0.5,-(this->seismic_viewer_->overlayImage_->boundingRect().height()-480)*0.5); //now sets the position


//		QPixmap pix1 = QPixmap("/home/felipe/lastrevision-1.png");
//
//		pix1 = pix1.scaled(640,480,Qt::KeepAspectRatio,Qt::FastTransformation);
//		QMatrix rm;
//		rm.rotate(180);
//		pix1 = pix1.transformed(rm);
//
//		this->seismic_viewer_->overlayImage_->setPixmap(pix1);


//		for (int it = 0; it < 30; it+=3 )
//		{
//			this->lines_checkpoint_[it] 	= new QGraphicsLineItem(0.0,48*it,640,48*it);
//			this->lines_checkpoint_[it]->setVisible(false);
//			this->seismic_viewer_->scene()->addItem(this->lines_checkpoint_[it]);
//
//			QPixmap pix = QPixmap("/home/felipe/lastrevision-1.png");
//			QIcon icon;
//
//			icon.addPixmap(pix);
//			QListWidgetItem * item = new QListWidgetItem(icon,QString::number(it));
//			item->setData(Qt::UserRole,it);
//			this->ui->listWidget->insertItem(it,item);
//		}

		std::cout << this->ui->listWidget->count();

		/// Added new slice
		connect(this->ui->seismic_pushButton_AddSlice_, SIGNAL (pressed()),this, SLOT(addSeismicSlice()) );
		connect(this->ui->listWidget, SIGNAL (itemDoubleClicked(QListWidgetItem *)), this, SLOT(setCurrentSeismicSlice( QListWidgetItem * )) );

		connect(this->ui->seismic_slices_verticalSlider_, SIGNAL (valueChanged(int)),this, SLOT(updateSeismicImage(int)) );
		connect(this->ui->seismic_pushButton_clear_, SIGNAL (pressed()),this, SLOT(clear()) );
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
	/// @see http://stackoverflow.com/a/17085612
	void SeismicWindow::showEvent( QShowEvent *)
	{
		this->seismic_viewer_->fitInView(this->seismic_viewer_->sceneRect(), Qt::KeepAspectRatio);
	}

	void SeismicWindow::updateSeismicImage( int _index )
	{
		if ( sketch_images_.count ( _index ) > 0 )
		{
			this->seismic_viewer_->overlayImage_->setPixmap ( sketch_images_[_index] );
		}
	}

	bool SeismicWindow::addSeismicSlice (  )
	{
		int index = this->ui->seismic_slices_verticalSlider_->value();

		if ( sketch_seismic_controller_.addSeismicSlice(index,QPixmap()) )
		{
			//this->seismic_viewer_->scene ( )->addItem ( new QGraphicsLineItem ( 0.0 , (480/297) * (index) , 640 , (480/297) * (index) ) );
			// Save the current Scene Image
			QImage image ( this->seismic_viewer_->sceneRect ( ).size ( ).toSize ( ) , QImage::Format_ARGB32 );  // Create the image with the exact size of the shrunk scene
			image.fill ( Qt::transparent );                      		                        // Start all pixels transparent

			QPainter painter ( &image );
			this->seismic_viewer_->scene()->render ( &painter );

			//QPixmap pix =  QPixmap::fromImage(image);
			QPixmap pix = sketch_images_[index];

			sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[index].image_ = this->sketch_seismic_controller_.convertQPixmap2RRMImage(pix);
			sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[index].initialize(0.0,0.0,pix.width(),pix.height());
			sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[index].id_ = index;

			QIcon icon;
			icon.addPixmap ( pix );
			SeismicThumbnailItem * item = new SeismicThumbnailItem ( icon , QString::number ( index ) );
			item->setData ( Qt::UserRole , index );

			this->ui->listWidget->insertItem ( index , item );

//			this->seismic_viewer_->overlayImage_->setPixmap(pix);
//			this->seismic_viewer_->overlayImage_->setPos((640-this->seismic_viewer_->overlayImage_->boundingRect().width())*0.5,-(this->seismic_viewer_->overlayImage_->boundingRect().height()-480)*0.5); //now sets the position

		}else
		{
			std::cout << "Slice Exist" << std::endl;
		}

		std::cout << " Cross Sections  "  << this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_.size() << std::endl;

		return false;
	}

	void SeismicWindow::clear()
	{
		this->ui->listWidget->clear();
		this->sketch_seismic_controller_.clear();
	}

	const SeismicWindow::SeismicSlices& SeismicWindow::getSeimicSlices() const
	{
		return this->sketch_seismic_controller_.getSeismicSlices();
	}

	void SeismicWindow::setCurrentSeismicSlice( QListWidgetItem * item )
	{
		QVariant qvPtr = item->data(Qt::UserRole);

		int index = qvPtr.value<int>();

		std::cout << " Log Seismic --- Send " << std::endl;

		 this->sketch_seismic_controller_.setCurrentSeismicSlice(index);

		emit currentCrossSection(sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[index]);

		this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[index];

		std::cout << "row :" << this->ui->listWidget->row(item) << "  " <<  index << std::endl;
	}

	void SeismicWindow::updateCrossSection ( const CrossSection& _cross_section)
	{

		/// Avoid insert the default CrossSection, aka. id_ == 0;
		if ( _cross_section.id_ == 0 )
		{
			return;
		}

		if ( this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_.count(_cross_section.id_)  != 0 )
		{
			this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[_cross_section.id_] = _cross_section;
			std::cout << " Log Seismic --- Back " << std::endl;

			// Notify the 3D viewer
			emit currentSeismicSlices(this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_);

			this->sketch_seismic_controller_.sketch_seismic_module_.seismic_slices_[_cross_section.id_].log();
		}


	}

} /* namespace RRM */





