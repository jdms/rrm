#ifndef SKETCHINGCONTROLLER_H
#define SKETCHINGCONTROLLER_H

#include <memory>
#include <map>


#include <QPointF>

#include "./core/models/controller.h"
#include "./models/volumeitem.h"


class SketchScene;
class SketchWindow;


/**
 *   Bridge between the viewer of sketch application and the controller of the RRM.
 */


class SketchingController
{
    public:

        /**
        * Constructor.
        */
        SketchingController();


        /**
        * Method to save a reference to the sketch window that holds the main cross-section scene, and to the its scene.
        * @param window_  sketch window that holds the main cross-section scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setMainWindow( const std::shared_ptr< SketchWindow >& window_);


        /**
        * Method to save a reference to the sketch window that will hold the top view cross-section scene, and to the its scene.
        * @param window_  sketch window that holds the top view cross-section scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setTopViewWindow( const std::shared_ptr< SketchWindow >& window_ );


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setController( const std::shared_ptr< Controller >& controller_ );


        /**
        * This method calls the 'createMainCrossSection' and 'createTopViewCrossSection' responsible for loading the both scenes ( main scene and top view scene ) with all the elements present in their corresponding cross-sections.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();


        /**
        * This method enables or disabled the sketching in both scenes ( main scene and top view scene ).
        * @param status_ boolean to set if the sketch should be enabled or disabled. It is true to enable it, and false otherwise.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void enableSketching( bool status_ );


        /**
        * This method calls the 'updateVolume' in both scenes ( main scene and top view scene ).
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateVolume();


        /**
        * This method loads the main scene with all the elements present in the main cross-section.
        * It calls the init of the SketchScene before loading the scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createMainCrossSection();


        /**
        * This method updates the main scene with all the elements present in the main cross-section.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateMainCrossSection();


        /**
        * This method loads the top view scene with all the elements present in the top view cross-section.
        * It calls the init of the SketchScene before loading the scene.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createTopViewCrossSection();


        /**
        * This method updates the top view scene with all the elements present in the top view cross-section.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateTopViewCrossSection();


        /**
        * This method loads a scene with all the elements present in a given fixed cross-section.
        * @param dir_ direction of the fixed cross-section
        * @param depth_ direction of the fixed cross-section
        * @param color of the marker associated to the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void viewCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_/*, bool fixed_ = false*/ );


        /**
        * This method adds a stratigraphy or structural in all scenes being viewed.
        * This graphical object will hold a const shared pointer for a data structure of Stratigraphy, so that it can know how to draw itself.
        * @param obj_ a const shared pointer for a data structure of Stratigraphy, which has all the geometry and others required information to draw it in the scene 2d.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addStratigraphy( const ObjectPtr& obj_ );


        /**
        * This method updates a stratigraphy or structural in all scenes being viewed.
        * @param index_ index of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateStratigraphy( const std::size_t& index_ );


        /**
        * This method updates all stratigraphies or structurals in all scenes being viewed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjects();


        /**
        * This method updates all trajectories of stratigraphies and/or structurals in all scenes being viewed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjectsTrajectories();


        /**
        * This method adds a region in all scenes being used.
        * This graphical object will hold a const shared pointer for a data structure of Regions, so that it can know how to draw itself.
        * @param reg_ a const shared pointer for a data structure of Regions, which has all the geometry and others required information to draw it in the scene 2d.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addRegion( const RegionsPtr& reg_  );


        /**
        * This method removes all regions in all scenes being viewed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clearRegions();


        /**
        * This method updates all regions in all scenes being viewed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateRegions();


        /**
        * This method checks if there is changes in the boundary (upper and lower) in the main scene and update them.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateBoundering();


        /**
        * This method remove a window associated to the given fixed cross-section.
        * @param dir_ direction of the fixed cross-section
        * @param depth_ direction of the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * This method removes all fixed cross-section on the width direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeWindowsDirectionX();


        /**
        * This method removes all fixed cross-section on the height direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeWindowsDirectionY();


        /**
        * This method removes all fixed cross-section on the depth direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeWindowsDirectionZ();


        /**
        * This method calls a clear to the both scenes and windows ( main and top-view )
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clear();


        /**
        * This method updates the image on scene removing or setting if needed.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateImageInScene();


        /**
        * This method updates the point used to guided extrusion in the top view scene.
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param depth_ depth of the cross-section associated to the current scene
        * @param dir_ direction of the cross-section associated to the current scene
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setPointGuidedExtrusionInPath( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method retrieves the point used to guided extrusion in the top view scene.
        * Note that meaning of the coordinates should change depending on which cross-section direction is being used.
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param pz_ z coordinate of the point
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void getGuidedExtrusionPoint( float& px_, float& py_, float& pz_ );


        inline void setCurrentColor( int red_, int green_, int blue_ )
        {
            controller->setCurrentColor( red_, green_, blue_ );
        }

    protected:


        /**
        * This method is responsible for add the objects from the cross-section to the scene and also update the cross-section information.
        * @param csection_ cross-section which holds the objects
        * @param scene_ scene to where the objects will be added
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ );


        /**
        * This method is responsible for calling updates for all the objects on scene, including the image and the boundary.
        * @param csection_ cross-section which holds the objects
        * @param scene_ scene to where the objects will be updated
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void updateObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ );



    protected:


        std::shared_ptr< SketchScene > main_scene;                                                  /**< Shared pointer to the main scene. It can view cross-section on the direction width and depth  */

        std::shared_ptr< SketchScene > topview_scene;                                               /**< Shared pointer to the top view scene. It can view cross-section only the direction height  */

        std::shared_ptr< SketchWindow > window;                                                     /**< Shared pointer to the window which holds the main canvas */

        std::shared_ptr< SketchWindow > topview_window;                                             /**< Shared pointer to the window which holds the top view canvas */

        std::map< double, std::shared_ptr< SketchScene > > scenesX;                                 /**< A map that associates the depth of the fixed cross-section on the direction X to the scene that views it*/

        std::map< double, std::shared_ptr< SketchScene > > scenesY;                                 /**< A map that associates the depth of the fixed cross-section on the direction Y to the scene that views it*/

        std::map< double, std::shared_ptr< SketchScene > > scenesZ;                                 /**< A map that associates the depth of the fixed cross-section on the direction Z to the scene that views it*/

        std::shared_ptr< Controller > controller;                                                   /**< Shared pointer to the RRM controller */


        /**
        * @struct GuidedExtrusionPoint
        * @brief This struct holds the data needed to represent the point used on the guided extrusion
        * @var px x coordinate of the point
        * @var py y coordinate of the point
        * @var depth depth of the cross-section in which the guided extrusion is being used
        * @var dir direction of the cross-section in which the guided extrusion is being used
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct GuidedExtrusionPoint
        {
            float px;
            float py;
            double depth;
            Settings::CrossSection::CrossSectionDirections dir;
        } guided_point;



};

#endif // SKETCHINGCONTROLLER_H
