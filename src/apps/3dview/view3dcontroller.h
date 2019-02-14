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
 * @file view3dcontroller.h
 * @author Clarissa C. Marques
 * @brief File containing the class View3dController
 */


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
        * @param window  sketch window that holds the main cross-section scene
        * @see Canvas3d
        * @return void.
        */
        void setMainWindow( const std::shared_ptr< Canvas3d >& window_);


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see Controller
        * @return void.
        */
        void setController( const std::shared_ptr< Controller >& controller_ );


        /**
        * This method calls the setupScene()
        * @see setupScene()
        * @return void.
        */
        void init();


        /**
        * Method to load the scene with all objectsin cross-sections
        * @return void.
        */
        void setupScene();


        /**
        * This method reload the main cross-section
        * @return void.
        */
        void setMainViewCrossSection();


        /**
        * This method reload the main cross-section as a top view cross-section
        * @return void.
        */
        void setTopViewCrossSection();


        /**
        * Method to update the volume in the scene.
        * Beside updates the volume in the scene it also updates the cross-section, since this latter depends on the volume dimensions.
        * @return void.
        */
        void updateVolume();


        /**
        * Method to update the main cross-section in the scene.
        * This update can be since updating the dimensions to its visibility.
        * @return void.
        */
        void updateMainCrossSection();


        /**
        * Method to add stratigraphies and structurals in the scene.
        * This stratigraphy will hold a const shared pointer to a stratigraphy data structure, so that it can retrieve the required information
        * to draw it on scene. Note that currently a structural has the same data structure as the stratigraphy.
        * @param obj data structure that contains the geometry and others required information to draw it on scene.
        * @see ObjectPtr
        * @return void.
        */
        void addStratigraphy( const ObjectPtr& obj_ );


        /**
        * Method to update all stratigraphies and structurals in the scene.
        * @return void.
        */
        void updateObjects();


        /**
        * Method to add regions in the scene.
        * This region will hold a const shared pointer to a region data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @return void.
        */
        void addRegions();


        /**
        * Method to update all regions in the scene.
        * @return void.
        */
        void updateRegions();


        /**
        * Method to remove all regions in the scene.
        * @return void.
        */
        void clearRegions();


        /**
        * Method to update all elements present on scene
        * @return void.
        */
        void updateScene();


        /**
        * Method to remove all elements present from the scene
        * @return void.
        */
        void clear();


        /**
        * Method to pass the vertical exaggeration to the scene.
        * @param scale the vertical exaggeration scale to be applied
        * @return void.
        */
        void setVerticalExaggeration( double scale_ );



    protected:

        std::shared_ptr< Scene3d > scene;                                               /**< Shared pointer to the 3d Scene */

        std::shared_ptr< Canvas3d > window;                                             /**< Shared pointer to the 3d Canvas */

        std::shared_ptr< Controller > controller;                                       /**< Shared pointer to the RRM controller */

};

#endif // VIEW3DCONTROLLER_H
