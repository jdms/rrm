#include "InputSketch.h"

InputSketch::InputSketch ( QColor color ) :QGraphicsPathItem ( )
{
    prepareGeometryChange ( );


    is_visible = false;
    is_inside = false;


	pen_color = color;
    pen_color.setCapStyle( Qt::RoundCap );
    pen_color.setJoinStyle( Qt::RoundJoin );


//    custom_stroker.setCapStyle( Qt::RoundCap );
//    custom_stroker.setCurveThreshold( 0.9 );
//    custom_stroker.setDashPattern( Qt::SolidLine );
//    custom_stroker.setJoinStyle( Qt::RoundJoin );

    setFlag ( QGraphicsItem::ItemIsSelectable );
    setAcceptTouchEvents ( true );


}


void InputSketch::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    pen_color.setWidth ( 3 );

    painter->setRenderHint ( QPainter::Antialiasing );
    painter->setPen ( pen_color );
    painter->setBrush ( Qt::NoBrush );

    QPainterPath outline_curve = custom_stroker.createStroke( curve );

//    if( curve.toSubpathPolygons().isEmpty() == true ) return;

//    painter->drawPolyline( curve.toSubpathPolygons()[0] );
    painter->drawPath( curve/*outline_curve*/ );



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

