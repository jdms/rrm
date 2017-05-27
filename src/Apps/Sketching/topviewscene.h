#ifndef TOPVIEWSCENE_H
#define TOPVIEWSCENE_H

#include "sketchscene_refactored.h"


class TopViewScene: public SketchScene_Refactored
{
    Q_OBJECT

    public:

        TopViewScene();

        inline void addCrossSection( double depth ){}
        inline void moveCurrentCrossSection( double depth ){}
        inline void showCrossSections( bool status ){}

};

#endif // TOPVIEWSCENE_H
