#ifndef STRATIGRAPHYITEM_H
#define STRATIGRAPHYITEM_H

#include "./core/models/stratigraphy.h"
#include "./core/models/crosssection.h"
#include "./qtgeometries/curveitem.h"

class StratigraphyItem: public CurveItem
{
    public:

        StratigraphyItem( QGraphicsItem *parent_ = nullptr );
        ~StratigraphyItem();

        void setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );
        const std::shared_ptr< Stratigraphy >& getRawStratigraphy() const;

        void setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        std::size_t getIndex() const;

        bool isVisible() const override;
        bool isSelected() const override;

        void update() override;
        void updateTrajectory();
        void updateLevelCurves();

        void clear();


    protected:

        void updateCurve();




    protected:

        std::shared_ptr< Stratigraphy > raw;

        Settings::CrossSection::CrossSectionDirections csection_direction = Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;
        double csection_depth = 0.0;

};

#endif // STRATIGRAPHYITEM_H