// Data created August 6, 2015

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

SketchBoard::SketchBoard ( QWidget *parent ) :
																QGraphicsView ( parent )
{

// The following is just according to local coordinates centred at (0,0)
//
//			                   |
//					 -y        |
//					  |        |
//			  -x<---(0,0)--->x | height
//					  |        |
//					  y        |
//			                   |
//            <-----width----->

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
	this->viewport ( )->grabGesture ( Qt::PanGesture );

	scale_in_  =  10;
	scale_out_ = -10;

}

SketchBoard::~SketchBoard ( )
{

}

void SketchBoard::keyPressEvent ( QKeyEvent *event )
{
	QGraphicsView::keyPressEvent(event);
}

void SketchBoard::wheelEvent ( QWheelEvent *event )
{

	this->setTransformationAnchor ( QGraphicsView::AnchorUnderMouse );
	// Scale the view / do the zoom
	double scaleFactor = 1.15;

	if ( event->delta ( ) > 0 )
	{
		// Zoom in
		if ( scale_in_ >= 0 )
		{
			this->scale ( scaleFactor , scaleFactor );
			scale_in_  -= 1;
			scale_out_ -= 1;
		}

	}
	else
	{
		// Zooming out
		if ( scale_out_ <= 0 )
		{
			scale_out_ += 1;
			scale_in_  += 1;
	               this->scale ( 1.0 / scaleFactor , 1.0 / scaleFactor );
		}

	}
}
