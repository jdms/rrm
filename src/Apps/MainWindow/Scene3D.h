#ifndef SCENE3D_H
#define SCENE3D_H

#include "Volume.h"
#include "Object.h"
#include "Region1.h"
#include "CrossSection.h"


class Scene3D
{
    public:

        Scene3D();

        inline bool addVolume( const Volume* volume_ ){ return true; }
        inline bool addObject( const Object* object_ ){ return true; }
        inline bool addCrossSection( const CrossSection1* csection_ ){ return true; }
        inline bool addRegion( const Region1* region_ ){ return true; }


    protected:


};

#endif // SCENE3D_H
