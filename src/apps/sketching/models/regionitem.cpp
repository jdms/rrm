#include <iostream>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "regionitem.h"

RegionItem::RegionItem( QGraphicsItem *parent_ ): PolygonItem( parent_ )
{
}

void RegionItem::setRawRegion( const std::shared_ptr< Regions >& raw_ )
{
    raw = raw_;

    const PolyCurve& upper_ = raw->getUpperBound();
    const PolyCurve& lower_ = raw->getLowerBound();

    QPolygonF pol_upper_ = SketchLibraryWrapper::fromCurve2DToQt( upper_.getSubcurve( 0 ) );
    QPolygonF pol_lower_ = SketchLibraryWrapper::fromCurve2DToQt( lower_.getSubcurve( 0 ) );

    QPolygonF pol_ = pol_upper_.intersected( pol_lower_ );
    setPolygon( pol_ );
}

const std::shared_ptr< Regions >& RegionItem::getRawRegion() const
{
    return raw;
}

//void RegionItem::paint( QPainter * painter_, const QStyleOptionGraphicsItem * option_, QWidget * widget_ )
//{
//    if( isVisible() == false )
//        return;


//    QPen border_;
//    border_.setColor( getBorderColor() );
//    border_.setWidth( getBorderWidth() );
//    border_.setCosmetic( true );

//    QBrush fill_;
//    QColor color_ = getFillColor();
//    fill_.setColor(  QColor( color_.red(), color_.green(), color_.blue(), 75 ) );
//    fill_.setStyle( Qt::SolidPattern );


//    painter_->setRenderHint( QPainter::Antialiasing );

//    if( has_border == true )
//        painter_->setPen( border_ );
//    else
//        painter_->setPen( Qt::NoPen );

//    if( RegionItem::isSelected() == true )
//    {
//        border_.setColor( Qt::blue );
//        painter_->setPen( border_ );
//        std::cout << "it was selected" << std::endl << std::flush;
//    }

//    painter_->setBrush( fill_ );
//    painter_->drawPolygon( plane );
//}


std::size_t RegionItem::getIndex() const
{
    return raw->getIndex();
}


bool RegionItem::isSelected() const
{
    if( raw == nullptr ) return false;
    return raw->isSelected();
}



bool RegionItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}

void RegionItem::clear()
{
    raw.reset();
    PolygonItem::clear();
}

RegionItem::~RegionItem()
{
    clear();
}
