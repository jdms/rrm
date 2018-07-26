#include "stratigraphyitem.h"

StratigraphyItem::StratigraphyItem( QGraphicsItem *parent_): CurveItem( parent_ )
{

}


void StratigraphyItem::setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    setCrossSection( dir_, depth_ );

}


const std::shared_ptr< Stratigraphy >& StratigraphyItem::getRawStratigraphy() const
{
    return raw;
}


void StratigraphyItem::setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    csection_depth = depth_;
    csection_direction = dir_;
    updateCurve();
}



void StratigraphyItem::update()
{
    updateCurve();
}


void StratigraphyItem::updateCurve()
{
    if( raw == nullptr ) return;
    if( raw->isEmpty() == true || raw->isActive() == false ) return;

    prepareGeometryChange();
    if( csection_direction == Settings::CrossSection::CrossSectionDirections::Y )
        setCurve( raw->getTrajectory() );
    else
        setCurve( raw->getCurve( csection_depth ) );
    QGraphicsPathItem::update();
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
