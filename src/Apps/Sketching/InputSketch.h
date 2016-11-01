#ifndef INPUTSKETCH_H
#define INPUTSKETCH_H

#include <QtCore/QVector>
#include <QPainter>
#include <QGraphicsPathItem>
#include <iostream>


#include "Sketching/PolyQtUtils.hpp"

#include "./libs/SketchLibrary/SketchLib.hpp"


using namespace std;


/**!
 * @brief The line defined by the user during the sketch processing.
 */

class InputSketch : public QGraphicsPathItem
{


    public:


        InputSketch( QColor color );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *w = 0 );
        QRectF boundingRect() const;

        void create( const QPointF& p );
        void add( const QPointF& p );
        void process( const QPointF& p );
        void clear();

        void setSketch( const QVector<QPointF> & _path );
        void setSketch( const QPolygonF & _path );
        QPolygonF getSketch( ) const;

        void setDone( bool option );

        bool isVisible() const;
        bool isInside() const;
        void isVisible( bool option );
        void isInside( bool option );

        QPainterPath shape() const;

        void setPen( const QPen& pen );
        void setColor( const QColor& _color);
        QColor getColor() const;

        inline bool isEmpty()
        {
            if( curve.toSubpathPolygons().empty() == true )
                return true;

            QList< QPolygonF > subcurves = curve.toSubpathPolygons();
            if ( subcurves[ 0 ].isEmpty() == true ) return true;
            if ( subcurves[ 0 ].size() < 5 ) return true;

            return false;
        }



    protected:

        QPen pen_color;
        QPainterPathStroker custom_stroker;

        QPainterPath curve;

        bool done;
        bool is_visible;
        bool is_inside;

        RRM::SketchLib sketchlib_;





};

#endif // INPUTSKETCH_H
