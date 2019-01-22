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



#ifndef VOLUME_H
#define VOLUME_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>


#include "./core/definitions/constants.hpp"
#include "./core/models/container.h"
#include "./core/models/object.h"
#include "./core/models/crosssection.h"


/**
 *  A graphical representation to the volume to save its metada
 */

class Volume: public Object
{


    public:


        /**
        * Constructor.
        */
        Volume();


        /**
        * Constructor.
        */
        Volume(const Volume & volume_);


        /**
        * Constructor.
        */
        Volume & operator=(const Volume & volume_);


        /**
        * Destructor.
        */
        ~Volume();


        /**
        * This method define the volume geometry, as its origin and dimensions
        * @param ox_ the x coordinate of the origin of the volume
        * @param oy_ the y coordinate of the origin of the volume
        * @param oz_ the z coordinate of the origin of the volume
        * @param w_ the volume width
        * @param h_ the volume height
        * @param l_ the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setGeometry( double ox_, double oy_, double oz_, double w_, double h_, double l_ );


        /**
        * This method gets the volume geometry, as its origin and dimensions
        * @param ox_ reference to the x coordinate of the origin of the volume
        * @param oy_ reference to the y coordinate of the origin of the volume
        * @param oz_ reference to the z coordinate of the origin of the volume
        * @param w_ reference to the volume width
        * @param h_ reference to the volume height
        * @param l_ reference to the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getGeometry( double& ox_, double& oy_, double& oz_, double& w_, double& h_, double& l_ ) const;


        /**
        * This method sets the origin volume
        * @param ox_ the x coordinate of the origin of the volume
        * @param oy_ the y coordinate of the origin of the volume
        * @param oz_ the z coordinate of the origin of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setOrigin( double ox_, double oy_, double oz_ );


        /**
        * This method gets the volume origin
        * @param ox_ reference to the x coordinate of the origin of the volume
        * @param oy_ reference to the y coordinate of the origin of the volume
        * @param oz_ reference to the z coordinate of the origin of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getOrigin( double& ox_, double& oy_, double& oz_ ) const;


        /**
        * This method define the volume dimensions
        * @param w_ the volume width
        * @param h_ the volume height
        * @param l_ the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setDimensions( double w_, double h_, double l_ );


        /**
        * This method gets the volume dimensions
        * @param w_ reference to the volume width
        * @param h_ reference to the volume height
        * @param l_ reference to the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getDimensions( double& w_, double& h_, double& l_ ) const;


        /**
        * This method define the volume width
        * @param w_ the volume width
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setWidth( double w_ );


        /**
        * This method return the volume width
        * @see testMeToo()
        * @see publicVar()
        * @return double the volume width
        */
        double getWidth() const;


        /**
        * This method define the volume height
        * @param h_ the volume height
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setHeight( double h_  );


        /**
        * This method gets the volume height
        * @see testMeToo()
        * @see publicVar()
        * @return double the volume height
        */
        double getHeight() const;


        /**
        * This method define the volume lenght
        * @param l_ the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void setLenght( double l_ );


        /**
        * This method gets the volume lenght
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        double getLenght() const;


        /**
        * This method gets the vertices which define the front face of the volume
        * @param vertices_ the vertices which define the front face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getFrontFace( std::vector< double >& vertices_ ) const;


        /**
        * This method gets the vertices which define the right face of the volume
        * @param vertices_ the vertices which define the right face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return double the volume lenght
        */
        void getRightFace( std::vector< double >& vertices_ ) const;


        /**
        * This method gets the vertices which define the back face of the volume
        * @param vertices_ the vertices which define the back face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getBackFace( std::vector< double >& vertices_ ) const;


        /**
        * This method gets the vertices which define the left face of the volume
        * @param vertices_ the vertices which define the left face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getLeftFace( std::vector< double >& vertices_ ) const;


        /**
        * This method gets the vertices which define the top face of the volume
        * @param vertices_ the vertices which define the top face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getTopFace( std::vector< double >& vertices_ ) const;


        /**
        * This method gets the vertices which define the bottom face of the volume
        * @param vertices_ the vertices which define the bottom face of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void getBottomFace( std::vector< double >& vertices_ ) const;


        /**
        * This method sets the total volume of the volume
        * @param volume_ the total volume of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        inline void setVolume( double volume_ ) { volume = volume_; }


        /**
        * This method returns the total volume of the volume
        * @see testMeToo()
        * @see publicVar()
        * @return double the total volume of the volume
        */
        inline double getVolume() const { return volume; }


        /**
        * This method clear all metada from volume
        * @see testMeToo()
        * @see publicVar()
        * @return Void
        */
        void clear() override;


    protected:

        const double DEFAULT_ORIGIN_X = 0.0;                                    /**< The default X coordinate of the origin */

        const double DEFAULT_ORIGIN_Y = 0.0;                                    /**< The default Y coordinate of the origin */

        const double DEFAULT_ORIGIN_Z = 0.0;                                    /**< The default Z coordinate of the origin */

        double originx = DEFAULT_ORIGIN_X;                                      /**< The current X coordinate of the origin */

        double originy = DEFAULT_ORIGIN_Y;                                      /**< The current Y coordinate of the origin */

        double originz = DEFAULT_ORIGIN_Z;                                      /**< The current Z coordinate of the origin */

        const double DEFAULT_WIDTH = 500.;                                      /**< The default width dimension of the volume */

        const double DEFAULT_HEIGHT = 500.;                                     /**< The default height dimension of the volume */

        const double DEFAULT_LENGHT = 500.;                                     /**< The default lenght dimension of the volume */

        double width = DEFAULT_WIDTH;                                           /**< The current width dimension the volume */

        double height = DEFAULT_HEIGHT;                                         /**< The current height dimension the volume */

        double lenght = DEFAULT_LENGHT;                                         /**< The current lenght dimension the volume */

        double volume = 0.0;                                                    /**< The total value */

};

#endif // VOLUME_H
