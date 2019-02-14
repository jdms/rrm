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
 * @file domain.cpp
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class Domain
 */


#include "domain.h"

Domain::Domain()
{
    //  initialize();
}


Domain::Domain( const Domain& dmn_ )
{
    this->index_regions = dmn_.index_regions;
}


Domain& Domain::operator =( const Domain& dmn_ )
{
    this->index_regions = dmn_.index_regions;
    return *this;
}


Domain::~Domain()
{
    clear();
}



void Domain::addRegion( const std::size_t& index_ )
{
    index_regions.insert( index_ );
}


void Domain::removeRegion( const std::size_t& index_ )
{
    index_regions.erase( index_ );
}


void Domain::removeRegions()
{
    index_regions.clear();
}


bool Domain::findRegion( const std::size_t& index_ ) const
{
    if( index_regions.find( index_ ) == index_regions.end() ) return false;
    return true;
}


void Domain::setRegions( const std::set< std::size_t >& regions_ )
{
    index_regions = regions_;
}



std::set< std::size_t > Domain::getRegions() const
{
    return index_regions;
}


void Domain::initialize()
{
    index = 0;

    setVisible( true );
    setColor( 255, 0, 0 );
}

void Domain::clear()
{
    volume = 0;
    index_regions.clear();
    Object::clear();
}
