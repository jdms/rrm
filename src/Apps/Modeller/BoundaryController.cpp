#include "Modeller/BoundaryController.h"

BoundaryController::BoundaryController( int width, int height ):QGraphicsPathItem()
{

    QPainterPath path0;
    path0.moveTo( -width*0.5, height*0.5 );
    path0.lineTo( width*0.5, height*0.5 );
    path0.lineTo( width*0.5, height*0.5 );
    path0.lineTo( width*0.5, -height*0.5 );

    QPainterPath path2;
    path2.moveTo( width*0.5, -height*0.5 );
    path2.lineTo( -width*0.5, -height*0.5 );
    path2.lineTo( -width*0.5, -height*0.5 );
    path2.lineTo( -width*0.5, height*0.5 );


    boundary.push_back( path0);
    boundary.push_back( path2);

}

BoundaryController::~BoundaryController()
{
   boundary.clear();
}


void BoundaryController::setNewBoundary (qreal x, qreal y,qreal width, qreal height)
{

	    boundary.clear();

	    QPainterPath path0;

	    path0.addRect(x,y,width,height);

	    boundary.push_back( path0);

}

void BoundaryController::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{

    painter->setRenderHint( QPainter::Antialiasing );

    QPen pen_color( QColor( 0, 100, 0 ) );
    pen_color.setWidth( 1 );
    QBrush brush;
    brush.setColor( QColor( 180 , 255 , 180 , 180 ) );
    brush.setStyle( Qt::SolidPattern );



    painter->setPen( pen_color );
    painter->setBrush( brush );


    for( auto crv : boundary )
        painter->drawPath( crv );

}


QRectF BoundaryController::boundingRect() const
{
    QPainterPath path;

    int ncurves = (int) boundary.size();
    for( int i = 0; i < ncurves; ++i )
    {
        QPainterPath curve = boundary.at( i );
        path.addPath( curve );
    }

    return path.boundingRect();
}
