// Data created August 6, 2015

#ifndef _SKETCHSESSION_HPP_
#define _SKETCHSESSION_HPP_

#include <iostream>

#include <QtCore/QDebug>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsItem>

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>

#include<QtCore/QVector>

#include <Modeller/InputSketch.h>
#include "Modeller/BoundaryItem.h"
/**
 * @author Felipe Moura de Carvalho
 * @date Sep, 2015
 * @brief Scene graph. Here, the user can input and visualize the stratigraphy
 */

class SketchSession : public QGraphicsScene
{
	Q_OBJECT
public:

	SketchSession( QObject *parent = 0 );
	virtual ~SketchSession();

    //void timerEvent(QTimerEvent*);
	void mousePressEvent	(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent 	(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent	(QGraphicsSceneMouseEvent* event);

	void dragEnterEvent ( QGraphicsSceneDragDropEvent *event );
	void dragMoveEvent 	( QGraphicsSceneDragDropEvent *event );
	void dragLeaveEvent ( QGraphicsSceneDragDropEvent *event );
	void dropEvent 		( QGraphicsSceneDragDropEvent *event );

	/// FIXME Helper object, used to remove the RAW SKETCH LINE
	QGraphicsItemGroup *input_line_group;

	void removeInputSketch();

	void drawPolyLine( std::vector<QPointF> points);

	QGraphicsItemGroup *halfedges_;
	QGraphicsPixmapItem * image;

public slots:

		void smoothCurveSlot (  QPolygonF smooth_skecth);
		bool initialization( qreal x , qreal y, qreal width, qreal height );
		bool initialization_with_image( const QPixmap& pixmap  );
		void updatePaths(std::vector<QPainterPath>& paths);
		void sketchNewBoundary();
	signals:

		void newSessionSignal   (QPixmap pixmap);
		void newSessionSignal   (qreal x, qreal y, qreal width, qreal height);
		void curve2DSignal      (QPolygonF polygon );
		void smoothSketchSignal (QPolygonF raw_sketch);


private:

	bool is_ready;

	QVector<QPointF> 	input_line_;

	QPointF				last_point_;

	QGraphicsRectItem*	boundary_;
		// The point where the we start draw the rectangle boundary_
		QPointF boundary_anchor_point_;
		// The size of the boundary_. Basically the mouse release event.
		QPointF boundary_size_;

		bool boundary_sketching_;

		QGraphicsLineItem* boundary_diagonal_;

	QVector<QPainterPath> paths_;
	QPainterPath sketch_path;
	QGraphicsPathItem* sketch_item;
	QVector<QPainterPath> target_paths_;
	///	Temporary container of point, know as Raw Sketching
	/// std::vector<QPointF> input_sketching_;
	QPen 	sketch_pen;
	QBrush 	sketch_brush;

	// Clarissa Interface
	BoundaryItem *boundaryc;
	InputSketch*        sketch;

};

#endif /* _SKETCHSESSION_HPP_ */