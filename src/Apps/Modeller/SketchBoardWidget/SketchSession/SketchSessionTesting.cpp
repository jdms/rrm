#include "SketchSessionTesting.hpp"


SketchSessionTesting::SketchSessionTesting ( QObject *parent ) : QGraphicsScene ( parent )
{
	mode_ = InteractionMode::SKETCHING;

	boundary_ = 0;
	/// Drag and Drop Feature
	image = new QGraphicsPixmapItem ( );
	this->addItem ( image );

	QPen pen;

	pen.setColor ( QColor ( 255 , 100 , 70 ) );
	pen.setWidth ( 5 );


	input_sketch_ = new InputSketch ( QColor ( 255 , 75 , 75 ) );
	input_sketch_->setPen ( pen );
	this->addItem ( input_sketch_ );
	this->input_sketch_->setZValue(1);


	QPen pen_boundary ( QColor ( 0 , 100 , 0 ) );
	pen_boundary.setWidth ( 1 );
	QBrush brush_boundary;
	brush_boundary.setColor ( QColor ( 180 , 255 , 180 , 180 ) );
	brush_boundary.setStyle ( Qt::SolidPattern );

	sketch_pen.setColor ( QColor ( 187 , 15 , 32 ) );

	boundaryc = new BoundaryItem ( 0 , 0 );
	this->addItem ( boundaryc );

	//this->addItem(this->boundary_);
	this->boundary_ = new QGraphicsRectItem ( );
	this->boundary_->setPen ( pen_boundary );
	this->boundary_->setBrush ( brush_boundary );
	this->boundary_sketching_ = true;

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );
	gradient.setColorAt ( 1.0 , QColor::fromRgb ( 170 , 170 , 170 ) );
	gradient.setColorAt ( 0.8 , QColor::fromRgb ( 180 , 180 , 180 ) );
	gradient.setColorAt ( 0.6 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.4 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.2 , QColor::fromRgb ( 210 , 210 , 210 ) );
	gradient.setColorAt ( 0.0 , QColor::fromRgb ( 215 , 215 , 215 ) );

	this->setBackgroundBrush ( QBrush ( gradient ) );

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

}

SketchSessionTesting::~SketchSessionTesting ( )
{
	 for( auto& curves_iterator: this->view_curves_)
	 {
	        delete	curves_iterator.second;
	 }
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
				this->boundary_->setRect ( x1 , y1 , x2 - x1 , y2 - y1 );
				this->boundaryc->setNewBoundary ( x1 , y1 , x2 - x1 , y2 - y1 );
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

		if ( !new_curve.isClosed ( ) )
		{
			emit newSketchCurve(new_curve);
		}

		if ( this->boundary_sketching_ == true )
		{
			emit newSessionSignal ( this->boundary_->rect ( ).x ( ) , this->boundary_->rect ( ).y ( ) , this->boundary_->rect ( ).width ( ) + this->boundary_->rect ( ).x ( ) ,
																			this->boundary_->rect ( ).height ( ) + this->boundary_->rect ( ).y ( ) );

			this->boundary_sketching_ = false;
		}
		else
		{
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

	 this->view_curves_.clear();
}

bool SketchSessionTesting::initializationWithImage ( const QPixmap& pixmap )
{
	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

	/// Creating and adding the boundary
	image->setPixmap ( pixmap );

	qreal x = pixmap.rect ( ).x ( );
	qreal y = pixmap.rect ( ).y ( );
	qreal w = pixmap.rect ( ).width ( );
	qreal h = pixmap.rect ( ).height ( );

	this->boundary_->setRect ( x , y , w , h );
	this->boundaryc->setNewBoundary ( x , y , w , h );
	this->setSceneRect ( this->boundary_->rect ( ) );

	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );
	gradient.setColorAt ( 1.0 , QColor::fromRgb ( 154.28 , 176.20, 199.16 ) );
	gradient.setColorAt ( 0.8 , QColor::fromRgb ( 180 , 180 , 180 ) );
	gradient.setColorAt ( 0.6 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.4 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.2 , QColor::fromRgb ( 210 , 210 , 210 ) );
	gradient.setColorAt ( 0.0 , QColor::fromRgb ( 215 , 215 , 215 ) );

	this->setBackgroundBrush ( QBrush ( gradient ) );

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
	this->setSceneRect ( x , y , w , h );
	this->boundary_->setRect ( x , y , w - x , h - y );
	this->boundaryc->setNewBoundary ( x , y , w - x , h - y );

	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup

	int step = 20;

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );

	for ( float radius = 0.0; radius <= 1.0; radius +=0.1)
	{
		gradient.setColorAt ( radius , QColor::fromRgb ( 154.28 + (radius*step) , 176.20 + (radius*step), 199.16 + (radius*step) ) );
	}

	this->setBackgroundBrush ( QBrush ( gradient ) );

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

void SketchSessionTesting::updateSBIM(const std::map<unsigned int, QPolygonF>& _polycurves, const std::map<unsigned int, QPointF>& _vertices)
{

	for ( auto& polycurve_iterator : _polycurves )
	{
		// todo if the curve exist, update it only it have changed
		if ( this->view_curves_.count(polycurve_iterator.first) )
		{
			std::cout << "The curve exist " <<  polycurve_iterator.first << std::endl;

			view_curves_[polycurve_iterator.first]->setSketch(polycurve_iterator.second);

		}
		// todo, create an appropriate QGraphicsItem from the new curve ( which geoObject which it represents)
		else
		{
			std::cout << " It's a new curve " << polycurve_iterator.first << std::endl;

			StratigraphyItem * new_view_curve = new StratigraphyItem(Qt::black);
			this->addItem(new_view_curve);

			new_view_curve->setSketch(polycurve_iterator.second);
			view_curves_[polycurve_iterator.first] = new_view_curve;
		}

		std::cout << " Size " << polycurve_iterator.second.size() << std::endl;
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


	update();
}
