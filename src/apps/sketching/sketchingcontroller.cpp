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
 * @file sketchingcontroller.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class SketchingController
 */
#include "sketchingcontroller.h"

#include "sketchscene.h"
#include "sketchwindow.h"

SketchingController::SketchingController()
{
    main_scene = std::make_shared< SketchScene >();
}



void SketchingController::setMainWindow( const std::shared_ptr< SketchWindow >& window_)
{
    window = window_;
    main_scene = window->createMainCanvas();

}

void SketchingController::setTopViewWindow( const std::shared_ptr< SketchWindow >& window_)
{
    topview_window = window_;
    topview_scene = topview_window->createTopViewCanvas();

}

void SketchingController::setController( const std::shared_ptr< Controller >& controller_ )
{
    controller = controller_;
}


void SketchingController::init()
{
    createMainCrossSection();
    createTopViewCrossSection();
}


void SketchingController::enableSketching( bool status_ )
{
    main_scene->setSketchingMode( status_ );
    topview_scene->setSketchingMode( status_ );
}


void SketchingController::updateVolume()
{
    if( main_scene != nullptr )
    {
        main_scene->updateVolume();
    }
    if( topview_scene != nullptr )
    {
        topview_scene->updateVolume();
    }

}


void SketchingController::createMainCrossSection()
{
    CrossSectionPtr csection_ = controller->getMainCrossSection();
    main_scene->init();
    setObjectsToScene( csection_, main_scene );

}


void SketchingController::updateMainCrossSection()
{
    const CrossSectionPtr& csection_ = controller->getMainCrossSection();
    updateObjectsToScene( csection_, main_scene );
}


void SketchingController::createTopViewCrossSection()
{
    CrossSectionPtr csection_ = controller->getTopViewCrossSection();
    topview_scene->init();
    setObjectsToScene( csection_, topview_scene );

}


void SketchingController::updateTopViewCrossSection()
{
    const CrossSectionPtr& csection_ = controller->getTopViewCrossSection();
    updateObjectsToScene( csection_, topview_scene );
}


void SketchingController::viewCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, QColor color_ )
{
    // this method is only to fixed cross-sections
    CrossSectionPtr csection_;
    controller->getCrossSection( dir_, depth_, csection_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        scenesX[ depth_ ] = window->addCanvas( depth_, Settings::CrossSection::CrossSectionDirections::X, color_ );
        setObjectsToScene( csection_, scenesX[ depth_ ] );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        scenesY[ depth_ ] = window->addCanvas( depth_, Settings::CrossSection::CrossSectionDirections::Y, color_ );
        setObjectsToScene( csection_, scenesY[ depth_ ] );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        scenesZ[ depth_ ] = window->addCanvas( depth_, Settings::CrossSection::CrossSectionDirections::Z, color_ );
        setObjectsToScene( csection_, scenesZ[ depth_ ] );
    }

}


void SketchingController::setObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ )
{

    // this method verifies any object existent on the csection to add it on the scene
    scene_->setCrossSectionInformation( csection_->getDirection(), csection_->getDepth() );

    VolumePtr volume_ = csection_->getVolume();
    scene_->createVolume( volume_ );


    if( csection_->hasImage() == false )
    {
        std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
        scene_->removeImageInCrossSection();
    }
    else
    {
        std::string file_;
        double ox_, oy_, w_, h_;
        csection_->getImage( file_, ox_, oy_, w_, h_ );
        scene_->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
    }


    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        ObjectPtr& obj_ = it.second;
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    updateBoundary();
    // the same for regions and wells

}


void SketchingController::updateObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ )
{
    scene_->setCrossSectionInformation( csection_->getDirection(), csection_->getDepth() );

    if( scene_ == main_scene )
    {
        main_scene->updateStratigraphies();

    }
    else if( scene_ == topview_scene )
    {
        topview_scene->updateStratigraphiesTrajectories();
    }

    if( csection_->hasImage() == false )
    {
        scene_->removeImageInCrossSection();
    }
    else
    {
        std::string file_;
        double ox_, oy_, w_, h_;
        csection_->getImage( file_, ox_, oy_, w_, h_ );
        scene_->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
    }

    updateBoundary();

}


void SketchingController::updateImageInScene()
{
     CrossSectionPtr csection_ = controller->getMainCrossSection();
     if( csection_->hasImage() == false )
     {
         std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
         main_scene->removeImageInCrossSection();
     }
     else
     {
         std::string file_;
         double ox_, oy_, w_, h_;
         csection_->getImage( file_, ox_, oy_, w_, h_ );
         main_scene->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
     }


     CrossSectionPtr csection_tv_ = controller->getTopViewCrossSection();
     if( csection_tv_->hasImage() == false )
     {
         std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
         topview_scene->removeImageInCrossSection();
     }
     else
     {
         std::string file_;
         double ox_, oy_, w_, h_;
         csection_tv_->getImage( file_, ox_, oy_, w_, h_ );
         topview_scene->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
     }


     for( auto it: scenesX )
     {
         double depth_;
         Settings::CrossSection::CrossSectionDirections dir_;

         std::shared_ptr< SketchScene > scene_ = it.second;
         scene_->getCrossSectionInformation( dir_, depth_ );

         CrossSectionPtr csection1_;
         controller->getCrossSection( dir_, depth_, csection1_ );


         if( csection1_->hasImage() == false )
         {
             std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
             scene_->removeImageInCrossSection();
         }
         else
         {
             std::string file_;
             double ox_, oy_, w_, h_;
             csection1_->getImage( file_, ox_, oy_, w_, h_ );
             scene_->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
         }

     }

     for( auto it: scenesY )
     {
         double depth_;
         Settings::CrossSection::CrossSectionDirections dir_;

         std::shared_ptr< SketchScene > scene_ = it.second;
         scene_->getCrossSectionInformation( dir_, depth_ );

         CrossSectionPtr csection1_;
         controller->getCrossSection( dir_, depth_, csection1_ );


         if( csection1_->hasImage() == false )
         {
             std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
             scene_->removeImageInCrossSection();
         }
         else
         {
             std::string file_;
             double ox_, oy_, w_, h_;
             csection1_->getImage( file_, ox_, oy_, w_, h_ );
             scene_->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
         }
     }

     for( auto it: scenesZ )
     {
         double depth_;
         Settings::CrossSection::CrossSectionDirections dir_;

         std::shared_ptr< SketchScene > scene_ = it.second;
         scene_->getCrossSectionInformation( dir_, depth_ );

         CrossSectionPtr csection1_;
         controller->getCrossSection( dir_, depth_, csection1_ );


         if( csection1_->hasImage() == false )
         {
             std::cout << "Csections doesnt have image!!!" << std::endl << std::flush;
             scene_->removeImageInCrossSection();
         }
         else
         {
             std::string file_;
             double ox_, oy_, w_, h_;
             csection1_->getImage( file_, ox_, oy_, w_, h_ );
             scene_->setImageInCrossSection( file_, ox_, oy_, w_, h_ );
         }
     }


}


void SketchingController::updateObjects()
{

    if( main_scene != nullptr )
        main_scene->updateStratigraphies();

    if( topview_scene != nullptr )
        topview_scene->updateStratigraphiesTrajectories();


    for( auto it: scenesX )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphies();
    }

    for( auto it: scenesY )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphies();
    }

    for( auto it: scenesZ )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphies();
    }
//     the same for regions and wells

}


void SketchingController::updateObjectsTrajectories()
{

    if( topview_scene != nullptr )
        topview_scene->updateStratigraphiesTrajectories();

}



void SketchingController::addStratigraphy( const ObjectPtr& obj_ )
{

    // this method add a stratigraphy or structural in the main scenes and in the scenes of the fixed cross-sections
    if( main_scene != nullptr )
        main_scene->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    if( topview_scene != nullptr )
        topview_scene->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );

    for( auto it: scenesX )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    for( auto it: scenesY )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    for( auto it: scenesZ )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }
}


void SketchingController::updateStratigraphy( const std::size_t& index_ )
{


}



void SketchingController::updateBoundary()
{
    PolyCurve lboundary_, uboundary_;

    bool status_ = false;

    if( controller->isDefineAboveActive( lboundary_ ) == false )
    {
       main_scene->clearLowerBoundaryCurve();
    }
    else
    {
        main_scene->defineLowerBoundaryCurve( lboundary_ );
        status_ = true;
    }


    if( controller->isDefineBelowActive( uboundary_ ) == false )
    {
        main_scene->clearUpperBoundaryCurve();
    }
    else
    {
        main_scene->defineUpperBoundaryCurve( uboundary_ );
        status_ = true;
    }


}



void SketchingController::addRegion( const RegionsPtr& reg_  )
{
    main_scene->addRegion( reg_ );
}


void SketchingController::clearRegions()
{

    if( main_scene != nullptr )
        main_scene->clearRegions();


}


void SketchingController::updateRegions()
{

    if( main_scene != nullptr )
        main_scene->updateRegions();

}


void SketchingController::setPointGuidedExtrusionInPath( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ )
{
    if( topview_scene == nullptr ) return;
    if( main_scene == nullptr ) return;


    guided_point.px = px_;
    guided_point.py = py_;
    guided_point.depth = depth_;
    guided_point.dir = dir_;


    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
        topview_scene->updatePointGuidedExtrusion( QPointF( depth_, px_ ) );

    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
        topview_scene->updatePointGuidedExtrusion( QPointF( px_, depth_ ) );

}


void SketchingController::getGuidedExtrusionPoint( float& px_, float& py_, float& pz_ )
{
    if( topview_scene == nullptr ) return;
    if( main_scene == nullptr ) return;

    py_ = guided_point.py;

    if( guided_point.dir == Settings::CrossSection::CrossSectionDirections::X )
    {
        px_ = guided_point.depth;
        pz_ = guided_point.px;
    }
    else if( guided_point.dir == Settings::CrossSection::CrossSectionDirections::Z )
    {
        px_ = guided_point.px;
        pz_ = guided_point.depth;
    }

}


void SketchingController::removeWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

    window->removeCanvas( depth_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        scenesX[ depth_ ]->clearScene();
        scenesX[ depth_ ].reset();
        scenesX.erase( depth_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        scenesY[ depth_ ]->clearScene();
        scenesY[ depth_ ].reset();
        scenesY.erase( depth_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        scenesZ[ depth_ ]->clearScene();
        scenesZ[ depth_ ].reset();
        scenesZ.erase( depth_ );
    }
}


void SketchingController::removeWindowsDirectionX()
{

    for( auto it: scenesX )
    {
        double depth_ = it.first;
        removeWindow( Settings::CrossSection::CrossSectionDirections::X, depth_ );
    }
    scenesX.clear();

}


void SketchingController::removeWindowsDirectionY()
{

    for( auto it: scenesY )
    {
        double depth_ = it.first;
        removeWindow( Settings::CrossSection::CrossSectionDirections::Y, depth_ );
    }
    scenesY.clear();

}


void SketchingController::removeWindowsDirectionZ()
{

    for( auto it: scenesZ )
    {
        double depth_ = it.first;
        removeWindow( Settings::CrossSection::CrossSectionDirections::Z, depth_ );
    }
    scenesZ.clear();

}


void SketchingController::clear()
{

    main_scene->clearScene();
    topview_scene->clearScene();

    window->reset();
    topview_window->reset();


}



