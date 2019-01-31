/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "./core/definitions/constants.hpp"
#include "./core/models/container.h"
#include "./core/widgets/objecttree.h"
#include "./core/models/irap_grid_exporter.hpp"
#include "./apps/3dview/scene3d.h"
#include "crosssection.h"
#include "regions.h"
#include "domain.h"
#include "rules_processor.hpp"


class Volume;
class Object;
class RRMApplication;


using VolumePtr = std::shared_ptr< Volume >;
using CrossSectionPtr = std::shared_ptr< CrossSection >;
using ObjectPtr = std::shared_ptr< Object >;
using RegionsPtr = std::shared_ptr< Regions >;
const std::size_t UNDEFINED_INDEX = 9999;


/**
 *  A controller to help manipulate the data and to handle the requests from the user interface
 */


class Controller
{
    public:


        /**
         *  \var TriangleMesh a simplified name to point to the TriangleMesh structure in the RulesProcessor
         */
        using TriangleMesh = RulesProcessor::TriangleMesh;


        /**
         *  \var CurveMesh a simplified name to point to the CurveMesh structure in the RulesProcessor
         */
        using CurveMesh = RulesProcessor::CurveMesh;


        /**
        * \enum MeshResolution
        * \brief Enum to represent the set of resolution level of the mesh
        */
        enum class MeshResolution{ LOW, MEDIUM, HIGH };


        /**
        * Constructor.
        */
        Controller();


        /**
        * Constructor.
        */
        Controller(const Controller & cont_);


        /**
        * Constructor.
        */
        Controller & operator=(const Controller & cont_);


        /**
        * Destructor.
        */
        ~Controller();


        /**
        * Method to set a reference to the application.
        * This create a brigde between the data and the interface by the controller
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setApplication( RRMApplication* const& app_ );


        /**
        * Method to initialize the controller.
        * This method adds the initial object and set the resolution of the mesh
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();


        /**
        * Method to create a volume.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createVolume();


        /**
        * Method to resize the volume dimensions
        * @param width_ the new witdth volume
        * @param height_ the new height volume
        * @param depth_ the new depth volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void resizeVolume( double width_, double height_, double depth_ );


        /**
        * Method to set the volume width
        * @param width_ the new witdth volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeWidth( double width_ );


        /**
        * Method to set the volume height
        * @param height_ the new height volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeHeight( double height_ );


        /**
        * Method to set the volume depth
        * @param depth_ the new depth volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeLenght( double lenght_ );


        /**
        * Method to get the volume geometry, i.e., the dimensions and the origin of the volume
        * @param ox_ the X coordinate of the volume origin
        * @param oy_ the Y coordinate of the volume origin
        * @param oz_ the Z coordinate of the volume origin
        * @param w_ the volume witdth
        * @param h_ the volume height
        * @param d_ the volume depth
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void getVolumeGeometry( double& ox_, double& oy, double& oz, double& w_, double& h_, double& d_ ) const;


        /**
        * Method to set a name to the volume
        * @param id_ a new name to the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setVolumeName( const std::string& name_ );


        /**
        * Method to set the volume visible or not
        * @param status_ if the value is true the volume is visible and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setVolumeVisibility( bool status_ );


        /**
        * This method returns the total volume of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return double the total volume of the volume
        */
        inline double getTotalVolume() const { return model.volume->getVolume(); }


        /**
        * Method to create the main cross-section.
        * The main cross-section can be in the 'WIDTH' and 'DEPTH' directions
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createMainCrossSection();


        /**
        * Method to set the direction of the main cross-section
        * @param dir_ the new cross-section direction.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void changeMainCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to get the current cross-section direction
        * @see testMeToo()
        * @see publicVar()
        * @return CrossSectionDirections the current cross-section direction.
        */
        inline Settings::CrossSection::CrossSectionDirections getCurrentDirection() const
        { return current_direction; }


        /**
        * Method to move the main cross-section to a new depth
        * @param depth_ the depth to where the cross-section should be moved
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void moveMainCrossSection( double depth_ );


        /**
        * Method to return a shared pointer to the main cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return CrossSectionPtr a shared pointer to the main cross-section
        */
        const CrossSectionPtr& getMainCrossSection() const;


        /**
        * Method to create the top view cross-section.
        * The top view cross-section can be only in the 'HEIGHT' Sdirection
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void createTopViewCrossSection();


        /**
        * Method to return a shared pointer to the top view cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return CrossSectionPtr a shared pointer to the top view cross-section
        */
        const CrossSectionPtr& getTopViewCrossSection() const;


        /**
        * Method to move the top view cross-section to a new depth
        * @param depth_ the height to where the cross-section should be moved
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void moveTopViewCrossSection( double depth_ );


        /**
        * Method to create a fixed cross-section.
        * The fixed cross-section can be in the three directions, 'WIDTH', 'DEPTH' and 'HEIGHT'
        * @param dir_ the direction of the fixed cross-section
        * @param depth_ the depth_ of the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double  );


        /**
        * Method to get the fixed cross-section, given the direction and the depth of the cross-section, if existent
        * @param dir_ the direction of the fixed cross-section
        * @param depth_ the depth_ of the fixed cross-section
        * @param csection_ a shared pointer to the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return returns true if the cross-section exists and false otherwise.
        */
        bool getCrossSection( const Settings::CrossSection::CrossSectionDirections & dir_, double depth_, CrossSectionPtr& csection_ );


        /**
        * Method to remove the a fixed cross-section.
        * @param dir_ the direction of the fixed cross-section
        * @param depth_ the depth_ of the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections & dir_, double depth_ );


        /**
        * Method to set an image in a given cross-section
        * @param file_ the path of the image file
        * @param dir_ the direction of the fixed cross-section
        * @param depth_ the depth_ of the fixed cross-section
        * @param ox_ the X coordinate of the image origin
        * @param oy_ the Y coordinate of the image origin
        * @param w_ the width of the image
        * @param h_ the height of the image
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_ );


        /**
        * Method to remove an image from a given cross-section
        * @param dir_ the direction of the fixed cross-section
        * @param depth_ the depth_ of the fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to set an image in the main cross-section, if exists an image in the current depth and direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateImageInMainCrossSection();


        /**
        * Method to set an image in the top view cross-section, if exists an image in the current height
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateImageInTopViewCrossSection();


        /**
        * Method to add a new object, i.e., a stratigraphy or structural
        * @param index_ an optional parameter to define a new index to the object
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        //TODO: addObject should be protected
        bool addObject( std::size_t index_ = UNDEFINED_INDEX );


        /**
        * Method to get an object, i.e., a stratigraphy or structural
        * @param index_ the index of the object
        * @see testMeToo()
        * @see publicVar()
        * @return ObjectPtr a shared pointer to the object.
        */
        const ObjectPtr& getObject( std::size_t index_ ) const;


        /**
        * Method to get the current object
        * @see testMeToo()
        * @see publicVar()
        * @return ObjectPtr a shared pointer to the object.
        */
        const ObjectPtr& getCurrentObject() const;


        /**
        * Method to return all the objects already defined
        * @see testMeToo()
        * @see publicVar()
        * @return std::map< std::size_t, ObjectPtr > a mapping between the objects and their indexes.
        */
        const std::map< std::size_t, ObjectPtr >& getObjects();


        /**
        * Method to set a name to the given object
        * @param index_ the index of the object
        * @param name_ the new name of the object
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectName( std::size_t index_, const std::string& name_ );


        /**
        * Method to get the object name
        * @param index_ the index of the object
        * @see testMeToo()
        * @see publicVar()
        * @return std::string the name of the object
        */
        std::string getObjectName( std::size_t index_) const;


        /**
        * Method to set the type of the current object
        * In this method is only allowed two types: 'STRATIGRAPHY' and 'STRUCTURAL'.
        * @param type_ type of the current object.
        * @see Settings::Objects::ObjectType
        * @see publicVar()
        * @return Void.
        */
        void setCurrentObjectType( const Settings::Objects::ObjectType& type_ );


        /**
        * Method to get the type of the current object
        * To this method, only two types are allowed: 'STRATIGRAPHY' and 'STRUCTURAL'.
        * @see Settings::Objects::ObjectType
        * @see publicVar()
        * @return ObjectType type of the current object.
        */
        Settings::Objects::ObjectType getCurrentObjectType() const;


        /**
        * Method to get the type of the object, which index is index_
        * To this method, only two types are allowed: 'STRATIGRAPHY' and 'STRUCTURAL'.
        * @param index_ the index of the object
        * @see Settings::Objects::ObjectType
        * @see publicVar()
        * @return ObjectType type of the current object.
        */
        Settings::Objects::ObjectType getObjectType( const std::size_t& index_ ) const;


        /**
        * Method to set if the object, i.e, a stratigraphy or structural, visible or not
        * @param index_ the index of the object
        * @param status_ if the value is true the object is visible and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectVisibility( std::size_t index_, bool status_ );


        /**
        * Method to set all objects, i.e, stratigraphies and structurals, as visible or not
        * @param status_ if the value is true the objects are visibles and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectsVisibility( bool status_ );


        /**
        * Method to set the color of the object, which index is index_
        * @param index_ the index of the object
        * @param r_ the red component of the color (integer)
        * @param g_ the green component of the color (integer)
        * @param b_ the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );



        /**
        * Method to set all objects, i.e, stratigraphies and structurals, as active or not
        * @param status_ if the value is true the objects are active and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectsActive( bool status_ );


        /**
        * Method to set the object, i.e, a stratigraphy or structural, as active or not
        * @param index_ the index of the object
        * @param status_ if the value is true the object is active and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectActive( std::size_t index_, bool status_ );


        /**
        * Method to check if the object, i.e, a stratigraphy or structural, is active or not
        * @param index_ the index of the object
        * @see publicVar()
        * @return boolean returns true if the object is active and false otherwise
        */
        bool isObjectActive( std::size_t index_ ) const;


        /**
        * Method to set the object, i.e, a stratigraphy or structural, as selectable or not
        * @param index_ the index of the object
        * @param status_ if the value is true the object is selectable and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectSelectable( std::size_t index_, bool status_ );


        /**
        * Method to check if the object, i.e, a stratigraphy or structural, is selectable or not
        * @param index_ the index of the object
        * @see publicVar()
        * @return boolean return true if the object is selectable and false otherwise
        */
        bool isObjectSelectable( std::size_t index_ ) const;


        /**
        * Method to set the object, i.e, a stratigraphy or structural, as selected or not
        * @param index_ the index of the object
        * @param status_ if the value is true the object is selected and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setObjectSelected( std::size_t index_, bool status_ );


        /**
        * Method to check if the object, i.e, a stratigraphy or structural, is selected or not
        * @param index_ the index of the object
        * @see publicVar()
        * @return boolean return true if the object is selected and false otherwise
        */
        bool isObjectSelected( std::size_t index_ ) const;


        /**
        * Method to set an object log information
        * @param index_ the index of the object
        * @param status_ an object log information
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setObjectLog( std::size_t index_, const QString& status_ );


        /**
        * Method to get the object log information
        * @param index_ the index of the object
        * @see testMeToo()
        * @see publicVar()
        * @return QString object log information
        */
        QString getObjectLog( std::size_t index_ ) const;


        /**
        * Method to add a curve to the current object
        * @param dir_ the direction of the cross-section which the curve was done
        * @param depth_ the depth_ of the cross-section which the curve was done
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the curve was added properly and false otherwise
        */
        bool addCurveToObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_, const PolyCurve& curve_ );


        /**
        * Method to remove a curve to the current object
        * @param dir_ the direction of the cross-section which the curve was done
        * @param depth_ the depth_ of the cross-section which the curve was done
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the curve was removed properly and false otherwise
        */
        bool removeCurveFromObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_ );


        /**
        * Method to add a trajectory to the current object
        * @param curve_ the trajectory made by the user
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void addTrajectoryToObject( const PolyCurve& curve_ );


        /**
        * Method to add the last trajectory entered to the current object
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void addLastTrajectoryToObject();


        /**
        * Method to remove the trajectory from the current object
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removeTrajectoryFromObject();


        /**
        * This method is responsible for calling the object surface creation and for managing if ir was successfull or not.
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the object surface creation was successful and false otherwise
        */
        bool commitObjectSurface();


        /**
        * This method calls the object surface creation depends on the kind of surface will be created
        * If the object has only a cross-section curve the method createExtrudedSurface() will be called,
        * or if the object has a surface the method createLinearExtrudedSurface(), or else the method
        * createGeneralSurface() will be called
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the object surface creation was successful and false otherwise
        */
        bool createObjectSurface();


        /**
        * This method creates the object surface with the cross-sections curves added to the object
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the object surface creation was successful and false otherwise
        */
        bool createGeneralSurface();


        /**
        * This method creates the object surface using a simple extrusion of the cross-section curved added
        * to the object
        * This method will be called if the object contains only one cross-section curve
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the object surface creation was successful and false otherwise
        */
        bool createExtrudedSurface();


        /**
        * This method creates the object surface using an extrusion aloong a path, i.e., the object
        * trajectory
        * This method will be called if the object contains only one cross-section curve and a trajectory
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the object surface creation was successful and false otherwise
        */
        bool createLinearExtrudedSurface();


        /**
        * This method is responsible for updating all curves and surfaces existents in the model
        * This method is called always a change that impacts all the surfaces existents, has happened in
        * the model, e.g., after an undo/redo, create a new surface, change the cross-section direction.
        * Note that to add a curve in an object does not call the method, since the rules are only applied
        * right before the surface creation
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateModel();


        /**
        * Method to update a surface of the object, which index is index_
        * @param index_ the index of the object
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateObjectSurface( const std::size_t& index_ );


        /**
        * Method to update a surface of the object, which index is index_
        * @param index_ the index of the object
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateObjectCurves( const std::size_t& index_ );


        /**
        * Method to update the preview surface of the object, which index is index_
        * Note that, the preview surface is always of the current object
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updatePreviewSurface();


        /**
        * Method to update curve of an object in a certain cross-section
        * @param index_ the index of the object
        * @param depth_ the depth of the cross-section. Note that, it is not needed to pass the direction
        * as parameter since the object can be done in only one direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateObjectCurveInCrossSection( const std::size_t& index_, double depth_ );


        /**
        * Method to update all object curves of in a certain cross-section
        * @param depth_ the depth of the cross-section. Note that, the direction was not passed as parameter,
        * then the direction considered is the current
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateObjectsCurvesInCrossSection( double depth_ );


        /**
        * Method to update all object curves in all X direction fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateCrossSectionsX();


        /**
        * Method to update all object curves in all Y direction fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateCrossSectionsY();


        /**
        * Method to update all object curves in all Z direction fixed cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateCrossSectionsZ();


        /**
        * This method calls the method clearAndSetCurveinCrossSectionFromRulesProcessor(), using the current
        * direction as the cross-section direction
        * @param index_ the index of the object
        * @param depth_ the depth of the cross-section.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_ , double depth_ );


        /**
        * Method to clear all curves from the objects, which are already done, and retrieve the new curves
        * from the rules processor in a given cross-section.
        * Note that this method only works for 'WIDHT'and 'DEPTH' direction, since the curves from the
        * height direction objects should not be removed
        * @param index_ the index of the object
        * @param dir_ the direction of the cross-section
        * @param depth_ the depth of the cross-section.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to get the order of all stratigraphies/structurals, actives or not
        * @see testMeToo()
        * @see publicVar()
        * @return std::vector<std::size_t > a vector of indexes of the stratigraphies and structurals
        * sorted  using the 'natural' order
        */
        std::vector<std::size_t > getOrderedSurfacesIndices();


        /**
        * Method to get the order of all active stratigraphies/structurals
        * @see testMeToo()
        * @see publicVar()
        * @return std::vector<std::size_t > a vector of indexes of the stratigraphies and structurals
        * sorted  using the 'natural' order
        */
        std::vector<std::size_t > getOrderedActiveSurfacesIndices();


        /**
        * Method in which the regions are defined
        * @see testMeToo()
        * @see publicVar()
        * @return std::vector<std::size_t > a vector of the regions indexes
        */
        std::vector<std::size_t > defineRegions();


        /**
        * Method to set all regions as visible or not
        * @param status_ if the value is true the regions are visibles and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionsVisible(bool status_);


        /**
        * Method to set if the region as visible or not
        * @param index_ the index of the region
        * @param status_ if the value is true the region is visible and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionVisible(std::size_t index_, bool status_);


        /**
        * Method to check if the region is visible or not
        * @param index_ the index of the object
        * @see publicVar()
        * @return boolean returns true if the object is visible and false otherwise
        */
        bool isRegionVisible(std::size_t index_) const;


        /**
        * Method to set the color of the region, which index is index_
        * @param index_ the index of the region
        * @param r_ the red component of the color (integer)
        * @param g_ the green component of the color (integer)
        * @param b_ the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRegionColor( std::size_t index_, int r_, int g_, int b_ );


        /**
        * Method to get the color of the region, which index is index_
        * @param index_ the index of the region
        * @param r_ a reference to the red component of the color (integer)
        * @param g_ a reference to the green component of the color (integer)
        * @param b_ a reference to the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getRegionColor( std::size_t index_, int& r_, int& g_, int& b_ ) const ;


        /**
        * Method to set all regions as active or not
        * @param status_ if the value is true the regions are active and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionsActive(bool status_);


        /**
        * Method to set the region as active or not
        * @param index_ the index of the region
        * @param status_ if the value is true the region is active and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionActive(std::size_t index_, bool status_);


        /**
        * Method to check if the region is active or not
        * @param index_ the index of the region
        * @see publicVar()
        * @return boolean returns true if the region is active and false otherwise
        */
        bool isRegionActive(std::size_t index_) const;


        /**
        * Method to set the region as selectable or not
        * @param index_ the index of the region
        * @param status_ if the value is true the region is selectable and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionSelectable(std::size_t index_, bool status_);


        /**
        * Method to check if the region is selectable or not
        * @param index_ the index of the region
        * @see publicVar()
        * @return boolean return true if the region is selectable and false otherwise
        */
        bool isRegionSelectable(std::size_t index_) const;


        /**
        * Method to set the region as selected or not
        * @param index_ the index of the region
        * @param status_ if the value is true the region is selected and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setRegionSelected(std::size_t index_, bool status_);


        /**
        * Method to check if the region is selected or not
        * @param index_ the index of the region
        * @see publicVar()
        * @return boolean return true if the region is selected and false otherwise
        */
        bool isRegionSelected(std::size_t index_) const;


        /**
        * Method to return the volume of the region
        * @param id_ the index of the region
        * @see testMeToo()
        * @see publicVar()
        * @return double the volume of the region
        */
        inline double getRegionVolume( std::size_t id_ )
        {
            if( model.regions.find( id_ ) == model.regions.end() ) return 0;
            return model.regions[ id_]->getVolume();
        }


       /**
        * Method to return all the regions defined
        * @see testMeToo()
        * @see publicVar()
        * @return std::map< std::size_t, RegionsPtr > a mapping between the regions and their indexes.
        */
        const std::map< std::size_t, RegionsPtr >& getRegions() const;


        /**
        * Method to get the 2d mesh of the region which index is index_
        * @param index_ index of the region
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns truf if the region exists and false otherwise
        */
        bool getRegionCrossSectionBoundary( std::size_t index_ );


        /**
        * Method to update all the regions, i.e., update the area defined by the region
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateRegions();


        /**
        * Method to remove all the regions
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removeRegions();


        /**
        * Method to create a domain. Also, it is possible to create a domain from a given set of regions.
        * @param indexes_ an optional parameter to define the regions which define the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        std::size_t createDomain( std::set<std::size_t> indexes_ = std::set< std::size_t >() );


        /**
        * Method to add a region into an existent domain
        * @param region_id_ the index of the region to be added into the domain
        * @param domain_id_ the index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        bool addRegionToDomain(std::size_t region_id_, std::size_t domain_id_);


        /**
        * Method to remove a region from an existent domain
        * @param region_id_ the index of the region to be removed from the domain
        * @param domain_id_ the index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        bool removeRegionFromDomain(std::size_t region_id_, std::size_t domain_id_);


       /**
        * Method to get all the regions from a specific domain
        * @param domain_id_ the index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return std::set<std::size_t> a vector of index of the regions contained in
        * the domain
        */
        std::set<std::size_t> getRegionsFromDomain(std::size_t domain_id_) const;


        /**
        * Method to delete a domain
        * @param domain_id_ the index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void removeDomain(std::size_t domain_id_);


        /**
        * Method to return the volume of the domain, i.e, the sum of the volume of the regions contained
        * in the domain
        * @param id_ the index of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return double the volume of the domain
        */
        inline double getDomainVolume( std::size_t id_ )
        {
            if( model.domains.find( id_ ) == model.domains.end() ) return 0;
            return model.domains[ id_].getDomainVolume();
        }


        /**
        * Method to set a name to the given domain
        * @param index_ the index of the domain
        * @param name_ the new name of the domain
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDomainName( std::size_t index_, const std::string& name_ );


        /**
        * Method to set the color of the domain, which index is index_
        * @param index_ the index of the domain
        * @param r_ the red component of the color (integer)
        * @param g_ the green component of the color (integer)
        * @param b_ the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setDomainColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method to get the color of the domain, which index is index_
        * @param index_ the index of the domain
        * @param r_ a reference to the red component of the color (integer)
        * @param g_ a reference to the green component of the color (integer)
        * @param b_ a reference to the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getDomainColor( std::size_t domain_id_, int &red_, int &green_, int& blue_ );


        /**
        * Method to send the domains to Flow Diagnostics module
        * This method verifies if all regions belongs to a domain, if one of them no belongs to any domain,
        * a domain is created to contain it.
        * @see testMeToo()
        * @see publicVar()
        * @return std::vector< std::size_t > a vector of indexes of the domains
        */
        std::vector< std::size_t > getDomainsToFlowDiagnostics();


        /**
        * Method to get all created domains
        * @see testMeToo()
        * @see publicVar()
        * @return std::vector< std::size_t > a vector of indexes of the domains
        */
        std::vector< std::size_t > getDomains();


        /**
        * Method to initialize the Rules Processor and set the initial data
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void initRulesProcessor();


        /**
        * Method to update the volume boundary in the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateBoundingBoxRulesProcessor();


        /**
        * Method to define the volume discretization
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setVolumeDiscretization();


        /**
        * Method to get the current discretization. It depends on the current direction of the cross-secion
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        std::size_t getCurrentDiscretization() const;


        /**
        * Method to get the current range that the cross-section can go through. It depends on the current direction of the cross-secion
        * @param min_ the minimum value that the depth of the cross-section can be
        * @param max_ the maximum value that the depth of the cross-section can be
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getCurrentRange( double& min_, double& max_ ) const;


        /**
        * This methods updates all objects bounding box that depends on the volume dimensions.
        * Every time the volume dimensions changes this method is called to update all the objects bounding
        * box
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateBoundingBoxInModel();


        /**
        * This is an auxiliary method to convert the depth of the cross-section in its discretization index
        * in the direction X, i.e., 'WIDTH'
        * @param value_ the depth of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return std::size_t the discretization index in the direction X
        */
        std::size_t indexCrossSectionX( double value_ ) const;


        /**
        * This is an auxiliary method to convert the depth of the cross-section in its discretization index
        * in the direction Z, i.e., 'LENGTH'
        * @param value_ the depth of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return std::size_t the discretization index in the direction Z
        */
        std::size_t indexCrossSectionZ( double value_ ) const;


        /**
        * This is an auxiliary method to set the current stratigraphic rule as 'REMOVE ABOVE'
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRemoveAbove();


        /**
        * This is an auxiliary method to set the current stratigraphic rule as 'REMOVE ABOVE INTERSECTION'
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRemoveAboveIntersection();


        /**
        * This is an auxiliary method to set the current stratigraphic rule as 'REMOVE BELOW'
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRemoveBelow();


        /**
        * This is an auxiliary method to set the current stratigraphic rule as 'REMOVE BELOW INTERSECTION'
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setRemoveBelowIntersection();


        /**
        * Method to set the current stratigraphic rule to the Rules Processor.
        * This method should be used right before creating the object surface
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void applyStratigraphicRule();


        /**
        * Method to set the 'PRESERVE ABOVE' as enabled or disabled
        * It, also, enable or disable the 'PRESERVE ABOVE' in the Rules Processor. If status_ is false,
        * it calls the method stopCreateAbove() to disable it in the Rules Processor.
        * @param status_ status_ is true if the 'PRESERVE ABOVE' is enabled and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void enablePreserveAbove( bool status_ );


        /**
        * Method to set the 'PRESERVE BELOW' as enabled or disabled
        * It, also, enable or disable the 'PRESERVE BELOW' in the Rules Processor. If status_ is false,
        * it calls the method stopCreateBelow() to disable it in the Rules Processor.
        * @param status_ status_ is true if the 'PRESERVE BELOW' is enabled and false otherwise
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void enablePreserveBelow( bool status_ );


        /**
        * Method to disable the 'PRESERVE ABOVE' in the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void stopCreateAbove();


        /**
        * Method to disable the 'PRESERVE ABOVE' in the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void stopCreateBelow();


        /**
        * Method to disable the 'PRESERVE REGION' in the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void stopCreateRegion();


        /**
        * Method to get which the 'PRESERVE' is being used currently
        * @see Settings::Objects::BoundaryRegion
        * @return BoundaryRegion the current 'PRESERVE' enabled
        */
        Settings::Objects::BoundaryRegion getCurrentBoundaryRegion() const;


        /**
        * Method to verify if the 'PRESERVE ABOVE' is active, and if so, which curve is being used as
        * the lower boundary
        * @param boundary_ the lower boundary
        * @see Settings::Objects::BoundaryRegion
        * @return boolean returns true if the 'PRESERVE ABOVE' is active, and false otherwise
        */
        bool isDefineAboveActive( PolyCurve& boundary_ );


        /**
        * Method to verify if the 'PRESERVE BELOW' is active, and if so, which curve is being used as
        * the upper boundary
        * @param boundary_ the upper boundary
        * @see Settings::Objects::BoundaryRegion
        * @return boolean returns true if the 'PRESERVE BELOW' is active, and false otherwise
        */
        bool isDefineBelowActive( PolyCurve& boundary_ );


        /**
        * Method to get the lower boundary, when the 'PRESERVE ABOVE' is active
        * @param boundary_ a reference to the lower boundary
        * @see Settings::Objects::BoundaryRegion
        * @return Void
        */
        void getLowerBoundary( PolyCurve& boundary_ );


        /**
        * Method to get the upper boundary, when the 'PRESERVE BELOW' is active
        * @param boundary_ a reference to the upper boundary
        * @see Settings::Objects::BoundaryRegion
        * @return Void
        */
        void getUpperBoundary( PolyCurve& boundary_ );


        /**
        * Method to set an object selected as a boundary
        * @param index_ the index of the selected object
        * @see none
        * @return Void
        */
        void setObjectSelectedAsBoundary( const std::size_t& index_ );


        /**
        * Method to update the boundaries curves that delimit a region, when the 'PRESERVE' operations
        * are actives
        * @param boundary_ boundary_ a reference to the boundary
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if any one of the 'PRESERVE' operations are active and false
        * otherwise
        */
        bool updateRegionBoundary( PolyCurve& boundary_ );


        /**
        * Method to select an object and its region using a selection sketch
        * @param curve_ the sketch used to mark an object as boundary
        * @param dir_ the direction of the cross-section current where the sketch was made
        * @param depth_ the depth of the cross-section current where the sketch was made
        * @param boundary_ the boundary curve resultant from the selection
        * @see none
        * @return boolean returns true if the operation was successful, and false otherwise
        */
        bool setRegionBySketchAsBoundary(const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, PolyCurve &boundary_ );


        /**
        * Method to select a region given a point inside it
        * @param px_ the X coordinate of the point
        * @param py_ the Y coordinate of the point
        * @param depth_ the depth of the cross-section current where the point was picked
        * @param dir_ the direction of the cross-section current where the point was picked
        * @see none
        * @return boolean returns true if the operation was successful, and false otherwise
        */
        bool setRegionByPointAsBoundary( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to set the mesh resolution
        * @param resolution_ define the current resolution of the meshes. It can be  'LOW', 'MEDIUM, 'HIGH'.
        * @see none
        * @return Void
        */
        void setMeshResolution( const Controller::MeshResolution& resolution_ );


        /**
        * Method to update the current color
        * @param red_ the red component of the color (integer)
        * @param green_ the green component of the color (integer)
        * @param blue_ the blue component of the color (integer)
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setCurrentColor( int red_, int green_, int blue_ );


        /**
        * Method to save the objects data and metadata in a file, using json
        * This method calls saveObjectsMetaData() method
        * @param file_ the path of the file
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void saveFile( const std::string& filename );


        /**
        * Method to save the objects and metadata in a file, using json
        * @param file_ the path of the file
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the file was saved properly and false otherwise
        */
        bool saveObjectsMetaData( const std::string& filename );


        /**
        * This method is responsible for calling the method to load a file for managing if it was successfull or not.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void loadFile( const std::string& filename, Controller::MeshResolution& resol_ );


        /**
        * Method to load objects and all data needed to start a new session.
        * If the load file no contains metadata, this method will call loadObjectNoMetaDatas(), otherwise will call
        * the method loadObjectMetaDatas()
        * @param file_ the path of the file
        * @param resol_ a reference to the mesh resolution
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void loadObjects( const std::string& filename, Controller::MeshResolution& resol_ );


        /**
        * Method to load the objects metadata
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void loadObjectMetaDatas( QFile& load_file );


        /**
        * Method to load the objects creating new metadatas for the objects
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void loadObjectNoMetaDatas();


        /**
        * Method to call the undo from the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the operation was successful and false otherwise
        */
        bool undo();


        /**
        * Method to call the redo from the Rules Processor
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the operation was successful and false otherwise
        */
        bool redo();


        /**
        * Method to verify with the Rules Processor if it is possible to perform an undo operation
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the operation is allowed and false otherwise
        */
        bool canUndo();


        /**
        * Method to verify with the Rules Processor if it is possible to perform a redo operation
        * @see testMeToo()
        * @see publicVar()
        * @return boolean returns true if the operation is allowed and false otherwise
        */
        bool canRedo();


        /**
        * Method to export the surfaces meshes to the file format Irap Grid
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void exportToIrapGrid();



        /**
        * Method to send the discretized surfaces meshes to the Flow Diagnostics module
        * @param points the vertices of the meshes
        * @param nu the number of discretization on the direction X
        * @param nv the number of discretization on the direction Y
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );


        /**
        * Method to send the surfaces meshes to the Flow Diagnostics module, including the boundary curves of each surface
        * @param triangles_meshes the data structure contained the triangle meshes
        * @param left_curves the left boundary curves of each surface
        * @param right_curves the right boundary curves of each surface
        * @param front_curves the front boundary curves of each surface
        * @param back_curves the back boundary curves of each surface
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setSurfacesMeshes( std::vector< TriangleMesh >& triangles_meshes,
                                            std::vector< CurveMesh >& left_curves,
                                            std::vector< CurveMesh >& right_curves,
                                            std::vector< CurveMesh > & front_curves,
                                            std::vector< CurveMesh >& back_curves );



        /**
        * Method to get the volumetric mesh of the regions
        * @param vertices the vertices of each region mesh
        * @param edges the edges of each region mesh
        * @param faces the faces of each region mesh
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        std::vector<int> getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces );


        /**
        * Method to clear all data and metadata existent
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear();


        //TODO: Check if both methods do the same thing
        /**
        * Method to set the point used to guide the extrusion in the guided extrusion
        * This method is not being used properly
        * @param px_ the X coordinate of the point
        * @param py_ the Y coordinate of the point
        * @param depth_ the depth of the cross-section current where the point was picked
        * @param dir_ the direction of the cross-section current where the point was picked
        * @see none
        * @return Void
        */
        void setPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to set the point used to guide the extrusion in the guided extrusion
        * This method is not being used properly
        * @param px_ the X coordinate of the point
        * @param py_ the Y coordinate of the point
        * @param depth_ the depth of the cross-section current where the point was picked
        * @param dir_ the direction of the cross-section current where the point was picked
        * @see setPointGuidedExtrusion
        * @return Void
        */
        void setGuidedExtrusion( float px_, float py_, float pz_, const PolyCurve& curve_ );


    protected:


        /**
        * @struct Model
        * @brief It is a data structure to represent the whole model.
        * @var csectionsX a mapping between the cross-sections  and its depth on the 'WIDTH'direction
        * @var csectionsY a mapping between the cross-sections  and its depth on the 'HEIGHT'direction
        * @var csectionsZ a mapping between the cross-sections  and its depth on the 'LENGTH'direction
        * @var objects a mapping between the objects, i.e., stratigraphies and structurals, and its indexes
        * @var regions a mapping between the regions and its indexes
        * @var domains1 a mapping between the domains and its indexes
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct Model
        {
            VolumePtr volume = nullptr;

            std::map< double, CrossSectionPtr > csectionsX;
            std::map< double, CrossSectionPtr > csectionsY;
            std::map< double, CrossSectionPtr > csectionsZ;

            std::map< std::size_t, ObjectPtr > objects;
            std::map< std::size_t, RegionsPtr > regions;
            std::map< std::size_t, Domain > domains;

        };


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct Color
        {

            int red = 255;
            int green = 0;
            int blue = 0;

        };

        /**
        * @struct ImageData
        * @brief It is a data structure to hold all the required information to set an image over a cross-section
        * @var file the image path file
        * @var ox the X coordinate of the origin of the image
        * @var oy the Y coordinate of the origin of the image
        * @var w the width of the image
        * @see h the heith of the image
        * @see publicVar()
        * @return Void.
        */
        struct ImageData
        {
            std::string file;
            double ox;
            double oy;
            double w;
            double h;
        };


        Model model;                                                                        /**< An instance of the struct Model to hold all the objects on the whole model */

        double csection_stepx = 1.0;                                                        /**< The discretization interval on the direction X, i.e., 'WIDTH' */

        double csection_stepz = 1.0;                                                        /**< The discretization interval on the direction Y, i.e., 'HEIGHT' */

        double csection_stepy = 1.0;                                                        /**< The discretization interval on the direction Z, i.e., 'LENGTH' */

        std::size_t current_object = 0;                                                     /**< The index of the current object */

        Settings::Objects::ObjectType current_object_type =                                 /**< The type of the current object */
                Settings::Objects::ObjectType::STRATIGRAPHY;

        RRMApplication* app = nullptr;                                                      /**< A reference to the application, which manages the main interface */

        CrossSectionPtr csection = nullptr;                                                 /**< A shared pointer to the main cross-section data structure, which holds its metadata */

        CrossSectionPtr topview = nullptr;                                                  /**< A shared pointer to the top view cross-section data structure, which holds its metadata */

        RulesProcessor rules_processor;                                                     /**< An instance of the library responsible for all curve, surface and rules processing */

        Settings::Stratigraphy::StratigraphicRules current_rule =                           /**< The current stratigraphic rule */
                Settings::Stratigraphy::DEFAULT_STRAT_RULES;

        PolyCurve last_trajectory;                                                          /**< A copy of the last trajectory added */

        std::vector< std::size_t > selectable_objects;                                      /**< A vector to hold all the objects allowed to be selected. It is used to mark the objects that
                                                                                                 can be selected as a boundary */

        Settings::Objects::BoundaryRegion boundary_region =                             /**< An enum to define which 'PRESERVE' is being used */
                Settings::Objects::BoundaryRegion::NONE ;

        std::size_t upper_index;                                                            /**< The index of the object selected as upper boundary */

        std::size_t bottom_index;                                                           /**< The index of the object selected as lower boundary */

        Settings::CrossSection::CrossSectionDirections current_direction =                  /**< The current direction of the cross-section */
                Settings::CrossSection::CrossSectionDirections::Z;

        std::map< double, ImageData > images_csectionsX;                                    /**< A mapping between the fixed cross-sections in the 'WIDTH' direction, and its images data */

        std::map< double, ImageData > images_csectionsY;                                    /**< A mapping between the fixed cross-sections in the 'HEIGTH' direction, and its images data */

        std::map< double, ImageData > images_csectionsZ;                                    /**< A mapping between the fixed cross-sections in the 'LENGHT' direction, and its images data */

        ImageData image_topview;                                                            /**< The image on the top view */

        std::set< std::size_t > regions_in_domains;                                         /**< The set of regions that belong to a domain */

        Color current_color;                                                                /**< The current color  */


};

#endif // CONTROLLER_H
