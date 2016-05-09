#include "SketchSessionTesting.hpp"


SketchSessionTesting::SketchSessionTesting ( QObject *parent ) : QGraphicsScene ( parent )
{

	mode_ = InteractionMode::SKETCHING;

	/// Drag and Drop Feature
	overlay_image_ = new QGraphicsPixmapItem ( );
	this->addItem ( overlay_image_ );

	QPen pen;

	pen.setColor ( QColor ( 255 , 100 , 70 ) );
	pen.setWidth ( 5 );


	input_sketch_ = new InputSketch ( QColor ( 255 , 75 , 75 ) );
	input_sketch_->setPen ( pen );
	this->addItem ( input_sketch_ );
	this->input_sketch_->setZValue(1);

	sketch_pen.setColor ( QColor ( 187 , 15 , 32 ) );

	boundaryc_ = new BoundaryItem(0.0,0.0) ;

	this->addItem ( boundaryc_ );

	this->boundary_sketching_ = false;

	setUpBackground();

	colors.push_back(QColor(Qt::white));
	colors.push_back(QColor(Qt::black));
	colors.push_back(QColor(Qt::cyan));
	colors.push_back(QColor(Qt::darkCyan));
	colors.push_back(QColor(Qt::red));
	colors.push_back(QColor(Qt::darkRed));
	colors.push_back(QColor(Qt::magenta));
	colors.push_back(QColor(Qt::darkMagenta));
	colors.push_back(QColor(Qt::green));
	colors.push_back(QColor(Qt::darkGreen));

	seismic_data_.read("/media/d/Temp/vol3comp_ushort_seismic-dist-id_596x291x297.raw");

	for ( int it = 0; it < 10; ++it )
	{
		std::cout << "sketching" << seismic_data_.seismic_data_[it] << std::endl;
	}

}

SketchSessionTesting::~SketchSessionTesting ( )
{
	 clear();
}




// View/Qt5 related functions
void SketchSessionTesting::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{

	if ( mode_ == InteractionMode::EDITING )
	{
		QGraphicsScene::mousePressEvent ( event );
		event->ignore();
	}
	else
	{
		if ( event->buttons ( ) & Qt::LeftButton )
		{
			qDebug ( ) << " Item Grabbed " << event->buttonDownScenePos ( Qt::LeftButton );

			last_point_ = event->buttonDownScenePos ( Qt::LeftButton );

			this->boundary_anchor_point_ = last_point_;

			if ( ( this->boundary_sketching_ == false ) )
			{
				input_sketch_->create ( event->scenePos ( ) );
			}
			else
			{
			}
		}
	}

	update ( );
}

void SketchSessionTesting::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{

	if ( mode_ == InteractionMode::EDITING )
	{
		QGraphicsScene::mouseMoveEvent ( event );
		event->ignore();
	}
	else
	{
		if ( event->buttons ( ) & Qt::LeftButton )
		{
			//this->svg->setPos(event->scenePos());

			if ( this->boundary_sketching_ == false )
			{
				input_sketch_->add ( event->scenePos ( ) );
				last_point_ = event->scenePos ( );
			}
			else
			{
				/// To create a box by any direction
				qreal x1 = boundary_anchor_point_.x ( );
				qreal y1 = boundary_anchor_point_.y ( );
				qreal x2 = event->scenePos ( ).x ( );
				qreal y2 = event->scenePos ( ).y ( );

				if ( boundary_anchor_point_.x ( ) < event->scenePos ( ).x ( ) )
				{
					x1 = boundary_anchor_point_.x ( );
					x2 = event->scenePos ( ).x ( );
				}
				else
				{
					x1 = event->scenePos ( ).x ( );
					x2 = boundary_anchor_point_.x ( );
				}

				if ( boundary_anchor_point_.y ( ) < event->scenePos ( ).y ( ) )
				{
					y1 = boundary_anchor_point_.y ( );
					y2 = event->scenePos ( ).y ( );
				}
				else
				{
					y1 = event->scenePos ( ).y ( );
					y2 = boundary_anchor_point_.y ( );
				}
				this->boundaryc_->setNewBoundary ( x1 , y1 , x2 - x1 , y2 - y1 );
			}
		}
	}

	update ( );
}

void SketchSessionTesting::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
{

	if ( mode_ == InteractionMode::EDITING )
	{
		QGraphicsScene::mouseReleaseEvent ( event );
		event->ignore();
	}
	else
	{
		QPolygonF new_curve = input_sketch_->getSketch();


		if ( this->boundary_sketching_ == true )
		{
			emit newSessionSignal ( this->boundaryc_->boundingRect ( ).x( ) ,
						this->boundaryc_->boundingRect ( ).y ( ) ,
						this->boundaryc_->boundingRect ( ).width ( ) + this->boundaryc_->boundingRect( ).x ( ) ,
						this->boundaryc_->boundingRect ( ).height ( ) + this->boundaryc_->boundingRect( ).y( ) );

			this->boundary_sketching_ = false;
		}
		else
		{
			if ( !new_curve.isClosed ( ) )
			{
				emit newSketchCurve(new_curve);
			}

			input_sketch_->clear ( );
		}

	}

	update ( );
}

void SketchSessionTesting::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	if ( event->mimeData ( )->hasText ( ) )
	{
		event->acceptProposedAction ( );
		qDebug ( ) << "Enter";
	}
	else
	{
		qDebug ( ) << "Not Text";
	}

	QStringList list = event->mimeData ( )->formats ( );

	QString str;
	if ( event->mimeData ( )->hasImage ( ) )
	{
		qDebug ( ) << "There is an image";
	}

//	foreach (str, list)
//	{
//		qDebug() << "Mine Type "<< str;
//	}
	event->accept ( );
}

void SketchSessionTesting::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	/// @see http://doc.qt.io/qt-5/qmimedata.html#hasImage
	//qDebug() << "Moving";
}

void SketchSessionTesting::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	qDebug ( ) << "Leaving";
}
/// Then, we invoke acceptProposedAction() on event, setting the drop action to the one proposed.
/// Lastly, we emit the changed() signal, with the data that was dropped and its MIME type information as a parameter.
/// For dragMoveEvent(), we just accept the proposed QDragMoveEvent object, event, with acceptProposedAction().
///The DropArea class's implementation of dropEvent() extracts the event's mime data and displays it accordingly.
void SketchSessionTesting::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
	const QMimeData *mimeData = event->mimeData ( );

	QString text;

	if ( event->mimeData ( )->hasUrls ( ) )
	{
		/// Its a list because I can drag and drop more than one file.
		/// Since, I assume only one Image begin drag and droping, thus text=url will work;
		/// @see http://doc.qt.io/qt-5/qurl.html#toLocalFile, I thing it works on Linux and Win32.
		/// FIXME: I did not know, it was possible to apply more than one predicate for the for looping
		QList<QUrl> urlList = mimeData->urls ( );
		for ( int i = 0; i < urlList.size ( ) && i < 32; ++i )
		{
			qDebug ( ) << "Url path" << urlList.at ( i ).toLocalFile ( );
			QString url = urlList.at ( i ).toLocalFile ( );
			text = url;			// + QString("\n");
		}
	}

	QString str = QDir::toNativeSeparators ( text );

	qDebug ( ) << "Before Removing " << str;

	/// @see http://www.qtcentre.org/threads/23100-QUrl-toLocalFile-not-working-as-exspected-under-Windows
	QPixmap pixmap ( str );

	qDebug ( ) << "After Removing " << str;

	boundary_sketching_ = true;

	emit newSessionSignal ( pixmap );

	//initializationWithImage(pixmap);
}

void SketchSessionTesting::clear()
{
	 /// clear the map of curves
	 /// @todo clear all attributes
	 for( auto& curve_iterator: this->view_curves_)
	 {
	        delete	curve_iterator.second;
	 }
	 /// clear the map of curves
	 /// @todo clear all attributes
	 for( auto& vertex_iterator: this->view_vertices_)
	 {
	        delete	vertex_iterator.second;
	 }

	 this->view_curves_.clear();
	 this->view_vertices_.clear();

	 this->boundaryc_->setNewBoundary(0.0,0.0,0.0,0.0);

	 update();
}

bool SketchSessionTesting::initializationWithImage ( const QPixmap& pixmap )
{
	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

	/// Creating and adding the boundary

	clear();

	overlay_image_->setPixmap ( pixmap );

	qreal x = pixmap.rect ( ).x ( );
	qreal y = pixmap.rect ( ).y ( );
	qreal w = pixmap.rect ( ).width ( );
	qreal h = pixmap.rect ( ).height ( );

	this->boundaryc_->setNewBoundary ( x , y , w , h );
	this->setSceneRect ( this->boundaryc_->boundingRect( ) );

	setUpBackground();

	return true;
}

bool SketchSessionTesting::initialization ( qreal x , qreal y , qreal w , qreal h )
{

	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

//	/// Creating and adding the boundary
//	this->boundary_->setRect ( x , y , w - x , h - y );
//	this->boundaryc->setNewBoundary ( x , y , w - x , h - y );
	this->boundaryc_->setNewBoundary ( x , y , w , h );
	this->setSceneRect ( this->boundaryc_->boundingRect( ) );


	setUpBackground();

	return true;
}

void SketchSessionTesting::sketchNewBoundary ( )
{
	this->boundary_sketching_ = true;
}

void SketchSessionTesting::curveSmoothed ( QPolygonF raw_skecth_ )
{

}

void SketchSessionTesting::insertCurve( unsigned int _id ,QPolygonF _curve )
{


//	HorizonController * curve = new HorizonController ( Qt::blue );
//
//	QPainterPath p;
//
//	p.addPolygon ( _curve );
//
//	curve->setSketching( p );
//
//	this->addItem ( curve );
//
//	curves_[_id] = curve;
//
//	this->update ( );

}

void SketchSessionTesting::setEditMode()
{
	mode_ = InteractionMode::EDITING;
}

void SketchSessionTesting::setSketchMode()
{
	mode_ = InteractionMode::SKETCHING;
}

void SketchSessionTesting::setUpBackground ( )
{
	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup

	int step = 20;

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );

	for ( float radius = 0.0; radius <= 1.0; radius +=0.1)
	{
		gradient.setColorAt ( radius , QColor::fromRgb ( 154.28 + (radius*step) , 176.20 + (radius*step), 199.16 + (radius*step) ) );
	}

	this->setBackgroundBrush ( QBrush ( gradient ) );
}

void SketchSessionTesting::updateSBIM(const std::map<unsigned int, QPolygonF>& _polycurves, const std::map<unsigned int, QPointF>& _vertices)
{

	for ( auto& polycurve_iterator : _polycurves )
	{
		// todo if the curve exist, update it only it have changed
		if ( this->view_curves_.count(polycurve_iterator.first) )
		{
			//std::cout << "The curve exist " <<  polycurve_iterator.first << std::endl;

			view_curves_[polycurve_iterator.first]->setSketch(polycurve_iterator.second);

		}
		// todo, create an appropriate QGraphicsItem from the new curve ( which geoObject which it represents)
		else
		{
			//std::cout << " It's a new curve " << polycurve_iterator.first << std::endl;

			StratigraphyItem * new_view_curve = new StratigraphyItem(Qt::black);
			this->addItem(new_view_curve);

			new_view_curve->setSketch(polycurve_iterator.second);
			view_curves_[polycurve_iterator.first] = new_view_curve;
		}

		//std::cout << " Size " << polycurve_iterator.second.size() << std::endl;
	}


	for (auto& vertice_iterator : _vertices)
	{

		// todo if the vertice exist, update it only it have changed
		if ( this->view_vertices_.count(vertice_iterator.first) )
		{
			view_vertices_[vertice_iterator.first]->setRect(vertice_iterator.second.x()-25,vertice_iterator.second.y()-25,50.0,50.0);
		}else
		{
			QGraphicsEllipseItem* node = new QGraphicsEllipseItem(vertice_iterator.second.x()-25,vertice_iterator.second.y()-25,50.0,50.0);
			view_vertices_[vertice_iterator.first] = node;
			this->addItem(node);
		}
	}


	setUpBackground();
	update();
}
