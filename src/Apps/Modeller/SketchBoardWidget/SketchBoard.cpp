// Data created August 6, 2015

#include "Modeller/SketchBoardWidget/SketchBoard.hpp"


#include <QtCore/QDir>
#include <QtWidgets/QApplication>

// Flow Simulator 
#include "Simulator/FlowComputation/region.h"
#include "Modeller/ExtrusionWidget/ExtrusionController.hpp"

SketchBoard::SketchBoard( QWidget *parent ) : QGraphicsView(parent)
{
    setup();
    createActions();
}


SketchBoard::~SketchBoard()
{

}


void SketchBoard::setup()
{

    lb_coordinates = new QLabel();

    setRenderHint( QPainter::Antialiasing, true );
    setOptimizationFlags( QGraphicsView::DontSavePainterState );
    setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
    setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
    setFocusPolicy( Qt::StrongFocus );
    setInteractive( true );
    setBackgroundRole( QPalette::Base );
    setAutoFillBackground( true );

    setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    viewport()->grabGesture( Qt::PinchGesture );
    viewport()->grabGesture( Qt::SwipeGesture );
    viewport()->grabGesture( Qt::PanGesture );

    // Invert the Coordinate System to match with OpenGL. X increases Left->Right and Y Bottom->Up.
    scale( 1, -1 );

    scale_in_ = 10;
    scale_out_ = -10;
    scaleFactor = 1.15;

    // XXX GraphScene where we can add Entities ( Curves, Icons ... )
    sketchSession_ = new SketchSessionTesting( this );

    setScene( sketchSession_ );

    sketch_controller = new SketchController();
    sketch_controller->updateSBIM();
    sketch_controller->setRule( RRM::GeologicRules::Sketch );


}


void SketchBoard::createActions()
{

    // Notify the controller the Sketch curve
    //connect ( this->sketchSession_    , SIGNAL( newSketchCurve(QPolygonF,QColor) ) ,
    //		  this->sketch_controller , SLOT  ( insertCurve(QPolygonF,QColor) ) );

    connect(this->sketchSession_, &SketchSessionTesting::newSketchCurve, [=](QPolygonF p, QColor c) { this->sketch_controller->insertCurve(p, c); emit currentCrossSection(this->sketch_controller->getCrossSection()); });

    connect(this->sketchSession_, SIGNAL(newSessionSignal(Real, Real, Real, Real)),
        this, SLOT(newSession(Real, Real, Real, Real)));

    connect(this->sketchSession_, SIGNAL(newSessionSignal(const QPixmap&)),
        this, SLOT(newSession(const QPixmap&)));

    connect(this->sketchSession_, SIGNAL(newBoundary(Real, Real, Real, Real)),
        this, SLOT(setBoundary(Real, Real, Real, Real)));

    // Notify the view with the new configuration of Lines
    connect(this->sketch_controller, SIGNAL(updateSBIM(const std::map<unsigned int, std::pair<QColor, QPolygonF> >&, const std::map<unsigned int, QPointF>&)),
        this->sketchSession_, SLOT(updateSBIM(const std::map<unsigned int, std::pair<QColor, QPolygonF> >&, const std::map<unsigned int, QPointF>&)));



}


void SketchBoard::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F2)
	{
		this->sketchSession_->overlay_image_->setVisible(false);
	}

	if (event->key() == Qt::Key_F3)
	{
		this->sketchSession_->overlay_image_->setVisible(true);
	}
	if (event->key() == Qt::Key_F9)
	{
		this->setModeSketch();
	}
	if (event->key() == Qt::Key_F10)
	{
		this->setModeRemoveBelowIntersection();
	}
	if (event->key() == Qt::Key_F11)
	{
		this->setModeRemoveAboveIntersection();
	}
	if (event->key() == Qt::Key_Up)
	{
		//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION );
		//status_text->setText ( "Remove Above Intersection" );
		this->sketchSession_->setSketchMode();
	}
	if (event->key() == Qt::Key_Down)
	{
		//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_BELOW_INTERSECTION );
		//status_text->setText ( "Remove Below Intersection" );
		this->sketchSession_->setEditMode();
	}
	if (event->key() == Qt::Key_Right)
	{
		//cross_section_.changeRule ( RRM::GeologicRules::REMOVE_BELOW_INTERSECTION );
		//status_text->setText ( "Remove Below Intersection" );
		this->sketchSession_->setEditMode();
	}
	if (event->key() == Qt::Key_Q)
	{
		emit currentCrossSection(this->sketch_controller->getCrossSection());
		//cross_section_.changeRule ( RRM::GeologicRules::Sketch );
		//status_text->setText ( "Sketch" );
	}
	if (event->key() == Qt::Key_Space)
	{
		if (sketchSession_->mode() == SketchSessionTesting::InteractionMode::REGION_POINT)
		{
			sketchSession_->reset();
		}
		else
		{
			sketchSession_->reset();
			this->sketch_controller->clear();
			emit currentCrossSection(this->sketch_controller->getCrossSection());
		}
	}
	if (event->key() == Qt::Key_Escape)
	{
		sketchSession_->clearSelection();
		this->newSession(0.0, 0.0, 700.0, 400.0);
	}
	/// Zhao Testing
	if (event->key() == Qt::Key_S)
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
		ex.exportBlankScreen("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces3.rrms", 3);

		REGION region;
		//region.tolerance(0.000000001, 0.0);
		//region.userinput("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\userinput_internalchannel_bsurface.txt");
		//region.readskeleton("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces.txt");
		//region.unstructuredsurfacemesh();
		//region.cornerpointgrid();
		//region.writesurfacemeshVTK("surface.vtk");
		//region.unstructuredvolumemesh();
		//region.writevolumemesh("crossSection.vtk");
		//region.writecornerpointgridVTK("CornerPointCrossSection.vtk");



		/// Unstructured 
		region.tolerance(0.000000001, 0.0);
		region.userinput("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\userinput_surfaces.txt");
		region.readskeleton("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\surfaces3.rrms");


		region.unstructuredsurfacemesh();
		//region.writesurfacemeshVTK("model3trisurfacemeshtry2.vtk");
		region.unstructuredvolumemesh();
		//region.writevolumemesh("surfacestry.vtk");
		region.modelpreparation();
		region.steadystateflowsolver();
		region.flowdiagnostics();
		region.writeresult("D:\\Workspace\\RRM\\build-msvc2013_x32\\build\\bin\\results_surfaces3_try.vtk");
	}

	QGraphicsView::keyPressEvent(event);
}

void SketchBoard::wheelEvent(QWheelEvent *event)
{
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	// Scale the view / do the zoom
	if (event->delta() > 0)
	{
		this->zoomIn();
	}
	else
	{
		// Zooming out
		this->zoomOut();
	}
}

void SketchBoard::setModeSketch()
{
	this->sketch_controller->setRule(RRM::GeologicRules::Sketch);
	this->sketchSession_->setOverSketchingMode();
//	status_text_->setText("Sketch");
}

void SketchBoard::setModeRemoveAboveIntersection()
{
	this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION);
	this->sketchSession_->setOverSketchingMode();
//	status_text_->setText("Remove Above Intersection");
}

void SketchBoard::setModeRemoveBelowIntersection()
{
	this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_BELOW_INTERSECTION);
	this->sketchSession_->setOverSketchingMode();
//	status_text_->setText("Remove Below Intersection");
}

void SketchBoard::setModeRegionPoint()
{
	this->sketchSession_->setRegionMode();
//	status_text_->setText("Select Region Points");
}

void SketchBoard::setModeEdition()
{
	this->sketchSession_->setEditMode();
//    status_text_->setText("Remove Below Intersection");
}

void SketchBoard::clear()
{
	sketchSession_->reset();
	this->sketch_controller->clear();
	emit currentCrossSection(this->sketch_controller->getCrossSection());
}

void SketchBoard::undo()
{
	this->sketch_controller->undo();
	emit currentCrossSection(this->sketch_controller->getCrossSection());
}

void SketchBoard::redo()
{
	this->sketch_controller->redo();
	emit currentCrossSection(this->sketch_controller->getCrossSection());

}

void SketchBoard::zoomIn()
{
	// Zoom in
	if (scale_in_ >= 0)
	{
		this->scale(scaleFactor, scaleFactor);
		scale_in_ -= 1;
		scale_out_ -= 1;
	}
}

void SketchBoard::zoomOut()
{
	// Zooming out
	if (scale_out_ <= 0)
	{
		scale_out_ += 1;
		scale_in_ += 1;
		this->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	}
}

void SketchBoard::newSketch()
{
	sketchSession_->newSktech();
	emit currentCrossSection(this->sketch_controller->getCrossSection());
}

void SketchBoard::screenShot()
{
	// @see http://stackoverflow.com/a/1604542
	QString selectedFilter;

	QStringList filters;
	filters << "JPG files (*.jpg)" << "BMP files (*.bmp)" << "PNG files (*.png)";

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save CrossSection Image"), "", tr("PNG (*.png);;SVG (*.svg)"), &selectedFilter);

	qDebug() << selectedFilter;

	if (selectedFilter == "PNG (*.png)")
	{
		saveRaster(fileName);
	}
	else if (selectedFilter == "SVG (*.svg)")
	{
		saveVector(fileName);
	}
	else
	{

	}
}

void SketchBoard::saveRaster(const QString& _fileName)
{
	QImage imageFile;
	QImage image(this->sketchSession_->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
	image.fill(Qt::transparent);                      		                        // Start all pixels transparent

	this->sketchSession_->overlay_image_->setVisible(false);

	QPainter painter(&image);
	this->sketchSession_->render(&painter);

	QTransform myTransform;
	myTransform.scale(1, -1);
	imageFile = image.transformed(myTransform);

	imageFile.save(_fileName);

	this->sketchSession_->overlay_image_->setVisible(true);
}

void SketchBoard::saveVector(const  QString& _fileName)
{
	QSvgGenerator svgGen;

	svgGen.setFileName(_fileName);
	svgGen.setSize(QSize(this->sketchSession_->width(), this->sketchSession_->height()));

	svgGen.setViewBox(this->sketchSession_->sceneRect());
	svgGen.setTitle(tr("Rapid Reservoir Modelling - SVG generated by Qt5"));
	svgGen.setDescription(tr("SVG output of Rapid Reservoir Modelling software"));

	QPainter painter(&svgGen);
	
	painter.scale(1.0, -1.0);
	QPointF	translationOffset;
	translationOffset.setX(0.0);
	translationOffset.setY(-this->sketchSession_->height());
	painter.translate(translationOffset);

	this->sketchSession_->render(&painter);
}

void SketchBoard::newBoundary()
{
	this->sketchSession_->sketchNewBoundary();
}

void SketchBoard::newSession(Real x, Real y, Real width, Real height)
{
	this->sketchSession_->initialization(x, y, width, height);
	this->sketch_controller->newSession(x, y, width, height);
}

void SketchBoard::newSession(const QPixmap& _image)
{
	this->sketchSession_->initializationWithImage(_image);
	this->sketch_controller->newSession(_image.rect().x(),
		_image.rect().y(),
		_image.rect().width(),
		_image.rect().height());
}

void SketchBoard::setBoundary(Real x, Real y, Real width, Real height)
{
	this->sketch_controller->newSession(x, y, width, height);
	this->sketchSession_->setBoundary(x, y, width - x, height - y);
}

void SketchBoard::setCrossSection(const CrossSection& _cross_section)
{
	/// Notify the Module with the current CrossSection in the board
	emit currentCrossSection(this->sketch_controller->getCrossSection());

	/// Delete all the scene and setup it with the incoming cross section
	this->sketchSession_->clear();
	//this->sketchSession_->initializationWithImage(_overlay_image);

	QPixmap pix;
	pix.loadFromData(_cross_section.image_.data(), _cross_section.image_.size());

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

void SketchBoard::updateCoordinates( float posx, float posy )
{
    lb_coordinates->setText( QString( "%1, %2" ).arg( posx ).arg( posy ) );

}
