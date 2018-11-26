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


class Volume: public Object
{


    public:

        Volume();
        Volume(const Volume & volume_);
        Volume & operator=(const Volume & volume_);
        ~Volume();


        void setGeometry( double ox_, double oy_, double oz_, double w_, double h_, double l_ );
        void getGeometry( double& ox_, double& oy_, double& oz_, double& w_, double& h_, double& l_ ) const;


        ///=================================================================

        void setOrigin( double ox_, double oy_, double oz_ );
        void getOrigin( double& ox_, double& oy_, double& oz_ ) const;

        void setDimensions( double w_, double h_, double l_ );
        void getDimensions( double& w_, double& h_, double& l_ ) const;

        void setWidth( double w_ );
        double getWidth() const;

        void setHeight( double h_  );
        double getHeight() const;

        void setLenght( double l_ );
        double getLenght() const;

        void getFrontFace( std::vector< double >& vertices_ ) const;
        void getRightFace( std::vector< double >& vertices_ ) const;
        void getBackFace( std::vector< double >& vertices_ ) const;
        void getLeftFace( std::vector< double >& vertices_ ) const;
        void getTopFace( std::vector< double >& vertices_ ) const;
        void getBottomFace( std::vector< double >& vertices_ ) const;

        inline void setVolume( double volume_ ) { volume = volume_; }
        inline double getVolume() const { return volume; }


        void clear() override;



    private:



protected:

    const double DEFAULT_ORIGIN_X = 0.0;
    const double DEFAULT_ORIGIN_Y = 0.0;
    const double DEFAULT_ORIGIN_Z = 0.0;
    double originx = DEFAULT_ORIGIN_X;
    double originy = DEFAULT_ORIGIN_Y;
    double originz = DEFAULT_ORIGIN_Z;

    const double DEFAULT_WIDTH = 500.;
    const double DEFAULT_HEIGHT = 500.;
    const double DEFAULT_LENGHT = 500.;
    double width = DEFAULT_WIDTH;
    double height = DEFAULT_HEIGHT;
    double lenght = DEFAULT_LENGHT;

    double volume = 0.0;

};

#endif // VOLUME_H
