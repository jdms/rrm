#ifndef CROSSSECTIONITEM_H
#define CROSSSECTIONITEM_H

#include <memory>

#include "./core/models/crosssection.h"
#include "./qtgeometries/lineitem.h"

/**
 *  A graphical representation to the cross-section in the scene 2d.
 *  It is represent by a line since it is the intersection between a polygon with another cross-section plane.
 *  It inherits from LineItem.
 */

class CrossSectionItem: public LineItem
{
    public:

        /**
        * Constructor. parent_ is passed to LineItem's constructor.
        * @param parent_ the parent item
        */
        CrossSectionItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~CrossSectionItem() override;


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @see CrossSection
        * @return void.
        */
        void setRawCrossSection( const std::shared_ptr< CrossSection >& raw_ );


        /**
        * Method to return the const shared point to the data structure of the cross-section
        * @see CrossSection
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< CrossSection >& getRawCrossSection() const;


        /**
        * This method checks if the cross-section should be visible or not
        * @return boolean. Returns true if the cross-section is visible, and false otherwise.
        */
        bool isVisible() const override;


        /**
        * This method reset the shared pointer to the cross-section data structure
        * @return void.
        */
        void clear();


    protected:

        std::shared_ptr< CrossSection > raw;                                        /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */


};

#endif // CROSSSECTIONITEM_H
