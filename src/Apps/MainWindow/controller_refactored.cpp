#include "controller_refactored.h"


#include "Sketching/csectionscene.h"
#include "Sketching/topviewscene.h"
#include "3dView/scene3d_refactored.h"
#include "Object_Tree/ObjectTree.h"

#include "./Core/Geology/Models/Volume.h"
#include "./Core/Geology/Models/object_refactored.h"


Controller_Refactored::Controller_Refactored()
{
    volume = nullptr;

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


void Controller_Refactored::setObjectTree( ObjectTree* const& ot )
{
    object_tree = ot;
}



void Controller_Refactored::init()
{
    addVolume();
    addVolumeToInterface();
}



void Controller_Refactored::addVolume()
{
    if( volume != nullptr )
        delete volume;

    volume = new Volume();
    initRulesProcessor();

}


void Controller_Refactored::addVolumeToInterface()
{
    scene3d->addVolume( volume );
    csection_scene->addVolume( volume );
    topview_scene->addVolume( volume );

    object_tree->addInputVolume();
}



void Controller_Refactored::setVolumeWidth( double width )
{
    volume->setWidth( width );
}


void Controller_Refactored::setVolumeHeight( double height )
{
    volume->setHeight( height );
}


void Controller_Refactored::setVolumeDepth( double depth )
{
    volume->setDepth( depth );
}


double Controller_Refactored::getVolumeWidth() const
{
    return volume->getWidth();
}


double Controller_Refactored::getVolumeHeight() const
{
    return volume->getHeight();
}


double Controller_Refactored::getVolumeDepth() const
{
    return volume->getDepth();
}


void Controller_Refactored::setVolumeVisibility( bool status )
{
    volume->setVisibility( status );
}


bool Controller_Refactored::getVolumeVisibility() const
{
    return volume->getVisibility();
}



void Controller_Refactored::addObject()
{
    Object_Refactored* obj = new Object();
    objects[ obj->getId() ] = obj;
    current_object = obj->getId();
}


void Controller_Refactored::setObjectType( const Object_Refactored::Type& type )
{
    Object_Refactored* object = objects[ current_object ];
    object->setType( type );
}


Object_Refactored::Type Controller_Refactored::getObjectType() const
{
    return objects[ current_object ]->getType();
}


void Controller_Refactored::setObjectName( const std::string& name )
{
    Object_Refactored* object = objects[ current_object ];
    object->setName( name );
}


std::string Controller_Refactored::getObjectName()
{
    return objects[ current_object ]->getName();
}

