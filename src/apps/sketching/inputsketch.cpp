#include <iostream>

#include <QPainterPathStroker>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QGraphicsSceneMouseEvent>

#include "inputsketch.h"


InputSketch::InputSketch( QGraphicsItem * parent ): QGraphicsPathItem( parent ),
                                                    current_color( Qt::red ), current_width( 3 )
{
    setPath( curve );
}




void InputSketch::create( const QPointF& p_ )
{
    prepareGeometryChange();
    curve.moveTo( p_);

    update();
}


void InputSketch::add( const QPointF& p_ )
{
    prepareGeometryChange();
    curve.lineTo( p_ );
    update();
}


void InputSketch::process()
{

    if( curve.isEmpty() == true ) return;


    prepareGeometryChange();

    QPolygonF path0_ = curve.toSubpathPolygons()[ 0 ], path1_;
    Curve2D sketch_;

    bool status_ = getSubpaths( path0_, path1_ );
    if( status_ == true )
        sketch_ = overSketch( path0_, path1_ );
    else
        sketch_ = fromQtToCurve2D( path0_ );

    sketch_ = SketchLibrary::xmonotonicSketch( sketch_ );
    sketch_ = SketchLibrary::smooth( sketch_ );

    setSketch( fromCurve2DToQt( sketch_ ) );

}


PolyCurve InputSketch::done()
{
    if( curve.isEmpty() == true ) return PolyCurve();

    std::vector< double > vertices_;
    std::vector< std::size_t > edges_;
    fromPainterPathToVector( curve, vertices_, edges_ );

    clear();

    return PolyCurve( vertices_, edges_ );
}


void InputSketch::clear()
{
    prepareGeometryChange();
    curve = QPainterPath();
    setPath( curve );
    update();
}


bool InputSketch::isEmpty() const
{
    return curve.toSubpathPolygons().empty();
}




void InputSketch::setCurrentColor( const QColor& color_ )
{
    current_color = color_;
    update();
}


QColor InputSketch::getCurrentColor() const
{
    return current_color;
}



void InputSketch::setCurrentWidth( int width_ )
{
    current_width = width_;
    update();
}


int InputSketch::getCurrentWidth() const
{
    return current_width;
}



void InputSketch::setSketch( const QPolygonF& sketch_ )
{
    clear();

    prepareGeometryChange();
    curve.addPolygon( sketch_ );
    setPath( curve );

    update();
}


QPolygonF InputSketch::getSketch() const
{
    return QPolygonF();
}


bool InputSketch::getSubpaths( QPolygonF& path0_, QPolygonF& path1_ ) const
{
    QList< QPolygonF > subpaths = curve.toSubpathPolygons();
    if( subpaths.size() < MIN_SUBPATHS_OVERSKETCH ) return false;


    if( subpaths[ 0 ].size() < SketchLibrary::SKETCH_MIN_LENGHT ) return false;


    path0_ = subpaths[ 0 ];
    path1_ = subpaths[ 1 ];

    return true;
}


Curve2D InputSketch::overSketch( const QPolygonF& path0_, const QPolygonF& path1_ )
{
    Curve2D sketch_ = fromQtToCurve2D( path0_ );
    Curve2D segment_ = fromQtToCurve2D( path1_ ); ;

    SketchLibrary::overSketch( sketch_, segment_ );
    return sketch_;
}



QRectF InputSketch::boundingRect() const
{
    return curve.boundingRect();
}


void InputSketch::paint( QPainter * painter, const QStyleOptionGraphicsItem * option,
                    QWidget * widget )
{
    QPainterPathStroker stroker;
    stroker.setCurveThreshold( 0.4 );
    stroker.setCapStyle( Qt::RoundCap );
    stroker.setJoinStyle( Qt::RoundJoin );
    stroker.createStroke( curve );

    QPen pen;
    pen.setColor( current_color );
    pen.setWidth( current_width );
    pen.setCosmetic( true );

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( pen );
    painter->drawPath( curve );
}



void InputSketch::fromPainterPathToVector( const QPainterPath& curve_,
                                     std::vector< double >& vertices_,
                                     std::vector< std::size_t >& edges_ )
{
    int number_of_elements_ = curve_.elementCount();

    for( int i = 0; i < number_of_elements_; ++i )
    {
        QPainterPath::Element p_ = curve_.elementAt( i );

        vertices_.push_back( p_.x );
        vertices_.push_back( p_.y );

        if( p_.isMoveTo() == true ) continue;

        int id0 = ( i - 1 < 0 )? 0: ( i - 1 );
        int id1 = i;
        edges_.push_back( static_cast< std::size_t >( id0 ) );
        edges_.push_back( static_cast< std::size_t >( id1 ) );

    }


}



Curve2D InputSketch::fromQtToCurve2D( const QPolygonF& pol_ )
{
    Curve2D curve_;
    std::size_t start = ( pol_.isClosed() ) ? 1 : 0 ;

    for( int i = start ; i < pol_.size() ; ++i )
        curve_.add( QPointFToPoint2D( pol_[ i ] ) );

    curve_.close( pol_.isClosed() );
    return curve_;

}


QPolygonF InputSketch::fromCurve2DToQt( const Curve2D& curve_ )
{

    QPolygonF pol_;

    for( std::size_t i = 0 ; i < curve_.size() ; ++i )
        pol_ << Point2DToQPointF( curve_[ i ] ) ;

    if( curve_.isClosed() && curve_.size() > 0 )
        pol_ << Point2DToQPointF( curve_[ 0 ] ) ;

    return pol_;
}


QPointF InputSketch::Point2DToQPointF( const Point2D & p )
{
    return QPointF( p.x() , p.y() ) ;
}


Point2D InputSketch::QPointFToPoint2D( const QPointF &p )
{
    return Point2D( p.x() , p.y() ) ;
}
