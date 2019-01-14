#ifndef VIEW3DCONTROLLER_H
#define VIEW3DCONTROLLER_H

#include <memory>

#include "./core/models/controller.h"


class Scene3d;
class Canvas3d;

/**
 *   Bridge between the viewer of the 3d application and the controller of the RRM.
 */

class View3dController
{

    public:


        /**
        * Constructor.
        */
        View3dController();


        /**
        * Method to save a reference to the 3d canvas and to the its scene.
        * @param window_  sketch window that holds the main cross-section scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setMainWindow( const std::shared_ptr< Canvas3d >& window_);


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setController( const std::shared_ptr< Controller >& controller_ );


        /**
        * This method calls the setupScene()
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();


        /**
        * Method to load the scene with all objectsin cross-sections
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setupScene();


        /**
        * This method reload the main cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setMainViewCrossSection();


        /**
        * This method reload the main cross-section as a top view cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setTopViewCrossSection();


        /**
        * Method to update the volume in the scene.
        * Beside updates the volume in the scene it also updates the cross-section, since this latter depends on the volume dimensions.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateVolume();


        /**
        * Method to update the main cross-section in the scene.
        * This update can be since updating the dimensions to its visibility.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateMainCrossSection();


        /**
        * Method to add stratigraphies and structurals in the scene.
        * This stratigraphy will hold a const shared pointer to a stratigraphy data structure, so that it can retrieve the required information
        * to draw it on scene. Note that currently a structural has the same data structure as the stratigraphy.
        * @param obj_ data structure that contains the geometry and others required information to draw it on scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addStratigraphy( const ObjectPtr& obj_ );


        /**
        * Method to update all stratigraphies and structurals in the scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjects();


        /**
        * Method to add regions in the scene.
        * This region will hold a const shared pointer to a region data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param region_ data structure that contains the geometry and others required information to draw it on scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegions();


        /**
        * Method to update all regions in the scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateRegions();


        /**
        * Method to remove all regions in the scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clearRegions();


        /**
        * Method to update all elements present on scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateScene();


        /**
        * Method to remove all elements present from the scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clear();


        /**
        * Method to pass the vertical exaggeration to the scene.
        * @see publicVar()
        * @return Void.
        */
        void setVerticalExaggeration( double scale_ );



    protected:

        std::shared_ptr< Scene3d > scene;                                               /**< Shared pointer to the 3d Scene */

        std::shared_ptr< Canvas3d > window;                                             /**< Shared pointer to the 3d Canvas */

        std::shared_ptr< Controller > controller;                                       /**< Shared pointer to the RRM controller */

};

#endif // VIEW3DCONTROLLER_H
