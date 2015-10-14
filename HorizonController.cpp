#include "HorizonController.h"

HorizonController::HorizonController()
{
    selected = false;
    show_intersection = true;

    color_inside = QPen( QColor( 0, 0, 128 ) );
    color_inside.setWidth( 2 );

    color_outside = QPen( QColor( 187, 15, 32 ) );
    color_outside.setWidth( 2 );

    style_invisible = Qt::DotLine;

}


void HorizonController::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    painter->setRenderHint( QPainter::Antialiasing );
    painter->setBrush( Qt::NoBrush );

    if( are_visible.empty() == true || are_inside.empty() == true )
        return;


    // drawing all the curves contained in the horizon following its properties.
    int id = 0;
    for( auto crv : curves )
    {


        // set different line pattern if the curve is non-visible.

        color_inside.setStyle( Qt::SolidLine );

        if( show_intersection == true && are_visible.at( id ) == false )
            color_inside.setStyle( style_invisible );

        painter->setPen( color_inside );



        // set different line pattern if the curve is outside of boundary.

        if( show_intersection == true && are_inside.at( id ) == false )
            painter->setPen( color_outside );

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
    // I have to smooth the curve
    // I have to compute the intersections
    // I have to get the inside and outside curves
    // I have to get the deleted curves
    // I have to fill all the respectives vectors

    QPainterPath ACurve, BCurve;

    QPointF p0A = sketch.pointAtPercent( 0.0f );
    ACurve.moveTo( p0A );

    QPointF p0B = sketch.pointAtPercent( 0.5f );
    BCurve.moveTo( p0B );


    for( float delta = 0.05f; delta < 0.5f; delta += 0.05f )
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

    are_inside.push_back( false );
    are_inside.push_back( true );

    are_visible.push_back( true );
    are_visible.push_back( false );

    intersections.push_back( p0B );

    return true;

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


