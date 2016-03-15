#ifndef HORIZONCONTROLLER_H
#define HORIZONCONTROLLER_H

#include <vector>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>


#include "Modeller/InputSketch.h"

/**!
 * @brief Future HorizoinController
 */

using namespace std;

class HorizonController: public QGraphicsPathItem
{

    public:


        enum RuleType{
            REMOVE_ABOVE,
            REMOVE_BELOW,
        };


        // this next enum should be in object geological: it will be implemented yet.

        enum ControllerType{
            HORIZON,
            BOUNDARY,
        };


        HorizonController( QColor color );
        ~HorizonController();

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );
        QRectF boundingRect() const;

        void setSketching( QPainterPath* curve );

        bool isSelected() const;
        void isSelected( bool option );
        bool showIntersection() const;
        void showIntersection( bool option );

        bool isValid( QGraphicsScene *scene );

        void setRule( HorizonController::RuleType rt );
        HorizonController::RuleType getRule() const;
        void applyRule();

        virtual int type() const;

        void setColor( int R, int G, int B );
        QColor getColor() const;

        void updateGeometry();

        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);


    signals:

        void sendColor( int R, int G, int B );


    protected:

        QPainterPath shape() const;



    protected:

        bool selected;
        bool show_intersection;


        QColor color_inside;
        QColor color_outside;
        Qt::PenStyle style_invisible;

        QPainterPath sketch;
        vector< QPainterPath > curves;
        vector< QPointF > intersections;
        vector< bool > are_inside;
        vector< bool > are_visible;

        RuleType rule_state;

};

#endif // HORIZONCONTROLLER_H
