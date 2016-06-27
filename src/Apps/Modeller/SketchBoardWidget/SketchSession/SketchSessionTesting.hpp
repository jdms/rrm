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
#include <QtWidgets/QLabel>

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtCore/QUrl>

#include <QtCore/QVector>

#include "Modeller/InputSketch.h"
#include "Modeller/StratigraphyItem.hpp"
#include "Modeller/BoundaryItem.h"

#include "SBIM/SeismicVolume.hpp"
#include "Core/Geometry/PolygonalCurve/CurveN.hpp"

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

		typedef qreal			Real;
		typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
		typedef PointN<Real, 2> 					    Point2D;

		enum class InteractionMode
		{
			EDITING, SKETCHING, OVERSKETCHING, REGION_POINT
		};

		SketchSessionTesting ( QObject *parent = 0 );
		virtual ~SketchSessionTesting ( );

		void removeInputSketch ( );

		void setUpBackground();

		QLabel * coordinates_;

		InteractionMode mode() const;


	protected:
		void mousePressEvent ( QGraphicsSceneMouseEvent* event );
		void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
		void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );

		void dragEnterEvent ( QGraphicsSceneDragDropEvent *event );
		void dragMoveEvent ( QGraphicsSceneDragDropEvent *event );
		void dragLeaveEvent ( QGraphicsSceneDragDropEvent *event );
		void dropEvent ( QGraphicsSceneDragDropEvent *event );

	public slots:

		// Ask Controller to smooth the raw sketch
		void curveSmoothed ( QPolygonF curve_smoohted_ );
		void insertCurve ( unsigned int _id , QPolygonF curve_ );
		bool initialization ( qreal x , qreal y , qreal width , qreal height );
		bool initializationWithImage ( const QPixmap& pixmap );
		// Notify the sketch module to draw a new rectangle boundary
		void sketchNewBoundary ( );

		void setColor(const QColor& _color);

		void newSktech();

		void setEditMode();
		void setSketchMode();
		void setRegionMode();
		void setOverSketchingMode();
		void clear();
		void reset();
		// Update the view with the new Cross Section configuration
		void updateSBIM(const std::map<unsigned int, std::pair<unsigned int,QPolygonF> >& _polycurves, const std::map<unsigned int, QPointF>& _vertices);

		void setBoundary ( Real x , Real y , Real width , Real height );

	signals:
		// Notify the controller, to reset the current arrangement of curves and set a new boundary, with the given an image
		void newSessionSignal ( const QPixmap& pixmap );
		// Notify the controller, to reset the current arrangement of curves and set a new boundary, with sketched rectangle
		void newSessionSignal ( Real x , Real y , Real width , Real height );
		// Notify the model and provides a new sketch line
		void newSketchCurve ( QPolygonF _sketch );
		// Request the controller to Smooth a curve Segment
		void smoothCurve ( QPolygonF raw_skecth_ );

		void newBoundary ( Real x , Real y , Real width , Real height );

	public:
		QGraphicsPixmapItem * overlay_image_;
	private:
		// Map between the sketch curves with the curve into the arrangement
		std::map<unsigned int, StratigraphyItem*> view_curves_;

		// Map between the sketch curves with the curve into the arrangement
		std::map<unsigned int, QGraphicsEllipseItem*> view_vertices_;

		QPointF last_point_;

		QColor current_color_;

		/// Rectangular Boundary
			// The point where the we start draw the rectangle boundary_
			QPointF boundary_anchor_point_;
			// If is an new Boundary is requested
			bool boundary_sketching_;

		QPen sketch_pen;
		QBrush sketch_brush;

		// Interface
		BoundaryItem* boundaryc_;
		InputSketch* input_sketch_;
		InputSketch* current_sketch_;

		Curve2D input_curve_;
		Curve2D over_sketch_;
		Curve2D rest_;

		Curve2D 	convert(QPolygonF _curve );
		QPolygonF 	convert(Curve2D	  _curve );

		std::vector<QColor> colors;

		InteractionMode mode_;
		
		/// FIXME Region Point
		void clearRegionMode();
		QGraphicsPixmapItem * region_pointer_;
		std::vector<QPointF>  region_points_;
		QGraphicsItemGroup * region_pointers_;
//
//		int next = 0;
//
//		std::vector<QImage>  images;
//		std::vector< std::vector<unsigned char> >  rrm_images_;
//
//		/// Testing
//		RRM::SeismicVolume seismic_data_;


};

#endif /* _SKETCHSESSION_HPP_ */


