#ifndef SCENE_H
#define SCENE_H


#include "./core/definitions/constants.hpp"

#include "volume.h"
#include "crosssection.h"
#include "object.h"
#include "regions.h"


class Scene
{

    public:

        Scene() = default;


        virtual void addVolume( Volume* const& raw_ ) = 0;
        virtual void clearVolume() = 0;
        virtual void updateVolume() = 0;


        virtual void addObject(  Object* const& raw_ ) = 0;
        virtual void updateObject(  const std::size_t& index_ ) = 0;
//        virtual void selectObject();


        virtual void addCrossSection( CrossSection* const& raw_ ) = 0;
        virtual void removeCrossSection( CrossSection* const& raw_ ) = 0 ;
//        virtual void updateCrossSection( CrossSection* const& raw_ ) = 0;
//        virtual void updateCrossSection();


//        virtual void addRegion() = 0;
//        virtual void updateRegion() = 0;


        virtual void setCurrentColor( int r, int g, int b ) = 0;
        virtual void getCurrentColor( int& r, int& g, int& b ) = 0;



};

#endif // SCENE_H
