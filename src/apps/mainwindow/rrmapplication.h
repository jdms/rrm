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

//#include "./core/widgets/pages_stack.h"
//#include "./apps/simulator/flow_window.h"
//#include "./core/widgets/objecttree.h"


class MainWindow;
class Controller;
//class FlowWindow;


class RRMApplication: public QObject
{
    Q_OBJECT

public:

        /**
        * Default constructor.
        */
        RRMApplication() = default;


        /**
        * Constructor.
        */
        RRMApplication(const RRMApplication & app_);


        RRMApplication & operator=(const RRMApplication & app_);


        /**
        * Destructor.
        */
        ~RRMApplication(){}


        /**
        * Method to save a reference to the RRM main window.
        * @param window_  main window
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setMainWindow( MainWindow* const& window_ );


        /**
        * Method to save a reference to RRM controller.
        * @param a const shared pointer to the RRM controller.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setController();


        /**
        * Method to call the controller initialization and set the default values to the main elements of the application
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void init();


    public slots:


        /**
        * Method (slot) to set a name to volume.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeName( const std::string& name_ );


        /**
        * Method (slot) to set the volume as visible/invisible
        * This method emit a signal so that the visibility is updated in all sub-components of the application
        * @param status_ boolean to indicate the visibility status of the volume. It will be visible if the value is true and invisible if false.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeVisible( bool status_ );


        /**
        * Method (slot) to update the volume dimensions
        * This method is called mainly when the user changes the volume dimensions using the Sketch application
        * @param dir_ direction of the cross-section in which the dimension was changed
        * @param width new width dimension
        * @param heihgt new height dimension
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );


        /**
        * Method (slot) to update the volume width
        * This method is called mainly when the user changes the volume width using the slider bar
        * @param width_ new width dimension
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeWidth( double width_ );


        /**
        * Method (slot) to update the volume height
        * This method is called mainly when the user changes the volume height using the slider bar
        * @param height_ new height dimension
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeHeight( double height_ );


        /**
        * Method (slot) to update the volume depth
        * This method is called mainly when the user changes the volume depth using the slider bar
        * @param lenght_ new depth dimension
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setVolumeDepth( double lenght_ );


        /**
        * Method (slot) to update the discretization of the surfaces meshes.
        * This also impacts the cross-section slider.
        * @param dir_ the current direction of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setDiscretization( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to update the resolution of the surfaces meshes.
        * @param resolution_ string to define the current resolution of the meshes. It can be  'LOW', 'MEDIUM, 'HIGH'.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setMeshResolution( const std::string& resolution_ );


        /**
        * Method (slot) to change the direction of the cross-section
        * @param dir_ represents the new direction of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ );


        /**
        * Method (slot) to move the cross-section to the depth value in the current direction.
        * @param depth_ new position of the cross-section in the current direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void moveMainCrossSection( double depth_ );


        /**
        * Method (slot) to add a new fixed cross-section to be viewed.
        * This method calls the addFixedCrossSectionWindow() from the Controller and emit a signal to the Sketch application
        * @param depth_ new position of the cross-section in the current direction
        * @param color_ the color of the marker
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void addFixedCrossSection( double depth_, QColor color_ );


        /**
        * Method (slot) to remove a fixed cross-section.
        * This method calls the removeCrossSection() from the Controller and emit a signal to the Sketch application
        * @param depth_ new position of the cross-section in the current direction
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void removeFixedCrossSection( double depth_ );


        /**
        * Method (slot) to save the image details into the given cross-section
        * This method calls the setImageToCrossSection() from the Controller and emit a signal to the Sketch application
        * @param file_ image path
        * @param dir_ direction of the cross-section
        * @param depth_ depth of the cross-section
        * @param ox_ x coordinate of the image origin
        * @param oy_ y coordinate of the image origin
        * @param w_ width of the image
        * @param h_ height of the image
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  );


        /**
        * Method (slot) to clear the image from the given cross-section
        * This method calls the clearImageInCrossSection() from the Controller and emit a signal to the Sketch application
        * @param dir_ direction of the cross-section
        * @param depth_ depth of the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to set a stratigraphy or structural visible
        * This method calls the setObjectVisibility() from the Controller and emit a signal
        * @param index_ the index of the stratigraphy or the structural
        * @param status_ If status_ is true, the stratigraphy/structural is made visible. Otherwise, the stratigraphy/structural is made invisible.
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectVisible( std::size_t index_, bool status_ );


        /**
        * Method (slot) to set a name to the stratigraphy/structural
        * This method calls the setObjectName() from the Controller
        * @param index_ the index of the stratigraphy or the structural
        * @param status_ a string which define the name of the stratigraphy/structural
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectName( std::size_t index_, const std::string& name_ );


        /**
        * Method (slot) to set a color to the stratigraphy/structural
        * This method calls the setObjectColor() from the Controller and emit a signal
        * @param index_ the index of the stratigraphy or the structural
        * @param red_ the red component of the new color
        * @param green_ the green component of the new color
        * @param blue_ the blue component of the new color
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to save a log information to the stratigraphy/structural
        * This method calls the setObjectLog() from the Controller
        * @param index_ the index of the stratigraphy or the structural
        * @param log_ a string to store the log information
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setObjectLog( std::size_t index_, const QString& log_ );


        /**
        * Method (slot) to get the log information of the stratigraphy/structural
        * This method calls the getObjectLog() from the Controller
        * @param index_ the index of the stratigraphy or the structural
        * @param log_ a reference to the string which stores the log information
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void getObjectLog( std::size_t index_, QString& log_ );


        /**
        * Method (slot) to change the type of the current object
        * This type comes from the enum 'Settings::Objects::ObjectType'
        * @param type_ a type of objects
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        void setCurrentObjectType( const Settings::Objects::ObjectType& type_ );


        /**
        * Method (slot) to add a curve to the current object, which can be a stratrigraphy or structural
        * @param curve_ a sketch (curve) made by the user
        * @param dir_ direction of the cross-section which the curve was made
        * @param depth_ depth of the cross-section which the curve was made
        * @see publicVar()
        * @return Void.
        */
        void addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to add a trajectory to the current object, which can be a stratrigraphy or structural
        * @param curve_ a sketch, i.e. the trajectory, made by the user
        * @see publicVar()
        * @return Void.
        */
        void addTrajectoryToObject( const PolyCurve& curve_ );


        /**
        * Method (slot) to remove a curve from the current object, which was made in the given cross-section
        * @param dir_ direction of the cross-section which the curve was made
        * @param depth_ depth of the cross-section which the curve was made
        * @see publicVar()
        * @return Void.
        */
        void removeLastCurve( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method (slot) to reuse the last trajectory added
        * @see publicVar()
        * @return Void.
        */
        void previewLastTrajectory();


        /**
        * Method (slot) to create the surface of the current object, from its curves
        * @see publicVar()
        * @return Void.
        */
        void createObjectSurface();


        /**
        * Method (slot) to show or hide the regions.
        * This method will compute or remove all the regions from the controller.
        * @param status_ the regions will be visible if the status value is true, and invisible otherwise.
        * @see publicVar()
        * @return Void.
        */
        void getRegions(bool status_);


        /**
        * Method (slot) to show or hide the regions.
        * This method will only set the visibility status to each region.
        * @param status_ the regions will be visible if the status value is true, and invisible otherwise.
        * @see publicVar()
        * @return Void.
        */
        void setRegionsVisible( bool status_ );


        /**
        * Method (slot) to show or hide the given region.
        * This method will only set the visibility status of the region.
        * @param index_ the index of the region
        * @param status_ the region will be visible if the status value is true, and invisible otherwise.
        * @see publicVar()
        * @return Void.
        */
        void setRegionVisible( std::size_t index_, bool status_ );


        /**
        * Method (slot) to set a name to the given region.
        * @param index_ the index of the region
        * @param name_ a string to represent the name of the region
        * @see publicVar()
        * @return Void.
        */
        void setRegionName( std::size_t index_, const std::string& name_ );


        /**
        * Method (slot) to change the color of the given region.
        * @param index_ the index of the region
        * @param red_ the red component of the new color
        * @param green_ the green component of the new color
        * @param blue_ the blue component of the new color
        * @see publicVar()
        * @return Void.
        */
        void setRegionColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to create a new domain
        * @see publicVar()
        * @return Void.
        */
        void createDomain();


        /**
        * Method (slot) to set a name to the given domain.
        * @param index_ the index of the domain
        * @param name_ a string to represent the name of the domain
        * @see publicVar()
        * @return Void.
        */
        void setDomainName( std::size_t index_, const std::string& name_ );


        /**
        * Method (slot) to change the color of the given domain.
        * @param index_ the index of the domain
        * @param red_ the red component of the new color
        * @param green_ the green component of the new color
        * @param blue_ the blue component of the new color
        * @see publicVar()
        * @return Void.
        */
        void setDomainColor( std::size_t index_, int red_, int green_, int blue_ );


        /**
        * Method (slot) to add a region to the given domain.
        * @param reg_id_ the index of the region to be added to the domain
        * @param domain_id_ tthe index of the domain
        * @see publicVar()
        * @return Void.
        */
        void addRegionToDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Method (slot) to remove a region from the given domain.
        * @param reg_id_ the index of the region to be removed
        * @param domain_id_ tthe index of the domain
        * @see publicVar()
        * @return Void.
        */
        void removeRegionFromDomain( std::size_t reg_id_, std::size_t domain_id_ );


        /**
        * Method (slot) to remove a given domain.
        * @param index_ tthe index of the domain
        * @see publicVar()
        * @return Void.
        */
        void removeDomain( std::size_t index_ );


        /**
        * Method (slot) to add a set of regions to the given domain.
        * @param domain_id_ the index of the domain
        * @param regions_ vector of regions indexes, which should be added to the domain
        * @see publicVar()
        * @return Void.
        */
        void addRegionsToDomain( std::size_t domain_id_, std::vector< std::size_t > regions_ );


        /**
        * Method (slot) to remove a set of regions from their domains
        * Each element of the vector is the index of a region, and its position in the regions vector 'regions_' corresponds to the same position of its domain in the domains vector 'domains_'.
        * The variable delete_ indicates if the domains should be deleted if there is no more regions inside it.
        * @param domain_id_ tthe index of the domain
        * @param regions_ vector of regions indexes, which should be removed form its domain
        * @see publicVar()
        * @return Void.
        */
        void removeRegionsFromDomains( const std::vector< std::size_t >& regions_, const std::vector< std::size_t >& domains_, bool delete_ = true );


        /**
        * Method (slot) to send the domains to the Flow Diagnostics application
        * @see publicVar()
        * @return std::vector< std::size_t > vector of the domains indexes.
        */
        std::vector< std::size_t > getDomainsToFlowDiagnostics() const ;


        /**
        * Method (slot) to change the current stratigraphy rule
        * @param rules_ the new rule to be applied in the current object
        * @see publicVar()
        * @return Void.
        */
        void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );


        /**
        * Method (slot) to set the 'Sketch Above' active or inactive
        * @param status_ boolean. If status_ is true, the 'Sketch Above' is active and inactive otherwise.
        * @see publicVar()
        * @return Void.
        */
        void setSketchAbove( bool status_ );


        /**
        * Method (slot) to set the 'Sketch Below' active or inactive
        * @param status_ boolean. If status_ is true, the 'Sketch Below' is active and inactive otherwise.
        * @see publicVar()
        * @return Void.
        */
        void setSketchBelow( bool status_ );


        /**
        * Method (slot) to set the 'Sketch Region' active or inactive
        * @param status_ boolean. If status_ is true, the 'Sketch Region' is active and inactive otherwise.
        * @see publicVar()
        * @return Void.
        */
        void setSketchRegion( bool status_ );


        /**
        * Method (slot) to change the current color
        * @param red_ the red component of the new color
        * @param green_ the green component of the new color
        * @param blue_ the blue component of the new color
        * @see publicVar()
        * @return Void.
        */
        void defineCurrentColor( int red_, int green_, int blue_ );


        /**
        * Method (slot) to perform a undo
        * This method emits a signal so that all scenes are updated
        * @see publicVar()
        * @return Void.
        */
        void undo();


        /**
        * Method (slot) to perform a undo
        * This method emits a signal so that all scenes are updated
        * @see publicVar()
        * @return Void.
        */
        void redo();


        /**
        * Method (slot) to check if the undo and redo are availables
        * This method emits a signal so that all scenes are updated
        * @see publicVar()
        * @return Void.
        */
        void checkUndoRedo();


        /**
        * Method (slot) to check if the status of the 'Preserve Above/Below' should be checked or not
        * @see publicVar()
        * @return Void.
        */
        void checkPreserveStatus();


        /**
        * Method (slot) to save the current session in a file which name is given by 'filename_'
        * @param filename_ name of the file to be saved
        * @see publicVar()
        * @return Void.
        */
        void save( const std::string& filename_ );


        /**
        * Method (slot) to load the session saved in the file called 'filename_'
        * @param filename_ name of the saved file
        * @see publicVar()
        * @return Void.
        */
        void load( const std::string& filename_ );


        /**
        * Method (slot) to load the object tree with all objects
        * This method is mainly called after load a new file
        * @see publicVar()
        * @return Void.
        */
        void loadObjectTree();


        /**
        * Method (slot) to update the object tree
        * @see publicVar()
        * @return Void.
        */
        void updateObjectTree();


        /**
        * Method (slot) to add the regions in the object tree
        * This method is called after loading a file
        * @see publicVar()
        * @return Void.
        */
        void loadRegions();


        /**
        * Method (slot) to add the domains in the object tree
        * This method is called after loading a file
        * @see publicVar()
        * @return Void.
        */
        void loadDomains();


        /**
        * Method (slot) to set a stratigraphy or structural as a boundary as using any of the preserves ('ABOVE' or 'BELOW')
        * @param index_ index of the stratigraphy/structural
        * @see publicVar()
        * @return Void.
        */
        void setObjectSelectedAsBoundering( const std::size_t& index_ );


        /**
        * Method (slot) to define a boundary from a selection sketch
        * @param dir_ direction of the cross-section which the selection sketch was made
        * @param depth_ depth of the cross-section which the selection sketch was made
        * @see publicVar()
        * @return Void.
        */
        void selectBounderingBySketch( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );


        /**
        * Method (slot) to select a region by picking a point inside
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param dir_ direction of the cross-section which the point was chosen
        * @param depth_ depth of the cross-section which the point was chosen
        * @see publicVar()
        * @return Void.
        */
        void getRegionByPointAsBoundering( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to define the current color randomly
        * @see publicVar()
        * @return Void.
        */
        void defineRandomColor();


        /**
        * Method (slot) to update the region in the Sketch application
        * @see publicVar()
        * @return Void.
        */
        void updateRegionBoundary();


        /**
        * Method (slot) to set a region as selected/unselected
        * @param id_ index of the region
        * @param status_ if the status_ is true the region is selected, and unselected otherwise
        * @see publicVar()
        * @return Void.
        */
        void setRegionSelected( const std::size_t& id_, bool status_ );


        /**
        * Method (slot) to set the vertical exaggeration.
        * This method emits a signal so that all the scenes can be updated with the vertical exaggeration
        * @see publicVar()
        * @return Void.
        */
        void setVerticalExaggeration( double scale_ );


        /**
        * Method (slot) to export the surfaces in Irap Grid format
        * @see publicVar()
        * @return Void.
        */
        void exportToIRAP();


        /**
        * Method (slot) to set the point of the guided extrusion to the controller
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param depth_ depth of the cross-section which the point was chosen
        * @param dir_ direction of the cross-section which the point was chosen
        * @see publicVar()
        * @return Void.
        */
        void setPointGuidedExtrusion( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method (slot) to set the point and the curve of the guided extrusion to the controller
        * This method is not being used properly
        * @param px_ x coordinate of the point
        * @param py_ y coordinate of the point
        * @param curve_ curve made by the user to the guided extrusion
        * @see publicVar()
        * @return Void.
        */
        void setGuidedExtrusion( float px_, float py_, float pz_, const PolyCurve& curve_ );


        /**
        * Method (slot) to get the legacy mesh of all surfaces in the model
        * This method is used by the Flow Diagnostics application
        * @param points points of all surfaces
        * @param nu discretization number in the x direction (width)
        * @param nv discretization number in the y direction (height)
        * @param num_extrusion_steps discretization number in the z direction (depth)
        * @see publicVar()
        * @return Void.
        */
        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );


        /**
        * Method (slot) to get the the surfaces meshes and their boundaries curves
        * This method is used by the Flow Diagnostics application
        * @param triangles_meshes the triangles meshes
        * @param left_curves discretization number in the x direction (width)
        * @param right_curves discretization number in the y direction (height)
        * @param front_curves discretization number in the z direction (depth)
        * @param back_curves discretization number in the z direction (depth)
        * @see publicVar()
        * @return Void.
        */
        void getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes,
                                                std::vector< FlowWindow::CurveMesh>& left_curves,
                                                std::vector< FlowWindow::CurveMesh >& right_curves,
                                                std::vector< FlowWindow::CurveMesh > & front_curves,
                                                std::vector< FlowWindow::CurveMesh >& back_curves );


        void getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces,
                                     std::vector< int >& regions, std::map< int, std::vector< float > >& colors_ );


        inline void removeMarkerFromSlider( double id_ ){ emit removeMarker( id_ ); }


        /**
        * Method (slot) to reset the all the application
        * @see publicVar()
        * @return Void.
        */
        void reset();


    signals:

        void addObject( const std::shared_ptr<Object>& obj_ );
        void addObjectinObjectTree( const std::shared_ptr<Object>& obj_ );

        void updateVolume();
        void defineVolumeGeometry( double ox_, double oy, double oz, double w_, double h_, double d_ );

        void updateObjects();
        void updateTrajectories();

        void updateMainCrossSection();
        void updateTopViewCrossSection();

        void changeToTopViewDirection();
        void changeToCrossSectionDirection();


        void addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void addFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_ );
        void removeFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        void addRegions();
        void updateRegions();
        void clearRegions();


        void updateDiscretization( const std::size_t& disc_ );
        void updateRange( double min_, double max_, bool inverted_ = true );

        void startApplication();
        void resetApplication();

        void disableVolumeResizing();
        void enableVolumeResizing();

        void lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
        void unlockDirections();

        void selectEnabled( const std::string& option_, bool status_ = true );

        void enablePreserve( const std::string& option_, bool status_ = true );

        void setCurrentColor( int r_, int g_, int b_ );

        void setCurveAsBoundering( const PolyCurve& boundary_ );
        void clearBounderingArea();

        void addRegionCrossSectionBoundary( const std::shared_ptr<Regions>& reg_ );

        void updateBoundary();
        void setVerticalExaggerationScale( double scale_ );

        void updateImageInCrossSection();

        void removeMarker( double id_ );

    protected:


        MainWindow* window;                                                         /**< Main Window */

        Controller* controller;                                                     /**< Controller of the main application */

};



#endif // RRMAPPLICATION_H
