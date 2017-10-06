#include "sketchscene.h"

SketchScene::SketchScene()
{
    volume = nullptr;
}


SketchScene::SketchScene( CrossSection* const& raw_ )
{
    volume = nullptr;
    readCrossSection( raw_ );
}


void SketchScene::readCrossSection( CrossSection* const& raw_ )
{
    addVolume( raw_->getVolume() );



}


void SketchScene::addVolume( Volume* const &raw_ )
{
    clearVolume();
    volume = new VolumeItemWrapper( raw_, VolumeItemWrapper::Section::XY );

    addItem( volume );
    update();
}


void SketchScene::clearVolume()
{
    if( volume != nullptr )
        delete volume;
    volume = nullptr;
}
