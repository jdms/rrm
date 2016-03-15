#include "MainWindow.h"

// Skecthing
void MainWindow::create2DModule ( )
{
	dc_2DModule = new Sketching2DModule( this );
	dc_2DModule->setWindowTitle ( "Sketching" );
	addDockWidget( Qt::LeftDockWidgetArea, dc_2DModule );

	// XXX Just the Canvas
	this->sketch_board_ = new SketchBoard ( );
	// XXX GraphScene where we can add Entities ( Curves, Icons ... )
	this->sketchSession_ = new SketchSession ( sketch_board_ );
	this->sketchSession_->initialization ( 0.0 , 0.0 , 700 , 400 );  // The View
	this->cross_section_.initialization ( 0.0 , 0.0 , 700 , 400 );   // The Model
	// FIXME
	this->sketch_board_->setScene ( sketchSession_ );
	this->setCentralWidget ( sketch_board_ );

	dc_2DModule->setWidget ( sketch_board_ );

	addDockWidget ( Qt::LeftDockWidgetArea , dc_2DModule );

	this->status_bar_ = new QStatusBar ( this );
	this->status_text = new QLabel ( "Sketch" , this );
	this->cross_section_.changeRule ( RRM::CrossSection<qreal>::GeologicRules::Sketch );
	this->status_bar_->addWidget ( status_text );
	this->setStatusBar ( this->status_bar_ );

	scale_in  =  10;
	scale_out = -10;

}

void MainWindow::create3DModule ( )
{
	dc_3DModule = new QDockWidget ( this );
	dc_3DModule->setAllowedAreas ( Qt::RightDockWidgetArea );
	dc_3DModule->setWindowTitle ( "3D View" );

	glWidget = new GLWidget ( this );

	QFrame *fr = new QFrame ( this );
	fr->setFrameStyle ( QFrame::Box | QFrame::Sunken );

	QVBoxLayout *hb_canvas3d = new QVBoxLayout (  );

	fr->setLayout ( hb_canvas3d );
	dc_3DModule->setWidget ( fr );
	addDockWidget ( Qt::RightDockWidgetArea , dc_3DModule );

	// Sketching
	connect ( this->sketchSession_ , SIGNAL( curve2DSignal(QPolygonF) ) , this , SLOT( curve2DSlot(QPolygonF) ) );

	connect ( this->sketchSession_ , SIGNAL( smoothSketchSignal(QPolygonF) ) , this , SLOT( smoothCurveSlot(QPolygonF) ) );

	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(QPixmap) ) , this , SLOT( newSessionSlot(QPixmap) ) );

	connect ( this->sketchSession_ , SIGNAL( newSessionSignal(qreal,qreal,qreal,qreal) ) , this , SLOT( newSessionSlot(qreal,qreal,qreal,qreal) ) );

	connect ( this->ac_newBoundary , SIGNAL( triggered() ) , this->sketchSession_ , SLOT( newBoundarySlot() ) );

	renderSegments ( );

        groupBox = new QGroupBox(dc_3DModule);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(156000, 100));
        //groupBox->setBaseSize(QSize(300, 400));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        horizontalSlider_curve = new QSlider(groupBox);
        horizontalSlider_curve->setObjectName(QStringLiteral("horizontalSlider_curve"));
        horizontalSlider_curve->setMinimum(1);
        horizontalSlider_curve->setMaximum(10);
        horizontalSlider_curve->setOrientation(Qt::Horizontal);

        formLayout->setWidget(0, QFormLayout::FieldRole, horizontalSlider_curve);

        horizontalSlider_surface = new QSlider(groupBox);
        horizontalSlider_surface->setObjectName(QStringLiteral("horizontalSlider_surface"));
        horizontalSlider_surface->setMinimum(1);
        horizontalSlider_surface->setMaximum(10);
        horizontalSlider_surface->setValue(10);
        horizontalSlider_surface->setOrientation(Qt::Horizontal);

        formLayout->setWidget(1, QFormLayout::FieldRole, horizontalSlider_surface);

        horizontalSlider_extrusion = new QSlider(groupBox);
        horizontalSlider_extrusion->setObjectName(QStringLiteral("horizontalSlider_extrusion"));
        horizontalSlider_extrusion->setMinimum(10);
        horizontalSlider_extrusion->setMaximum(1000);
        horizontalSlider_extrusion->setSingleStep(1);
        horizontalSlider_extrusion->setValue(500);
        horizontalSlider_extrusion->setOrientation(Qt::Horizontal);

        formLayout->setWidget(2, QFormLayout::FieldRole, horizontalSlider_extrusion);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        groupBox->setTitle(QApplication::translate("ControllerView", "Resolutions", 0));
        label->setText(QApplication::translate("ControllerView", "Curve", 0));
        label_2->setText(QApplication::translate("ControllerView", "Surface", 0));
        label_3->setText(QApplication::translate("ControllerView", "Extrusion", 0));

        hb_canvas3d->addWidget(groupBox);

        hb_canvas3d->addWidget ( glWidget );

}

void MainWindow::newSessionSlot ( qreal x , qreal y , qreal width , qreal height )
{
	std::cout << "New Boundary " << std::endl;

	this->sketchSession_->initialization ( x , y , width , height );// THE VIEW
	this->cross_section_.initialization ( x , y , width , height ); // THE MODEL
}

void MainWindow::newSessionSlot ( QPixmap pixmap )
{
	this->sketchSession_->initialization_with_image ( pixmap );  // THE VIEW
	this->cross_section_.initialization ( pixmap.rect ( ).x ( ) ,   // THE MODEL
                                              pixmap.rect ( ).y ( ) ,
					      pixmap.rect ( ).width ( ) ,
					      pixmap.rect ( ).height ( ) );
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

void MainWindow::update3DExtrusion ( float stepx, float stepz, float lenght  )
{

	RRM::CrossSection<qreal>::Segment_iterator it;
	std::vector<std::vector<Eigen::Vector3f> > patches;
	std::vector<Eigen::Vector3f> temp;

	for ( it = cross_section_.topology_.halfedges_begin ( ); it != cross_section_.topology_.halfedges_end ( ); it++ )
	{

		if( it->is_boundary )
		{
			min_ = Eigen::Vector3f (it->segment.curve.front().x(), it->segment.curve.front().y(),0.0);
			max_ = Eigen::Vector3f (it->segment.curve.back().x() , it->segment.curve.back().y(),lenght);
		}

		else if ( it->is_visible )
		{
			temp.clear ( );
			temp.resize ( it->segment.curve.size ( ) );

			for ( std::size_t point = 0; point < it->segment.curve.size ( ); point++ )
			{
				if ( it->is_boundary)
				{
					temp[point] = Eigen::Vector3f ( it->segment.curve[point].x ( ) , it->segment.curve[point].y ( ) , 1.0f );
				}else
				{
					temp[point] = Eigen::Vector3f ( it->segment.curve[point].x ( ) , it->segment.curve[point].y ( ) , 0.0f );
				}
			}

			patches.push_back ( temp );
		}
	}

	std::cout << "Update Extrusion";

	std::vector<Eigen::Vector3f> points = { min_ , max_};

	Celer::BoundingBox3<float> box;

	box.fromPointCloud(points.begin(),points.end());

	glWidget->createCube(box);
						   // Curve  //Volume    Step Volume
	glWidget->createSurfacePatchies ( patches , stepx    , stepz   , lenght , box.center(), box.diagonal() );
}

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
// FIXME , Call  Curve Engine.
// This function broke Model View, because the RRM::CrossSection<qreal>::Curve2D curve;
// It should call Curve Engine for curve algorithms.
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


	if ( event->key ( ) == Qt::Key_F1 )
	{
		this->sketchSession_->image->setVisible(false);
	}

	if ( event->key ( ) == Qt::Key_F2 )
	{
		this->sketchSession_->image->setVisible(true);
	}

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
		update3DExtrusion ( (float)horizontalSlider_curve->value(),(float)horizontalSlider_surface->value(), (float)horizontalSlider_extrusion->value() );
	}

	if ( event->key ( ) == Qt::Key_P )
	{

		std::cout << "Salvando !!!" << std::endl;
		QImage image ( this->sketchSession_->sceneRect ( ).size ( ).toSize ( ) , QImage::Format_ARGB32 );  // Create the image with the exact size of the shrunk scene
		image.fill ( Qt::transparent );                      		                        // Start all pixels transparent

		this->sketchSession_->image->setVisible ( false );

		QPainter painter ( &image );
		this->sketchSession_->render ( &painter );
		image.save ( "CrossSection.png" );

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
		if ( scale_in >= 0 )
		{
			this->sketch_board_->scale ( scaleFactor , scaleFactor );
			scale_in  -= 1;
			scale_out -= 1;
		}

	}
	else
	{
		// Zooming out
		if ( scale_out <= 0 )
		{
			scale_out += 1;
			scale_in  += 1;
	               this->sketch_board_->scale ( 1.0 / scaleFactor , 1.0 / scaleFactor );
		}

	}
}

void MainWindow::on_horizontalSlider_curve_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_surface_valueChanged()
{
	//update3DExtrusion();
}

void MainWindow::on_horizontalSlider_extrusion_valueChanged()
{
	//update3DExtrusion();
}
