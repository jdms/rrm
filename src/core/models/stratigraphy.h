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
 * @file stratigraphy.h
 * @author Clarissa C. Marques
 * @brief File containing the class Stratigraphy
 */



#pragma once
#include "object.h"

#include <memory>
#include <string>
#include <map>


/**
 *  A graphical representation to the stratigraphy to save its metadata
 */

class Stratigraphy : public Object
{
    public:

        unsigned int number_of_stratigraphies;

        /**
        * Constructor.
        */
        Stratigraphy();


        /**
        * Copy constructor.
        * @param strat_ a const reference to another instance of Stratigraphy
        */
        Stratigraphy(const Stratigraphy & strat_);


        /**
        * Assignment operator.
        * @param strat_ a const reference to another instance of Stratigraphy
        */
        Stratigraphy & operator=(const Stratigraphy & strat_);


        /**
        * Destructor.
        */
        ~Stratigraphy();


        /**
        * Method to set a new index to the stratigraphy
        * @param id a new index to the stratigraphy
        * @return void.
        */
        void setIndex(std::size_t id_ ) override;


        /**
        * This method clear all metada from stratigraphy
        * @return void.
        */
        void clear() override;


    protected:


        /**
        * This method defines automatically the index of the stratigraphy
        * @return void.
        */
        void defineId();


};

