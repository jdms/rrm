#include "SketchSession.hpp"

SketchSession::SketchSession ( QObject *parent ) : QGraphicsScene ( parent )
{
	is_ready = false;
	boundary_ = 0;
	/// Drag and Drop Feature
	image = new QGraphicsPixmapItem ( );
	this->addItem ( image );

	/// Clarissa
	QPen pen;

	pen.setColor ( QColor ( 255 , 100 , 70 ) );
	pen.setWidth ( 5 );

	sketch = new InputSketch ( QColor ( 255 , 75 , 75 ) );
	sketch->setPen ( pen );
	this->addItem ( sketch );

	this->sketch->setZValue(1);
	/// Clarissa


	QPen pen_boundary ( QColor ( 0 , 100 , 0 ) );
	pen_boundary.setWidth ( 1 );
	QBrush brush_boundary;
	brush_boundary.setColor ( QColor ( 180 , 255 , 180 , 180 ) );
	brush_boundary.setStyle ( Qt::SolidPattern );

	sketch_pen.setColor ( QColor ( 187 , 15 , 32 ) );

	boundaryc = new BoundaryController ( 0 , 0 );
	this->addItem ( boundaryc );

	//this->addItem(this->boundary_);
	this->boundary_ = new QGraphicsRectItem ( );
	this->boundary_->setPen ( pen_boundary );
	this->boundary_->setBrush ( brush_boundary );
	this->boundary_diagonal_ = new QGraphicsLineItem ( );
	this->boundary_sketching_ = false;

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );
	gradient.setColorAt ( 1.0 , QColor::fromRgb ( 170 , 170 , 170 ) );
	gradient.setColorAt ( 0.8 , QColor::fromRgb ( 180 , 180 , 180 ) );
	gradient.setColorAt ( 0.6 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.4 , QColor::fromRgb ( 190 , 190 , 190 ) );
	gradient.setColorAt ( 0.2 , QColor::fromRgb ( 210 , 210 , 210 ) );
	gradient.setColorAt ( 0.0 , QColor::fromRgb ( 215 , 215 , 215 ) );

	this->setBackgroundBrush ( QBrush ( gradient ) );

	this->halfedges_ = new QGraphicsItemGroup ( );

	this->addItem ( halfedges_ );

}

bool SketchSession::initialization_with_image ( const QPixmap& pixmap )
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
	if ( this->halfedges_ != nullptr )
	{
		this->removeItem ( this->halfedges_ );
		QList<QGraphicsItem *> children = this->halfedges_->childItems ( );
		if ( children.size ( ) > 0 )
		{
			qDeleteAll ( children );

		}
		delete this->halfedges_;
		this->halfedges_ = 0;
		this->update ( );
	}

	this->halfedges_ = nullptr;
	input_line_group = nullptr;

	this->halfedges_ = new QGraphicsItemGroup ( );
	this->addItem ( halfedges_ );


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

bool SketchSession::initialization ( qreal x , qreal y , qreal w , qreal h )
{

	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

	/// Creating and adding the boundary
	this->boundary_->setRect ( x , y , w - x , h - y );
	this->boundaryc->setNewBoundary ( x , y , w - x , h - y );
	this->setSceneRect ( x , y , w - x , h - y );


	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup
	if ( this->halfedges_ != nullptr )
	{
		this->removeItem ( this->halfedges_ );
		QList<QGraphicsItem *> children = this->halfedges_->childItems ( );
		if ( children.size ( ) > 0 )
		{
			qDeleteAll ( children );

		}
		delete this->halfedges_;
		this->halfedges_ = 0;
		this->update ( );
	}

	this->halfedges_ = nullptr;
	input_line_group = nullptr;

	this->halfedges_ = new QGraphicsItemGroup ( );
	this->addItem ( halfedges_ );

	int step = 20;

	QRadialGradient gradient ( (qreal) width ( ) / 2.0 , (qreal) height ( ) / 2.0 , this->width ( ) );

	for ( float radius = 0.0; radius <= 1.0; radius +=0.1)
	{
		gradient.setColorAt ( radius , QColor::fromRgb ( 154.28 + (radius*step) , 176.20 + (radius*step), 199.16 + (radius*step) ) );
	}

	this->setBackgroundBrush ( QBrush ( gradient ) );

	return true;
}

SketchSession::~SketchSession ( )
{

}

void SketchSession::mousePressEvent ( QGraphicsSceneMouseEvent* event )
{

	if ( event->buttons ( ) & Qt::LeftButton )
	{
		qDebug ( ) << " Item Grabbed " << event->buttonDownScenePos ( Qt::LeftButton );
		last_point_ = event->buttonDownScenePos ( Qt::LeftButton );
		input_line_.clear ( );

		this->boundary_anchor_point_ = last_point_;

		if ( ( this->boundary_sketching_ == false ) )
		{
			sketch->create ( event->scenePos ( ) );
		}else
		{
			/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup
			if ( this->halfedges_ != nullptr )
			{
				this->removeItem ( this->halfedges_ );
				QList<QGraphicsItem *> children = this->halfedges_->childItems ( );
				if ( children.size ( ) > 0 )
				{
					qDeleteAll ( children );

				}
				delete this->halfedges_;
				this->halfedges_ = 0;
				this->update ( );
			}

			this->halfedges_ = nullptr;
			input_line_group = nullptr;

			this->halfedges_ = new QGraphicsItemGroup ( );
			this->addItem ( halfedges_ );
		}

	}

	update ( );
}

void SketchSession::mouseReleaseEvent ( QGraphicsSceneMouseEvent* event )
{

	QPolygonF p ( input_line_ );

	if ( !p.isClosed ( ) )
	{
		emit smoothSketchSignal ( p );
	}

	if ( this->boundary_sketching_ == true )
	{
		emit newSessionSignal ( this->boundary_->rect ( ).x ( ) , this->boundary_->rect ( ).y ( ) , this->boundary_->rect ( ).width ( ) + this->boundary_->rect ( ).x ( ) ,
																		this->boundary_->rect ( ).height ( ) + this->boundary_->rect ( ).y ( ) );

		this->boundary_sketching_ = false;
	}
	else
	{
		sketch->clear ( );
	}
	update ( );
}

void SketchSession::mouseMoveEvent ( QGraphicsSceneMouseEvent* event )
{
	if ( event->buttons ( ) & Qt::LeftButton )
	{
		//this->svg->setPos(event->scenePos());

		if ( this->boundary_sketching_ == false )
		{
			input_line_.push_back ( event->scenePos ( ) );
			sketch->add ( event->scenePos ( ) );
			last_point_ = event->scenePos ( );
		}
		else
		{
			/// To create a box by any direction
			QLineF l;
			l.setPoints ( boundary_anchor_point_ , event->scenePos ( ) );
			this->boundary_diagonal_->setLine ( l );

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

	update ( );
}

void SketchSession::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
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

	foreach (str, list){
	qDebug() << "Mine Type "<< str;
}
	event->accept ( );
}
void SketchSession::dragLeaveEvent ( QGraphicsSceneDragDropEvent * event )
{
	qDebug ( ) << "Leaving";
}
/// Then, we invoke acceptProposedAction() on event, setting the drop action to the one proposed.
/// Lastly, we emit the changed() signal, with the data that was dropped and its MIME type information as a parameter.
/// For dragMoveEvent(), we just accept the proposed QDragMoveEvent object, event, with acceptProposedAction().
void SketchSession::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	/// @see http://doc.qt.io/qt-5/qmimedata.html#hasImage
	//qDebug() << "Moving";
}
///The DropArea class's implementation of dropEvent() extracts the event's mime data and displays it accordingly.
void SketchSession::dropEvent ( QGraphicsSceneDragDropEvent * event )
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
}

void SketchSession::newBoundarySlot ( )
{
	this->boundary_sketching_ = true;
}

void SketchSession::smoothCurveSlot ( QPolygonF smooth_sketch )
{
	this->sketch_path.addPolygon ( smooth_sketch );
	//sketch_item = this->addPath(sketch_path);
}

void SketchSession::updatePaths ( std::vector<QPainterPath>& paths )
{

}

void SketchSession::removeInputSketch ( )
{
}

void SketchSession::drawPolyLine ( std::vector<QPointF> points )
{

}

