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
    prepareGeometryChange ( );

//    points_list.clear();
//    points_list.push_back ( p );
    curve.moveTo( p );
}


void InputSketch::add ( const QPointF &p )
{
    prepareGeometryChange ( );
//    points_list.push_back ( p );
    curve.lineTo( p );
}





void InputSketch::clear ( )
{
    prepareGeometryChange ( );
//    points_list.clear ( );
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

//    points_list.clear();
//    points_list = QPolygonF(_path);


}


void InputSketch::setSketch ( const QPolygonF & _path )
{
    prepareGeometryChange ( );

//    points_list.clear();
//    points_list = QPolygonF(_path);
    curve = QPainterPath();
    curve.addPolygon( _path );

}


QPolygonF InputSketch::getSketch ( ) const
{

    return curve.toFillPolygon();
//    return points_list;
}


void InputSketch::process( const QPointF& p )
{

    std::cout << "\nEntrei em process" << std::endl;

    QList< QPolygonF > subpaths = curve.toSubpathPolygons();
    if( subpaths.size() < 2 ) return;


    std::cout << "Passei pelo numero de subpaths: " << subpaths.size() << std::endl;

    Curve2D c = PolyQtUtils::qPolyginFToCurve2D( subpaths[ 1 ] );
    Curve2D whole_curve = PolyQtUtils::qPolyginFToCurve2D( subpaths[ 0 ] );

    size_t original_curve_size = whole_curve.size();
    std::cout << "curva original, tamanho = " << whole_curve.size() << std::endl;


    bool oversketch_ok = sketchlib_.overSketching( whole_curve, c );
//    sketchlib_.ensure_x_monotonicity( whole_curve );
    size_t oversketched_curve_size = whole_curve.size();


    std::cout << "Passei pelo oversketching, whole_curve = " << whole_curve.size() << std::endl;



    curve = QPainterPath();
    curve.addPolygon( PolyQtUtils::curve2DToQPolyginF( whole_curve ) );


    std::cout << "criei uma nova curva, tamanho = " << whole_curve.size() << std::endl;
    std::cout << "Adicionei um subpath comecando em p\n" << std::endl;

//    points_list.clear();
//    points_list.push_back( p );


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

