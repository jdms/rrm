#include "crosssectionitem.h"

CrossSectionItem::CrossSectionItem( QGraphicsItem *parent_ ):LineItem( parent_ )
{
    line.setLine( 0, 250, 500, 250 );
}

void CrossSectionItem::setRawCrossSection( const std::shared_ptr< CrossSection >& raw_ )
{
    raw = raw_;
}


const std::shared_ptr< CrossSection >& CrossSectionItem::getRawCrossSection() const
{
    return raw;
}

bool CrossSectionItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}

void CrossSectionItem::clear()
{
    raw.reset();
    LineItem::clear();
    update();
}

CrossSectionItem::~CrossSectionItem()
{
    clear();
}
