#include <QPainter>
#include <QPainterPath>

#include "lineitem.h"

LineItem::LineItem( QGraphicsItem *parent_ ):QGraphicsLineItem( parent_ )
{
    setLine( line );
}

LineItem::~LineItem()
{
    clear();
}

QRectF LineItem::boundingRect() const
{
    QPainterPath path_;
    path_.moveTo( line.p1() );
    path_.lineTo( line.p2() );
//    return QGraphicsLineItem::boundingRect();
    return path_.boundingRect();
}

void LineItem::clear()
{
    line = QLineF();
    update();
}

bool LineItem::isEmpty() const
{
    return line.isNull();
}

void LineItem::setLineColor( int r_, int g_, int b_ )
{
    prepareGeometryChange();
    line_color.red   = r_;
    line_color.green = g_;
    line_color.blue  = b_;
    update();
}

void LineItem::getLineColor( int& r_, int& g_, int& b_ ) const
{
    r_ = line_color.red  ;
    g_ = line_color.green;
    b_ = line_color.blue ;
}


void LineItem::setLineColor( QColor color_ )
{
    setLineColor( color_.red(), color_.green(), color_.blue() );
}


QColor LineItem::getLineColor() const
{
    return QColor( line_color.red, line_color.green, line_color.blue );
}


void LineItem::setLineWidth( int width_ )
{
    prepareGeometryChange();
    line_width = width_;
    update();
}

int LineItem::getLineWidth() const
{
    return line_width;
}

void LineItem::resetToDefaultLineColor()
{
    setLineColor( DEFAULT_LINE_COLOR.red, DEFAULT_LINE_COLOR.green, DEFAULT_LINE_COLOR.blue );
}


void LineItem::paint( QPainter* painter_, const QStyleOptionGraphicsItem* option_,
                      QWidget* widget_ )
{
    QPen line_;
    line_.setColor( getLineColor() );
    line_.setWidth( getLineWidth() );
    line_.setCosmetic( true );

    if( isSelected() == true )
    {
        line_.setColor( Qt::blue );
    }

    painter_->setRenderHint( QPainter::Antialiasing );
    painter_->setPen( line_ );
    painter_->drawLine( line );
}
