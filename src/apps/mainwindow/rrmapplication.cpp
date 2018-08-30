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


#include <QFileDialog>

#include "rrmapplication.h"
#include "mainwindow.h"

#include "./core/models/controller.h"
#include "./sketching/sketchingcontroller.h"
#include "./3dview/view3dcontroller.h"



std::size_t Object::number_of_objects = 0;
std::size_t CrossSection::number_of_csections = 0;



RRMApplication::RRMApplication(const RRMApplication & app_)
{
    this->controller = app_.controller;

}


RRMApplication& RRMApplication::operator=(const RRMApplication & app_)
{
    this->controller = app_.controller;
    return *this;
}



void RRMApplication::setMainWindow( MainWindow* const& window_ )
{
    window = window_;
    setController();
}


void RRMApplication::setController()
{
    controller = window->controller;
}


///================================================================================


void RRMApplication::init()
{

    controller->init();
    emit startApplication();

    window->object_tree->addInputVolume();
    setDiscretization( Settings::CrossSection::CrossSectionDirections::Z );

}




void RRMApplication::setVolumeName( const std::string& name_ )
{
    controller->setVolumeName( name_ );
}


void RRMApplication::setVolumeVisible( bool status_ )
{
    controller->setVolumeVisibility( status_ );
    emit updateVolume();
}


void RRMApplication::setVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ )
{
    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        controller->setVolumeLenght( width_ );
        controller->setVolumeHeight( height_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        controller->setVolumeWidth( width_ );
        controller->setVolumeLenght( height_ );
    }
    else
    {
        controller->setVolumeWidth( width_ );
        controller->setVolumeHeight( height_ );
    }

    emit updateVolume();

    double ox_, oy, oz, w_, h_, d_;
    controller->getVolumeGeometry( ox_, oy, oz, w_, h_, d_ );
    emit defineVolumeGeometry( ox_, oy, oz, w_, h_, d_ );


}


void RRMApplication::setVolumeWidth( double width_ )
{
    controller->setVolumeWidth( width_ );
    emit updateVolume();
}

void RRMApplication::setVolumeHeight( double height_ )
{
    controller->setVolumeHeight( height_ );
    emit updateVolume();
}

void RRMApplication::setVolumeDepth( double lenght_ )
{
    controller->setVolumeLenght( lenght_ );
    emit updateVolume();
}




void RRMApplication::setDiscretization( const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    bool inverted_ = ( dir_ == Settings::CrossSection::CrossSectionDirections::Y? true:false );

    emit updateDiscretization( controller->getCurrentDiscretization() );
    double min_ = 0.0, max_ = 0.0;
    controller->getCurrentRange( min_, max_ );
    emit updateRange( min_, max_, inverted_ );
}



void RRMApplication::moveMainCrossSection( double depth_ )
{
    if( controller->getMainCrossSection()->getDirection() == Settings::CrossSection::CrossSectionDirections::Y )
    {
        controller->moveTopViewCrossSection( depth_ );
        emit updateTopViewCrossSection();
        return;
    }

    controller->moveMainCrossSection( depth_ );
    emit updateMainCrossSection();

}


void RRMApplication::changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ )
{
    controller->changeMainCrossSectionDirection( dir_ );
    setDiscretization( dir_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
        emit changeToTopViewDirection();
    else
        emit changeToCrossSectionDirection();

//    emit updateObjects();
//    emit updateTrajectories();
}


void RRMApplication::addFixedCrossSection( double depth_, QColor color_ )
{
    controller->addCrossSection( controller->getMainCrossSection()->getDirection(), depth_ );
    emit addFixedCrossSectionWindow( controller->getMainCrossSection()->getDirection(), depth_/*, color_*/ );
}


void RRMApplication::removeFixedCrossSection( double depth_ )
{
    controller->removeCrossSection( controller->getMainCrossSection()->getDirection(), depth_ );
    emit removeFixedCrossSectionWindow( controller->getMainCrossSection()->getDirection(), depth_/*, color_*/ );
}


void RRMApplication::setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  )
{
    controller->setImageToCrossSection( file_, dir_, depth_, ox_, oy_, w_, h_ );
}


void RRMApplication::clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    controller->clearImageInCrossSection( dir_, depth_ );
}



void RRMApplication::setObjectVisible( std::size_t index_, bool status_ )
{
    controller->setObjectVisibility( index_, status_ );
    emit updateObjects();
}


void RRMApplication::setObjectName( std::size_t index_, const std::string& name_ )
{
    controller->setObjectName( index_, name_ );
}


void RRMApplication::setObjectColor( std::size_t index_, int red_, int green_, int blue_ )
{
    controller->setObjectColor( index_, red_, green_, blue_ );
    emit updateObjects();
}


void RRMApplication::setObjectSelectedAsBoundering( const std::size_t& index_ )
{
    controller->setObjectSelectedAsBoundering( index_ );
    emit selectEnabled( "NONE" );
}




void RRMApplication::setCurrentObjectType( const Settings::Objects::ObjectType& type_ )
{
    controller->setCurrentObjectType( type_ );
}


void RRMApplication::addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

    bool new_obj_ = controller->getCurrentObject()->isEmpty();

    bool status_ = controller->addCurveToObject( dir_, depth_, curve_ );
    if( status_ == false ) return;

    emit updateObjects();
    emit updateMainCrossSection();
    emit updateTopViewCrossSection();

    if( new_obj_ == true )
    {
        const ObjectPtr& obj_ = controller->getCurrentObject();

        int r_, g_, b_;
        obj_->getColor( r_, g_, b_ );

        window->object_tree->addObject( obj_->getIndex(), obj_->getType(), obj_->getName(), r_, g_, b_ );

        emit disableVolumeResizing();
        emit lockDirection( dir_ );
    }


    if( dir_ != Settings::CrossSection::CrossSectionDirections::Y )
        emit addCrossSection( dir_, depth_ );




}


void RRMApplication::addTrajectoryToObject( const PolyCurve& curve_ )
{
    if( controller->getMainCrossSection()->getDirection() ==
            Settings::CrossSection::CrossSectionDirections::Y )
    {
        addCurveToObject( curve_, Settings::CrossSection::CrossSectionDirections::Y, 0.0 );
//        controller->addCurveToObject( Settings::CrossSection::CrossSectionDirections::Y, 0.0, curve_ );
//        emit updateObjects();
        return;
    }

    controller->addTrajectoryToObject( curve_ );
    emit updateTrajectories();


}


void RRMApplication::createObjectSurface()
{

    bool status_ = controller->commitObjectSurface();
    if( status_ == false ) return;



    emit addObject( controller->getCurrentObject() );
    emit updateObjects();

    checkUndoRedo();


    updateObjectTree();
    defineRandomColor();

    emit unlockDirections();

}


void RRMApplication::getRegions()
{
    controller->defineRegions();

    window->object_tree->addOutputVolume();
    const std::map< std::size_t, RegionsPtr >& regions_ = controller->getRegions();
    for( auto it: regions_ )
    {
        RegionsPtr & reg_ = (it.second);

        int r_, g_, b_;
        reg_->getColor( r_, g_, b_ );
        window->object_tree->addRegion( reg_->getIndex(), reg_->getName(), r_, g_, b_ );
    }

    emit addRegions();
}


void RRMApplication::setRegionVisible( std::size_t index_, bool status_ )
{
    controller->setRegionVisible( index_, status_ );
    emit updateRegions();
}


void RRMApplication::setRegionName( std::size_t index_, const std::string& name_ )
{
//    controller->setRegionName( index_, name_ );
    emit updateRegions();
}


void RRMApplication::setRegionColor( std::size_t index_, int red_, int green_, int blue_ )
{
    controller->setRegionColor( index_, red_, green_, blue_ );
    emit updateRegions();
}



void RRMApplication::setSketchAbove( bool status_ )
{

    if( status_ == true )
    {
        bool enabled_ = controller->requestCreateAbove();
        if( enabled_ )
            emit selectEnabled( "ABOVE" );
        else
            emit selectEnabled( "NONE" );
    }
    else
    {
        controller->stopCreateAbove();
        emit selectEnabled( "NONE" );
    }

    emit updateObjects();

}


void RRMApplication::setSketchBelow( bool status_ )
{

    if( status_ == true )
    {
        bool enabled_ = controller->requestCreateBelow();
        if( enabled_ )
            emit selectEnabled( "BELOW" );
        else
            emit selectEnabled( "NONE" );
    }
    else
    {
        controller->stopCreateBelow();
        emit selectEnabled( "NONE" );
    }

    emit updateObjects();

}


void RRMApplication::setSketchRegion( bool status_ )
{

//    if( status_ == true )
//    {
//        bool enabled_ = controller->requestCreateRegion();
//        if( enabled_ )
//            emit selectEnabled( "REGION" );
//        else
//            emit selectEnabled( "NONE" );
//    }
//    else
//    {
//        controller->stopCreateRegion();
//        emit selectEnabled( "NONE" );
//    }

//    emit updateObjects();

}


void RRMApplication::getRegionByPointAsBoundering( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    controller->setRegionByPointAsBoundering( px_, py_, depth_, dir_ );
}



void RRMApplication::selectBounderingBySketch(  const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  )
{
    PolyCurve upper_, bottom_;

    bool status_ = controller->setRegionBySketchAsBoundering( curve_, dir_, depth_, upper_, bottom_ );

    if( status_ == true )
    {
//        emit
    }
    else
    {

    }

}


void RRMApplication::definedBounderingBySketch()
{
    controller->definedRegionBounderingBySketch();
    emit updateObjects();
}


void RRMApplication::setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ )
{
    if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE )
        controller->setRemoveAbove();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_ABOVE_INTERSECTION )
        controller->setRemoveAboveIntersection();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW )
        controller->setRemoveBelow();
    else if( rules_ == Settings::Stratigraphy::StratigraphicRules::REMOVE_BELOW_INTERSECTION )
        controller->setRemoveBelowIntersection();

}


void RRMApplication::reset()
{

    emit resetApplication();
    controller->clear();

    init();


}


void RRMApplication::previewLastTrajectory()
{
    controller->addLastTrajectoryToObject();
    emit updateObjects();
}


void RRMApplication::defineCurrentColor( int red_, int green_, int blue_ )
{
    controller->setCurrentColor( red_, green_, blue_ );
    emit setCurrentColor( red_, green_, blue_ );
    emit updateObjects();
}


void RRMApplication::undo()
{
    bool status_ = controller->undo();

    checkUndoRedo();
    checkPreserveStatus();

    if( status_ == false ) return;

    emit updateObjects();
    emit updateTrajectories();

    updateObjectTree();

}


void RRMApplication::redo()
{
    bool status_ = controller->redo();

    checkUndoRedo();
    checkPreserveStatus();

    if( status_ == false ) return;
    emit updateObjects();

    updateObjectTree();

}


void RRMApplication::checkUndoRedo()
{
    bool undo_ = controller->canUndo();
    bool redo_ = controller->canRedo();

    window->ac_undo->setEnabled( undo_ );
    window->ac_redo->setEnabled( redo_ );
}


void RRMApplication::checkPreserveStatus()
{
     bool above_ = controller->isDefineAboveActive();
     bool below_ = controller->isDefineBelowActive();

     window->ac_sketch_above->setChecked( above_ );
     window->ac_sketch_below->setChecked( below_ );

}


void RRMApplication::save( const std::string& filename_ )
{
    controller->saveFile( filename_ );
}


void RRMApplication::load( const std::string& filename_ )
{
    emit resetApplication();

    Controller::MeshResolution resol_;
    controller->loadFile( filename_, resol_ );

    emit startApplication();
    emit updateObjects();

    loadObjectTree();

    checkUndoRedo();
    checkPreserveStatus();

}


void RRMApplication::loadObjectTree()
{
    window->object_tree->addInputVolume();

    const std::map< std::size_t, ObjectPtr >& objects_ = controller->getObjects();

    for( auto it: objects_ )
    {
        const ObjectPtr& obj_ = it.second;
        if( obj_->getIndex() == controller->getCurrentObject()->getIndex() ) continue;

        int r_, g_, b_;
        obj_->getColor( r_, g_, b_ );
        window->object_tree->addObject( obj_->getIndex(), obj_->getType(), obj_->getName(), r_, g_, b_ );
    }
}


void RRMApplication::updateObjectTree()
{

    const std::map< std::size_t, ObjectPtr >& objects_ = controller->getObjects();

    for( auto it: objects_ )
    {
        const ObjectPtr& obj_ = it.second;
        if( obj_->getIndex() == controller->getCurrentObject()->getIndex() ) continue;

        if( obj_->isActive() == false )
            window->object_tree->setObjectVisibility( obj_->getIndex(), false );
        else
            window->object_tree->setObjectVisibility( obj_->getIndex(), true );

    }
}


void RRMApplication::defineRandomColor()
{
    std::random_device rd;
    std::mt19937 eng( rd() );
    std::uniform_int_distribution< size_t > distr( 0, 255 );

    int r_ = distr( eng );
    int g_ = distr( eng );
    int b_ = distr( eng );

   defineCurrentColor( r_, g_, b_ );

}


void RRMApplication::setMeshResolution( const std::string& resolution_ )
{
    if( resolution_.compare( "HIGH" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::HIGH );
    else if( resolution_.compare( "MEDIUM" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::MEDIUM );
    else if( resolution_.compare( "LOW" ) == 0 )
        controller->setMeshResolution( Controller::MeshResolution::LOW );

    setDiscretization( controller->getMainCrossSection()->getCrossSectionDirection() );

}
