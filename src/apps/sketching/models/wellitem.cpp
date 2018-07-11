#include <QPainter>

#include "wellitem.h"

WellItem::WellItem()
{
    triangle = new PolygonItem();

}

void WellItem::define()
{
    if( triangle == nullptr ) return;

    if( raw->getType() == Well::Type::INJECTOR )
    {
        setInjectorShape();
    }
    else
    {
        setProductorShape();
    }
    addToGroup( triangle );
}

void WellItem::setInjectorShape()
{

    triangle->addPoint( QPointF( 0, 0 ) );
    triangle->addPoint( QPointF( 10, 0 ) );
    triangle->addPoint( QPointF( 5, 10 ) );
    triangle->setDone( true );
}


void WellItem::setProductorShape()
{
    triangle->addPoint( QPointF( 0, 10 ) );
    triangle->addPoint( QPointF( 10, 10 ) );
    triangle->addPoint( QPointF( 5, 0 ) );
    triangle->setDone( true );
}


void WellItem::setRawWell( const std::shared_ptr< Well >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    define();
}

const std::shared_ptr< Well >& WellItem::getRawWell() const
{
    return raw;
}

void WellItem::clear()
{
    raw = 0;
    if( triangle == nullptr ) return;
    triangle->clear();
//    curve.clear();
}


QRectF WellItem::boundingRect() const
{
    if( triangle == nullptr ) return QRectF();
    return triangle->boundingRect();
}


WellItem::~WellItem()
{
    clear();

    if( triangle != nullptr )
        delete triangle;
    triangle = nullptr;

}
