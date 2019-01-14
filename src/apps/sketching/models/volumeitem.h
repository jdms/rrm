#ifndef VOLUMEITEM_H
#define VOLUMEITEM_H

#include "./core/models/volume.h"
#include "./core/models/crosssection.h"
#include "./qtgeometries/planeitem.h"


/**
 *  A graphical representation to the volume in the scene 2d.
 *  It always is represent by a plane since it is a slice of the volume in a direction and depth.
 *  It inherits from PlaneItem
 */

class VolumeItem: public PlaneItem
{
    public:

        /**
        * Constructor.
        */
        VolumeItem( QGraphicsItem *parent_ = nullptr );

        /**
        * Destructor.
        */
        ~VolumeItem() override;

        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw_ a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to return the const shared point to the data structure of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< Volume >& getRawVolume() const;


        /**
        * Method to update the dimensions given a new direction.
        * @param dir_ a new direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateDimensions( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method calls the method 'updateDimensions'.
        * @param dir_ a new direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void update( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method checks if the volume should be visible or not
        * @see testMeToo()
        * @see publicVar()
        * @return boolean. Returns true if the volume is visible, and false otherwise.
        */
        virtual bool isVisible() const override ;


        /**
        * This method clears all the data of the class, including to reset the shared pointer of the Volume data structure.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear();

    private:

        std::shared_ptr< Volume > raw;                                          /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

        Settings::CrossSection::CrossSectionDirections csection_direction =     /**< Direction of the slice of the volume */
                Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;

        double csection_depth = 0.0;                                            /**< Depth of the slice of the volume in the set direction */

};

#endif // VOLUMEITEM_H
