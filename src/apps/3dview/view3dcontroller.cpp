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
    scene = window->getScene();

    VolumePtr volume_ = controller->getMainCrossSection()->getVolume();
    scene->addVolume( volume_ );

    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        ObjectPtr& obj_ = it.second;
        scene->addStratigraphy( std::static_pointer_cast< Stratigraphy >( obj_ ) );
    }

}


void View3dController::updateScene()
{
    scene->updateVolume();
    updateObjects();

}


void View3dController::updateObjects()
{
    std::map< std::size_t, ObjectPtr > objects_ = controller->getObjects();
    for( auto it: objects_ )
    {
        ObjectPtr& obj_ = it.second;
        scene->updateStratigraphy( it.first );
    }
}
