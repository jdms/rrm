#ifndef WELLITEM_H
#define WELLITEM_H

#include <QGraphicsItemGroup>

#include <memory>

#include "./core/models/well.h"
#include "./core/models/cross_section.h"
#include "./qtgeometries/curveitem.h"
#include "./qtgeometries/polygonitem.h"

class WellItem: public  QGraphicsItemGroup
{
    public:

        WellItem();
        ~WellItem();

        void define();

        QRectF boundingRect() const;

        void setRawWell( const std::shared_ptr< Well >& raw_ , const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        const std::shared_ptr< Well >& getRawWell() const;

        void clear();

    protected:

        void setInjectorShape();
        void setProductorShape();


//        CurveItem curve;
        PolygonItem *triangle = nullptr;


    private:

        std::shared_ptr< Well > raw;
};

#endif // WELLITEM_H
