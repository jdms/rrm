/*
 * SeismicView.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicView.hpp"

namespace RRM
{
	// The parent destructor are responsible to delete all children
	// @see http://stackoverflow.com/questions/2491707/memory-management-in-qt

	SeismicView::SeismicView ( QWidget* parent ) : QGraphicsView(parent)
	{
		// @see https://forum.qt.io/topic/48564/force-qgraphicsview-size-to-match-qgraphicsscene-size/4
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		this->setRenderHint ( QPainter::Antialiasing , true );
		this->setOptimizationFlags ( QGraphicsView::DontSavePainterState );
		this->setViewportUpdateMode ( QGraphicsView::SmartViewportUpdate );
		this->setTransformationAnchor ( QGraphicsView::AnchorUnderMouse );
		this->setFocusPolicy ( Qt::StrongFocus );
		this->setInteractive ( true );
		this->setBackgroundRole ( QPalette::Base );
		this->setAutoFillBackground ( true );

		this->setViewportUpdateMode ( QGraphicsView::FullViewportUpdate );
		this->viewport ( )->grabGesture ( Qt::PinchGesture );
		this->viewport ( )->grabGesture ( Qt::SwipeGesture );

		// Invert y direction to fit with opengl coordinate system
		this->scale(1,-1);

		this->scene_ = new QGraphicsScene(this);

		this->overlayImage_ = new QGraphicsPixmapItem();

		overlayImage_->setZValue(0);

		this->scene_->setSceneRect(0.0,0.0,640.0,480.0);

		this->scene_->addItem ( overlayImage_ );

		this->setScene(this->scene_);

		this->setFixedSize(this->sceneRect().width(),this->sceneRect().height());

		// -- Reading Seismic Data
//		seismic_data_.read("/media/d/Temp/vol3comp_ushort_seismic-dist-id_596x291x297.raw");
//
//		images = std::vector<QImage>(seismic_data_.width,QImage(seismic_data_.width, seismic_data_.depth, QImage::Format_RGB32));
//
//		// Reading Seismic, Distance and ID
//		for ( int h = 0; h < seismic_data_.height; h++ )
//		{
//			for ( int w = 0; w < seismic_data_.width; w++ )
//			{
//				for ( int d = 0; d < seismic_data_.depth; d++ )
//				{
//					float f = this->seismic_data_.seismic_data_[(d * seismic_data_.width * seismic_data_.height + h * seismic_data_.width + w)];
//					// http://stackoverflow.com/a/1914172
//					float f2 = std::max ( 0.0f , std::min ( 1.0f , f ) );
//					int b = floor ( f2 == 1.0 ? 255 : f2 * 256.0 );
//					QColor c(b,b,b,255);
//					images[h].setPixel ( w , d , c.rgb());
//				}
//			}
//		}
//
//		for ( int it = 0; it < 10; ++it )
//		{
//			std::cout << "sketching" << seismic_data_.seismic_data_[it] << std::endl;
//		}

		//this->overlay_image_->setPixmap(QPixmap::fromImage(images[0]));

	}

	SeismicView::~SeismicView ( )
	{

	}



} /* namespace RRM */


