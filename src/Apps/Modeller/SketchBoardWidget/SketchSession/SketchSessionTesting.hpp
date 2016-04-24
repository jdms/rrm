// Data created August 6, 2015

#ifndef _SKETCHSESSIONTESTING_HPP_
#define _SKETCHSESSIONTESTING_HPP_


// Standard C++11
#include <iostream>
#include <map>

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

#include "Modeller/InputSketch.h"
#include "Modeller/StratigraphyItem.hpp"
#include "Modeller/BoundaryItem.h"

/**
 * @author Felipe Moura de Carvalho
 * @author Clarissa Marques
 * @date   Sep, 2015
 * @brief  Scene graph. The user can input and visualise stratigraphy curves
 */

// TODO temporary name
class SketchSessionTesting: public QGraphicsScene
{
	Q_OBJECT
	public:

		enum class InteractionMode
		{
			EDITING, SKETCHING,
		};

		SketchSessionTesting ( QObject *parent = 0 );
		virtual ~SketchSessionTesting ( );

		void mousePressEvent ( QGraphicsSceneMouseEvent* event );
		void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
		void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );

		void dragEnterEvent ( QGraphicsSceneDragDropEvent *event );
		void dragMoveEvent ( QGraphicsSceneDragDropEvent *event );
		void dragLeaveEvent ( QGraphicsSceneDragDropEvent *event );
		void dropEvent ( QGraphicsSceneDragDropEvent *event );

		void removeInputSketch ( );

		QGraphicsPixmapItem * image;

	public slots:

		// Ask Controller to smooth the raw sketch
		void curveSmoothed ( QPolygonF curve_smoohted_ );
		void insertCurve ( unsigned int _id , QPolygonF curve_ );
		bool initialization ( qreal x , qreal y , qreal width , qreal height );
		bool initializationWithImage ( const QPixmap& pixmap );
		// Notify the sketch module to draw a new rectangle boundary
		void sketchNewBoundary ( );

		void setEditMode();
		void setSketchMode();
		void clear();
		// Update the view with the new Cross Section configuration
		void updateSBIM(const std::map<unsigned int, QPolygonF>& _polycurves, const std::map<unsigned int, QPointF>& _vertices);

	signals:
		// Notify the controller, to reset the current arrangement of curves and set a new boundary, with the given an image
		void newSessionSignal ( QPixmap pixmap );
		// Notify the controller, to reset the current arrangement of curves and set a new boundary, with sketched rectangle
		void newSessionSignal ( qreal x , qreal y , qreal width , qreal height );
		// Notify the model and provides a new sketch line
		void newSketchCurve ( QPolygonF _sketch );
		// Request the controller to Smooth a curve Segment
		void smoothCurve ( QPolygonF raw_skecth_ );

	private:
		// Map between the sketch curves with the curve into the arrangement
		std::map<unsigned int, StratigraphyItem*> view_curves_;

		// Map between the sketch curves with the curve into the arrangement
		std::map<unsigned int, QGraphicsEllipseItem*> view_vertices_;

		QPointF last_point_;

		/// Rectangular Boundary
			QGraphicsRectItem* boundary_;
			// The point where the we start draw the rectangle boundary_
			QPointF boundary_anchor_point_;
			// If is an new Boundary is requested
			bool boundary_sketching_;

		QPen sketch_pen;
		QBrush sketch_brush;

		// Interface
		BoundaryItem* boundaryc;
		InputSketch* input_sketch_;

		InteractionMode mode_;

};

#endif /* _SKETCHSESSION_HPP_ */
