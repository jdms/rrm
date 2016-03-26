// Data created August 6, 2015

#ifndef _SKETCHSESSIONTESTING_HPP_
#define _SKETCHSESSIONTESTING_HPP_

#include <iostream>

#include <QtCore/QDebug>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsItem>
#include <QtSvg/QGraphicsSvgItem>
#include <QtSvg/QSvgRenderer>

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>

#include <QtCore/QVector>

#include <Modeller/BoundaryController.h>
#include <Modeller/InputSketch.h>


/**
 * @author Felipe Moura de Carvalho
 * @author Clarissa Marques
 * @date Sep, 2015
 * @brief Scene graph. Here, the user can input and visualize the stratigraphy
 */

// TODO temporary name
class SketchSessionTesting : public QGraphicsScene
{
	Q_OBJECT
public:

	SketchSessionTesting( QObject *parent = 0 );
	virtual ~SketchSessionTesting();

	void mousePressEvent	(QGraphicsSceneMouseEvent* event);
	void mouseMoveEvent 	(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent	(QGraphicsSceneMouseEvent* event);

	void dragEnterEvent ( QGraphicsSceneDragDropEvent *event );
	void dragMoveEvent 	( QGraphicsSceneDragDropEvent *event );
	void dragLeaveEvent ( QGraphicsSceneDragDropEvent *event );
	void dropEvent 		( QGraphicsSceneDragDropEvent *event );

	void removeInputSketch();

	void drawPolyLine( std::vector<QPointF> points);

	QGraphicsPixmapItem * image;

public slots:

		// Ask Controller to smooth the raw sketch
		void curveSmoothed (  QPolygonF curve_smoohted_);
		bool initialization( qreal x , qreal y, qreal width, qreal height );
		bool initializationWithImage( const QPixmap& pixmap  );
		// Notify the sketch module to draw a new rectangle boundary
		void sketchNewBoundary();
	signals:

		// Notify the model, to reset the current arrangement of curves and set a new boundary, given an image
		void newSessionSignal   (QPixmap pixmap);
		// Notify the model, to reset the current arrangement of curves and set a new boundary, given sketch rectangle
		void newSessionSignal   (qreal x, qreal y, qreal width, qreal height);
		// Notify the model and provides a new sketch line
		void newSketchLine      (QPolygonF polygon );
		void smoothCurve 	(QPolygonF raw_skecth_ );

private:

	/// Rectangular Boundary
	QGraphicsRectItem*	boundary_;
		// The point where the we start draw the rectangle boundary_
		QPointF boundary_anchor_point_;

		bool boundary_sketching_;

		QPointF last_point_;

	QPen 	sketch_pen;
	QBrush 	sketch_brush;

	// Clarissa Interface
	BoundaryController* boundaryc;
	InputSketch*        input_sketch_;

};

#endif /* _SKETCHSESSION_HPP_ */
