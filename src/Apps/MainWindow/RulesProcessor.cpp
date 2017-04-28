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

#include "RulesProcessor.hpp"
//namespace RRM
//{}

RulesProcessor::RulesProcessor()
{
    modeller_.useOpenGLCoordinateSystem();
}

std::vector<std::size_t> RulesProcessor::getSurfaces()
{ 
    return modeller_.getSurfacesIndices();
}


std::size_t RulesProcessor::getWidthResolution()
{
    return modeller_.getDiscretizationWidth();
}


std::size_t RulesProcessor::getDepthResolution()
{
    return modeller_.getDiscretizationDepth();
}


//
// brief:
// Verifies whether ir is possible to define a new 'drawing' region,
// either above or below.
// Argument: `eligible_surfaces` stores the indices of surfaces that can
// be used as input either for an `defineAbove()` or a `defineBelow()`.
// Return: true if at least one elegible surface was found.
//
/* bool requestDefineRegion( std::vector<size_t> &eligible_surfaces ); */

bool RulesProcessor::requestCreateAbove( std::vector<size_t> &eligible_surfaces )
{ 
    return modeller_.requestCreateAbove(eligible_surfaces);
}

bool RulesProcessor::requestCreateBelow( std::vector<size_t> &eligible_surfaces )
{ 
    return modeller_.requestCreateBelow(eligible_surfaces);
}




/* Change the model's properties */



bool RulesProcessor::setLowResolution()
{ 
    return modeller_.tryChangeDiscretization(16, 16);
}

bool RulesProcessor::setMediumResolution()
{ 
    return modeller_.tryChangeDiscretization(64, 64);
}

bool RulesProcessor::setHighResolution()
{   
    return modeller_.tryChangeDiscretization(128, 128);
}


void RulesProcessor::setOrigin( double opengl_x, double opengl_y, double opengl_z )
{
    origin_.x = opengl_x;
    origin_.y = opengl_y;
    origin_.z = opengl_z;

    modeller_.setOrigin(opengl_x, opengl_y, opengl_z);
}

bool RulesProcessor::setLenght( double opengl_x, double opengl_y, double opengl_z )
{ 
    if ( modeller_.setLenght(opengl_x, opengl_y, opengl_z) == false )
    {
        return false;
    }

    lenght_.x = opengl_x;
    lenght_.y = opengl_y;
    lenght_.z = opengl_z;

    return true;
}


/* Begin methods to interface with GUI */


/* Clean up */
void RulesProcessor::clear()
{
    modeller_.clear();
}


/* Query or modify the automatum state */


//
// brief:
// Define new input region above surface which index is `surface_index`.
//
bool RulesProcessor::defineAbove( size_t surface_index )
{ 
    return modeller_.createAbove(surface_index); 
}


//
// brief:
// Clear any previous `defineAbove()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineAbove()
{
    return modeller_.stopCreateAbove();
}

//
// brief:
// Define new input region below surface which index is `surface_index`.
//
bool RulesProcessor::defineBelow( size_t surface_index )
{ 
    return modeller_.createBelow(surface_index); 
}

//
// brief:
// Clear any previous `defineBelow()` call.
// Safe to call anytime.
//
void RulesProcessor::stopDefineBelow()
{
    modeller_.stopCreateBelow();
}

bool RulesProcessor::defineAboveIsActive()
{ 
    return modeller_.createAboveIsActive(); 
}

bool RulesProcessor::defineBelowIsActive()
{ 
    return modeller_.createBelowIsActive();
}


void RulesProcessor::removeAbove()
{
    return modeller_.removeAbove();
}

void RulesProcessor::removeAboveIntersection()
{
    return modeller_.removeAboveIntersection();
}

void RulesProcessor::removeBelow()
{
    return modeller_.removeBelow();
}

void RulesProcessor::removeBelowIntersection()
{
    return modeller_.removeBelowIntersection();
}



bool RulesProcessor::canUndo()
{ 
    return modeller_.canUndo();
}

bool RulesProcessor::undo()
{ 
    return modeller_.undo(); 
}

bool RulesProcessor::canRedo()
{ 
    return modeller_.canRedo(); 
}

bool RulesProcessor::redo()
{ 
    return modeller_.redo(); 
}

//{} // namespace RRM


