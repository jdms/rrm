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
 * @file view3dcontroller.cpp
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class View3dController
 */


#include "view3dcontroller.h"

#include "scene3d.h"
#include "canvas3d.h"

View3dController::View3dController()
{
    scene = std::make_shared< Scene3d >();
}

void View3dController::setMainWindow( const std::shared_ptr< Canvas3d >& window_)
{
    window = window_;
    scene = window->getScene();
}


void View3dController::setController( const std::shared_ptr< Controller >& controller_ )
{
    controller = controller_;
}


void View3dController::init()
{
    setupScene();
}


void View3dController::setupScene()
{

    //TODO: probably y-plane is not showing. Fix that!
    const CrossSectionPtr& csection_ = controller->getMainCrossSection();

    VolumePtr volume_ = csection_->getVolume();
    scene->addVolume( volume_ );

    scene->addMainCrossSection( csection_ );

    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        ObjectPtr& obj_ = it.second;
        scene->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

}


void View3dController::updateVolume()
{
    scene->updateVolume();
    scene->updateMainCrossSection();
}


void View3dController::updateScene()
{
    scene->updateVolume();
    updateObjects();

}


void View3dController::updateMainCrossSection()
{
    scene->updateMainCrossSection();
}


void View3dController::setMainViewCrossSection()
{
    const CrossSectionPtr& csection_ = controller->getMainCrossSection();
    scene->changeCrossSectionDirection( csection_ );
}


void View3dController::setTopViewCrossSection()
{
    const CrossSectionPtr& csection_ = controller->getTopViewCrossSection();
    scene->changeCrossSectionDirection( csection_ );
}



void View3dController::addStratigraphy( const ObjectPtr& obj_ )
{
    scene->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    updateObjects();

}


void View3dController::updateObjects()
{
    scene->updateStratigraphies();
}


void View3dController::addRegions()
{

    std::map< std::size_t, RegionsPtr > regions_ = controller->getRegions();
    for( auto it: regions_ )
    {
        RegionsPtr& reg_ = it.second;
        scene->addRegion( reg_ );
    }
}


void View3dController::updateRegions()
{
    scene->updateRegions();
}


void View3dController::clearRegions()
{
    scene->clearRegions();
}


void View3dController::setVerticalExaggeration( double scale_ )
{
    scene->setVerticalExaggeration( scale_ );
}


void View3dController::clear()
{
    scene->clearScene();
//    window->clear();
}
