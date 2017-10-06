#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>

#include "./item_wrappers/volume_item_wrapper.h"
#include "./core/base/models/scene.h"

class SketchScene: public QGraphicsScene, public Scene
{

    public:

        SketchScene();
        SketchScene( CrossSection* const& raw_ );

        void readCrossSection( CrossSection* const& raw_ );


        virtual void addVolume( Volume* const& raw_ );
        virtual void clearVolume();


        void addCrossSection( CrossSection* const& raw_ ){}
        void removeCrossSection( CrossSection* const& raw_ ){}



    protected:

        VolumeItemWrapper* volume;

};

#endif // SKETCHSCENE_H
