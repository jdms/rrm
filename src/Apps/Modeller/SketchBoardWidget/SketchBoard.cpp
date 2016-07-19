// Data created August 6, 2015
#include "Modeller/SketchBoardWidget/SketchBoard.hpp"

SketchBoard::SketchBoard(QWidget *parent) : QGraphicsView(parent)
{
	this->setRenderHint(QPainter::Antialiasing, true);
	this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setFocusPolicy(Qt::StrongFocus);
	this->setInteractive(true);
	this->setBackgroundRole(QPalette::Base);
	this->setAutoFillBackground(true);

	this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	this->viewport()->grabGesture(Qt::PinchGesture);
	this->viewport()->grabGesture(Qt::SwipeGesture);
	this->viewport()->grabGesture(Qt::PanGesture);

	// Invert the Coordinate System to match with OpenGL. X increases Left->Right and Y Bottom->Up.
	scale_in_ = 10;
	scale_out_ = -10;
	scaleFactor = 1.15;

	// XXX GraphScene where we can add Entities ( Curves, Icons ... )
	this->sketchSession_ = new SketchSessionTesting(this);

	this->setScene(sketchSession_);

	this->sketch_controller = new SketchController();

	// Notify the controller the Sketch curve
	connect(this->sketchSession_, &SketchSessionTesting::newSketchCurve, [=](QPolygonF p, QColor c) { this->sketch_controller->insertCurve(p, c); emit currentCrossSection(this->sketch_controller->getCrossSection()); });

	// Notify the controller with a new Session Scene
	connect(this->sketchSession_, SIGNAL(newSessionSignal(Real, Real, Real, Real)),
		this, SLOT(newSession(Real, Real, Real, Real)));

	// Notify the controller with a new Session Scene using image
	connect(this->sketchSession_, SIGNAL(newSessionSignal(const QPixmap&)),
		this, SLOT(newSession(const QPixmap&)));

	// Notify the controller with a new Session with a new boundary
	connect(this->sketchSession_, SIGNAL(newBoundary(Real, Real, Real, Real)),
		this, SLOT(setBoundary(Real, Real, Real, Real)));

	// Notify the view with the new configuration of Lines
	connect(this->sketch_controller, SIGNAL(updateSBIM(const std::map<unsigned int, std::pair<QColor, QPolygonF> >&, const std::map<unsigned int, QPointF>&)),
		this->sketchSession_, SLOT(updateSBIM(const std::map<unsigned int, std::pair<QColor, QPolygonF> >&, const std::map<unsigned int, QPointF>&)));

	this->sketch_controller->updateSBIM();

	/// Interface
	status_text_ = new QLabel("Sketch", this);
	this->sketch_controller->setRule(RRM::GeologicRules::Sketch);
}

SketchBoard::~SketchBoard()
{

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
	if (event->key() == Qt::Key_Q)
	{
		emit currentCrossSection(this->sketch_controller->getCrossSection());
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

void SketchBoard::setModeSketch()
{
	this->sketch_controller->setRule(RRM::GeologicRules::Sketch);
	this->sketchSession_->setOverSketchingMode();
	status_text_->setText("Sketch");
}

void SketchBoard::setModeRemoveAboveIntersection()
{
	this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_ABOVE_INTERSECTION);
	this->sketchSession_->setOverSketchingMode();
	status_text_->setText("Remove Above Intersection");
}

void SketchBoard::setModeRemoveBelowIntersection()
{
	this->sketch_controller->setRule(RRM::GeologicRules::REMOVE_BELOW_INTERSECTION);
	this->sketchSession_->setOverSketchingMode();
	status_text_->setText("Remove Below Intersection");
}

void SketchBoard::setModeRegionPoint()
{
	this->sketchSession_->setRegionMode();
	status_text_->setText("Select Region Points");
}

void SketchBoard::setModeEdition()
{
	this->sketchSession_->setEditMode();
	status_text_->setText("Remove Below Intersection");
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
	QImage image(this->sketchSession_->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
	image.fill(Qt::transparent); // Fill it witg transparent pixels

	this->sketchSession_->overlay_image_->setVisible(false);

	QPainter painter(&image);
	this->sketchSession_->render(&painter);

	image.save(_fileName);

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

	this->sketchSession_->render(&painter);
}

void SketchBoard::newBoundary()
{
	this->sketchSession_->sketchNewBoundary();
}

void SketchBoard::newSession(Real x, Real y, Real width, Real height)
{
	//@FIXME get all the geometry information from CrossSection Arrangment
	//this->sketchSession_->initialization(x,y,width,height);
	this->sketch_controller->newSession(x, -y, width, -height);
	this->sketchSession_->initialization(this->sketch_controller->getCrossSection().viewPort_.first.x(), -this->sketch_controller->getCrossSection().viewPort_.first.y(),
										 this->sketch_controller->getCrossSection().viewPort_.second.x(), -this->sketch_controller->getCrossSection().viewPort_.second.y());
}

void SketchBoard::newSession(const QPixmap& _image)
{
	//@FIXME get all the geometry information from CrossSection Arrangment
	//this->sketchSession_->initializationWithImage(_image);
	this->sketch_controller->newSession(_image.rect().x(),
									   -_image.rect().y(),
										_image.rect().width(),
									   -_image.rect().height());
	this->sketchSession_->initializationWithImage(_image);

}

void SketchBoard::setBoundary(Real x, Real y, Real width, Real height)
{
	//@FIXME get all the geometry information from CrossSection Arrangment
	this->sketch_controller->newSession(x, -y, width, -height);
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

	/// Update the data structure
	this->sketch_controller->setCrossSection(_cross_section);

	/// Make the overlay image visible
	this->sketchSession_->overlay_image_->setVisible(true);

	/// Rebuild the Scene
	this->sketch_controller->updateSBIM();

}
