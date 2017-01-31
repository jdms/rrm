/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "InputSketch.h"

InputSketch::InputSketch ( QColor color ) :QGraphicsPathItem ( )
{
    prepareGeometryChange ( );


    is_visible = false;
    is_inside = false;


	pen_color = color;
    pen_color.setCapStyle( Qt::RoundCap );
    pen_color.setJoinStyle( Qt::RoundJoin );


    setFlag ( QGraphicsItem::ItemIsSelectable );
    setAcceptTouchEvents ( true );


}


void InputSketch::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    pen_color.setWidth ( 3 );

    painter->setRenderHint ( QPainter::Antialiasing );
    painter->setPen ( pen_color );
    painter->setBrush ( Qt::NoBrush );
    painter->drawPath( curve );



}


QRectF InputSketch::boundingRect ( ) const
{

    return curve.boundingRect();

}


void InputSketch::create ( const QPointF &p )
{
    prepareGeometryChange( );

    curve.moveTo( p );
}


void InputSketch::add ( const QPointF &p )
{

    prepareGeometryChange ( );
    curve.lineTo( p );
}





void InputSketch::clear ( )
{
    prepareGeometryChange ( );
    curve = QPainterPath();
}


void InputSketch::setDone ( bool option )
{
    done = option;
}


bool InputSketch::isVisible ( ) const
{
    return is_visible;
}


bool InputSketch::isInside ( ) const
{
    return is_inside;
}

void InputSketch::isVisible ( bool option )
{
    is_visible = option;
}


void InputSketch::isInside ( bool option )
{
    is_inside = option;
}


void InputSketch::setSketch ( const QVector<QPointF> & _path )
{
    prepareGeometryChange ( );

    curve.addPolygon( _path );

}


void InputSketch::setSketch ( const QPolygonF & _path )
{
    prepareGeometryChange ( );

    curve = QPainterPath();
    curve.addPolygon( _path );

}


QPolygonF InputSketch::getSketch ( ) const
{

////    std::cout << "Polygon is close? " << curve.toFillPolygon().isClosed() << "\n" << std::flush;
    if( curve.toSubpathPolygons().isEmpty() == true ) return QPolygonF();

    return curve.toSubpathPolygons()[ 0 ];//toFillPolygon();

//    return curve1;
}


void InputSketch::process( const QPointF& p )
{


    QList< QPolygonF > subpaths = curve.toSubpathPolygons();


    if( subpaths.size() < 2 ) return;

    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( subpaths[ 1 ] );
    Curve2D whole_curve = PolyQtUtils::qPolyginFToCurve2D( subpaths[ 0 ] );

    bool oversketch_ok = sketchlib_.overSketching( whole_curve, c );

    curve = QPainterPath();
    curve.addPolygon( PolyQtUtils::curve2DToQPolyginF( whole_curve ) );


}


QPainterPath InputSketch::shape ( ) const
{
    return curve;
}


void InputSketch::setPen ( const QPen& pen )
{
    pen_color = pen;
}


void InputSketch::setColor(const QColor& _color)
{
    pen_color.setColor(_color);
}


QColor InputSketch::getColor() const
{
    return pen_color.color();
}

