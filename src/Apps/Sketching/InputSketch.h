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


        inline bool isValid(){ if( points_list.size() < 5 ) return false; return true; }


        bool isVisible() const;
        bool isInside() const;
        void isVisible( bool option );
        void isInside( bool option );

        QPainterPath shape() const;

        void setPen( const QPen& pen );
        void setColor( const QColor& _color);
        QColor getColor() const;



    protected:

        QPen pen_color;

        QPolygonF points_list;
        QPainterPath curve;


        bool done;
        bool is_visible;
        bool is_inside;

        RRM::SketchLib sketchlib_;




};

#endif // INPUTSKETCH_H
