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

#include "PolyQtUtils.hpp"

bool PolyQtUtils::containsPoint( const Curve2D &c, const Point2D &p,
                                Qt::FillRule fillRule )
{

    QPointF qp = PolyQtUtils::Point2DToQPointF( p ) ;
    return PolyQtUtils::curve2DToQPolyginF( c ).containsPoint( qp , fillRule ) ;
	
}

std::vector<bool> PolyQtUtils::containsPoints( const Curve2D & c,
                                          const std::vector< Point2D > & points,
                                          Qt::FillRule fr )
{
    QPolygonF pol = PolyQtUtils::curve2DToQPolyginF( c );
    
	std::vector<bool> isInside( points.size() ) ;
	
    for( size_t i = 0 ; i < points.size() ; ++i )
    {
        QPointF p = PolyQtUtils::Point2DToQPointF( points[ i ] );
        isInside[ i ] = ( pol.containsPoint( p ,fr ) );
    }
    
	return isInside ;
}

QPolygonF PolyQtUtils::curve2DToQPolyginF( const Curve2D &c )
{
    QPolygonF pol ;
    
    for( size_t i = 0 ; i < c.size() ; ++i)
        pol << PolyQtUtils::Point2DToQPointF( c[ i ] ) ;
    
	if( c.isClosed() && c.size() > 0 )
        pol << PolyQtUtils::Point2DToQPointF( c[ 0 ] ) ;
    
	return pol;
}

Curve2D PolyQtUtils::qPolyginFToCurve2D( const QPolygonF &pol )
{
    Curve2D c ;
	
    int start = ( pol.isClosed() ) ? 1 : 0 ;
    
    for( size_t i = start ; i < pol.size() ; ++i )
        c.add( QPointFToPoint2D( pol[ i ] ) );
    
	c.close( pol.isClosed() );
    
	return c ;
	
}

QPointF PolyQtUtils::Point2DToQPointF( const Point2D & p )
{
    return QPointF( p.x() , p.y() ) ;
}

Point2D PolyQtUtils::QPointFToPoint2D( const QPointF &p )
{
    return Point2D( p.x() , p.y() ) ;
}
