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
    setObjectsToScene( csection_, main_scene );
    main_scene->init();
}


void SketchingController::updateMainCrossSection()
{
    CrossSectionPtr csection_ = controller->getMainCrossSection();
    updateObjectsToScene( csection_, main_scene );
}


void SketchingController::createTopViewCrossSection()
{
    CrossSectionPtr csection_ = controller->getTopViewCrossSection();
    setObjectsToScene( csection_, topview_scene );
    topview_scene->init();
}


void SketchingController::updateTopViewCrossSection()
{
    CrossSectionPtr csection_ = controller->getTopViewCrossSection();
    updateObjectsToScene( csection_, topview_scene );
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
        std::cout << "Has no image!" << std::endl << std::flush;
        scene_->removeImageInCrossSection();
    }
    else
    {
        std::cout << "Has image!" << std::endl << std::flush;
        std::string file_;
        double ox_, oy_, w_, h_;
        csection_->getImage( file_, ox_, oy_, w_, h_ );
        scene_->updateImageToCrossSection( file_, ox_, oy_, w_, h_ );
    }
}




void SketchingController::updateObjects()
{

//    if( main_scene != nullptr )
//        main_scene->updateStratigraphies();

//    if( topview_scene != nullptr )
//        topview_scene->updateStratigraphies();

//    for( auto it: scenesX )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphies();
//    }

//    for( auto it: scenesY )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphies();
//    }

//    for( auto it: scenesZ )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphies();
//    }
    // the same for regions and wells

}


void SketchingController::updateObjectsTrajectories()
{

    if( topview_scene != nullptr )
        topview_scene->updateStratigraphiesTrajectories();

}





void SketchingController::addStratigraphy( const ObjectPtr& obj_ )
{
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

//    if( main_scene != nullptr )
//        main_scene->updateStratigraphy( index_ );

//    if( topview_scene != nullptr )
//        topview_scene->updateStratigraphy( index_ );

//    for( auto it: scenesX )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphy( index_ );
//    }

//    for( auto it: scenesY )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphy( index_ );
//    }

//    for( auto it: scenesZ )
//    {
//        std::shared_ptr< SketchScene > scene_ = it.second;
//        scene_->updateStratigraphy( index_ );
//    }
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

//    main_scene.reset();
//    topview_scene.reset();


////    window->clear();
////    topview_window->clear();

    removeWindowsDirectionX();
    removeWindowsDirectionY();
    removeWindowsDirectionZ();


}



