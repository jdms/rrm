#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>


#include "./core/base/models/container.h"
#include "./apps/mainwindow/widgets/objecttree.h"
#include "./apps/3dview/scene3d.h"
#include "crosssection.h"
#include "rules_processor.hpp"


class Volume;
class Object;



class Controller
{
    public:


        Controller() = default;


        void setScene3d( Scene3d* const& sc_ );
        void setObjectTree( ObjectTree* const& ot_ );


        void init();


        void addVolume();

        void acceptVolumeDimensions( CrossSection::Direction dir_, double w_, double h_ );
        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;

        void setVolumeVisibility( bool status_ );
        bool getVolumeVisibility() const;



        void addCrossSection( const CrossSection::Direction& dir_, double depth_ );
        void removeCrossSection( const CrossSection::Direction& dir_, double depth_ );
        CrossSection* getCrossection( const double& depth_ );

        void setCurrentCrossSection( const double& depth_ );
        CrossSection* getCurrentCrossSection();



        void addObject();
        void addObjectCurve( PolyCurve curve_ );
        void removeObjectCurve( std::size_t csection_ );
        Object* getCurrentObject();

        void setObjectVisibility( std::size_t index_, bool status_ );
        bool getObjectVisibility( std::size_t index_ );


        std::size_t getIndexCurrentObject() const;



        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();



    protected:

        Scene3d* scene3d;
        ObjectTree* object_tree;

        std::size_t current_object;
        double current_csection;

        Volume* volume;
        Container< std::size_t, Object* > objects;
        Container< double, CrossSection* > actives_csections;

        RulesProcessor rules_processor;
};

#endif // CONTROLLER_H
