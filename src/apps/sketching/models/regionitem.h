#ifndef REGIONITEM_H
#define REGIONITEM_H

#include <memory>

#include "./core/models/regions.h"
#include "./qtgeometries/polygonitem.h"
#include "./qtgeometries/curveitem.h"


class RegionItem: public PolygonItem
{
    public:

        RegionItem( QGraphicsItem *parent_ = nullptr );
        ~RegionItem();

        void setRawRegion( const std::shared_ptr< Regions >& raw_ );
        const std::shared_ptr< Regions >& getRawRegion() const;

        std::size_t getIndex() const;


        virtual bool isVisible() const override;
        virtual bool isSelected() const override;

        void updateBoundary();

        void clear();

    private:

        std::shared_ptr< Regions > raw;

};

#endif // REGIONITEM_H