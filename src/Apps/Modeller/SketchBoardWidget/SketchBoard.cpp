// Data created August 6, 2015

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"


#include <QtCore/QDir>
#include <QtWidgets/QApplication>

// Flow Simulator 
#include "Simulator/FlowComputation/region.h"
#include "Modeller/ExtrusionWidget/ExtrusionController.hpp"

SketchBoard::SketchBoard ( QWidget *parent ) :	QGraphicsView ( parent )
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
	this->viewport ( )->grabGesture ( Qt::PanGesture );

	// Invert the Coordinate System to match with OpenGL. X increases Left->Right and Y Bottom->Up.
	this->scale(1, -1);

	scale_in_  =  10;
	scale_out_ = -10;

	// XXX GraphScene where we can add Entities ( Curves, Icons ... )
	this->sketchSession_ = new SketchSessionTesting ( this );

	this->setScene ( sketchSession_ );

	this->sketch_controller = new SketchController();

	// Notify the controller the Sketch curve
	connect ( this->sketchSession_    , SIGNAL( newSketchCurve(QPolygonF) ) ,
		  this->sketch_controller , SLOT  ( insertCurve(QPolygonF) ) );

	connect ( this->sketchSession_    , SIGNAL( newSessionSignal ( Real, Real, Real, Real ) ) ,
		  this 			  , SLOT  ( newSession ( Real, Real, Real, Real ) ) );

	connect ( this->sketchSession_    , SIGNAL( newSessionSignal ( const QPixmap& ) ) ,
		  this 			  , SLOT  ( newSession ( const QPixmap& ) ) );

	connect ( this->sketchSession_    , SIGNAL( newBoundary ( Real, Real, Real, Real ) ) ,
		  this 			  , SLOT  ( setBoundary ( Real, Real, Real, Real ) ) );

	// Notify the view with the new configuration of Lines
	connect ( this->sketch_controller , SIGNAL( updateSBIM(const std::map<unsigned int, std::pair<unsigned int,QPolygonF> >&, const std::map<unsigned int, QPointF>& ) ) ,
		  this->sketchSession_    , SLOT  ( updateSBIM(const std::map<unsigned int, std::pair<unsigned int,QPolygonF> >&, const std::map<unsigned int, QPointF>& ) ) );

	this->sketch_controller->updateSBIM();

	/// Interface
	status_text = new QLabel ( "Sketch" , this );
	this->sketch_controller->setRule(RRM::GeologicRules::Sketch);

}

SketchBoard::~SketchBoard ( )
{

}

void SketchBoard::keyPressEvent ( QKeyEvent *event )
{
		if ( event->key ( ) == Qt::Key_F2 )
		{
			this->sketchSession_->overlay_image_->setVisible(false);
		}

		if ( event->key ( ) == Qt::Key_F3 )
		{
			this->sketchSession_->overlay_image_->setVisible(true);
		}
		if ( event->key ( ) == Qt::Key_F4 )
		{

		}
		if ( event->key ( ) == Qt::Key_F9 )
		{
			this->sketch_controller->setRule(RRM::GeologicRules::Sketch);
			status_text->setText ( "Sketch" );
		}
		if ( event->key ( ) == Qt::Key_F10 )
		{
			this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_BELOW_INTERSECTION);
			status_text->setText ( "Remove Below Intersection");
		}
		if ( event->key ( ) == Qt::Key_F11 )
		{
			this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION);
			status_text->setText ( "Remove Above Intersection" );
		}

		if ( event->key ( ) == Qt::Key_Up )
		{
			//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION );
			//status_text->setText ( "Remove Above Intersection" );
			this->sketchSession_->setSketchMode();
		}
		if ( event->key ( ) == Qt::Key_Down )
		{
			//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_BELOW_INTERSECTION );
			//status_text->setText ( "Remove Below Intersection" );
			this->sketchSession_->setEditMode();
		}
		if ( event->key ( ) == Qt::Key_Right )
		{
			//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_BELOW_INTERSECTION );
			//status_text->setText ( "Remove Below Intersection" );
			this->sketchSession_->setEditMode();
		}
		if ( event->key ( ) == Qt::Key_Q )
		{
			emit currentCrossSection(this->sketch_controller->getCrossSection());
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
			sketchSession_->reset();
			this->sketch_controller->clear();
		}

		if ( event->key ( ) == Qt::Key_Escape)
		{
				sketchSession_->clearSelection();
				this->newSession(0.0,0.0,700.0,400.0);
		}

		if( event->key () == Qt::Key_I)
		{
			sketchSession_->newSktech();
		}
		/// Zhao Testing

		if  (event->key() == Qt::Key_S)
		{
			std::cout << "Saving Sktech Lines" << std::endl;
			sketch_controller->getCrossSection().outputsketchlines(sketch_controller->getCrossSection(), "lines.txt");
		}

		if (event->key() == Qt::Key_X)
		{
			std::cout << "Saving Skeleton Surfaces" << std::endl;
			//sketch_controller->getCrossSection().extrudeandoutputskeleton(sketch_controller->getCrossSection(), "D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces.txt");

			RRM::ExtrusionController ex;

			ex.setBlackScreenCrossSection(sketch_controller->getCrossSection());
			ex.exportBlankScreen("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces.txt", 40);
		
			REGION region;
			region.tolerance(0.000000001, 0.0);
			/*
			char * c = (shaderDirectory + "inputs/userinput_internalchannel_bsurface.txt").toStdString().c_str();

			char * s = (shaderDirectory + "surface.txt").toStdString().c_str();
			*/

			region.userinput("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\userinput_internalchannel_bsurface.txt");
			region.readskeleton("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces.txt");
			//region.unstructuredsurfacemesh();
			region.cornerpointgrid();
			//region.writesurfacemeshVTK("surface.vtk");
			//region.unstructuredvolumemesh();
			//region.writevolumemesh("crossSection.vtk");
			region.writecornerpointgridVTK("CornerPointCrossSection.vtk");
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

void SketchBoard::clear()
{
	sketchSession_->reset();
	this->sketch_controller->clear();
}

void SketchBoard::newSession ( Real x , Real y , Real width , Real height )
{
	this->sketchSession_->initialization(x,y,width,height);
	this->sketch_controller->newSession(x,y,width,height);
}

void SketchBoard::newSession ( const QPixmap& _image )
{
	this->sketchSession_->initializationWithImage(_image);
	this->sketch_controller->newSession(_image.rect ( ).x(),
                                            _image.rect ( ).y(),
					    _image.rect ( ).width(),
					    _image.rect ( ).height());
}

void SketchBoard::setBoundary ( Real x , Real y , Real width , Real height )
{
	this->sketch_controller->newSession(x,y,width,height);
	this->sketchSession_->setBoundary(x,y,width-x,height-y);
}

void SketchBoard::setCrossSection(const CrossSection& _cross_section)
{
	/// Notify the Module with the current CrossSection in the board
	emit currentCrossSection(this->sketch_controller->getCrossSection());

	/// Delete all the scene and setup it with the incoming cross section
	this->sketchSession_->clear();
	//this->sketchSession_->initializationWithImage(_overlay_image);

	 QPixmap pix;
	 pix.loadFromData( _cross_section.image_.data(), _cross_section.image_.size());

	/// Update the overlay image
	 this->sketchSession_->initializationWithImage(pix);

	//this->sketchSession_->overlay_image_->setPixmap(pix);

	/// Update the data structure
	this->sketch_controller->setCrossSection(_cross_section);

	/// Make the overlay image visible
	this->sketchSession_->overlay_image_->setVisible(true);

	/// Rebuild the Scene
	this->sketch_controller->updateSBIM();

}
