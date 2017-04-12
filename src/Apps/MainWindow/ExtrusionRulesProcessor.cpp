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

#include <cmath>

#include "ExtrusionRulesProcessor.hpp"

#include "stratigraphy_utilities.hpp"

#define _RP_UNUSED(x) (void)(x) 

namespace RRM
{
    ExtrusionRulesProcessor::ExtrusionRulesProcessor()
    {
        init();
    }

    // 
    // Begin methods to interface with GUI
    // 
    void ExtrusionRulesProcessor::setOrigin( double opengl_x, double opengl_y, double opengl_z )
    {
        modeller.useOpenGLCoordinateSystem();
        modeller.setOrigin(opengl_x, opengl_y, opengl_z); 

        origin_z_ = opengl_z;
    }

    bool ExtrusionRulesProcessor::setLenght( double opengl_x, double opengl_y, double opengl_z )
    {
        modeller.useOpenGLCoordinateSystem();
        return modeller.setLenght(opengl_x, opengl_y, opengl_z); 
    }

    bool ExtrusionRulesProcessor::requestChangeDiscretizationLevel( size_t level_width, size_t level_depth )
    {
        _RP_UNUSED(level_width); _RP_UNUSED(level_depth);

        return false;
    }


    bool ExtrusionRulesProcessor::init()
    {
        modeller.useOpenGLCoordinateSystem();
        return true; 
    }

    void ExtrusionRulesProcessor::clear()
    {
        modeller.clear();
    }

    bool ExtrusionRulesProcessor::update(State s)
    {
        if ( isInitialized() == false ) { 
            return false; 
        }

        if ( s == State::UNDEFINED ) 
        {
            return false; 
        }

        switch(s)
        {
            case State::SKETCHING:
                modeller.disableGeologicRules();
                break;

            case State::RA_SKETCHING:
                modeller.removeAbove();
                break;

            case State::RAI_SKETCHING:
                modeller.removeAboveIntersection();
                break;

            case State::RB_SKETCHING:
                modeller.removeBelow();
                break;

            case State::RBI_SKETCHING:
                modeller.removeBelowIntersection();
                break;

            default:
                break;
        }

        current_state_ = s;

        return true; 
    }


    bool ExtrusionRulesProcessor::defineAbove( size_t surface_index )
    {
        return modeller.defineAbove(surface_index);
    }


    void ExtrusionRulesProcessor::stopDefineAbove()
    {
        modeller.stopDefineAbove();
    }

    bool ExtrusionRulesProcessor::defineAboveIsActive()
    {
        return modeller.defineAboveIsActive();
    }

    bool ExtrusionRulesProcessor::defineBelowIsActive()
    {
        return modeller.defineBelowIsActive();
    }

    ExtrusionRulesProcessor::State ExtrusionRulesProcessor::currentGelogicRule()
    {
        return current_state_;
    }

    bool ExtrusionRulesProcessor::defineBelow( size_t surface_index )
    {
        return modeller.defineBelow(surface_index);
    }


    void ExtrusionRulesProcessor::stopDefineBelow()
    {
        modeller.stopDefineBelow();
    }


    void ExtrusionRulesProcessor::removeAbove()
    {
        return modeller.removeAbove();
    }

    void ExtrusionRulesProcessor::removeAboveIntersection()
    {
        return modeller.removeAboveIntersection();
    }

    void ExtrusionRulesProcessor::removeBelow()
    {
        return modeller.removeBelow();
    }

    void ExtrusionRulesProcessor::removeBelowIntersection()
    {
        return modeller.removeBelowIntersection();
    } 


    bool ExtrusionRulesProcessor::canUndo() 
    {
        return modeller.canUndo();
    }

    bool ExtrusionRulesProcessor::undo()
    {
        return modeller.undo();
    }

    bool ExtrusionRulesProcessor::canRedo() 
    {
        return modeller.canRedo();
    }

    bool ExtrusionRulesProcessor::redo()
    {
        return modeller.redo();
    }


    //
    // End methods to interface with GUI
    // 
    //

    /* DEPRECATED */
    bool ExtrusionRulesProcessor::isInitialized() const
    {
        /* return modeller.isInitialized(); */
        return true;
    }

    std::vector<size_t> ExtrusionRulesProcessor::getActiveSurfaces()
    {
        return modeller.getSurfacesIndices();
    }

    bool ExtrusionRulesProcessor::requestDefineAbove( std::vector<size_t> &eligible_surfaces )
    {
        return modeller.requestDefineAbove(eligible_surfaces);
    }

    bool ExtrusionRulesProcessor::requestDefineBelow( std::vector<size_t> &eligible_surfaces )
    {
        return modeller.requestDefineBelow(eligible_surfaces);
    }

    bool ExtrusionRulesProcessor::addSurface( const std::vector<double> &curve, size_t given_index, 
            std::vector<size_t> lbound_indices, std::vector<size_t> ubound_indices )
    {
        bool status;

        if ( current_state_ == State::SKETCHING )
        {
            std::vector<size_t> intersected_surfaces; 
            status = modeller.tryInsertExtrudedSurface(given_index, curve, lbound_indices, ubound_indices, intersected_surfaces);

            if ( status == false ) {
                std::cout << "Surface: " << given_index << " was discarded. It intersected surfaces: ";
                for ( auto &s : intersected_surfaces )
                {
                    std::cout << s << " ";
                }
                std::cout << "\n\n";
            }
        }
        else
        {
            status = modeller.insertExtrudedSurface(given_index, curve, lbound_indices, ubound_indices);
        }

        return status;
    }


    size_t ExtrusionRulesProcessor::getLegacyMeshes( std::vector<double> &points, 
            std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
    {
        StratigraphyUtilities utils(modeller);
        return utils.getLegacyMeshes(points, nu, nv, num_extrusion_steps);
    }

} /* namespace RRM */
