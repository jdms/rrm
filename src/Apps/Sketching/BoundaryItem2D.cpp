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
    origin_x = 0;
    origin_y = 0;
    width = 0;
    height = 0;

    boundary.setCoords( 0, 0, 0, 0 );
    image_file.clear();
    background_image = QImage();
    image_position = QPointF( 0.0f, 0.0f );
}


void BoundaryItem2D::load()
{

    // retirar

    prepareGeometryChange();

    boundary.setRect( origin_x, origin_y, width, height );
    image_position = QPointF( 0, 0 );

}


void BoundaryItem2D::edit( const qreal& x, const qreal& y, const qreal& width, const qreal& height )
{


    prepareGeometryChange();
    boundary.setRect( x, y, width, height );

    image_position = QPointF( boundary.bottomLeft().x(), boundary.bottomLeft().y() );
    loadBackGroud();

}


void BoundaryItem2D::paint( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    painter->setRenderHint ( QPainter::Antialiasing );


    QPen pen_color ( QColor ( 0 , 200 , 0 ) );
    pen_color.setWidth ( 1 );

    QBrush brush;
    brush.setColor( QColor( 55, 100, 55, 75 ) );
    brush.setStyle( Qt::SolidPattern );


    if( background_image.isNull() == false )
    {
        brush.setTextureImage( background_image  );
        painter->setBrushOrigin( image_position.x(), image_position.y() );
    }

    painter->setBrush( brush );
    painter->setPen ( pen_color );
    painter->drawRect( boundary );


}


void BoundaryItem2D::setBackGroundImage( const QString& url )
{
    image_file = url;
    loadBackGroud();
}


void BoundaryItem2D::loadBackGroud()
{

    if( image_file.isEmpty() || image_file.isNull() ) return;

    background_image.load( image_file );
    background_image = background_image.scaled( abs( int ( boundary.width() ) ), abs( int ( boundary.height() ) ) );
    background_image = background_image.mirrored( false, true );
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
    image_position = QPointF( 0, 0 );


}

