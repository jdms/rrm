#include <QPainter>
#include <iostream>
#include "curveitem.h"

CurveItem::CurveItem( QGraphicsItem *parent_ ): QGraphicsPathItem( parent_ )
{
    setZValue( std::numeric_limits<float>::max() );
    setPath( curve );
}



QRectF CurveItem::boundingRect() const
{
    return curve.boundingRect();
}

void CurveItem::paint( QPainter * painter_, const QStyleOptionGraphicsItem * option_, QWidget * widget_ )
{
    QPainterPathStroker stroker_;
    stroker_.setCurveThreshold( 0.4 );
    stroker_.setCapStyle( Qt::RoundCap );
    stroker_.setJoinStyle( Qt::RoundJoin );
    stroker_.createStroke( curve );

    QPen pen_;
    pen_.setColor( getColor() );
    pen_.setWidth( getWidth() );
    pen_.setCosmetic( true );

    if( isSelected() == true )
    {
        pen_.setColor( Qt::blue );
    }

    painter_->setRenderHint( QPainter::Antialiasing );
    painter_->setPen( pen_ );
    painter_->drawPath( curve );

}


bool CurveItem::create( const QPointF& p_ )
{
    if( isDone() == false ) return false;

    prepareGeometryChange();
    curve.moveTo( p_ );
    is_done = false;
    return true;
}


bool CurveItem::add( const QPointF& p_ )
{
    if( isDone() == true ) return false;

    prepareGeometryChange();
    curve.lineTo( p_ );
    return true;
}

bool CurveItem::hasSubpaths()
{
    return ( curve.toSubpathPolygons().size() > 1/*MIN_SUBPATHS_OVERSKETCH*/ );
}

void CurveItem::connect()
{
    setDone();

    bool status_ = !isEmpty();
    status_ &= hasSubpaths();

    if( status_ == false ) return;

    prepareGeometryChange();
    QList< QPolygonF > subpaths_ = curve.toSubpathPolygons();
    curve = SketchLibraryWrapper::overSketchQt( subpaths_.at( 0 ), subpaths_.at( 1 ) );
    update();
}

void CurveItem::setDone()
{
    setPath( curve );
    is_done = true;
}

bool CurveItem::isDone() const
{
    return is_done;
}


PolyCurve CurveItem::getCurve()
{
    if( curve.isEmpty() == true ) return PolyCurve();
    Curve2D curve_ = SketchLibraryWrapper::fromQtToCurve2D( curve.toSubpathPolygons()[0] );
    return PolyCurve( curve_ );
}

void CurveItem::clear()
{
    prepareGeometryChange();

    curve = QPainterPath();
    setPath( curve );

    is_done = true;
    update();
}

bool CurveItem::isEmpty() const
{
    return curve.isEmpty();
}


void CurveItem::setColor( int r_, int g_, int b_ )
{
    line_color.red = r_;
    line_color.green = g_;
    line_color.blue = b_;
    update();
}

void CurveItem::getColor( int& r_, int& g_, int& b_ )
{
    r_ = line_color.red;
    g_ = line_color.green;
    b_ = line_color.blue;
}


void CurveItem::setColor( const QColor& color_ )
{
    line_color.red = color_.red();
    line_color.green = color_.green();
    line_color.blue = color_.blue();
    update();
}

QColor CurveItem::getColor() const
{
    return QColor( line_color.red, line_color.green, line_color.blue );
}


void CurveItem::setWidth( int width_ )
{
    line_width = width_;
    update();
}

int CurveItem::getWidth() const
{
    return line_width;
}


void CurveItem::resetToDefaultColor()
{
    line_color = DEFAULT_LINE_COLOR;
    update();
}

void CurveItem::resetToDefaultWidth()
{
    line_width = DEFAULT_LINE_WIDTH;
    update();
}



CurveItem::~CurveItem()
{
    clear();
}
