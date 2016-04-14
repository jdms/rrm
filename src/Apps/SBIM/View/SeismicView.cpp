/*
 * SeismicView.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicView.hpp"

namespace RRM
{

	SeismicView::SeismicView ( QWidget* parent ) : QGraphicsView(parent)
	{
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

		ui = new Ui::SeismicViewerWidget();
		ui->setupUi ( this );
	}

	SeismicView::~SeismicView ( )
	{

	}

	void SeismicView::wheelEvent ( QWheelEvent* event )
	{
	}

	void SeismicView::keyPressEvent ( QKeyEvent* event )
	{
	}


} /* namespace RRM */


