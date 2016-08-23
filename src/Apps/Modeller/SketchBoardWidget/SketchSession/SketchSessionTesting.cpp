#include "SketchSessionTesting.hpp"


SketchSessionTesting::SketchSessionTesting(QObject *parent) : QGraphicsScene(parent)
{

    connect( this, SIGNAL( sendCoordinates( float, float ) ), this->parent(), SLOT( updateCoordinates( float, float ) ) );


    setup();


}


SketchSessionTesting::~SketchSessionTesting()
{
	clear();

}


void SketchSessionTesting::setup()
{

    current_mode  = InteractionMode::OVERSKETCHING;

    sketch = new InputSketch( QColor( 255, 75, 75 ) );
    addItem( sketch );


    boundary = new BoundaryItem( 0.0, 0.0 );
    boundary->setZValue( 0 );
    this->addItem( boundary );










    mode_ = InteractionMode::OVERSKETCHING;


    // Default Color
    this->current_color_ = QColor(255, 75, 75);

    QPen pen;
    pen.setColor(this->current_color_);
    pen.setWidth(3);

    /// Drag and Drop Feature
    overlay_image_ = new QGraphicsPixmapItem();
    this->addItem(overlay_image_);
    overlay_image_->setZValue(0);




    /* felipe's code

    boundaryc_ = new BoundaryItem(0.0, 0.0, QColor(55, 100, 55, 75));
    this->addItem(boundaryc_);
    boundaryc_->setZValue(0);

    input_sketch_ = new InputSketch(this->current_color_);
    input_sketch_->setPen(pen);
    input_sketch_->setZValue(2);


    current_sketch_ = new InputSketch(this->current_color_);
    current_sketch_->setPen(pen);
//    this->addItem(current_sketch_);
    this->current_sketch_->setZValue(2);

    */

    this->boundary_sketching_ = false;



    setUpBackground();

    /// qualitative pastel color @see http://colorbrewer2.org/
    //	141,211,199
    //	255,255,179
    //	190,186,218
    //	251,128,114
    //	128,177,211


//	colors.push_back(QColor(Qt::darkGreen));
//	colors.push_back(QColor(141, 211, 199));
//	colors.push_back(QColor(255, 255, 179));
//	colors.push_back(QColor(190, 186, 218));
//	colors.push_back(QColor(251, 128, 114));
//	colors.push_back(QColor(128, 177, 211));
//	colors.push_back(QColor(Qt::darkRed));
//	colors.push_back(QColor(Qt::magenta));
//	colors.push_back(QColor(Qt::darkMagenta));
//	colors.push_back(QColor(Qt::green));
//	colors.push_back(QColor(Qt::darkGreen));


    /// FIXME Region Point
    region_pointer_ = new QGraphicsPixmapItem();

    QPixmap pointer(":/images/icons/regionPointer.png");

    region_pointer_->setPixmap(pointer);

//    this->addItem(region_pointer_);

    region_pointers_ = new QGraphicsItemGroup();

    region_pointers_->setVisible(false);

//    this->addItem(region_pointers_);


}


void SketchSessionTesting::addSketching()
{

    if( sketch != NULL )
    {
        emit addSkecthing( sketch );

        sketch = new InputSketch( QColor( 255, 75, 75 ) );
        addItem( sketch );


    }



}


void SketchSessionTesting::mousePressEvent( QGraphicsSceneMouseEvent* event )
{


    std::cout << "SketchSessionTesting::mousePressEvent coord" << std::endl;
    std::cout << event->scenePos().x() << ", " << event->scenePos().y() << std::endl;


    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_mode == InteractionMode::BOUNDARY )
        {
            boundary_anchor = event->buttonDownScenePos( Qt::LeftButton );

        }
        else
        {

            current_mode = InteractionMode::OVERSKETCHING;

            temp_sketch = new InputSketch( QColor(255, 75, 75) );
            temp_sketch->create( event->scenePos() );

            addItem( temp_sketch );

        }

    }


    else if (event->buttons() & Qt::RightButton )
    {
        current_mode = InteractionMode::INSERTING;
        addSketching();

    }


    QGraphicsScene::mousePressEvent( event );
    update();


/* FELIPE'S CODE

    if (mode_ == InteractionMode::REGION_POINT)
    {
        QGraphicsPixmapItem * temp_pix_item = new QGraphicsPixmapItem();
        QPixmap temp_pixmap(":/images/icons/regionPointer.png");
        temp_pix_item->setPixmap(temp_pixmap);
        temp_pix_item->setPos(event->scenePos().x() - region_pointer_->pixmap().width()*0.5, event->scenePos().y() - region_pointer_->pixmap().height()*0.5);
        region_pointers_->addToGroup(temp_pix_item);
        region_points_.push_back(event->scenePos());

    }
    else if (mode_ == InteractionMode::EDITING)
    {
        QGraphicsScene::mousePressEvent(event);
        event->ignore();
    }
    else
    {
        if (event->buttons() & Qt::LeftButton)
        {
            qDebug() << " Item Grabbed " << event->buttonDownScenePos(Qt::LeftButton);

            last_point_ = event->buttonDownScenePos(Qt::LeftButton);

            this->boundary_anchor_point_ = last_point_;

            if ((this->boundary_sketching_ == false))
            {
                input_sketch_->create(event->scenePos());
            }
            else
            {
            }
        }
        else if (event->buttons() & Qt::RightButton)
        {
            newSktech();
        }
    }
    event->ignore();


    update();


*/


}


void SketchSessionTesting::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

    emit sendCoordinates( event->scenePos().x(), event->scenePos().y() );



    if ( event->buttons() & Qt::LeftButton )
    {

        if( current_mode == InteractionMode::OVERSKETCHING )
            temp_sketch->add( event->scenePos() );

        else if( current_mode == InteractionMode::BOUNDARY )
        {
            int w = event->scenePos().x() - boundary_anchor.x();
            int h = event->scenePos().y() - boundary_anchor.y();

            setSceneRect( boundary_anchor.x(), boundary_anchor.y(), w,  h );
            boundary->setNewBoundary( boundary_anchor.x(), boundary_anchor.y(), w,  h );


        }



    }


    QGraphicsScene::mouseMoveEvent( event );
    update();


/* FELIPE'S CODE

    if (mode_ == InteractionMode::REGION_POINT)
    {
        region_pointer_->setPos(event->scenePos().x() - region_pointer_->pixmap().width()*0.5, event->scenePos().y() - region_pointer_->pixmap().height()*0.5);

    }
    else if (mode_ == InteractionMode::EDITING)
    {
        QGraphicsScene::mouseMoveEvent(event);
        event->ignore();
    }
    else
    {
        if (event->buttons() & Qt::LeftButton)
        {
            //this->svg->setPos(event->scenePos());

            if (this->boundary_sketching_ == false)
            {
                input_sketch_->add(event->scenePos());
                last_point_ = event->scenePos();
            }
            else
            {
                /// To create a box by any direction
                qreal x1 = boundary_anchor_point_.x();
                qreal y1 = boundary_anchor_point_.y();
                qreal x2 = event->scenePos().x();
                qreal y2 = event->scenePos().y();

                if (boundary_anchor_point_.x() < event->scenePos().x())
                {
                    x1 = boundary_anchor_point_.x();
                    x2 = event->scenePos().x();
                }
                else
                {
                    x1 = event->scenePos().x();
                    x2 = boundary_anchor_point_.x();
                }

                if (boundary_anchor_point_.y() < event->scenePos().y())
                {
                    y1 = boundary_anchor_point_.y();
                    y2 = event->scenePos().y();
                }
                else
                {
                    y1 = event->scenePos().y();
                    y2 = boundary_anchor_point_.y();
                }
                this->boundaryc_->setNewBoundary(x1, y1, x2 - x1, y2 - y1);
                setSceneRect( boundary->boundingRect() );
            }
        }


    }
    event->ignore();



    update();

    */



}


void SketchSessionTesting::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{

    if ( current_mode == InteractionMode::OVERSKETCHING  )
    {

        sketch->addSegment( *temp_sketch );
        removeItem( temp_sketch );

        if( temp_sketch != NULL )
        {
            delete temp_sketch;
            temp_sketch = NULL;
        }

    }

    else if( current_mode == InteractionMode::BOUNDARY )
    {
        int w = event->scenePos().x() - boundary_anchor.x();
        int h = event->scenePos().y() - boundary_anchor.y();

        setSceneRect( boundary_anchor.x(), boundary_anchor.y(), w, h );
        boundary->setNewBoundary( boundary_anchor.x(), boundary_anchor.y(), w, h );


    }


    QGraphicsScene::mouseReleaseEvent( event );
    update();



/* FELIPE'S CODE

    if (mode_ == InteractionMode::REGION_POINT)
    {
        //emit regionPoints(region_points_);
        //region_points_.clear();
    }
    else if (mode_ == InteractionMode::EDITING)
    {
        QGraphicsScene::mouseReleaseEvent(event);
        event->ignore();
    }
    else if (mode_ == InteractionMode::SKETCHING)
    {
        QPolygonF new_curve = input_sketch_->getSketch();


        if (this->boundary_sketching_ == true)
        {

            this->boundary_sketching_ = false;

            emit newBoundary(this->boundaryc_->boundingRect().x(),
                this->boundaryc_->boundingRect().y(),
                this->boundaryc_->boundingRect().width() + this->boundaryc_->boundingRect().x(),
                this->boundaryc_->boundingRect().height() + this->boundaryc_->boundingRect().y());

        }
        else
        {
            if (!new_curve.isClosed())
            {
                emit newSketchCurve(new_curve, input_sketch_->getColor());
            }

            input_sketch_->clear();
        }

    }
    else if (mode_ == InteractionMode::OVERSKETCHING)
    {

        if (this->boundary_sketching_ == true)
        {

            this->boundary_sketching_ = false;

            emit newBoundary(this->boundaryc_->boundingRect().x(),
                this->boundaryc_->boundingRect().y(),
                this->boundaryc_->boundingRect().width() + this->boundaryc_->boundingRect().x(),
                this->boundaryc_->boundingRect().height() + this->boundaryc_->boundingRect().y());

        }
        else
        {
            QPolygonF new_curve = input_sketch_->getSketch();

            // Sketch Too Short
            if (input_sketch_->getSketch().size() < 10)
            {
                event->ignore();
                return;
            }

            if (current_sketch_->getSketch().size() == 0)
            {
                current_sketch_->create(new_curve[0]);
            }

            if (input_curve_.size() == 0)
            {
                input_curve_ = convert(new_curve);
            }
            else
            {
                over_sketch_ = convert(new_curve);

                input_curve_ = over_sketch_.overSketch(input_curve_, rest_, 1, 16);
                input_curve_.douglasPeuckerSimplify(over_sketch_, 1.0);

                input_curve_ = over_sketch_;

                new_curve = convert(input_curve_);
            }

            current_sketch_->clear();
            current_sketch_->create(new_curve[0]);

            for (int p_it = 1; p_it < new_curve.size(); p_it++)
            {
                current_sketch_->add(new_curve[p_it]);
            }

            input_sketch_->clear();
        }

    }
    event->ignore();

    update();

      */


}


void SketchSessionTesting::dragEnterEvent( QGraphicsSceneDragDropEvent * event )
{

    event->accept();

    /* FELIPE'S CODE

    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
        qDebug() << "Enter";
    }
    else
    {
        qDebug() << "Not Text";
    }

    QStringList list = event->mimeData()->formats();

    QString str;
    if (event->mimeData()->hasImage())
    {
        qDebug() << "There is an image";
    }

    //	foreach (str, list)
    //	{
    //		qDebug() << "Mine Type "<< str;
    //	}
    event->accept();

    */

}


void SketchSessionTesting::dropEvent( QGraphicsSceneDragDropEvent * event )
{

    const QMimeData *mime_data = event->mimeData();


    if ( mime_data->hasUrls() == false )
        return;


    QString url_file = mime_data->urls().at( 0 ).toLocalFile();
    url_file = QDir::toNativeSeparators( url_file );

    boundary->setBackGroundImage( url_file );

//    QPixmap background( url_file );
//    addPixmap( background );




    /* FELIPE'S CODE

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
            text = url;			// + QString("\n");
        }
    }

    QString str = QDir::toNativeSeparators(text);

    qDebug() << "Before Removing " << str;

    /// @see http://www.qtcentre.org/threads/23100-QUrl-toLocalFile-not-working-as-exspected-under-Windows
    QPixmap pixmap(str);

    qDebug() << "After Removing " << str;

    boundary_sketching_ = true;

    emit newSessionSignal(pixmap);

    //initializationWithImage(pixmap);

    */

}













































void SketchSessionTesting::initScene()
{
    clear();
    sketches.clear();

}




void SketchSessionTesting::newSktech()
{

/* FELIPE'S CODE


	QPolygonF new_curve = current_sketch_->getSketch();
	emit newSketchCurve(new_curve, current_sketch_->getColor());

	std::cout << " Emit" << std::endl;

	input_sketch_->clear();
	current_sketch_->clear();

	input_curve_.clear();
	over_sketch_.clear();


	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<unsigned int> distr(0, 255);

	int r = distr(eng);  // generates number in the range 1..6
	int g = distr(eng);  // generates number in the range 1..6
	int b = distr(eng);  // generates number in the range 1..6


	setColor(QColor(r, g, b));
*/

}
// View/Qt5 related functions


/*

void SketchSessionTesting::dragEnterEvent(QGraphicsSceneDragDropEvent * event)
{
	if (event->mimeData()->hasText())
	{
		event->acceptProposedAction();
		qDebug() << "Enter";
	}
	else
	{
		qDebug() << "Not Text";
	}

	QStringList list = event->mimeData()->formats();

	QString str;
	if (event->mimeData()->hasImage())
	{
		qDebug() << "There is an image";
	}

	//	foreach (str, list)
	//	{
	//		qDebug() << "Mine Type "<< str;
	//	}
	event->accept();
}


void SketchSessionTesting::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
	/// @see http://doc.qt.io/qt-5/qmimedata.html#hasImage
	//qDebug() << "Moving";
}


void SketchSessionTesting::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
	qDebug() << "Leaving";
}

/// Then, we invoke acceptProposedAction() on event, setting the drop action to the one proposed.
/// Lastly, we emit the changed() signal, with the data that was dropped and its MIME type information as a parameter.
/// For dragMoveEvent(), we just accept the proposed QDragMoveEvent object, event, with acceptProposedAction().
///The DropArea class's implementation of dropEvent() extracts the event's mime data and displays it accordingly.


void SketchSessionTesting::dropEvent(QGraphicsSceneDragDropEvent * event)
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
			text = url;			// + QString("\n");
		}
	}

	QString str = QDir::toNativeSeparators(text);

	qDebug() << "Before Removing " << str;

	/// @see http://www.qtcentre.org/threads/23100-QUrl-toLocalFile-not-working-as-exspected-under-Windows
	QPixmap pixmap(str);

	qDebug() << "After Removing " << str;

	boundary_sketching_ = true;

	emit newSessionSignal(pixmap);

	//initializationWithImage(pixmap);
}
*/



void SketchSessionTesting::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    /// @see http://doc.qt.io/qt-5/qmimedata.html#hasImage
    //qDebug() << "Moving";
}


void SketchSessionTesting::dragLeaveEvent(QGraphicsSceneDragDropEvent * event)
{
    qDebug() << "Leaving";
}


void SketchSessionTesting::setColor(const QColor& _color)
{
	current_color_ = _color;
    /*felipe's code

	input_sketch_->setColor(_color);
	current_sketch_->setColor(_color);

    */
}


void SketchSessionTesting::reset()
{


	if (mode_ == InteractionMode::REGION_POINT)
	{
		// Clear Region Point
		clearRegionMode();
		this->region_pointers_->setVisible(true);
		this->region_pointer_->setVisible(true);
	}
	else
	{
		/// clear the map of curves
		/// @todo clear all attributes
		for (auto& curve_iterator : this->view_curves_)
		{
			delete curve_iterator.second;
		}
		/// clear the map of curves
		/// @todo clear all attributes
		for (auto& vertex_iterator : this->view_vertices_)
		{
			delete vertex_iterator.second;
		}

		this->view_curves_.clear();
		this->view_vertices_.clear();

        /* felipe's code

		input_sketch_->clear();
		current_sketch_->clear();

		input_curve_.clear();
		over_sketch_.clear();

        */

		clearRegionMode();

		update();
	}

}


void SketchSessionTesting::clear()
{

	if (mode_ == InteractionMode::REGION_POINT)
	{
		// Clear Region Point
		clearRegionMode();
		this->region_pointers_->setVisible(true);
		this->region_pointer_->setVisible(true);
	}
	else
	{
		/// clear the map of curves
		/// @todo clear all attributes
		for (auto& curve_iterator : this->view_curves_)
		{
			delete curve_iterator.second;
		}
		/// clear the map of curves
		/// @todo clear all attributes
		for (auto& vertex_iterator : this->view_vertices_)
		{
			delete vertex_iterator.second;
		}

		this->view_curves_.clear();
		this->view_vertices_.clear();

        /* felipe's code
		input_sketch_->clear();
		current_sketch_->clear();

		input_curve_.clear();
		over_sketch_.clear();

      this->boundaryc_->setNewBoundary(0.0, 0.0, 0.0, 0.0);
      setSceneRect( boundary->boundingRect() );
*/



			clearRegionMode();

		update();
	}




}


void SketchSessionTesting::clearSketch()
{
    /* felipe's code

	input_sketch_->clear();
	current_sketch_->clear();

	input_curve_.clear();
	over_sketch_.clear();

	update();

    */


}


bool SketchSessionTesting::initializationWithImage(const QPixmap& pixmap)
{
	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

	/// Creating and adding the boundary
    ///
    ///

    std::cout << "SketchSessionTesting::initializationWithImage" << std::endl;

	clear();

	qreal x = pixmap.rect().x();
	qreal y = pixmap.rect().y();
	qreal w = pixmap.rect().width();
	qreal h = pixmap.rect().height();

    this->initialization(x, y, w, h);

	QPixmap temp;

	QTransform myTransform;
	myTransform.scale(1, -1);
	temp = pixmap.transformed(myTransform);

	overlay_image_->setPixmap(temp);

	//	this->boundaryc_->setNewBoundary ( x , y , w , h );
	//	this->setSceneRect ( this->boundaryc_->boundingRect( ) );

	//	setUpBackground();

	return true;
}


bool SketchSessionTesting::initialization(qreal x, qreal y, qreal w, qreal h)
{

	// The bounding rectangle of the scene
	// The scene rectangle defines the extent of the scene.
	// It is primarily used by QGraphicsView
	// to determine the view's default scroll able area,
	// and by QGraphicsScene to manage item indexing.

	//	/// Creating and adding the boundary
	//	this->boundary_->setRect ( x , y , w - x , h - y );
	//	this->boundaryc->setNewBoundary ( x , y , w - x , h - y );

	// clean up the scene
	this->clear();



    setSceneRect( x, y, w , h );
//    boundary->setNewBoundary( x, y, w , h );


    std::cout << "SketchSessionTesting::initialization: coord" << std::endl;
//    std::cout << x << ", " << y << std::endl;
    std::cout << sceneRect().x()  << ", " << sceneRect().y() << ", " << sceneRect().width() << ", " << sceneRect().height() << std::endl;



    /* FELIPE'S CODE


	this->boundaryc_->setNewBoundary(x, y, w, h);
    this->setSceneRect(this->boundaryc_->boundingRect());

    */



	setUpBackground();

	return true;
}


void SketchSessionTesting::setBoundary(Real x, Real y, Real width, Real height)
{

    boundary->setNewBoundary( x, y, width, height );
    setSceneRect( boundary->boundingRect() );

    /* FELIPE'S CODE
	this->boundaryc_->setNewBoundary(x, y, width, height);
	this->setSceneRect(this->boundaryc_->boundingRect());
    */
}


void SketchSessionTesting::sketchNewBoundary()
{
    current_mode = InteractionMode::BOUNDARY;
//	this->boundary_sketching_ = true;
}


void SketchSessionTesting::insertCurve(unsigned int _id, QPolygonF _curve)
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


void SketchSessionTesting::setOverSketchingMode()
{
	clearRegionMode();
	mode_ = InteractionMode::OVERSKETCHING;
}


void SketchSessionTesting::setEditMode()
{
	clearRegionMode();
	mode_ = InteractionMode::EDITING;
}


void SketchSessionTesting::setSketchMode()
{
	clearRegionMode();
	mode_ = InteractionMode::SKETCHING;
}


void SketchSessionTesting::setRegionMode()
{
	clearRegionMode();
	mode_ = InteractionMode::REGION_POINT;
	this->region_pointers_->setVisible(true);
	this->region_pointers_->setVisible(true);
}


void SketchSessionTesting::clearRegionMode()
{


	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup
	if (this->region_pointers_ != nullptr)
	{
		QList<QGraphicsItem *> children = this->region_pointers_->childItems();
		if (children.size() > 0)
		{
			qDeleteAll(children);
		}
		this->update();
	}

	this->region_pointer_->setVisible(false);
	this->region_pointers_->setVisible(false);
	region_points_.clear();


}


SketchSessionTesting::InteractionMode SketchSessionTesting::mode() const
{
	return this->mode_;
}


void SketchSessionTesting::setUpBackground()
{
	/// @see http://www.qtcentre.org/threads/3322-Delete-all-members-in-a-QGraphicsItemGroup

	int step = 20;

	QRadialGradient gradient((qreal)width() / 2.0, (qreal)height() / 2.0, this->width());

	for (float radius = 0.0f; radius <= 1.0f; radius += 0.1f)
	{
		gradient.setColorAt(radius, QColor::fromRgb(154.28f + (radius*step), 176.20f + (radius*step), 199.16f + (radius*step)));
	}

	this->setBackgroundBrush(QBrush(gradient));
}


void SketchSessionTesting::updateSBIM(const std::map<unsigned int, std::pair<QColor, QPolygonF> >& _polycurves, const std::map<unsigned int, QPointF>& _vertices)
{


	/// clear the map of curves
	/// @todo clear all attributes
	for (auto& curve_iterator : this->view_curves_)
	{
		delete curve_iterator.second;
	}
	/// clear the map of curves
	/// @todo clear all attributes
	for (auto& vertex_iterator : this->view_vertices_)
	{
		delete vertex_iterator.second;
	}

	this->view_curves_.clear();
	this->view_vertices_.clear();

	for (auto& polycurve_iterator : _polycurves)
	{
		// todo if the curve exist, update it only it have changed
		if (this->view_curves_.count(polycurve_iterator.first))
		{
			//std::cout << "The curve exist " <<  polycurve_iterator.first << std::endl;

			view_curves_[polycurve_iterator.first]->setPen(QPen(polycurve_iterator.second.first));
			view_curves_[polycurve_iterator.first]->setSketch(polycurve_iterator.second.second);

		}
		// todo, create an appropriate QGraphicsItem from the new curve ( which geoObject which it represents)
		else
		{
			//std::cout << " It's a new curve " << polycurve_iterator.first << std::endl;

			StratigraphyItem * new_view_curve = new StratigraphyItem(polycurve_iterator.second.first);
			new_view_curve->setZValue(1);
			this->addItem(new_view_curve);

			new_view_curve->setSketch(polycurve_iterator.second.second);
			view_curves_[polycurve_iterator.first] = new_view_curve;
		}

		//std::cout << " Size " << polycurve_iterator.second.size() << std::endl;
	}


	//	for (auto& vertice_iterator : _vertices)
	//	{
	//
	//		// todo if the vertice exist, update it only it have changed
	//		if ( this->view_vertices_.count(vertice_iterator.first) )
	//		{
	//			view_vertices_[vertice_iterator.first]->setRect(vertice_iterator.second.x()-25,vertice_iterator.second.y()-25,50.0,50.0);
	//		}else
	//		{
	//			QGraphicsEllipseItem* node = new QGraphicsEllipseItem(vertice_iterator.second.x()-25,vertice_iterator.second.y()-25,50.0,50.0);
	//			view_vertices_[vertice_iterator.first] = node;
	//
	//			//QGraphicsTextItem* text = new QGraphicsTextItem(QString::number(vertice_iterator.first));
	////			text->setPos(vertice_iterator.second.x(),vertice_iterator.second.y());
	////			text->setRotation(180);
	////			this->addItem(text);
	//			this->addItem(node);
	//
	//		}
	//	}


	setUpBackground();
	update();



}
/// Model Related Function


SketchSessionTesting::Curve2D SketchSessionTesting::convert(QPolygonF _curve)
{
	Curve2D rrm_curve;

	for (auto p : _curve)
	{
		rrm_curve.push_back(Point2D(p.x(), p.y()));
	}

	return rrm_curve;
}


/// Model Related Function
QPolygonF SketchSessionTesting::convert(Curve2D _curve)
{
	QPolygonF qt_curve;

	for (std::size_t p_it = 0; p_it < _curve.size(); p_it++)
	{
		qt_curve.push_back(QPointF(_curve[p_it].x(), _curve[p_it].y()));
	}

	return qt_curve;
}


