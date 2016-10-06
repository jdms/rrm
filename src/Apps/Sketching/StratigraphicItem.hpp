#ifndef STRATIGRAPHICITEM_HPP
#define STRATIGRAPHICITEM_HPP

#include <QGraphicsPathItem>
#include <QPolygonF>

#include "Model/StratigraphyRenderable.hpp"
#include "Sketching/InputSketch.h"

#include "Sketching/PolyQtUtils.hpp"

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"


class StratigraphicItem: public QGraphicsPathItem, public StratigraphyRenderable
{

	public:


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
		
        void update( const Eigen::Affine3f &m, const float& d );

        inline void getId() const { strat->getId(); }

	
	protected:

		
        QPolygonF points;
		QPainterPath curve;

		QColor current_color;
};


#endif
