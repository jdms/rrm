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
 * @file regions.h
 * @author Clarissa C. Marques
 * @brief File containing the class Regions
 */



#ifndef REGIONS_H
#define REGIONS_H

#include <iostream>
#include <string>
#include <vector>

#include "object.h"
#include "./core/definitions/constants.hpp"


/**
 * @struct Domains
 * \brief A graphical representation to domains.
 * \var regions_set a set of regions which belongs to domain
 */
struct Domains
{
    std::set< std::size_t > regions_set;
};


/**
 *  A graphical representation to regions to save its metadata
 */

class Regions: public Object
{
    public:


        /**
        * Constructor.
        */
        Regions();


        /**
        * Copy constructor.
        * @param a const reference to another instance of Regions
        */
        Regions( const Regions& reg_ );


        /**
        * Assignment operator.
        * @param a const reference to another instance of Regions
        */
        Regions& operator=(const Regions& reg_);


        /**
        * Destructor.
        */
        ~Regions();


        /**
        * Method to set a new index to the region
        * @param id a new index to the region
        * @return void.
        */
        void setIndex( std::size_t id_ );


        /**
        * Method to return the index of the region
        * @return std::size_t the index of the region
        */
        std::size_t getIndex() const;


        /**
        * Method to return the decompose and return the tetrahedrals into triangles
        * This method is mainly used to get the triangles to draw using opengl
        * @param cells the vector of the tetrahedrals decomposed into triangles
        */
        void getTriangleCells( std::vector< std::size_t >& cells_ ) const;


        /**
        * Method to set the tetrahedrals cells of the region
        * @param cells a vector of the indexes that defines a cell for all cells
        * @return void.
        */
        void setTetrahedralCells( const std::vector< std::size_t >& cells_ );


        /**
        * Method to return the tetrahedrals cells of the region
        * @return std::vector< std::size_t > a vector of the indexes that defines a cell for all cells
        */
        void getTetrahedralCells( std::vector< std::size_t >& cells_ ) const;


        /**
        * Method to remove all cells from the structure
        * @return void.
        */
        void clearCells();


        /**
        * Method to set the vertices of the mesh of the region
        * @param vertices a vector of the vertices coordinates that belongs to the region mesh
        * @return void.
        */
        void setVertices( const std::vector< double >& vertices_  );


        /**
        * Method to get the vertices of the mesh of the region
        * @param vertices a vector of the vertices coordinates that belongs to the region mesh
        */
        void getVertices( std::vector< double >& vertices_  ) const;


        /**
        * Method to remove all the vertices from the structure
        * @return void.
        */
        void clearVertices();


        /**
        * Method to define a point inside of the region
        * @param x the X coordinate of the point
        * @param y the Y coordinate of the point
        * @param z the Z coordinate of the point
        * @return void.
        */
        void setPoint( double x_, double y_, double z_ );


        /**
        * Method to get the saved point which is inside of the region
        * @param x the reference to the X coordinate of the point
        * @param y the reference to the Y coordinate of the point
        * @param z the reference to the Z coordinate of the point
        * @return void.
        */
        void getPoint( double& x_, double& y_, double& z_ ) const;


        /**
        * Method to set a name to the region
        * @param name a new name to the region
        * @return void.
        */
        void setName( const std::string& name_ );


        /**
        * Method to return the name of the region
        * @return std::string the name of the region
        */
        std::string getName() const;


        /**
        * Method to set the lower boundary of the region
        * @param lower the lower curve which delimits the region
        * @see PolyCurve
        * @return void.
        */
        void setLowerBound( const PolyCurve& lower_ );


        /**
        * Method to set the upper boundary of the region
        * @param upper the upper curve which delimits the region
        * @see PolyCurve
        * @return void.
        */
        void setUpperBound( const PolyCurve& upper_ );


        /**
        * Method to return the lower boundary of the region
        * @see PolyCurve
        * @return PolyCurve the lower curve which delimits the region
        */
        const PolyCurve& getLowerBound() const;


        /**
        * Method to return the upper boundary of the region
        * @see PolyCurve
        * @return PolyCurve the upper curve which delimits the region
        */
        const PolyCurve& getUpperBound() const;


        /**
        * Method to set the bounding box of the region
        * @param maxx the maximum value of X
        * @param maxy the maximum value of Y
        * @param maxz the maximum value of Z
        * @param minx the minimum value of X
        * @param miny the minimum value of Y
        * @param minz the minimum value of Z
        * @return void.
        */
        void setMaxMin( double maxx_, double maxy_, double maxz_,
                                double minx_, double miny_, double minz_ );


        /**
        * Method to get the bounding box of the region
        * @param maxx reference to the maximum value of X
        * @param maxy reference to the maximum value of Y
        * @param maxz reference to the maximum value of Z
        * @param minx reference to the minimum value of X
        * @param miny reference to the minimum value of Y
        * @param minz reference to the minimum value of Z
        * @return void.
        */
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                                double& minx_, double& miny_, double& minz_ ) const;


        /**
        * Method to set the volume of the region
        * @param volume volume of the region
        * @return void.
        */
        void setVolume( double volume_ );


        /**
        * Method to get the volume of the region
        * @return double the volume of the region
        */
        double getVolume() const;


        /**
        * Method to set the color of the region
        * @param r the red component of the color (integer)
        * @param g the green component of the color (integer)
        * @param b the blue component of the color (integer)
        * @return void.
        */
        void setColor( int r_, int g_, int b_ );


        /**
        * Method to get the color of the region
        * @param r reference to the red component of the color (integer)
        * @param g reference to the green component of the color (integer)
        * @param b reference to the blue component of the color (integer)
        * @return void.
        */
        void getColor( int& r_, int& g_, int& b_ ) const;


        /**
        * This method clear all metada from volume
        * @return void.
        */
        void clear() override;


        /**
        * This method remove all metadata from the structure and set the default values for the data.
        * @return void.
        */
        void initialize();


        /**
        * This method set the domain to which the region belongs
        * @param id the index of the domain
        * @return void.
        */
        inline void setDomain( std::size_t id_ ){ domain_index = id_; indomain = true; }


        /**
        * This method clear the data domain and define that the regions does not belong to any domain
        * @return void.
        */
        inline void removeFromDomain(){ indomain = false; }


        /**
        * This method gets the domain to which the region belongs
        * @param id the index of the domain
        * @return std::size_t the index of the domain
        */
        inline bool getDomain( std::size_t& id_ ) const { id_ = domain_index; return indomain; }


        /**
        * This method saves the metadata into a file using json
        * @param json a JSON object from Qt
        * @see QJsonObject
        * @return void.
        */
        inline virtual void write( QJsonObject& json_ ) const override
        {
            json_[ "index" ] = static_cast< int >( index );
            Object::write( json_ );
        }


        /**
        * This method reads the metadata from a file using json
        * @param json a reference to a JSON object from Qt
        * @see QJsonObject
        * @return void.
        */

        inline virtual void read( const QJsonObject& json_ ) override
        {
            Object::read( json_ );
        }



    private:

        /**
        * @struct Point
        * @brief It is a data structure to represent a point
        * @var x x coordinate of the point
        * @var y y coordinate of the point
        * @var z z coordinate of the point
        */
        struct  Point
        {
            double x;
            double y;
            double z;
        };


        /**
        * @struct Color
        * @brief It is a data structure to represent color
        * @var red red component of a color
        * @var green green component of a color
        * @var blue blue component of a color
        */
        struct Color
        {
          int r = 255;
          int g = 0;
          int b = 0;
        };


        std::size_t index;                                              /**< The index of the region */

        Point max;                                                      /**< The maximum point of the bounding box */

        Point min;                                                      /**< The minimum point of the bounding box */

        Point center;                                                   /**< The centre of the bounding box */

        bool is_visible;                                                /**< A boolean to define if the regions is visible or not */

        std::string name;                                               /**< The name of the region */

        std::vector< std::size_t > index_cells;                         /**< A vector of the vertices indexes that defines a face to each face from the region mesh*/

        std::vector< double > vertices;                                 /**< A vector of the vertices coordinates of the region mesh */

        PolyCurve lower;                                                /**< The lower curve which delimits the region */

        PolyCurve upper;                                                /**< The upper curve which delimits the region */

        std::size_t domain_index;                                       /**< The index of the domain to which the region belongs */

        bool indomain = false;                                          /**< A boolean to indicate if the region belongs to any domain */

        double volume = 0;                                              /**< The volume of the region */

        Color color;                                                    /**< The color of the region */
};

#endif // REGIONS_H
