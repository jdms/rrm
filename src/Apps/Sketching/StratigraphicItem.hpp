#ifndef STRATIGRAPHICITEM_HPP
#define STRATIGRAPHICITEM_HPP

#include <QGraphicsPathItem>
#include <QPolygonF>

#include "model_new/StratigraphyRenderable.hpp"
#include "Sketching/InputSketch.h"

#include "Sketching/PolyQtUtils.hpp"

//#include "Core/Geometry/PolygonalCurve/CurveN.hpp"



class StratigraphicItem: public QGraphicsPathItem, public StratigraphyRenderable
{

	public:

        typedef qreal			Real;
        typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Curve2D;
        typedef PointN<Real, 2> 					    Point2D;

	
		StratigraphicItem();
		
		
		void create( const QPointF& p );
		void add( const QPointF& p );
		
		void addSegment( const InputSketch& segment );	
		void copySegment( const QPolygonF& s );
		
		void clear();
		
		QRectF boundingRect() const;
        QPainterPath shape() const;

		
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

		
		inline QPolygonF getCurve(){ return points; }
		
		inline void setColor( const QColor& c ){ current_color = c; }
		inline QColor setColor(){ return current_color; }
		

	
	protected:
		

		Stratigraphy* strat;

		
        QPolygonF points;
		QPainterPath curve;

		QColor current_color;
};


#endif
