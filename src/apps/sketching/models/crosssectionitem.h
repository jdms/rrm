#ifndef CROSSSECTIONITEM_H
#define CROSSSECTIONITEM_H

#include <memory>

#include "./core/models/cross_section.h"
#include "./qtgeometries/lineitem.h"

class CrossSectionItem: public LineItem
{
    public:

        CrossSectionItem( QGraphicsItem *parent_ = nullptr );
        ~CrossSectionItem();

        void setRawCrossSection( const std::shared_ptr< CrossSection >& raw_ );
        const std::shared_ptr< CrossSection >& getRawCrossSection() const;

        void clear();

    protected:

        std::shared_ptr< CrossSection > raw;


};

#endif // CROSSSECTIONITEM_H
