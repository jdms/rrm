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
 * @file scene3d.h
 * @author Clarissa C. Marques
 * @brief File containing the class Scene3d
 */


#ifndef SCENE3D_H
#define SCENE3D_H

#include <memory>

#include <QObject>
#include <QColor>

#include "Eigen/Dense"

class QString;
class QOpenGLContext;
class QSurface;

class VolumeShader;
class PlaneShader;
class SurfaceShader;
class RegionShader;


#include "./core/models/scene.h"
#include "./core/models/crosssection.h"
#include "./core/models/stratigraphy.h"


/**
 *  The 3d scene.
 *  This scene is responsible for viewing the elements present on the whole model
 *  It inherits from QObject, so that it can send signal.
 */

class Scene3d: public QObject
{
    Q_OBJECT

    public:


       /**
       * Constructor.
       */
        Scene3d();

       /**
       * Destructor.
       */
        ~Scene3d(){}


        /**
        * Method to create a volume in the scene.
        * This volume will hold a const shared pointer to a volume data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @see Volume
        * @return Void.
        */
        void addVolume( const std::shared_ptr< Volume >& raw_ );


        /**
        * Method to update a volume in the scene.
        * This update can be since updating the dimensions as its visibility.
        * @see Volume
        * @return Void.
        */
        void updateVolume();


        /**
        * Method to add the main cross-section in the scene.
        * This cross-section will hold a const shared pointer to a cross-section data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param csection data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return Void.
        */
        void addMainCrossSection( const std::shared_ptr< CrossSection>& csection_ );


        /**
        * Method to change the main cross-section direction.
        * @param csection data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return Void.
        */
        void changeCrossSectionDirection( const std::shared_ptr< CrossSection>& raw_ );



        /**
        * Method to update the main cross-section in the scene.
        * This update can be since updating the dimensions to its visibility.
        * @see CrossSection
        * @return Void.
        */
        void updateMainCrossSection();


        /**
        * Method to add a new cross-section in the scene.
        * This cross-section will hold a const shared pointer to a cross-section data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param csection data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return Void.
        */
        void addCrossSection( const std::shared_ptr< CrossSection >& csection_ );


        /**
        * Method to update a cross-section in the scene.
        * This update can be since updating the dimensions, position to its visibility.
        * @param dir direction which cross-section is done.
        * @param depth depth of the cross-section in that direction.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return Void.
        */
        void updateCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to remove a cross-section from the scene.
        * @param dir direction which cross-section is done.
        * @param depth depth of the cross-section in that direction.
        * @see Settings::CrossSection::CrossSectionDirections
        * @return Void.
        */
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ ){}


        /**
        * Method to update all cross-sections on the scene.
        * @see CrossSection
        * @return Void.
        */
        void updateCrossSections();


        /**
        * Method to add stratigraphies and structurals in the scene.
        * This stratigraphy will hold a const shared pointer to a stratigraphy data structure, so that it can retrieve the required information
        * to draw it on scene. Note that currently a structural has the same data structure as the stratigraphy.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @see Stratigraphy
        * @return Void.
        */
        void addStratigraphy( const std::shared_ptr< Stratigraphy >& raw_ );


        /**
        * Method to update the stratigraphy or structural, which index is id_.
        * This update can be since updating the dimensions, color to its visibility.
        * @param index index of the stratigraphy to be updated.
        * @see Stratigraphy
        * @return Void.
        */
        void updateStratigraphy( const std::size_t& index_ );


        /**
        * Method to update all stratigraphies and structurals in the scene.
        * @see Stratigraphy
        * @return Void.
        */
        void updateStratigraphies();


        /**
        * Method to add regions in the scene.
        * This region will hold a const shared pointer to a region data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param region data structure that contains the geometry and others required information to draw it on scene.
        * @see Regions
        * @return Void.
        */
        void addRegion( const std::shared_ptr< Regions >& region_ );


        /**
        * Method to update the region which index is id_.
        * This update can be since updating the dimensions, color to its visibility.
        * @param index index of the region to be updated.
        * @see Regions
        * @return Void.
        */
        void updateRegion( const std::size_t& index_ );


        /**
        * Method to update all regions in the scene.
        * @see Regions
        * @return Void.
        */
        void updateRegions();


        /**
        * Method to remove all regions in the scene.
        * @see Regions
        * @return Void.
        */
        void clearRegions();


        /**
        * Method to render the whole scene
        * @param V it is the view matrix
        * @param P it is the projection matrix
        * @param w it is the canvas width
        * @param h it is the canvas height
        * @see Eigen::Affine3f
        * @see Eigen::Matrix4f
        * @return Void
        */
        void draw( const Eigen::Affine3f& V, const Eigen::Matrix4f& P, const int& w, const int& h );


        /**
        * Method to set the OpenGL context on the scene.
        * @see QOpenGLContext
        * @return Void.
        */
        void setOpenGLContext( QOpenGLContext* ctxt );


        /**
        * Method to remove and delete all objects from the scene.
        * @return Void.
        */
        void clearScene();


        /**
        * Method to set vertical exaggeration in the scene.
        * @param scale value of the vertical exaggeration
        * @return Void.
        */
        void setVerticalExaggeration( double scale_ );


    signals:


        /**
        * Signal emitted to update the OpenGL canvas
        * @return Void.
        */
        void updateCanvas();


    private:


        QOpenGLContext* context;                                                        /**< OpenGL context  */

        QSurface* surface;                                                              /**< Abstraction to renderable objects  */

        double v_exag = 1.0;                                                            /**< double value of the vertical exaggeration */

        std::shared_ptr < VolumeShader > volume;                                        /**< Shared pointer to the OpenGL representation of  the volume in the 3d scene. */

        std::shared_ptr< PlaneShader > main_csection;                                   /**< Shared pointer to the OpenGL representation of  the cross-section in the 3d scene. */

        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsX;        /**< A map of wrapper cross-section instances in the  width direction. The first value of the map is the index of the cross-section */

        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsY;        /**< A map of wrapper cross-section instances in the  height direction. The first value of the map is the index of the cross-section */

        std::map< std::size_t, std::shared_ptr< PlaneShader > > cross_sectionsZ;        /**< A map of wrapper cross-section instances in the  depth direction. The first value of the map is the index of the cross-section */

        std::map< std::size_t, std::shared_ptr< SurfaceShader > > stratigraphies;       /**< A map of wrapper stratigraphies instances. The first value of the map is the index of the stratigraphy */

        std::map< std::size_t, std::shared_ptr< RegionShader > > regions;               /**< A map of wrapper regions instances. The first value of the map is the index of the region */


};

#endif // SCENE3D_H
