/*
 * SeismicView.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: felipe
 */

#include "SeismicView.hpp"

namespace RRM
{
	// The parent destructor are responsible to delete all children
	// @see http://stackoverflow.com/questions/2491707/memory-management-in-qt

	SeismicView::SeismicView ( QWidget* parent ) : QWidget(parent)
	{
		Qt::WindowFlags flags = Qt::Window;

		this->setWindowFlags(flags);

		this->image_viewer = new QGraphicsView(this);

		// @see https://forum.qt.io/topic/48564/force-qgraphicsview-size-to-match-qgraphicsscene-size/4

		this->image_viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->image_viewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

		this->image_viewer->setRenderHint ( QPainter::Antialiasing , true );
		this->image_viewer->setOptimizationFlags ( QGraphicsView::DontSavePainterState );
		this->image_viewer->setViewportUpdateMode ( QGraphicsView::SmartViewportUpdate );
		this->image_viewer->setTransformationAnchor ( QGraphicsView::AnchorUnderMouse );
		this->image_viewer->setFocusPolicy ( Qt::StrongFocus );
		this->image_viewer->setInteractive ( true );
		this->image_viewer->setBackgroundRole ( QPalette::Base );
		this->image_viewer->setAutoFillBackground ( true );

		this->image_viewer->setViewportUpdateMode ( QGraphicsView::FullViewportUpdate );
		this->image_viewer->viewport ( )->grabGesture ( Qt::PinchGesture );
		this->image_viewer->viewport ( )->grabGesture ( Qt::SwipeGesture );

		this->ui = new Ui::SeismicViewerWidget();
		this->ui->setupUi ( this );

		this->scene = new QGraphicsScene(this->image_viewer);
		this->image_viewer->setScene(this->scene);

		QGraphicsRectItem* rect = new QGraphicsRectItem(0.0,0.0,640.0,480.0);
		QGraphicsLineItem* line = new QGraphicsLineItem(0.0,10.0,640.0,10.0);

		line->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

		rect->setPen(QPen(Qt::green));

		this->scene->addItem(rect);
		this->scene->addItem(line);

		this->image_viewer->setFixedSize(this->image_viewer->sceneRect().width(),this->image_viewer->sceneRect().height());

		ui->seismic_viewer_frame_->insertWidget(0,this->image_viewer);
	}

	SeismicView::~SeismicView ( )
	{
		delete ui;
	}

	void SeismicView::wheelEvent ( QWheelEvent* event )
	{

	}

	void SeismicView::keyPressEvent ( QKeyEvent* event )
	{

	}

	void SeismicView::fitInView( )
	{
		this->image_viewer->fitInView(this->image_viewer->sceneRect(), Qt::KeepAspectRatio);
	}


} /* namespace RRM */


