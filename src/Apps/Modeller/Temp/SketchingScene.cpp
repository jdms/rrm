#include "Apps/Modeller/SketchingScene.h"

SketchingScene::SketchingScene( QObject *parent ): QGraphicsScene( parent )
{
    sketch = NULL;
    horizonc = NULL;
    boundaryc = NULL;

    mode = SketchingScene::InteractionMode::SKETCHING;

    createBoundary();

    ghost_image = new QGraphicsPixmapItem ();
    this->addItem(ghost_image);



    boundary_sketching_ = 1;

}

SketchingScene::~SketchingScene()
{
    if( horizonc == NULL )
        delete horizonc;
    horizonc = NULL;

    if( sketch == NULL )
        delete sketch;
    sketch = NULL;

    if( boundaryc == NULL )
        delete boundaryc;
    boundaryc = NULL;
}

void SketchingScene::createBoundary()
{

    boundaryc = new BoundaryController(  600,  400 );
    this->addItem( boundaryc );
}

bool SketchingScene::initialization_with_image( const QPixmap& pixmap  )
{
    // The bounding rectangle of the scene
    // The scene rectangle defines the extent of the scene.
    // It is primarily used by QGraphicsView
    // to determine the view's default scroll able area,
    // and by QGraphicsScene to manage item indexing.

    /// Creating and adding the boundary

	ghost_image->setPixmap(pixmap);

	qreal x = pixmap.rect().x();
	qreal y = pixmap.rect().y();
	qreal w = pixmap.rect().width();
	qreal h = pixmap.rect().height();

	this->boundaryc->setNewBoundary( x, y, w, h );

	// @see http://doc.qt.io/qt-4.8/qgraphicsitem.html#stackBefore
	this->ghost_image->stackBefore(boundaryc);
//	this->setSceneRect(x,y,w,h);


//    /// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup
//	if(this->halfedges_ != nullptr)
//	{
//		this->removeItem(this->halfedges_);
//		QList<QGraphicsItem *>	children = this->halfedges_->childItems();
//		if(children.size() > 0)
//		{
//			qDeleteAll(children);
//
//		}
//		delete this->halfedges_;
//		this->halfedges_ = 0;
//		this->update();
//	}
//
//    this->halfedges_ = nullptr;
//    input_line_group = nullptr;
//
//    this->halfedges_ = new QGraphicsItemGroup();
//    this->addItem(halfedges_);

    return true;
}


void SketchingScene::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    if( mode == InteractionMode::SELECT )
        return;

    if( event->buttons() == Qt::MouseButton::LeftButton  )
    {

        sketch = new InputSketch( currentColor );
        sketch->create( event->scenePos() );
        this->addItem( sketch );

    }

    this->boundary_anchor_point_ = event->scenePos();
	
    QGraphicsScene::mousePressEvent( event );
    update();
}



void SketchingScene::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
	if (event->mimeData()->hasText())
	{
		event->acceptProposedAction();
		qDebug() << "Enter";
	}else
	{
		qDebug() << "Not Text";
	}

	QStringList list = event->mimeData()->formats();

	QString str;
	if (event->mimeData()->hasImage())
	{
		qDebug() << "There is an image";
	}

	foreach (str, list)
	{
		qDebug() << "Mine Type "<< str;
	}
	event->accept();
}
void SketchingScene::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
	qDebug() << "Leaving";
}
/// Then, we invoke acceptProposedAction() on event, setting the drop action to the one proposed.
/// Lastly, we emit the changed() signal, with the data that was dropped and its MIME type information as a parameter.
/// For dragMoveEvent(), we just accept the proposed QDragMoveEvent object, event, with acceptProposedAction().
void SketchingScene::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
	/// @see http://doc.qt.io/qt-5/qmimedata.html#hasImage
	//qDebug() << "Moving";
}
///The DropArea class's implementation of dropEvent() extracts the event's mime data and displays it accordingly.
void SketchingScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
	const QMimeData *mimeData = event->mimeData();

	QString text;

	if (event->mimeData()->hasUrls())
	{
			/// Its a list because I can drag and drop more than one file.
			/// Since, I assume only one Image begin drag and droping, thus text=url will work;
			/// @see http://doc.qt.io/qt-5/qurl.html#toLocalFile, I thing it works on Linux and Win32.
			/// FIXME: I did not know, it was possible to apply more than one predicate for the for looping
			QList<QUrl> urlList = mimeData->urls();
			for (int i = 0; i < urlList.size() && i < 32; ++i)
			{
				qDebug() << "Url path" << urlList.at(i).toLocalFile();
				QString url = urlList.at(i).toLocalFile();
				text = url;// + QString("\n");
			}
	}

	QString str = QDir::toNativeSeparators(text);

	qDebug() << "Before Removing "<< str;

	/// @see http://www.qtcentre.org/threads/23100-QUrl-toLocalFile-not-working-as-exspected-under-Windows
	QPixmap pixmap(str);

	qDebug() << "After Removing " << str;

	initialization_with_image(pixmap);

//	boundary_sketching_ = true;
//	emit newSessionSignal(pixmap);
}

void SketchingScene::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{

    if( mode == InteractionMode::SELECT )
        return;

    if( event->buttons() == Qt::MouseButton::LeftButton )
    {
	if ( boundary_sketching_ )
	{
		QLineF l;
		l.setPoints ( boundary_anchor_point_ , event->scenePos ( ) );

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
		this->boundaryc->setNewBoundary( x1 , y1 , x2 - x1 , y2 - y1 );
	}
	else
	{
		 sketch->add( event->scenePos() );
	}

    }


    QGraphicsScene::mouseMoveEvent( event );
    update();
}


void SketchingScene::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{

    if( mode == InteractionMode::SELECT )
        return;

    sketch->setDone( true );
    horizonc = new HorizonController( currentColor );
    QPainterPath input = sketch->getSketch();
    horizonc->setSketching( &input );

    this->removeItem( sketch );
    bool valid = horizonc->isValid( this );

    if( valid == false ){
        delete sketch;

        QGraphicsScene::mouseReleaseEvent( event );
        update();
        return;
    }

    horizonc->applyRule();
    this->addItem( horizonc );
    updateAllElements();

    QGraphicsScene::mouseReleaseEvent( event );
    update();


}




void SketchingScene::applyRemoveAbove()
{
    horizonc->setRule( HorizonController::RuleType::REMOVE_ABOVE );
}



void SketchingScene::applyRemoveBelow()
{
    horizonc->setRule( HorizonController::RuleType::REMOVE_BELOW );
}



void SketchingScene::updateAllElements()
{

    // only a test to verify if the updateGeometry is being called correctly
//    for( auto item: this->items() )
//    {
//        if( item->type() == HorizonController::ControllerType::HORIZON )
//        {
//            HorizonController *hc = (HorizonController*) item;
//            hc->updateGeometry();

//        }


//    }

}


void SketchingScene::setColor( int R, int G, int B )
{
    currentColor = QColor( R, G, B );
//    horizonc->setColor( R, G, B );
}


void SketchingScene::getColor( int R, int G, int B )
{
    R = currentColor.red();
    G = currentColor.green();
    B = currentColor.blue();

}


void SketchingScene::setModeSelect()
{
    mode = InteractionMode::SELECT;
}

void SketchingScene::setModeSketching()
{
    mode = InteractionMode::SKETCHING;
}
