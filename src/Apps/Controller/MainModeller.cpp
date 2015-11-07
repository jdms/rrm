#include "MainWindow.h"

/// Skecthing
void MainWindow::create3DModule ( )
{

	dc_3DModule = new QDockWidget ( this );
	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
	dc_3DModule->setWindowTitle ( "3D View" );

	glWidget = new GLWidget ( this );

	QFrame *fr = new QFrame ( this );
	fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );

	QHBoxLayout *hb_canvas3d = new QHBoxLayout ( this );

	hb_canvas3d->addWidget ( glWidget );

	fr->setLayout ( hb_canvas3d );
	dc_3DModule->setWidget ( fr );
	addDockWidget ( Qt::RightDockWidgetArea , dc_3DModule );


//	QDockWidget* dc_3DModule2 = new QDockWidget ( this );
//	dc_3DModule2->setAllowedAreas ( Qt::RightDockWidgetArea );
//	dc_3DModule2->setWindowTitle ( "3D View" );
//
//	GLWidget* glWidget2 = new GLWidget ( this );
//
//	QFrame *fr2 = new QFrame ( this );
//	fr2->setFrameStyle ( QFrame::Box | QFrame::Sunken );
//
//	QHBoxLayout *hb_canvas3d2 = new QHBoxLayout ( this );
//
//	hb_canvas3d2->addWidget ( glWidget2 );
//
//	fr2->setLayout ( hb_canvas3d2 );
//	dc_3DModule2->setWidget ( fr2 );
//	addDockWidget ( Qt::RightDockWidgetArea , dc_3DModule2 );



	connect ( this->sketchSession_ , SIGNAL( curve2DSignal(QPolygonF) ) , this , SLOT( curve2DSlot(QPolygonF) ) );

	connect ( this->sketchSession_ , SIGNAL( smoothSketchSignal(QPolygonF) ) , this , SLOT( smoothCurveSlot(QPolygonF) ) );

	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(QPixmap) ) , this , SLOT( newSessionSlot(QPixmap) ) );

	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(qreal,qreal,qreal,qreal) ) , this , SLOT( newSessionSlot(qreal,qreal,qreal,qreal) ) );

	connect ( this->ac_newBoundary , SIGNAL( triggered() ) , this->sketchSession_ , SLOT( newBoundarySlot() ) );

	renderSegments ( );

}

void MainWindow::newSessionSlot ( qreal x , qreal y , qreal width , qreal height )
{
	std::cout << "New Boundary " << std::endl;

	this->sketchSession_->initialization ( x , y , width , height ); /// THE VIEW
	this->cross_section_.initialization ( x , y , width , height ); /// THE MODEL
}

void MainWindow::newSessionSlot ( QPixmap pixmap )
{
	this->sketchSession_->initialization_with_image ( pixmap );  /// THE VIEW
	this->cross_section_.initialization ( pixmap.rect ( ).x ( ) ,    /// THE MODEL
																	pixmap.rect ( ).y ( ) , pixmap.rect ( ).width ( ) , pixmap.rect ( ).height ( ) );
}
void MainWindow::curve2DSlot ( QPolygonF polygon )
{

	std::vector<RRM::CrossSection<qreal>::Point2D> curve;

	curve.resize ( polygon.size ( ) );

	for ( std::size_t i; i < polygon.size ( ); i++ )
	{
		curve[i][0] = polygon[i].x ( );
		curve[i][1] = polygon[i].y ( );
	}

	//cross_section_.insertCurve( curve );
}

/// Short Time
void MainWindow::update3DExtrusion ( )
{

	RRM::CrossSection<qreal>::Segment_iterator it;
	std::vector<std::vector<Eigen::Vector3f> > patches;
	std::vector<Eigen::Vector3f> temp;

	for ( it = cross_section_.topology_.halfedges_begin ( ); it != cross_section_.topology_.halfedges_end ( ); it++ )
	{
		if ( it->is_visible )    // and it->is_boundary )
		{
			temp.clear ( );
			temp.resize ( it->segment.curve.size ( ) );

			for ( std::size_t point = 0; point < it->segment.curve.size ( ); point++ )
			{
				temp[point] = Eigen::Vector3f ( it->segment.curve[point].x ( ) , it->segment.curve[point].y ( ) , 0.0f );
			}

			patches.push_back ( temp );
		}
	}

	std::cout << "Update Extrusion";

	glWidget->createSurfacePatchies ( patches , 10.0f , 10.0f , 500.0f );
}

/// Short time
void MainWindow::renderSegments ( )
{
	RRM::CrossSection<qreal>::Segment_iterator it;
	std::vector<QPainterPath> paths;

	if ( this->sketchSession_->halfedges_ != nullptr )
	{
		/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup
		this->sketchSession_->removeItem ( this->sketchSession_->halfedges_ );
		QList<QGraphicsItem *> children = this->sketchSession_->halfedges_->childItems ( );
		if ( children.size ( ) > 0 )
		{
			qDeleteAll ( children );

		}

		delete this->sketchSession_->halfedges_;
		this->sketchSession_->halfedges_ = 0;
		this->sketchSession_->update ( );
		this->sketch_board_->viewport ( )->update ( );
	}

	this->sketchSession_->halfedges_ = new QGraphicsItemGroup ( );

	for ( it = cross_section_.topology_.halfedges_begin ( ); it != cross_section_.topology_.halfedges_end ( ); it++ )
	{
		if ( it->is_visible && !it->is_boundary )
		{
			QPolygonF p = convert ( it->segment.curve );
			QPainterPath pa;

			InputSketch* horizon = new InputSketch ( QColor ( 0 , 0 , 255 ) );
			pa.addPolygon ( p );

			horizon->setSketch ( pa );

			horizon->setZValue(1);

			this->sketchSession_->halfedges_->addToGroup ( horizon );
		}
	}

	this->sketchSession_->addItem ( this->sketchSession_->halfedges_ );
	update ( );

}

/// FIXME , Call  Curve Engine.
/// This function broke Model View, because the RRM::CrossSection<qreal>::Curve2D curve;
/// It should call Curve Engine for curve algorithms.
void MainWindow::smoothCurveSlot ( QPolygonF raw_sketch )
{
	std::vector<RRM::CrossSection<qreal>::Point2D> points;

	points.resize ( raw_sketch.size ( ) );

	for ( std::size_t i = 0; i < raw_sketch.size ( ); i++ )
	{
		points[i][0] = raw_sketch[i].x ( );
		points[i][1] = raw_sketch[i].y ( );
	}

	RRM::CrossSection<qreal>::Curve2D curve;

	curve.setCurve ( points );

	curve.superSample ( (qreal) 3.0f );
	curve.meanFilter ( );
	curve.meanFilter ( );

	std::cout << " Curve Size " << curve.size ( ) << std::endl;

	raw_sketch.resize ( curve.size ( ) );

	std::cout << " Raw Sketch Size " << raw_sketch.size ( ) << std::endl;

	for ( std::size_t i = 0; i < raw_sketch.size ( ); i++ )
	{
		raw_sketch[i].setX ( curve[i][0] );
		raw_sketch[i].setY ( curve[i][1] );
	}

	sketchSession_->smoothCurveSlot ( raw_sketch );

	bool b = cross_section_.insertCurve ( curve );

	if ( b )
	{

	}
	else
	{
		sketchSession_->removeInputSketch ( );
	}

	renderSegments ( );
}

void MainWindow::keyPressEvent ( QKeyEvent *event )
{
	event->accept ( );

	if ( event->key ( ) == Qt::Key_Up )
	{
		cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::REMOVE_ABOVE_INTERSECTION );
		status_text->setText ( "Remove Above Intersection" );

	}
	else if ( event->key ( ) == Qt::Key_Down )
	{
		cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::REMOVE_BELOW_INTERSECTION );
		status_text->setText ( "Remove Below Intersection" );

	}
	else
	{
		cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::Sketch );
		status_text->setText ( "Sketch" );
	}

	if ( event->key ( ) == Qt::Key_Q )
	{
		update3DExtrusion ( );
	}

	if ( event->key ( ) == Qt::Key_P )
	{

		std::cout << "Salvando !!!" << std::endl;
		QImage image ( this->sketchSession_->sceneRect ( ).size ( ).toSize ( ) , QImage::Format_ARGB32 );  // Create the image with the exact size of the shrunk scene
		image.fill ( Qt::transparent );                      		                        // Start all pixels transparent

		this->sketchSession_->image->setVisible ( false );

		QPainter painter ( &image );
		this->sketchSession_->render ( &painter );
		image.save ( "file_name.png" );

		this->sketchSession_->image->setVisible ( true );

//		QString fileName = "RESULT_IMAGE.png";
//		QPixmap pixMap = QPixmap::grabWidget ( this->sketch_board_ );
//		pixMap.save ( fileName );
	}

	if ( event->key ( ) == Qt::Key_Space )
	{
		cross_section_.clear ( );

		renderSegments ( );
	}

}

void MainWindow::mousePressEvent ( QMouseEvent* event )
{
	if ( event->buttons ( ) & Qt::MiddleButton )
	{
		cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::REMOVE_BELOW_INTERSECTION );
		status_text->setText ( "Remove Below Intersection" );
	}

	if ( event->buttons ( ) & Qt::RightButton )
	{
		cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::REMOVE_ABOVE_INTERSECTION );
		status_text->setText ( "Remove Above Intersection" );
	}

	event->ignore ( );
}

void MainWindow::wheelEvent ( QWheelEvent *event )
{

	this->sketch_board_->setTransformationAnchor ( QGraphicsView::AnchorUnderMouse );
	// Scale the view / do the zoom
	double scaleFactor = 1.15;

	if ( event->delta ( ) > 0 )
	{
		// Zoom in
		this->sketch_board_->scale ( scaleFactor , scaleFactor );

	}
	else
	{
		// Zooming out
		this->sketch_board_->scale ( 1.0 / scaleFactor , 1.0 / scaleFactor );
	}

}
