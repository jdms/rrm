#ifndef STRATIGRAPHICITEM_HPP
#define STRATIGRAPHICITEM_HPP

#include <QGraphicsPathItem>
#include <QPainterPathStroker>
#include <QPolygonF>

#include "./src/Core/Geology/Models/StratigraphyRenderable.hpp"
#include "Sketching/InputSketch.h"

#include "Sketching/PolyQtUtils.hpp"

#include "Eigen/Dense"
#include "Tucano/Trackball.hpp"
#include "Tucano/Shader.hpp"
#include <QTransform>


class StratigraphicItem: public QGraphicsPathItem, public StratigraphyRenderable
{

	public:


		StratigraphicItem();
		

        void initSetup();
		
		void create( const QPointF& p );
		void add( const QPointF& p );
		
		void copySegment( const QPolygonF& s );

        void clear();
        void update(const QTransform &m, const float& d );


		QRectF boundingRect() const;
        QPainterPath shape() const;

		
		void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

		
		inline void setColor( const QColor& c ){ current_color = c; }
		inline QColor setColor(){ return current_color; }
		

        inline size_t getId() const { return strat->getId(); }
        inline QPolygonF getCurve(){ return points; }

        inline QPainterPath getPath() const { return curve; }


        inline void setSelection( bool option ){ is_selected = option;  }
        inline bool getSelection(){ return is_selected; }
        inline void setAllowed( bool option ){ setFlag( QGraphicsItem::ItemIsSelectable, option ); is_allowed = option; }
        inline void setUnderOperation( bool option ){ under_operation = option; if( option == false ){ is_allowed = false; is_selected = false; } }


        QList< QPolygonF > getSubCurves();
        std::vector< Curve2D > getSubCurves2D();
	
	protected:

		
        QPolygonF points;
		QPainterPath curve;
        QPainterPathStroker custom_stroker;

		QColor current_color;
        QPen pen_allowed;
        QPen pen_selected;
        QPen pen_normal;

        bool under_operation;
        bool is_allowed;
        bool is_selected;

};


#endif
