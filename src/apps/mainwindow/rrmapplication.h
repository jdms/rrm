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
 * @file rrmapplication.h
 * @author Clarissa C. Marques
 * @brief File containing the class RRMApplication
 */



#ifndef RRMAPPLICATION_H
#define RRMAPPLICATION_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <unordered_map>
#include <random>


#include <QObject>


#include "./apps/3dview/canvas3d.h"
#include "./apps/sketching/sketchwindow.h"
#include "./apps/simulator/flow_window.h"

#include "./core/definitions/constants.hpp"
#include "./core/widgets/realfeaturedslider.h"


class MainWindow;
class Controller;


class RRMApplication: public QObject
{
    Q_OBJECT

public:

        /**
        * Default constructor.
        */
        RRMApplication() = default;


        /**
        * Copy constructor.
        * @param app a const reference to another RRMApplication
        */
        RRMApplication(const RRMApplication & app_);


        /**
        * Assignment operator.
        * @param app a const reference to another RRMApplication
        */
        RRMApplication & operator=(const RRMApplication & app_);


        /**
        * Destructor.
        */
        ~RRMApplication(){}


        /**
        * Method to save a reference to the RRM main window.
        * @param window a const reference to main window
        * @see MainWindow
        * @return void.
        */
        void setMainWindow( MainWindow* const& window_ );


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see Controller
        * @return void.
        */
        void setController();


        /**
        * Method to call the controller initialization and set the default values to the main elements of the application
        * @return void.
        */
        void init();


    public slots:


        /**
        * Method (slot) to set a name to volume.
        * @param name the new name of the volume
        * @return void.
        */
        void setVolumeName( const std::string& name_ );


        /**
        * Method (slot) to set the volume as visible/invisible
        * This method emit a signal so that the visibility is updated in all sub-components of the application
        * @param status boolean to indicate the visibility status of the volume. It will be visible if the value is true and invisible if false.
        * @return void.
        */
        void setVolumeVisible( bool status_ );


        /**
        * Method (slot) to update the volume dimensions
        * This method is called mainly when the user changes the volume dimensions using the Sketch application
        * @param dir direction of the cross-section in which the dimension was changed
        * @param width new width dimension
        * @param heihgt new height dimension
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );


        /**
        * Method (slot) to update the volume width
        * This method is called mainly when the user changes the volume width using the slider bar
        * @param width new width dimension
        * @return void.
        */
        void setVolumeWidth( double width_ );


        /**
        * Method (slot) to update the volume height
        * This method is called mainly when the user changes the volume height using the slider bar
        * @param height new height dimension
        * @return void.
        */
        void setVolumeHeight( double height_ );


        /**
        * Method (slot) to update the volume depth
        * This method is called mainly when the user changes the volume depth using the slider bar
        * @param lenght new depth dimension
        * @return void.
        */
        void setVolumeDepth( double lenght_ );


        /**
        * Method (slot) to update the discretization of the surfaces meshes.
        * This also impacts the cross-section slider.
        * @param dir the current direction of the cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setDiscretization( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to update the resolution of the surfaces meshes.
        * @param resolution string to define the current resolution of the meshes. It can be  'LOW', 'MEDIUM, 'HIGH'.
        * @return void.
        */
        void setMeshResolution( const std::string& resolution_ );


        /**
        * Method (slot) to change the direction of the cross-section
        * @param dir represents the new direction of the cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ );


        /**
        * Method (slot) to move the cross-section to the depth value in the current direction.
        * @param depth new position of the cross-section in the current direction
        * @return void
        */
        void moveMainCrossSection( double depth_ );


        /**
        * Method (slot) to add a new fixed cross-section to be viewed.
        * This method calls the addFixedCrossSectionWindow() from the Controller and emit a signal to the Sketch application
        * @param depth new position of the cross-section in the current direction
        * @param color the color of the marker
        * @see QColor
        * @return void.
        */
        void addFixedCrossSection( double depth_, QColor color_ );


        /**
        * Method (slot) to remove a fixed cross-section.
        * This method calls the removeCrossSection() from the Controller and emit a signal to the Sketch application
        * @param depth new position of the cross-section in the current direction
        * @return void.
        */
        void removeFixedCrossSection( double depth_ );


        /**
        * Method (slot) to save the image details into the given cross-section
        * This method calls the setImageToCrossSection() from the Controller and emit a signal to the Sketch application
        * @param file image path
        * @param dir direction of the cross-section
        * @param depth depth of the cross-section
        * @param ox x coordinate of the image origin
        * @param oy y coordinate of the image origin
        * @param w width of the image
        * @param h height of the image
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  );


        /**
        * Method (slot) to clear the image from the given cross-section
        * This method calls the clearImageInCrossSection() from the Controller and emit a signal to the Sketch application
        * @param dir direction of the cross-section
        * @param depth depth of the cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to set a stratigraphy or structural visible
        * This method calls the setObjectVisibility() from the Controller and emit a signal
        * @param index the index of the stratigraphy or the structural
        * @param status if status is true, the stratigraphy/structural is made visible. Otherwise, the stratigraphy/structural is made invisible.
        * @return void.
        */
        void setObjectVisible( std::size_t index_, bool status_ );


        /**
        * Method (slot) to set a name to the stratigraphy/structural
        * This method calls the setObjectName() from the Controller
        * @param index the index of the stratigraphy or the structural
        * @param status a string which define the name of the stratigraphy/structural
        * @return void.
        */
        void setObjectName( std::size_t index_, const std::string& name_ );


        /**
        * Method (slot) to set a color to the stratigraphy/structural
        * This method calls the setObjectColor() from the Controller and emit a signal
        * @param index the index of the stratigraphy or the structural
        * @param red the red component of the new color
        * @param green the green component of the new color
        * @param blue the blue component of the new color
        * @return void.
        */
        void setObjectColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to save a log information to the stratigraphy/structural
        * This method calls the setObjectLog() from the Controller
        * @param index the index of the stratigraphy or the structural
        * @param log a string to store the log information
        * @return void.
        */
        void setObjectLog( std::size_t index_, const QString& log_ );


        /**
        * Method (slot) to get the log information of the stratigraphy/structural
        * This method calls the getObjectLog() from the Controller
        * @param index the index of the stratigraphy or the structural
        * @param log a reference to the string which stores the log information
        * @see QString
        * @return void.
        */
        void getObjectLog( std::size_t index_, QString& log_ );


        /**
        * Method (slot) to change the type of the current object
        * This type comes from the enum 'Settings::Objects::ObjectType'
        * @param type a type of objects
        * @see Settings::Objects::ObjectType
        * @return void.
        */
        void setCurrentObjectType( const Settings::Objects::ObjectType& type_ );


        /**
        * Method (slot) to add a curve to the current object, which can be a stratrigraphy or structural
        * @param curve a sketch (curve) made by the user
        * @param dir direction of the cross-section which the curve was made
        * @param depth depth of the cross-section which the curve was made
        * @see PolyCurve
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to add a trajectory to the current object, which can be a stratrigraphy or structural
        * @param curve a sketch, i.e. the trajectory, made by the user
        * @see PolyCurve
        * @return void.
        */
        void addTrajectoryToObject( const PolyCurve& curve_ );


        /**
        * Method (slot) to remove a curve from the current object, which was made in the given cross-section
        * @param dir direction of the cross-section which the curve was made
        * @param depth depth of the cross-section which the curve was made
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to reuse the last trajectory added
        * @return void.
        */
        void previewLastTrajectory();


        /**
        * Method (slot) to create the surface of the current object, from its curves
        * @return void.
        */
        void createObjectSurface();


        /**
        * Method (slot) to show or hide the regions.
        * This method will compute or remove all the regions from the controller.
        * @param status the regions will be visible if the status value is true, and invisible otherwise.
        * @return void.
        */
        void getRegions(bool status_);


        /**
        * Method (slot) to show or hide the regions.
        * This method will only set the visibility status to each region.
        * @param status the regions will be visible if the status value is true, and invisible otherwise.
        * @return void.
        */
        void setRegionsVisible( bool status_ );


        /**
        * Method (slot) to show or hide the given region.
        * This method will only set the visibility status of the region.
        * @param index the index of the region
        * @param status the region will be visible if the status value is true, and invisible otherwise.
        * @return void.
        */
        void setRegionVisible( std::size_t index_, bool status_ );


        /**
        * Method (slot) to set a name to the given region.
        * @param std::size_t the index of the region
        * @param std::string a string to represent the name of the region
        * @return void.
        */
        void setRegionName( std::size_t , const std::string&  );


        /**
        * Method (slot) to change the color of the given region.
        * @param index the index of the region
        * @param red the red component of the new color
        * @param green the green component of the new color
        * @param blue the blue component of the new color
        * @return void.
        */
        void setRegionColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to create a new domain
        * @return void.
        */
        void createDomain();


        /**
        * Method (slot) to set a name to the given domain.
        * @param index the index of the domain
        * @param name a string to represent the name of the domain
        * @return void.
        */
        void setDomainName( std::size_t index_, const std::string& name_ );


        /**
        * Method (slot) to change the color of the given domain.
        * @param index the index of the domain
        * @param red the red component of the new color
        * @param green the green component of the new color
        * @param blue the blue component of the new color
        * @return void.
        */
        void setDomainColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to add a region to the given domain.
        * @param reg_id the index of the region to be added to the domain
        * @param domain_id tthe index of the domain
        * @return void.
        */
        void addRegionToDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Method (slot) to remove a region from the given domain.
        * @param reg_id the index of the region to be removed
        * @param domain_id tthe index of the domain
        * @return void.
        */
        void removeRegionFromDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Method (slot) to remove a given domain.
        * @param index tthe index of the domain
        * @return void.
        */
        void removeDomain( std::size_t index_ );


        /**
        * Method (slot) to add a set of regions to the given domain.
        * @param domain_id the index of the domain
        * @param regions vector of regions indexes, which should be added to the domain
        * @return void.
        */
        void addRegionsToDomain( std::size_t domain_id_, std::vector< std::size_t > regions_ );


        /**
        * Method (slot) to remove a set of regions from their domains
        * Each element of the vector is the index of a region, and its position in the regions vector 'regions' corresponds to the same position of its domain in the domains vector 'domains_'.
        * The variable delete indicates if the domains should be deleted if there is no more regions inside it.
        * @param domain_id tthe index of the domain
        * @param regions vector of regions indexes, which should be removed form its domain
        * @param delete indicates if the domains should be deleted if there is no more regions inside it.
        * @return void.
        */
        void removeRegionsFromDomains( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_, bool delete_ = true );


        /**
        * Method (slot) to send the domains to the Flow Diagnostics application
        * @return std::vector< std::size_t > vector of the domains indexes.
        */
        std::vector< std::size_t > getDomainsToFlowDiagnostics() const ;


        /**
        * Method (slot) to change the current stratigraphy rule
        * @param rules the new rule to be applied in the current object
        * @see Settings::Stratigraphy::StratigraphicRules
        * @return void.
        */
        void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );


        /**
        * Method (slot) to set the 'Sketch Above' active or inactive
        * @param status boolean. If status is true, the 'Sketch Above' is active and inactive otherwise.
        * @return void.
        */
        void setSketchAbove( bool status_ );


        /**
        * Method (slot) to set the 'Sketch Below' active or inactive
        * @param status boolean. If status is true, the 'Sketch Below' is active and inactive otherwise.
        * @return void.
        */
        void setSketchBelow( bool status_ );


        /**
        * Method (slot) to set the 'Sketch Region' active or inactive
        * @param status boolean. If status is true, the 'Sketch Region' is active and inactive otherwise.
        * @return void.
        */
        void setSketchRegion( bool status_ );


        /**
        * Method (slot) to change the current color
        * @param red the red component of the new color
        * @param green the green component of the new color
        * @param blue the blue component of the new color
        * @return void.
        */
        void defineCurrentColor( int red_, int green_, int blue_ );


        /**
        * Method (slot) to perform a undo
        * This method emits a signal so that all scenes are updated
        * @return void.
        */
        void undo();


        /**
        * Method (slot) to perform a undo
        * This method emits a signal so that all scenes are updated
        * @return void.
        */
        void redo();


        /**
        * Method (slot) to check if the undo and redo are availables
        * This method emits a signal so that all scenes are updated
        * @return void.
        */
        void checkUndoRedo();


        /**
        * Method (slot) to check if the status of the 'Preserve Above/Below' should be checked or not
        * @return void.
        */
        void checkPreserveStatus();


        /**
        * Method (slot) to save the current session in a file which name is given by 'filename_'
        * @param filename name of the file to be saved
        * @return void.
        */
        void save( const std::string& filename_ );


        /**
        * Method (slot) to load the session saved in the file called 'filename_'
        * @param filename name of the saved file
        * @return void.
        */
        void load( const std::string& filename_ );


        /**
        * Method (slot) to load the object tree with all objects
        * This method is mainly called after load a new file
        * @return void.
        */
        void loadObjectTree();


        /**
        * Method (slot) to update the object tree
        * @return void.
        */
        void updateObjectTree();


        /**
        * Method (slot) to add the regions in the object tree
        * This method is called after loading a file
        * @return void.
        */
        void loadRegions();


        /**
        * Method (slot) to add the domains in the object tree
        * This method is called after loading a file
        * @return void.
        */
        void loadDomains();


        /**
        * Method (slot) to set a stratigraphy or structural as a boundary as using any of the preserves ('ABOVE' or 'BELOW')
        * @param index index of the stratigraphy/structural
        * @return void.
        */
        void setObjectSelectedAsBoundary( const std::size_t& index_ );


        /**
        * Method (slot) to define a boundary from a selection sketch
        * @param curve the sketch used to the selection
        * @param dir direction of the cross-section which the selection sketch was made
        * @param depth depth of the cross-section which the selection sketch was made
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void selectBoundaryBySketch( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );


        /**
        * Method (slot) to select a region by picking a point inside
        * @param px x coordinate of the point
        * @param py y coordinate of the point
        * @param depth depth of the cross-section which the point was chosen
        * @param dir direction of the cross-section which the point was chosen
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void getRegionByPointAsBoundary( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to define the current color randomly
        * @return void.
        */
        void defineRandomColor();


        /**
        * Method (slot) to update the region in the Sketch application
        * @return void.
        */
        void updateRegionBoundary();


        /**
        * Method (slot) to set a region as selected/unselected
        * @param id index of the region
        * @param status if the status is true the region is selected, and unselected otherwise
        * @return void.
        */
        void setRegionSelected( const std::size_t& id_, bool status_ );


        /**
        * Method (slot) to set the vertical exaggeration.
        * This method emits a signal so that all the scenes can be updated with the vertical exaggeration
        * @param scale the value of the vertical exaggeration
        * @return void.
        */
        void setVerticalExaggeration( double scale_ );


        /**
        * Method (slot) to export the surfaces in Irap Grid format
        * @return void.
        */
        void exportToIRAP();


        /**
        * Method (slot) to set the point of the guided extrusion to the controller
        * This method is not being used properly
        * @param px x coordinate of the point
        * @param py y coordinate of the point
        * @param depth depth of the cross-section which the point was chosen
        * @param dir direction of the cross-section which the point was chosen
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to set the point and the curve of the guided extrusion to the controller.
        * Note that the point selected was 2d, however the third coordinate was determined by the depth of the cross-section.
        * This method is not being used properly
        * @param px x coordinate of the point
        * @param py y coordinate of the point
        * @param pz z coordinate of the point
        * @param curve curve made by the user to the guided extrusion
        * @see PolyCurve
        * @return void.
        */
        void setGuidedExtrusion( float px_, float py_, float pz_, const PolyCurve& curve_ );


        /**
        * Method (slot) to get the legacy mesh of all surfaces in the model
        * This method is used by the Flow Diagnostics application
        * @param points points of all surfaces
        * @param nu discretization number in the x direction (width)
        * @param nv discretization number in the y direction (height)
        * @param num_extrusion_steps discretization number in the z direction (depth)
        * @return void.
        */
        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );


        /**
        * Method (slot) to get the surfaces meshes and their boundaries curves
        * This method is used by the Flow Diagnostics application
        * @param triangles_meshes the triangles meshes
        * @param left_curves vector of the left curve of each surface boundary
        * @param right_curves vector of the right curve of each surface boundary
        * @param front_curves vector of the front curve of each surface boundary
        * @param back_curves vector of the back curve of each surface boundary
        * @see FlowWindow::TriangleMesh
        * @see FlowWindow::CurveMesh
        * @return void.
        */
        void getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
                                                std::vector< FlowWindow::CurveMesh>& left_curves,
                                                std::vector< FlowWindow::CurveMesh >& right_curves,
                                                std::vector< FlowWindow::CurveMesh > & front_curves,
                                                std::vector< FlowWindow::CurveMesh >& back_curves );


         /**
        * Method (slot) to get the tetrahedrons from the regions volumes
        * @param vertices vector of vertices coordinates of each tetrahedron
        * @param faces vector of vertices indexes that create a face, for each cell of the region
        * @param regions vector of indexes of the domains
        * @param colors reference to a vector of the domains colors
        * @return void.
        */
        void getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& faces,
                                     std::vector< int >& regions, std::map< int, std::vector< float > >& colors_ );


         /**
        * Inline method (slot) to notify the cross-section controller to remove the marker related to a fixed cross-section removed
        * @param id it corresponds to the value of the depth of the cross-section to be removed
        * @see View3dInterface::sl_depth_csection
        * @return void.
        */
        inline void removeMarkerFromSlider( double id_ ){ emit removeMarker( id_ ); }


        /**
        * Method (slot) to reset the all the application
        * @return void.
        */
        void reset();


    signals:


        /**
        * Signal emitted to notify that a new stratigraphy/structural was added
        * @param obj a const shared pointer to the stratigraphy/structural data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved, is sent
        * @see Object
        * @return void.
        */
        void addObject( const std::shared_ptr<Object>& obj_ );


        /**
        * Signal emitted to notify that a new stratigraphy/structural should be added in the object tree
        * @param obj a const shared pointer to the stratigraphy/structural data structure, from where the geometry, visibility and others relevants
        * information to the rendering should be retrieved, is sent
        * @see Object
        * @return void.
        */
        void addObjectinObjectTree( const std::shared_ptr<Object>& obj_ );


        /**
        * Signal emitted to notify the scenes to update the volume
        * @return void.
        */
        void updateVolume();


        /**
        * Signal emitted to notify that the volume has changed its geometry
        * @param ox the x coordinate of the volume origin
        * @param oy the y coordinate of the volume origin
        * @param oz the z coordinate of the volume origin
        * @param w the width of the volume
        * @param h the height of the volume
        * @param d the depth of the volume
        * @return void.
        */
        void defineVolumeGeometry( double ox_, double oy, double oz, double w_, double h_, double d_ );


        /**
        * Signal emitted to notify the scenes to update the stratigraphies and structurals
        * @return void.
        */
        void updateObjects();


        /**
        * Signal emitted to notify the scenes to update the surfaces trajectories
        * @return void.
        */
        void updateTrajectories();


        /**
        * Signal emitted to notify that the main cross-section was changed and need to be updated in the scenes
        * @return void.
        */

        void updateMainCrossSection();


        /**
        * Signal emitted to notify that the top view cross-section was changed and need to be updated in the scenes
        * @return void.
        */
        void updateTopViewCrossSection();


        /**
        * Signal emitted to notify that the cross-section direction was changed to top view, i.e, the direction was width or depth and was changed to height
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void changeToTopViewDirection();


        /**
        * Signal emitted to notify that the cross-section direction was changed to main view, i.e, the direction was height and was changed to width or depth
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void changeToCrossSectionDirection();


        /**
        * Signal emitted to notify the scenes to add a cross-section
        * @param dir direction of the new cross-section
        * @param depth depth of the new cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Signal emitted to notify the scenes to add a fixed cross-section
        * @param dir direction of the new fixed cross-section
        * @param depth depth of the new fixed cross-section
        * @param color the color used to identify the depth of cross-section on the cross-section slider
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void addFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_ );


        /**
        * Signal emitted to notify the scenes to remove a fixed cross-section
        * @param dir direction of the fixed cross-section
        * @param depth depth of the fixed cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void removeFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Signal emitted to notify the scenes that regions are ready to be added
        * @return void.
        */
        void addRegions();


        /**
        * Signal emitted to notify the scenes to update the regions
        * @return void.
        */
        void updateRegions();


        /**
        * Signal emitted to notify the scenes to remove all regions
        * @return void.
        */
        void clearRegions();


        /**
        * Signal emitted to update the current discretization.
        * This signal is emitted mainly when the user changes the cross-section direction
        * @param disc the new discretization
        * @return void.
        */
        void updateDiscretization( const std::size_t& disc_ );


        /**
        * Signal emitted to update the the cross-section slider range
        * This signal is emitted mainly when the user changes the cross-section direction
        * @param min the minimum value of the range
        * @param max the maximum value of the range
        * @param inverted if inverted is true the appearance of the slider should be inverted anf false otherwise
        * @return void.
        */
        void updateRange( double min_, double max_, bool inverted_ = true );


        /**
        * Signal emitted to indicate that the application has started
        * @return void.
        */
        void startApplication();


        /**
        * Signal emitted to indicate that all the applications should be resetted
        * @return void.
        */
        void resetApplication();


        /**
        * Signal emitted when a curve (sketch) was added in the scene and therefore volume resizing is not allowed anymore
        * @return void.
        */
        void disableVolumeResizing();


        /**
        * Signal emitted to allow the user resize the volume
        * @return void.
        */
        void enableVolumeResizing();


        /**
        * Signal emmited to the main interface to disable the cross-section directions differents from dir_
        * This method was created because a surface only can be made in one direction per time
        * @param dir current cross-section diretion
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Signal emmited to the main interface to enable all the cross-section directions
        * @return void.
        */
        void unlockDirections();


        /**
        * Signal emmited to notify the scenes that a object is selected or not. The objects can be 'VOLUME', 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'NONE'. This latter indicates that all selection should be clear and the mode 'SKETCHING' should be activate.
        * @param option which kind of object is selected or not. They can be 'VOLUME', 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'NONE'. This latter indicates that all selection should be clear and the mode 'SKETCHING' should be activate.
        * @param status if status_ is true, the object should be selected and false otherwise
        * @return void.
        */
        void selectEnabled( const std::string& option_, bool status_ = true );


        /**
        * Signal emmited to notify the scenes that an option of preserve is active or not.
        * @param option which kind of preserve is active or not. They can be 'ABOVE', 'BELOW', OR 'REGIONS'.
        * @param status if status is true, the preserve is active and false otherwise
        * @return void..
        */
        void enablePreserve( const std::string& option_, bool status_ = true );


        /**
        * Signal emmited to update the current color
        * @param r the red component of the color (integer)
        * @param g the green component of the color (integer)
        * @param b the blue component of the color (integer)
        * @return void.
        */
        void setCurrentColor( int r_, int g_, int b_ );


        /**
        * Signal emmited to notify the Sketch appliaction to add a region in the scene
        * @param reg a const shared pointer for a data structure of Regions, which has all the geometry and others required information to draw it in the scene 2d.
        * @see Regions
        * @return void.
        */
        void addRegionCrossSectionBoundary( const std::shared_ptr<Regions>& reg_ );


        /**
        * Signal emmited to notify the Sketch appliaction to update the regions boundary
        * @return void.
        */
        void updateBoundary();


        /**
        * Signal emmited to notify all the applications to update the vertical exaggeration
        * @param scale new value of vertical exaggeration
        * @return void.
        */
        void setVerticalExaggerationScale( double scale_ );


        /**
        * Signal emmited to notify the Sketch appliaction to update the images in the cross-sections
        * @return void.
        */
        void updateImageInCrossSection();


        /**
        * Signal emmited to notify the cross-section slider to remover the marker associated to the fixed cross-section was removed
        * @param id the value of the fixed cross-section depth, which is currently the index of the marker
        * @see View3dInterface::sl_depth_csection
        * @return void.
        */
        void removeMarker( double id_ );


    protected:


        MainWindow* window;                                                         /**< Main Window */

        Controller* controller;                                                     /**< Controller of the main application */

};



#endif // RRMAPPLICATION_H
