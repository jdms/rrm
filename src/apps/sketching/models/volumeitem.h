#ifndef VOLUMEITEM_H
#define VOLUMEITEM_H

#include "./core/models/volume.h"
#include "./core/models/cross_section.h"
#include "./qtgeometries/planeitem.h"

class VolumeItem: public PlaneItem
{
    public:

        VolumeItem( QGraphicsItem *parent_ = nullptr );
        ~VolumeItem();

        void setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        const std::shared_ptr< Volume >& getRawVolume() const;

        void update();
        void clear();

    private:

        std::shared_ptr< Volume > raw;

        Settings::CrossSection::CrossSectionDirections csection_direction = Settings::CrossSection::CrossSectionDirections::Z;
        double csection_depth = 0.0;

};

#endif // VOLUMEITEM_H
