#ifndef SCENE_H
#define SCENE_H


#include "volume.h"
#include "crosssection.h"


class Scene
{

    public:

        Scene() = default;


        virtual void addVolume( Volume* const& raw_ ) = 0;
        virtual void clearVolume() = 0;
//        virtual void updateVolume();


//        virtual void addObject() = 0;
//        virtual void updateObject();
//        virtual void selectObject();


        virtual void addCrossSection( CrossSection* const& raw_ ) = 0;
        virtual void removeCrossSection( CrossSection* const& raw_ ) = 0 ;
//        virtual void updateCrossSection();


//        virtual void addRegion() = 0;
//        virtual void updateRegion() = 0;


//        virtual void setCurrentColor( int r, int g, int b ) = 0;
//        virtual void getCurrentColor( int& r, int& g, int& b ) = 0;



};

#endif // SCENE_H
