#include "BoundaryItem2D.hpp"

#include <iostream>

BoundaryItem2D::BoundaryItem2D()
{

    prepareGeometryChange();

}

BoundaryItem2D::~BoundaryItem2D()
{
}



void BoundaryItem2D::clear()
{
    boundary.setCoords( 0, 0, 0, 0 );
    image_position = QPointF( 0.0f, 0.0f );
}



void BoundaryItem2D::edit( const qreal& x, const qreal& y, const qreal& width, const qreal& height )
{


    prepareGeometryChange();
    boundary.setRect( x, y, width, height );

}


void BoundaryItem2D::paint( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    painter->setRenderHint ( QPainter::Antialiasing );


    QPen pen_color ( QColor ( 0 , 200 , 0 ) );
    pen_color.setWidth ( 1 );

    QBrush brush;
    brush.setColor( QColor( 55, 100, 55, 75 ) );
    brush.setStyle( Qt::SolidPattern );


    painter->setBrush( brush );
    painter->setPen ( pen_color );
    painter->drawRect( boundary );


}


QRectF BoundaryItem2D::boundingRect() const
{
    return boundary;
}


void BoundaryItem2D::update( const Eigen::Affine3f& m )
{

    prepareGeometryChange();


    Eigen::Vector4f A( bd->getX(),                   bd->getY(),                   0.0f, 1.0f );
    Eigen::Vector4f B( bd->getX() + bd->getWidth(),  bd->getY() + bd->getHeight(), 0.0f, 1.0f );

    A = m*A;
    B = m*B;

    int width  = ( int )( B.x() - A.x() );
    int height = ( int )( B.y() - A.y() );

    boundary.setRect( A.x(), A.y(), width, height );


}

