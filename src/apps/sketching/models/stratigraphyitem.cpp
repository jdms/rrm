#include "stratigraphyitem.h"

StratigraphyItem::StratigraphyItem( QGraphicsItem *parent_): CurveItem( parent_ )
{

}


void StratigraphyItem::setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;

    csection_depth = depth_;
    csection_direction = dir_;
}

const std::shared_ptr< Stratigraphy >& StratigraphyItem::getRawStratigraphy() const
{
    return raw;
}


void StratigraphyItem::update()
{
}


void StratigraphyItem::clear()
{
    raw.reset();
    CurveItem::clear();
}


StratigraphyItem::~StratigraphyItem()
{
    clear();
}
