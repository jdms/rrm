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
#include <cmath>
#include <set>

#include "volume.h"
#include "object.h"


class CrossSection
{
    public:

        CrossSection()
        {
            initialize();
        }

        inline void setVolume( Volume* vol_ ){ volume = vol_; }
        inline Volume* const& getVolume(){ return volume; }


        inline void setDimensions( double width_, double height_ )
        {
            setWidth( width_ );
            setHeight( height_ );
        }

        inline void getDimensions( double& width_, double& height_ ) const
        {
            width_ = volume->getWidth();
            height_ = volume->getHeight();
        }


        inline void setWidth( double width_ )
        {
            volume->setWidth( std::fabs( width_ ) );
        }

        inline double getWidth() const
        {
            return volume->getWidth();
        }


        inline void setHeight( double height_ )
        {
            volume->setHeight( std::fabs( height_ ) );
        }

        inline double getHeight() const
        {
            return volume->getHeight();
        }


        inline void setZCoordinate( double z_ )
        {
            z_coordinate = z_;
        }

        inline double getZCoordinate() const
        {
            return z_coordinate;
        }


        inline void setObjectsReferenced( const std::set< Object* >& objects_ )
        {
            objects_referenced = objects_;
        }

        inline void addObjectReferenced( Object* const& object_ )
        {
            objects_referenced.insert( object_ );
        }


        inline const std::set< Object* >& getObjectsReferenced() const
        {
            return objects_referenced;
        }


        inline void initialize()
        {
            volume = nullptr;
            z_coordinate = 0.0f;
        }


        inline void clear()
        {
            for( auto &ob_: objects_referenced )
            {
                if( ob_ != nullptr )
                    (ob_)->clear();
            }
            objects_referenced.clear();

            if( volume != nullptr )
                volume->clear();
            volume = nullptr;

            z_coordinate = 0.0f;

        }



    protected:

        Volume* volume;
        double z_coordinate;

        std::set< Object* > objects_referenced;

};

#endif // CROSSSECTION_H
