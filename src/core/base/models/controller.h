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

        void acceptVolumeDimensions( CrossSection::Direction dir_, double w_, double h_ );
        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;




        void addCrossSection( const CrossSection::Direction& dir_, double depth_ );
        void removeCrossSection( const CrossSection::Direction& dir_, double depth_ );

        void setCurrentCrossSection( const double& depth_ );
        CrossSection* getCurrentCrossSection();

        CrossSection* getCrossection( const double& depth_ );





    protected:

        Scene3d* scene3d;

        std::size_t current_object;
        double current_csection;

        Volume* volume;
        std::map< std::size_t, Object* > objects;
        Container< double, CrossSection* > actives_csections;
};

#endif // CONTROLLER_H
