#include "stratigraphyitem.h"

StratigraphyItem::StratigraphyItem( QGraphicsItem *parent_): CurveItem( parent_ )
{
    setStyle( Qt::PenStyle::DotLine );
}


void StratigraphyItem::setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    raw = raw_;
    csection_direction = dir_;
    csection_depth = depth_;

    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );

    setColor( r_, g_, b_ );
    setStyle( Qt::PenStyle::DotLine );

}


const std::shared_ptr< Stratigraphy >& StratigraphyItem::getRawStratigraphy() const
{
    return raw;
}


void StratigraphyItem::setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    csection_depth = depth_;
    csection_direction = dir_;
}


bool StratigraphyItem::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();
}


bool StratigraphyItem::isSelected() const
{
    if( raw == nullptr ) return false;
    return raw->isSelected();
}



void StratigraphyItem::update()
{
    updateCurve();
}


void StratigraphyItem::updateCurve()
{
    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();

    if( raw->isSelectable() == true )
        setColor( 255, 255, 0 );
    else
    {
        int r_, g_, b_;
        raw->getColor( r_, g_, b_ );
        setColor( r_, g_, b_ );
    }
    prepareGeometryChange();
    setCurve( raw->getCurve( csection_depth ) );

    QGraphicsPathItem::update();
}


std::size_t StratigraphyItem::getIndex() const
{
    return raw->getIndex();
}

void StratigraphyItem::updateLevelCurves()
{

    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();

    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setColor( r_, g_, b_ );


    prepareGeometryChange();
    if( raw->getCrossSectionDirection() != Settings::CrossSection::CrossSectionDirections::Y ) return;

    if( csection_direction == Settings::CrossSection::CrossSectionDirections::X )
        setCurves( raw->getCurves(), true );
    else
        setCurves( raw->getCurves() );


}


void StratigraphyItem::updateTrajectory()
{
    if( raw == nullptr ) return;
    if( raw->isDone() )
        resetToDefaultStyle();

    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setColor( r_, g_, b_ );


    if( raw->getCrossSectionDirection() == Settings::CrossSection::CrossSectionDirections::Y ) return;
    prepareGeometryChange();

    setCurve( raw->getTrajectory() );
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
