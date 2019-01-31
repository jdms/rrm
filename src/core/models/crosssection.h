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



#ifndef CROSSSECTION_H
#define CROSSSECTION_H


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>

#include "./core/definitions/constants.hpp"
#include "container.h"
#include "object.h"


class Volume;
class PolyCurve;


/**
 *  A graphical representation to cross-sections to save its metadata
 */


class CrossSection: public Object
{

    public:


       /**
        *  \var ObjectsContainer a simplified name to mean a container of polycurves
        */
        using ObjectsContainer = Container< size_t, PolyCurve* >;


        /**
        * Constructor.
        */
        CrossSection();


        /**
        * Constructor.
        */
        CrossSection( const std::shared_ptr< Volume >& volume_, const Settings::CrossSection::CrossSectionDirections& direction_, double depth_ );


        /**
        * Constructor.
        */
        CrossSection( const CrossSection & csection_ );


        /**
        * Constructor.
        */
        CrossSection & operator=( const CrossSection & csection_ );


        /**
        * Destructor.
        */
        ~CrossSection();


        /**
        * Method to update the cross-section dimensions from the volume dimensions
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void updateDimensions();


        /**
        * Method to set a new index to the cross-sections
        * @param id_ a new index to the cross-sections
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setIndex( std::size_t id_ );


        /**
        * Method to return the index of the cross-sections
        * @see testMeToo()
        * @see publicVar()
        * @return std::size_t the index of the cross-sections
        */
        std::size_t getIndex() const;


        /**
        * Method to set the volume to which it belongs
        * @param volume_ a shared pointer to the volume to which it belongs
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setVolume( const std::shared_ptr< Volume >& volume_ );


        /**
        * Method to set the volume to which it belongs
        * @see testMeToo()
        * @see publicVar()
        * @return std::shared_ptr< Volume > a shared pointer to the volume to which it belongs
        */
        const std::shared_ptr< Volume >& getVolume() const;


        /**
        * Method to set the direction in which the cross-section was defined
        * @param dir_ the direction in which the cross-section was defined
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to get the direction in which the cross-section was defined
        * @see Settings::CrossSection::CrossSectionDirections
        * @see publicVar()
        * @return CrossSectionDirections the direction in which the cross-section was defined
        */
        Settings::CrossSection::CrossSectionDirections getDirection() const;


        /**
        * Method to set the depth in which the cross-section was defined
        * @param depth_ the depth in which the cross-section was defined
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setDepth( double depth_ );


        /**
        * Method to get the depth in which the cross-section was defined
        * @see testMeToo()
        * @see publicVar()
        * @return double the depth in which the cross-section was defined
        */
        double getDepth() const;


        /**
        * This method defines automatically the index of the stratigraphy
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void defineIndex();


        /**
        * Method to get the vertices coordinates which define the cross-section plane
        * @param vertices_ the vertices coordinates which define the cross-section plane
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getCoordinates( std::vector< double >& vertices_ );


        /**
        * Method to set the bounding box of the cross-section
        * @param maxx_ the maximum value of X
        * @param maxy_ the maximum value of Y
        * @param maxz_ the maximum value of Z
        * @param minx_ the minimum value of X
        * @param miny_ the minimum value of Y
        * @param minz_ the minimum value of Z
        * @see publicVar()
        * @return Void
        */
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        /**
        * Method to set the image data which will appear in the cross-section
        * @param path_ the file path of the image
        * @param ox_ the X coordinate of the image origin
        * @param oy_ the Y coordinate of the image origin
        * @param x_ the X coordinate of the top-right of the image
        * @param y_ the Y coordinate of the top-right of the image
        * @see publicVar()
        * @return Void
        */
        void setImage( const std::string& path_, double ox_, double oy_, double x_, double y_ );


        /**
        * Method to get the image data which appears in the cross-section
        * @param path_ reference to the file path of the image
        * @param ox_ reference to the X coordinate of the image origin
        * @param oy_ reference to the Y coordinate of the image origin
        * @param x_ reference to the X coordinate of the top-right of the image
        * @param y_ reference to the Y coordinate of the top-right of the image
        * @see publicVar()
        * @return Void
        */
        void getImage( std::string& path_, double& ox_, double& oy_, double& x_, double& y_ );


        /**
        * Method to remove all image data from the cross-section
        * @see publicVar()
        * @return Void
        */
        void clearImage();


        /**
        * Method to return if there is any image in the cross-section
        * @see publicVar()
        * @return boolean return true if there is any image in the cross-section and false otherwise
        */
        bool hasImage();


        /**
        * Method to set if the cross-section is visible or invisible
        * @param status_ status_ is true if the cross-section is visible and false otherwise
        * @see publicVar()
        * @return Void
        */
        void setVisible( const bool status_ );


        /**
        * Method to return if the cross-section is visible
        * @see publicVar()
        * @return boolean return true if the cross-section is visible and false otherwise
        */
        bool isVisible() const;


        /**
        * Method to add a curve of an object in the cross-section
        * @param id_ the object index
        * @param Polycurve the curve of the object
        * @see publicVar()
        * @return boolean return true if the curve was properly added and false otherwise
        */
        bool addObject(const std::size_t& id_, PolyCurve* const& curve_  );


        /**
        * Method to return the curve of an object in the cross-section
        * @param id_ the object index
        * @see publicVar()
        * @return Polycurve the curve of the object
        */
        const PolyCurve *getObjectCurve( const std::size_t& id_ );


        /**
        * Method to remove a curve of an object in the cross-section
        * @param id_ the object index
        * @see publicVar()
        * @return boolean return true if the curve was properly removed and false otherwise
        */
        bool removeObjectCurve( const std::size_t& id_ ) ;


        /**
        * Method to get all the object curves contained in the cross-section
        * @see ObjectsContainer()
        * @return ObjectsContainer mapping between the index of the object and its curve
        */
        ObjectsContainer getObjects() const;


        /**
        * Method to remove all object curves from the cross-section
        * @see publicVar()
        * @return Void
        */
        void removeObjects();


        /**
        * Method to return if there is at least one object curve in the cross-section
        * @see publicVar()
        * @return boolean return true if there is at least one object curve in the cross-section, and false otherwise
        */
        bool hasObjects() const;


        /**
        * Method to set the boundary of a region in the cross-section
        * @param vupper_ vector of the vertices of the upper curve which delimits the region
        * @param edupper_ vector of the edges that defines the upper curve
        * @param vlower_ vector of the vertices of the lower curve which delimits the region
        * @param edlower_ vector of the edges that defines the lower curve
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setBoundaryArea( const std::vector< float >& vupper_,  const std::vector< std::size_t >& edupper_, const std::vector< float >& vlower_,  const std::vector< std::size_t >& edlower_ );


        /**
        * Method to get the boundary of a region in the cross-section
        * @param vupper_ a reference to the vector of vertices of the upper curve which delimits the region
        * @param edupper_ a reference to the vector of edges that defines the upper curve
        * @param vlower_ a reference to the vector of vertices of the lower curve which delimits the region
        * @param edlower_ a reference to the vector of edges that defines the lower curve
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getBoundaryArea( std::vector< float >& vupper_,  std::vector< std::size_t >& edupper_, std::vector< float >& vlower_,  std::vector< std::size_t >& edlower_ ) const;


        /**
        * Method to clear all the vectors of vertices and edges that defines the lower and upper boundary of a region
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clearBoundaryArea();


        /**
        * Method to return if there is a boundary defined in the cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return boolean return true if there is a boundary defined in the cross-section, or false otherwise
        */
        bool hasBoundaryArea() const;



        /**
        * Method to clear all metadata from cross-section
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear() override;


        /**
        * This method remove all metadata from the structure and set the default values for the data.
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void initialize();


        /**
        * This method reset the number of cross-section to zero in all application
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        static void resetAllCrossSections();



    private:



        /**
        * @struct Point
        * @brief It is a data structure to represent a point
        * @var x x coordinate of the point
        * @var y y coordinate of the point
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct Point
        {
            double x;
            double y;
        };


        /**
        * @struct BoundaryArea
        * @brief It is a data structure to hold a boundary region data
        * @param vertices_upper vector of the vertices of the upper curve which delimits the region
        * @param edges_upper vector of the edges that defines the upper curve
        * @param vertices_lower vector of the vertices of the lower curve which delimits the region
        * @param edges_lower vector of the edges that defines the lower curve
        * @param empty a boolean to indicate if a boundary area is defined or not
        * @see testMeToo()
        * @see publicVar()
        * @return Void.
        */
        struct BoundaryArea
        {
            std::vector<float> vertices_upper;
            std::vector<size_t> edges_upper;
            std::vector<float> vertices_lower;
            std::vector<size_t> edges_lower;
            bool empty = true;

        } boundary;


        std::shared_ptr< Volume > volume = nullptr;                                 /**< A shared pointer to the volume to which it belongs */

        double width = 0.0;                                                         /**< The width dimension of the cross-section */

        double height = 0.0;                                                        /**< The height dimension of the cross-section */

        double depth = 0.0;                                                         /**< The depth dimension of the cross-section, the depth where the cross-section was defined */

        Settings::CrossSection::CrossSectionDirections direction;                   /**< The direction which the cross-section was defined */

        std::size_t index;                                                          /**< The index of the cross-section */

        static std::size_t number_of_csections;                                     /**< The number of cross-sections existents in the application, i.e., the number of instances
                                                                                          already created in the application */

        bool is_visible;                                                            /**< A boolean to define if the cross-section is visible or not */

        ObjectsContainer objects;                                                   /**< A container to hold the object curves */

        std::string image_path;                                                     /**< The path of the image file */

        Point image_origin;                                                         /**< A point to define the origin of the image */

        Point image_top_right;                                                      /**< A point to define the right-top corner of the image */



};

#endif // CROSSSECTION_H
