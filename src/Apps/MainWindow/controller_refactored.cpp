#include "controller_refactored.h"


#include "Sketching/csectionscene.h"
#include "Sketching/topviewscene.h"
#include "3dView/scene3d_refactored.h"

#include "./Core/Geology/Models/Volume.h"


Controller_Refactored::Controller_Refactored()
{

}


void Controller_Refactored::setScene3d( Scene3d_refactored* const& sc )
{
    scene3d = sc;
}


void Controller_Refactored::setCSectionScene( CSectionScene* const& sc )
{
    csection_scene = sc;
}


void Controller_Refactored::setTopViewScene( TopViewScene* const& sc )
{
    topview_scene = sc;
}


void Controller_Refactored::init()
{
    addVolume();
}

void Controller_Refactored::addVolume()
{
    if( volume != nullptr )
        delete volume;

    volume = new Volume();
}


void Controller_Refactored::addVolumeToInterface()
{
    scene3d->addVolume();
    csection_scene->addVolume();
    topview_scene->addVolume();

    //object tree
}
