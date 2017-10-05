#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>


#include "./core/base/models/container.h"
#include "./apps/3dview/scene3d.h"
#include "crosssection.h"


class Volume;
class Object;



class Controller
{
    public:


        Controller() = default;


        void setScene3d( Scene3d* const& sc_ );


        void init();
        void addVolume();

        void addCrossSection( const CrossSection::Direction& dir_, double depth_ );
        CrossSection* getCurrentCrossSection();





    protected:

        Scene3d* scene3d;

        std::size_t current_object;
        double current_csection;

        Volume* volume;
        std::map< std::size_t, Object* > objects;
//        mutable std::map< double, CrossSection* > actives_csections;
        Container< double, CrossSection* > actives_csections;
};

#endif // CONTROLLER_H
