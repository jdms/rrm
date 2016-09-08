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
	
    for( int i = 0 ; i < points.size() ; ++i )
    {
        QPointF p = PolyQtUtils::Point2DToQPointF( points[ i ] );
        isInside[ i ] = ( pol.containsPoint( p ,fr) );
    }
    
	return isInside ;
}

QPolygonF PolyQtUtils::curve2DToQPolyginF( const Curve2D &c )
{
    QPolygonF pol ;
    
    for( int i = 0 ; i < c.size() ; ++i)
        pol << PolyQtUtils::Point2DToQPointF( c[ i ] ) ;
    
	if( c.isClosed() && c.size() > 0 )
        pol << PolyQtUtils::Point2DToQPointF( c[ 0 ] ) ;
    
	return pol;
}

PolyQtUtils::Curve2D PolyQtUtils::qPolyginFToCurve2D( const QPolygonF &pol )
{
    Curve2D c ;
	
    int start = ( pol.isClosed() ) ? 1 : 0 ;
    
	for( int i = start ; i < pol.size() ; ++i )
        c.add( QPointFToPoint2D( pol[ i ] ) );
    
	c.close( pol.isClosed() );
    
	return c ;
	
}

QPointF PolyQtUtils::Point2DToQPointF( const Point2D & p )
{
    return QPointF( p.x() , p.y() ) ;
}

PolyQtUtils::Point2D PolyQtUtils::QPointFToPoint2D( const QPointF &p )
{
    return Point2D( p.x() , p.y() ) ;
}
