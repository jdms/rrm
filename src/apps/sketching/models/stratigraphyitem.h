#ifndef STRATIGRAPHYITEM_H
#define STRATIGRAPHYITEM_H

#include "./core/models/stratigraphy.h"
#include "./core/models/cross_section.h"
#include "./qtgeometries/curveitem.h"

class StratigraphyItem: public CurveItem
{
    public:

        StratigraphyItem( QGraphicsItem *parent_ = nullptr );
        ~StratigraphyItem();

        void setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );
        const std::shared_ptr< Stratigraphy >& getRawStratigraphy() const;

        void update();
        void clear();

    protected:

        std::shared_ptr< Stratigraphy > raw;

        Settings::CrossSection::CrossSectionDirections csection_direction = Settings::CrossSection::CrossSectionDirections::Z;
        double csection_depth = 0.0;


};

#endif // STRATIGRAPHYITEM_H
