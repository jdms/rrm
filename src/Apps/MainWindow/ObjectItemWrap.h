#ifndef OBJECTITEMWRAP_H
#define OBJECTITEMWRAP_H

#include <set>

#include <QPainter>
#include <QGraphicsPathItem>
#include <QPen>
#include <QPolygonF>
#include <QPointF>

#include "Object.h"
#include "./Sketching/PolyQtUtils.hpp"


class ObjectItemWrap: public QGraphicsPathItem
{
    public:

        enum class State: int { NONE = 0, ALLOWED = 1, SELECTED = 2 };

        ObjectItemWrap();

        QRectF boundingRect() const;
        void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w );

        void clear();
        void setObjectRaw( Object* obj_, const double depth_ );
        inline std::size_t getId() const { return object->getId(); }


        void updateCrossSection( double depth_ );


        void setColor( const QColor& c_ ){ current_color = c_; setupPens(); }

        inline void setState( ObjectItemWrap::State st_ )
        {
            switch ( st_ ) {
            case State::NONE:
                temp_color = current_color;
                break;

            case State::ALLOWED:
                temp_color = QColor( 0, 0, 255 );
                setFlag( QGraphicsItem::ItemIsSelectable, true );
                break;

            case State::SELECTED:
                temp_color = QColor( 255, 0, 0 );
                setFlag( QGraphicsItem::ItemIsSelectable, false );
                break;

            default:
                break;
            }

            setupPens();
        }

    private:

        void setupPens();

    protected:


        Object* object;

        QPainterPath curve;
        std::set<QPointF> intersection_points;
        std::vector<bool> hidden_subpaths;

        QColor current_color;
        QColor temp_color;

        QPen hidden_subpaths_pen;
        QPen visible_subpaths_pen;


};

#endif // OBJECTITEMWRAP_H
