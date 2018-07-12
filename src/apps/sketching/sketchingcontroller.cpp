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
}

void SketchingController::setController( const std::shared_ptr< Controller >& controller_ )
{
    controller = controller_;
}



void SketchingController::createMainCrossSection()
{
    main_scene = window->createMainCanvas();
    CrossSectionPtr csection_ = controller->getMainCrossSection();
    setObjectsToScene( csection_, main_scene );
}


void SketchingController::updateMainCrossSection()
{
    CrossSectionPtr csection_ = controller->getMainCrossSection();
    updateObjectsToScene( csection_, main_scene );
}



void SketchingController::viewCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{
    CrossSectionPtr csection_;
    controller->getCrossSection( dir_, depth_, csection_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        scenesX[ depth_ ] = window->addCanvas( depth_ );
        setObjectsToScene( csection_, scenesX[ depth_ ] );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        scenesY[ depth_ ] = window->addCanvas( depth_ );
        setObjectsToScene( csection_, scenesY[ depth_ ] );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
        scenesZ[ depth_ ] = window->addCanvas( depth_ );
        setObjectsToScene( csection_, scenesZ[ depth_ ] );
    }

}


void SketchingController::setObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ )
{
    scene_->setCrossSectionInformation( csection_->getDirection(), csection_->getDepth() );

    VolumePtr volume_ = csection_->getVolume();
    scene_->createVolume( volume_ );

    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        ObjectPtr& obj_ = it.second;
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    // the same for regions and wells

}


void SketchingController::updateObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ )
{
    scene_->setCrossSectionInformation( csection_->getDirection(), csection_->getDepth() );

    scene_->updateVolume();
    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        scene_->updateStratigraphy( it.first );
    }


    // the same for regions and wells

}



void SketchingController::updateObjects()
{

    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        updateStratigraphy( it.first );
    }


    // the same for regions and wells

}




void SketchingController::addStratigraphy( const std::size_t& index_ )
{
    for( auto it: scenesX )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        ObjectPtr obj_ = controller->getObject( index_ );
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    for( auto it: scenesY )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        ObjectPtr obj_ = controller->getObject( index_ );
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

    for( auto it: scenesZ )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        ObjectPtr obj_ = controller->getObject( index_ );
        scene_->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }
}


void SketchingController::updateStratigraphy( const std::size_t& index_ )
{
    main_scene->updateStratigraphy( index_ );

    for( auto it: scenesX )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphy( index_ );
    }

    for( auto it: scenesY )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphy( index_ );
    }

    for( auto it: scenesZ )
    {
        std::shared_ptr< SketchScene > scene_ = it.second;
        scene_->updateStratigraphy( index_ );
    }
}




void SketchingController::removeWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ )
{

    window->removeCanvas( depth_ );

    if( dir_ == Settings::CrossSection::CrossSectionDirections::X )
    {
        scenesX[ depth_ ].reset();
        scenesX.erase( depth_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Y )
    {
        scenesY[ depth_ ].reset();
        scenesY.erase( depth_ );
    }
    else if( dir_ == Settings::CrossSection::CrossSectionDirections::Z )
    {
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






