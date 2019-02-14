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
 * @file crosssection.h
 * @author Clarissa C. Marques
 * @brief File containing the class CrossSection
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
        * Constructor. Creates a new cross-section given a volume, direction and depth of the cross-section
        */
        CrossSection( const std::shared_ptr< Volume >& volume_, const Settings::CrossSection::CrossSectionDirections& direction_, double depth_ );


        /**
        * Copy constructor.
        */
        CrossSection( const CrossSection & csection_ );


        /**
        * Assignment operator.
        */
        CrossSection & operator=( const CrossSection & csection_ );


        /**
        * Destructor.
        */
        ~CrossSection();


        /**
        * Method to update the cross-section dimensions from the volume dimensions
        * @return void.
        */
        void updateDimensions();


        /**
        * Method to set a new index to the cross-sections
        * @param id a new index to the cross-sections
        * @return void.
        */
        void setIndex( std::size_t id_ );


        /**
        * Method to return the index of the cross-sections
        * @return std::size_t the index of the cross-sections
        */
        std::size_t getIndex() const;


        /**
        * Method to set the volume to which it belongs
        * @param volume a shared pointer to the volume to which it belongs
        * @see Volume
        * @return void.
        */
        void setVolume( const std::shared_ptr< Volume >& volume_ );


        /**
        * Method to set the volume to which it belongs
        * @see Volume
        * @return std::shared_ptr< Volume > a shared pointer to the volume to which it belongs
        */
        const std::shared_ptr< Volume >& getVolume() const;


        /**
        * Method to set the direction in which the cross-section was defined
        * @param dir the direction in which the cross-section was defined
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * Method to get the direction in which the cross-section was defined
        * @see Settings::CrossSection::CrossSectionDirections
        * @return CrossSectionDirections the direction in which the cross-section was defined
        */
        Settings::CrossSection::CrossSectionDirections getDirection() const;


        /**
        * Method to set the depth in which the cross-section was defined
        * @param depth the depth in which the cross-section was defined
        * @return void.
        */
        void setDepth( double depth_ );


        /**
        * Method to get the depth in which the cross-section was defined
        * @return double the depth in which the cross-section was defined
        */
        double getDepth() const;


        /**
        * This method defines automatically the index of the stratigraphy
        * @return void.
        */
        void defineIndex();


        /**
        * Method to get the vertices coordinates which define the cross-section plane
        * @param vertices the vertices coordinates which define the cross-section plane
        * @return void.
        */
        void getCoordinates( std::vector< double >& vertices_ );


        /**
        * Method to set the bounding box of the cross-section
        * @param maxx the maximum value of X
        * @param maxy the maximum value of Y
        * @param maxz the maximum value of Z
        * @param minx the minimum value of X
        * @param miny the minimum value of Y
        * @param minz the minimum value of Z
        * @return void.
        */
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        /**
        * Method to set the image data which will appear in the cross-section
        * @param path_ the file path of the image
        * @param ox the X coordinate of the image origin
        * @param oy the Y coordinate of the image origin
        * @param x the X coordinate of the top-right of the image
        * @param y the Y coordinate of the top-right of the image
        * @return void.
        */
        void setImage( const std::string& path_, double ox_, double oy_, double x_, double y_ );


        /**
        * Method to get the image data which appears in the cross-section
        * @param path reference to the file path of the image
        * @param ox reference to the X coordinate of the image origin
        * @param oy reference to the Y coordinate of the image origin
        * @param x reference to the X coordinate of the top-right of the image
        * @param y reference to the Y coordinate of the top-right of the image
        * @return void.
        */
        void getImage( std::string& path_, double& ox_, double& oy_, double& x_, double& y_ );


        /**
        * Method to remove all image data from the cross-section
        * @return void.
        */
        void clearImage();


        /**
        * Method to return if there is any image in the cross-section
        * @return boolean return true if there is any image in the cross-section and false otherwise
        */
        bool hasImage();


        /**
        * Method to set if the cross-section is visible or invisible
        * @param status status is true if the cross-section is visible and false otherwise
        * @return void.
        */
        void setVisible( const bool status_ );


        /**
        * Method to return if the cross-section is visible
        * @return boolean return true if the cross-section is visible and false otherwise
        */
        bool isVisible() const;


        /**
        * Method to add a curve of an object in the cross-section
        * @param id the object index
        * @param curve the curve of the object
        * @see PolyCurve
        * @return boolean return true if the curve was properly added and false otherwise
        */
        bool addObject(const std::size_t& id_, PolyCurve* const& curve_  );


        /**
        * Method to return the curve of an object in the cross-section
        * @param id the object index
        * @see PolyCurve
        * @return Polycurve the curve of the object
        */
        const PolyCurve *getObjectCurve( const std::size_t& id_ );


        /**
        * Method to remove a curve of an object in the cross-section
        * @param id the object index
        * @return boolean return true if the curve was properly removed and false otherwise
        */
        bool removeObjectCurve( const std::size_t& id_ ) ;


        /**
        * Method to get all the object curves contained in the cross-section
        * @see ObjectsContainer
        * @return ObjectsContainer mapping between the index of the object and its curve
        */
        ObjectsContainer getObjects() const;


        /**
        * Method to remove all object curves from the cross-section
        * @return void.
        */
        void removeObjects();


        /**
        * Method to return if there is at least one object curve in the cross-section
        * @return boolean return true if there is at least one object curve in the cross-section, and false otherwise
        */
        bool hasObjects() const;


        /**
        * Method to set the boundary of a region in the cross-section
        * @param vupper vector of the vertices of the upper curve which delimits the region
        * @param edupper vector of the edges that defines the upper curve
        * @param vlower vector of the vertices of the lower curve which delimits the region
        * @param edlower vector of the edges that defines the lower curve
        * @return void.
        */
        void setBoundaryArea( const std::vector< float >& vupper_,  const std::vector< std::size_t >& edupper_, const std::vector< float >& vlower_,  const std::vector< std::size_t >& edlower_ );


        /**
        * Method to get the boundary of a region in the cross-section
        * @param vupper a reference to the vector of vertices of the upper curve which delimits the region
        * @param edupper a reference to the vector of edges that defines the upper curve
        * @param vlower a reference to the vector of vertices of the lower curve which delimits the region
        * @param edlowe_ a reference to the vector of edges that defines the lower curve
        * @return void.
        */
        void getBoundaryArea( std::vector< float >& vupper_,  std::vector< std::size_t >& edupper_, std::vector< float >& vlower_,  std::vector< std::size_t >& edlower_ ) const;


        /**
        * Method to clear all the vectors of vertices and edges that defines the lower and upper boundary of a region
        * @return void.
        */
        void clearBoundaryArea();


        /**
        * Method to return if there is a boundary defined in the cross-section
        * @return boolean return true if there is a boundary defined in the cross-section, or false otherwise
        */
        bool hasBoundaryArea() const;



        /**
        * Method to clear all metadata from cross-section
        * @return void.
        */
        void clear() override;


        /**
        * This method remove all metadata from the structure and set the default values for the data.
        * @return void.
        */
        void initialize();


        /**
        * This method reset the number of cross-section to zero in all application
        * @return void.
        */
        static void resetAllCrossSections();



    private:



        /**
        * @struct Point
        * @brief It is a data structure to represent a point
        * @var x x coordinate of the point
        * @var y y coordinate of the point
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
