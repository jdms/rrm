#include "BoundaryItem2D.hpp"

#include <iostream>

BoundaryItem2D::BoundaryItem2D ()
{

	prepareGeometryChange ( );

//    boundary.setRect( 0, 0, width, height );
//    image_position = QPointF( 0, 0 );

}

BoundaryItem2D::~BoundaryItem2D ( )
{
}


void BoundaryItem2D::load()
{

    prepareGeometryChange ( );

    boundary.setRect( 0, 0, bd->getWidth(), bd->getHeight() );
    image_position = QPointF( 0, 0 );

}


void BoundaryItem2D::edit ( qreal x , qreal y , qreal width , qreal height )
{


    prepareGeometryChange();
    boundary.setRect( x, y, width, height );

    image_position = QPointF(boundary.bottomLeft().x(), boundary.bottomLeft().y() );
    loadBackGroud();

}


void BoundaryItem2D::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    painter->setRenderHint ( QPainter::Antialiasing );


    QPen pen_color ( QColor ( 0 , 200 , 0 ) );
    pen_color.setWidth ( 1 );

    QBrush brush;
    brush.setColor ( QColor( 55, 100, 55, 75 ) );
    brush.setStyle ( Qt::SolidPattern );


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


QRectF BoundaryItem2D::boundingRect ( ) const
{
    return boundary;
}


void BoundaryItem2D::update()
{
    boundary.setRect( bd->getX(), bd->getY(), bd->getWidth(), bd->getHeight() );
}

