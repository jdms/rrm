/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file sketchingcontroller.h
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class SketchingController
 */


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
        * @see SketchWindow
        * @return void.
        */
        void setMainWindow( const std::shared_ptr< SketchWindow >& window_);


        /**
        * Method to save a reference to the sketch window that will hold the top view cross-section scene, and to the its scene.
        * @param window_  sketch window that holds the top view cross-section scene
        * @see SketchWindow
        * @return void.
        */
        void setTopViewWindow( const std::shared_ptr< SketchWindow >& window_ );


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see Controller
        * @return void.
        */
        void setController( const std::shared_ptr< Controller >& controller_ );


        /**
        * This method calls the 'createMainCrossSection' and 'createTopViewCrossSection' responsible for loading the both scenes ( main scene and top view scene ) with all the elements present in their corresponding cross-sections.
        * @see createMainCrossSection()
        * @see createTopViewCrossSection()
        * @return void.
        */
        void init();


        /**
        * This method enables or disabled the sketching in both scenes ( main scene and top view scene ).
        * @param status_ boolean to set if the sketch should be enabled or disabled. It is true to enable it, and false otherwise.
        * @return void.
        */
        void enableSketching( bool status_ );


        /**
        * This method calls the 'updateVolume' in both scenes ( main scene and top view scene ).
        * @see updateVolume()
        * @return void.
        */
        void updateVolume();


        /**
        * This method loads the main scene with all the elements present in the main cross-section.
        * It calls the init of the SketchScene before loading the scene.
        * @return void.
        */
        void createMainCrossSection();


        /**
        * This method updates the main scene with all the elements present in the main cross-section.
        * @return void.
        */
        void updateMainCrossSection();


        /**
        * This method loads the top view scene with all the elements present in the top view cross-section.
        * It calls the init of the SketchScene before loading the scene.
        * @return void.
        */
        void createTopViewCrossSection();


        /**
        * This method updates the top view scene with all the elements present in the top view cross-section.
        * @return void.
        */
        void updateTopViewCrossSection();


        /**
        * This method loads a scene with all the elements present in a given fixed cross-section.
        * @param dir_ direction of the fixed cross-section
        * @param depth_ direction of the fixed cross-section
        * @param color of the marker associated to the fixed cross-section
        * @see Settings::CrossSection::CrossSectionDirections&
        * @return void.
        */
        void viewCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_/*, bool fixed_ = false*/ );


        /**
        * This method adds a stratigraphy or structural in all scenes being viewed.
        * This graphical object will hold a const shared pointer for a data structure of Stratigraphy, so that it can know how to draw itself.
        * @param obj_ a const shared pointer for a data structure of Stratigraphy, which has all the geometry and others required information to draw it in the scene 2d.
        * @see ObjectPtr
        * @return void.
        */
        void addStratigraphy( const ObjectPtr& obj_ );


        /**
        * This method updates a stratigraphy or structural in all scenes being viewed.
        * @param index_ index of the stratigraphy/structural
        * @return void.
        */
        void updateStratigraphy( const std::size_t& index_ );


        /**
        * This method updates all stratigraphies or structurals in all scenes being viewed.
        * @return void.
        */
        void updateObjects();


        /**
        * This method updates all trajectories of stratigraphies and/or structurals in all scenes being viewed.
        * @return void.
        */
        void updateObjectsTrajectories();


        /**
        * This method adds a region in all scenes being used.
        * This graphical object will hold a const shared pointer for a data structure of Regions, so that it can know how to draw itself.
        * @param reg_ a const shared pointer for a data structure of Regions, which has all the geometry and others required information to draw it in the scene 2d.
        * @see RegionsPtr
        * @return void.
        */
        void addRegion( const RegionsPtr& reg_  );


        /**
        * This method removes all regions in all scenes being viewed.
        * @return void.
        */
        void clearRegions();


        /**
        * This method updates all regions in all scenes being viewed.
        * @return void.
        */
        void updateRegions();


        /**
        * This method checks if there is changes in the boundary (upper and lower) in the main scene and update them.
        * @return void.
        */
        void updateBoundary();


        /**
        * This method remove a window associated to the given fixed cross-section.
        * @param dir_ direction of the fixed cross-section
        * @param depth_ direction of the fixed cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * This method removes all fixed cross-section on the width direction
        * @return void.
        */
        void removeWindowsDirectionX();


        /**
        * This method removes all fixed cross-section on the height direction
        * @return void.
        */
        void removeWindowsDirectionY();


        /**
        * This method removes all fixed cross-section on the depth direction
        * @return void.
        */
        void removeWindowsDirectionZ();


        /**
        * This method calls a clear to the both scenes and windows ( main and top-view )
        * @return void.
        */
        void clear();


        /**
        * This method updates the image on scene removing or setting if needed.
        * @return void.
        */
        void updateImageInScene();


        /**
        * This method updates the point used to guided extrusion in the top view scene.
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param depth_ depth of the cross-section associated to the current scene
        * @param dir_ direction of the cross-section associated to the current scene
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setPointGuidedExtrusionInPath( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method retrieves the point used to guided extrusion in the top view scene.
        * Note that meaning of the coordinates should change depending on which cross-section direction is being used.
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param pz_ z coordinate of the point
        * @return void.
        */
        void getGuidedExtrusionPoint( float& px_, float& py_, float& pz_ );


    protected:


        /**
        * This method is responsible for add the objects from the cross-section to the scene and also update the cross-section information.
        * @param csection_ cross-section which holds the objects
        * @param scene_ scene to where the objects will be added
        * @see CrossSectionPtr
        * @see SketchScene
        * @return void.
        */
        void setObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ );


        /**
        * This method is responsible for calling updates for all the objects on scene, including the image and the boundary.
        * @param csection_ cross-section which holds the objects
        * @param scene_ scene to where the objects will be updated
        * @see CrossSectionPtr
        * @see SketchScene
        * @return void.
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
        * @see Settings::CrossSection::CrossSectionDirections
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
