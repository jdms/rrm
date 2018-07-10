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

#include "./core/definitions/constants.hpp"
#include "container.h"
#include "object.h"


class Volume;
class PolyCurve;

class CrossSection: public Object
{
    public:


        using Object = std::size_t;
        using ObjectsContainer = Container< Object, PolyCurve* >;



        CrossSection();
        CrossSection( const Volume* volume_, const Settings::CrossSection::CrossSectionDirections& direction_, double depth_ );
        CrossSection( const CrossSection & csection_ );
        CrossSection & operator=( const CrossSection & csection_ );
        ~CrossSection();

///========================================================================

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setVolume( const Volume* raw_ );
        const Volume* getVolume() const;

///========================================================================


//        CrossSection();
//        CrossSection( const Volume* raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

//        CrossSection& operator=( const CrossSection& other_ );

        void defineIndex();






        void getCoordinates( std::vector< double >& vertices_ );
        void getMaxMin( double& maxx_, double& maxy_, double& maxz_,
                        double& minx_, double& miny_, double& minz_ ) const;


        void setDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
        Settings::CrossSection::CrossSectionDirections getDirection() const;


        void setDepth( double depth_ );
        double getDepth() const;


        void setImage( const std::string& path_, double ox_, double oy_, double x_, double y_ );
        void getImage( std::string& path_, double& ox_, double& oy_, double& x_, double& y_ );
        void clearImage();
        bool hasImage();

        void setVisible( const bool status_ );
        bool isVisible() const;


        bool addObject(const Object& id_, PolyCurve* const& curve_  );
        const PolyCurve *getObjectCurve( const Object& id_ );
        bool removeObjectCurve( const Object& id_ ) ;


        ObjectsContainer getObjects() const;
        void removeObjects();
        bool hasObjects() const;


        void clear();
        void initialize();

        static void resetAllCrossSections();


    private:

        struct Point
        {
            double x;
            double y;
        };

        std::size_t index;
        static std::size_t number_of_csections;

        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;
        Settings::CrossSection::CrossSectionDirections direction;

        bool is_visible;


        ObjectsContainer objects;
        const Volume* volume;

        std::string image_path;
        Point image_origin;
        Point image_top_right;



};

#endif // CROSSSECTION_H
