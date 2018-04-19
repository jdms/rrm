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



#ifndef WELL_H
#define WELL_H

#include <iostream>
#include <string>
#include <map>



#include "./core/definitions/constants.hpp"
#include "./polycurve.h"


class Well
{

    enum class Type{ INJECTOR, PRODUCTOR };

    public:

        Well();

        void defineIndex();

        void setIndex( const std::size_t id_ );
        std::size_t getIndex() const;


        void setName( const std::string& name_ );
        void getName( std::string& name_ ) const;


        void setType( const Settings::Well::WellType& type_ );
        void getType( Settings::Well::WellType& type_ ) const;


        void setStartPoint( double x_, double y_, double z_ );
        void getStartPoint( double& x_, double& y_, double& z_ ) const;

        void setEndPoint( double x_, double y_, double z_ );
        void getEndPoint( double& x_, double& y_, double& z_ ) const;


        void setCurve( const PolyCurve& curve_ );
        PolyCurve getCurve() const;
        void removeCurve();


        void setVisible( const bool status_ );
        bool isVisible() const;


        void clear();
        void initialize();


    private:

        struct Point{ double x, y, z; } ;

        std::size_t index;
        std::size_t number_of_wells;

        std::string name;
        Settings::Well::WellType type;

        Point start;
        Point end;

        PolyCurve curve;

        bool is_visible;

};

#endif // WELL_H
