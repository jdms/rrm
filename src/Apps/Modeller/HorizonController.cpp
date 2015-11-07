#include "Apps/Modeller/HorizonController.h"

#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>



HorizonController::HorizonController( QColor color ):QGraphicsPathItem()
{
    selected = false;
    show_intersection = true;

    color_inside = QColor( color.red(), color.green(), color.blue() );//0, 0, 128 );
    color_outside = QColor( 187, 15, 32 );
    style_invisible = Qt::DotLine;

    rule_state = RuleType::REMOVE_ABOVE;

    setAcceptHoverEvents(true);
    setFlags( QGraphicsItem::ItemIsSelectable );

}

HorizonController::~HorizonController()
{
   curves.clear();
   intersections.clear();
   are_inside.clear();
   are_visible.clear();
}


QPainterPath HorizonController::shape() const
{
    QPainterPath path;

    int ncurves = (int) curves.size();
    for( int i = 0; i < ncurves; ++i )
    {
        QPainterPath curve = curves.at( i );
        path.addPath( curve );
    }
    return path;
}


void HorizonController::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setBrush( Qt::NoBrush );

    if( are_visible.empty() == true || are_inside.empty() == true )
        return;


    QColor cl_inside = color_inside;
    QColor cl_outside = color_outside;


    if( option->state & QStyle::State_Selected )
    {
        cl_inside = color_inside.light( 200 );
        cl_outside = color_outside.light( 150 );
    }


    QPen pen;

    // drawing all the curves contained in the horizon following its properties.
    int id = 0;
    for( auto crv : curves )
    {

        pen.setColor( cl_inside );
        pen.setWidth( 2 );
        pen.setStyle( Qt::SolidLine );


        // set different line pattern if the curve is non-visible.

        if( show_intersection == true && are_visible.at( id ) == false )
            pen.setStyle( style_invisible );

        // set different line pattern if the curve is outside of boundary.

        if( show_intersection == true && are_inside.at( id ) == false )
            pen.setColor( cl_outside );



        painter->setPen( pen );
        painter->drawPath( crv );
        id++;


    }



    // drawing all the intersections related to this horizon.
    if( show_intersection == true )
    {

        painter->setPen( QColor( 255 , 0 , 255 ) );
        for( int i = 0; i < intersections.size(); ++i  )
        {
            QPointF its = intersections.at( i );
            painter->drawEllipse( its , 5. , 5. );

        }


    }



}


QRectF HorizonController::boundingRect() const
{

    return QRectF( -sketch.boundingRect().width()/ 2, -sketch.boundingRect().height()/ 2, sketch.boundingRect().width(), sketch.boundingRect().height());

}


void HorizonController::setSketching( QPainterPath* curve )
{
    sketch = *curve;
}


bool HorizonController::isValid( QGraphicsScene *scene )
{

    // temporary

    // I have to verify if it is valid, if it is not return immediately


    return true;

}


void HorizonController::applyRule()
{
    // temporary

    // I have to verify if it is valid, if it is not return immediately
    // I have to smooth the curve
    // I have to compute the intersections using the state_rule
    // I have to get the inside and outside curves
    // I have to get the deleted curves
    // I have to fill all the respectives vectors
    // Change QPainterPath to InputSketch

    QPainterPath ACurve, BCurve;

    QPointF p0A = sketch.pointAtPercent( 0.0f );
    ACurve.moveTo( p0A );

    QPointF p0B = sketch.pointAtPercent( 0.5f );
    BCurve.moveTo( p0B );


    for( float delta = 0.01f; delta < 0.5f; delta += 0.01f )
    {
        QPointF pA = sketch.pointAtPercent( delta );
        ACurve.lineTo( pA );

        float deltaB = delta + 0.5f ;
        QPointF pB = sketch.pointAtPercent( deltaB );
        BCurve.lineTo( pB );

    }

    QPointF p1A = sketch.pointAtPercent( 0.5f );
    ACurve.lineTo( p1A );

    QPointF p1B = sketch.pointAtPercent( 1.0f );
    BCurve.lineTo( p1B );


    curves.push_back( ACurve );
    curves.push_back( BCurve );

    are_inside.push_back( true );
    are_inside.push_back( true );

    are_visible.push_back( true );
    are_visible.push_back( false );

    intersections.push_back( p0B );
}


bool HorizonController::isSelected() const
{
    return selected;
}


void HorizonController::isSelected( bool option )
{
    selected = option;
}


bool HorizonController::showIntersection() const
{
    return show_intersection;
}


void HorizonController::showIntersection( bool option )
{
    show_intersection = option;
}


void HorizonController::setRule( HorizonController::RuleType rt )
{
    rule_state = rt;
}


HorizonController::RuleType HorizonController::getRule() const
{
    return rule_state;
}


void HorizonController::setColor( int R, int G, int B )
{
    color_inside = QColor( R, G, B );
}


int HorizonController::type() const
{
    return HorizonController::ControllerType::HORIZON;
}

QColor HorizonController::getColor() const
{
    return color_inside;
}


void HorizonController::updateGeometry()
{
//    color_inside = QColor( 255, 255, 0 );
}

void HorizonController::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    this->scene()->clearSelection();
    setSelected(true);
//    myContextMenu->exec(event->screenPos());

}

