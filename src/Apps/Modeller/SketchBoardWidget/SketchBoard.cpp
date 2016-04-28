// Data created August 6, 2015

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

SketchBoard::SketchBoard ( RRM::CrossSection<qreal>& _cross_section, QWidget *parent ) :	QGraphicsView ( parent )
{
// The following is just according to local coordinates centred at (0,0)

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

	// Invert the Coordinate System to match with OpenGL. X increase Left->Right and Y Top->Down.
	this->scale(1, -1);

	scale_in_  =  10;
	scale_out_ = -10;

	// XXX GraphScene where we can add Entities ( Curves, Icons ... )
	this->sketchSession_ = new SketchSessionTesting ( this );
	this->sketchSession_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View

	this->setScene ( sketchSession_ );

	this->sketch_controller = new SketchController(_cross_section);

	// Notify the controller the Sketch curve
	connect ( this->sketchSession_    , SIGNAL( newSketchCurve(QPolygonF) ) ,
		  this->sketch_controller , SLOT  ( insertCurve(QPolygonF) ) );

	connect ( this->sketchSession_    , SIGNAL( newSessionSignal ( qreal, qreal, qreal, qreal ) ) ,
		  this->sketch_controller , SLOT  ( newSession ( qreal, qreal, qreal ,qreal  ) ) );

	connect ( this->sketch_controller , SIGNAL( updateSBIM(const std::map<unsigned int, QPolygonF>&, const std::map<unsigned int, QPointF>& ) ) ,
		  this->sketchSession_    , SLOT  ( updateSBIM(const std::map<unsigned int, QPolygonF>&, const std::map<unsigned int, QPointF>& ) ) );

	this->sketch_controller->updateSBIM();
}

SketchBoard::~SketchBoard ( )
{

}

void SketchBoard::keyPressEvent ( QKeyEvent *event )
{

		if ( event->key ( ) == Qt::Key_F1 )
		{
			this->sketchSession_->overlay_image_->setVisible(false);
		}

		if ( event->key ( ) == Qt::Key_F2 )
		{
			this->sketchSession_->overlay_image_->setVisible(true);
		}

		if ( event->key ( ) == Qt::Key_Up )
		{
			//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION );
			//status_text->setText ( "Remove Above Intersection" );
			this->sketchSession_->setSketchMode();
		}
		else if ( event->key ( ) == Qt::Key_Down )
		{
			//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_BELOW_INTERSECTION );
			//status_text->setText ( "Remove Below Intersection" );
			this->sketchSession_->setEditMode();
		}
		else
		{
			//cross_section_.changeRule ( RRM::GeologicRules::Sketch );
			//status_text->setText ( "Sketch" );
		}

		if ( event->key ( ) == Qt::Key_P )
		{

			QImage image ( this->sketchSession_->sceneRect ( ).size ( ).toSize ( ) , QImage::Format_ARGB32 );  // Create the image with the exact size of the shrunk scene
			image.fill ( Qt::transparent );                      		                        // Start all pixels transparent

			this->sketchSession_->overlay_image_->setVisible ( false );

			QPainter painter ( &image );
			this->sketchSession_->render ( &painter );
			image.save ( "CrossSection.png" );

			this->sketchSession_->overlay_image_->setVisible ( true );

	//		QString fileName = "RESULT_IMAGE.png";
	//		QPixmap pixMap = QPixmap::grabWidget ( this->sketch_board_ );
	//		pixMap.save ( fileName );
		}

		if ( event->key ( ) == Qt::Key_Space )
		{
			//cross_section_.clear ( );
		}

		if ( event->key ( ) == Qt::Key_Escape)
		{
				sketchSession_->clearSelection();
		}


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

void SketchBoard::setCrossSection(const CrossSection& _cross_section, const QPixmap& _overlay_image)
{

	emit currentCrossSection(this->sketch_controller->getCrossSection());

	this->sketchSession_->clear();

	//this->sketchSession_->initializationWithImage(_overlay_image);
	this->sketchSession_->overlay_image_->setPixmap(_overlay_image);

	this->sketch_controller->setCrossSection(_cross_section);

	this->sketchSession_->overlay_image_->setVisible(true);

	this->sketch_controller->updateSBIM();

}
