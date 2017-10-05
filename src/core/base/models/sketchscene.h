#ifndef SKETCHSCENE_H
#define SKETCHSCENE_H

#include <QGraphicsScene>
#include "scene.h"

class SketchScene: public QGraphicsScene, public Scene
{

    public:

        SketchScene();

        void readCrossSection();


        void addVolume( Volume* const& raw_ );


        void addCrossSection( CrossSection* const& raw_ ){}
        void removeCrossSection( CrossSection* const& raw_ ){}




};

#endif // SKETCHSCENE_H
