#ifndef STRATIGRAPHYITEM_H
#define STRATIGRAPHYITEM_H

#include "./core/models/stratigraphy.h"
#include "./core/models/crosssection.h"
#include "./qtgeometries/curveitem.h"


/**
 *  A graphical representation to the stratigraphy and structural in the scene 2d.
 *  It is represent by a curve since it is the intersection between a surface with the cross-section plane.
 *  It inherits from CurveItem.
 */

class StratigraphyItem: public CurveItem
{
    public:

        /**
        * Constructor.
        */
        StratigraphyItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~StratigraphyItem() override;


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @param dir_ the direction of the cross-section that cutted the surface
        * @param depth_ the depth of the cross-section that cutted the surface
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );


        /**
        * Method to return the const shared point to the data structure of the stratigraphy
        * @see testMeToo()
        * @see publicVar()
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< Stratigraphy >& getRawStratigraphy() const;


        /**
        * Method to set the cross-section which cutted the surface
        * @see testMeToo()
        * @see publicVar()
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        void setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to return the index of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return a std::size_t representing the index of the stratigraphy/structural
        */
        std::size_t getIndex() const;


        /**
        * This method checks if the stratigraphy/structural should be visible or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the stratigraphy/structural is visible, and false otherwise.
        */
        bool isVisible() const override;


        /**
        * This method checks if the stratigraphy/structural is selected or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the stratigraphy/structural is selected, and false otherwise.
        */
        bool isSelected() const override;


        /**
        * This method calls the method 'updateCurve'
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void update() override;


        /**
        * This method updates only the trajectory of the stratigraphy/structural if it exists
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateTrajectory();


        /**
        * This method updates only the level curves  of the stratigraphy/structural if the curve was made in the height direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateLevelCurves();


        /**
        * This method reset the shared pointer to the stratigraphy/structural data structure and clear the cross-section information
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear();


    protected:


        /**
        * This method updates the geometry and others rendering properties
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateCurve();


    protected:

        std::shared_ptr< Stratigraphy > raw;                                                    /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

        Settings::CrossSection::CrossSectionDirections csection_direction =                     /**< Direction of the cross-section that cutted the surface */
                Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;

        double csection_depth = 0.0;                                                            /**< Depth of the cross-section that cutted the surface */

};

#endif // STRATIGRAPHYITEM_H
