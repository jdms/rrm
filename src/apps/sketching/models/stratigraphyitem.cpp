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
    updateCurve();
}


void StratigraphyItem::update()
{
    updateCurve();
}


void StratigraphyItem::updateCurve()
{
    if( raw == nullptr ) return;
//    if( raw->isEmpty() == true || raw->isActive() == false ) return;

    if( raw->isDone() )
        resetToDefaultStyle();

    int r_, g_, b_;
    raw->getColor( r_, g_, b_ );
    setColor( r_, g_, b_ );

    prepareGeometryChange();

    changeDirection();

    QGraphicsPathItem::update();
}


void StratigraphyItem::changeDirection()
{
    Settings::CrossSection::CrossSectionDirections dir_ = raw->getCrossSectionDirection();


    if( csection_direction == dir_ )
        setCurve( raw->getCurve( csection_depth ) );
    else
    {
        if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        {
            updateTrajectory();
        }

        else if( csection_direction == Settings::CrossSection::CrossSectionDirections::Y  )
        {
            return;
        }

        else
            setCurve( raw->getCurve( csection_depth ) );
    }
}


void StratigraphyItem::updateTrajectory()
{
    if( raw == nullptr ) return;
    if( raw->isEmpty() == true || raw->isActive() == false ) return;

    if( raw->isDone() )
        resetToDefaultStyle();

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
