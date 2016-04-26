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

	}

	SeismicView::~SeismicView ( )
	{

	}



} /* namespace RRM */


