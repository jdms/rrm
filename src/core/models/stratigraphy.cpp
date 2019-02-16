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
 * @file stratigraphy.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class Stratigraphy
 */



#include "stratigraphy.h"

#include <iostream>
#include <vector>


Stratigraphy::Stratigraphy()
{
	defineId();
}

Stratigraphy::Stratigraphy(const Stratigraphy& strat_)
{
}


Stratigraphy& Stratigraphy::operator=(const Stratigraphy& strat_)
{
    return *this;
}


void Stratigraphy::setIndex(std::size_t id_ )
{
    index = id_;
    number_of_stratigraphies = id_ + 1;
}

void Stratigraphy::defineId()
{
    index = number_of_stratigraphies;
    ++number_of_stratigraphies;
}

void Stratigraphy::clear()
{
}



Stratigraphy::~Stratigraphy()
{
    clear();
}
