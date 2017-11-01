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

        void setCurrentColor( int r, int g, int b );
        void getCurrentColor( int& r, int& g, int& b ) const ;


        void init();


        void addVolume();

        void acceptVolumeDimensions( CrossSection::Direction dir_, double w_, double h_ );
        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;

        void setVolumeVisibility( bool status_ );
        bool getVolumeVisibility() const;



        bool setMainCrossSection( const CrossSection::Direction& dir_, double depth_ );
        bool addCrossSection( const CrossSection::Direction& dir_, double depth_ );
        bool removeCrossSection( const CrossSection::Direction& dir_, double depth_ );
        CrossSection* getActiveCrossSection( const double& depth_ );
        CrossSection* getCrossSection( const double& depth_ );

        void setCurrentCrossSection( const double& depth_ );
        double getCurrentCrossSection();



        bool addObject();
        bool addObjectCurve( PolyCurve curve_ );

        bool removeObjectCurve( double csection_ );
        Object* getCurrentObject();

        void setObjectColor( std::size_t index_, int r_, int g_, int b_);
        void getObjectColor( std::size_t index_, int& r_, int& g_, int& b_);


        void setObjectName( std::size_t index_, const std::string& name_ );


        void setObjectVisibility( std::size_t index_, bool status_ );
        bool getObjectVisibility( std::size_t index_ );

        bool createObjectSurface();


        std::size_t getIndexCurrentObject() const;



        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();

        std::size_t setupCrossSectionDiscretization();
        std::size_t indexCrossSection( double value_ ) const;
        double depthCrossSection( std::size_t index_ ) const;



        void updateModel();
        void updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_id_ );
        void updateObjectSurfaces( std::size_t object_id_ );




    protected:

        Scene3d* scene3d;
        ObjectTree* object_tree;


        Volume* volume;


        struct Color
        {
            int r = 255;
            int g = 0;
            int b = 0;
        } current_color;

        std::size_t current_object = 0;
        Container< std::size_t, Object* > objects;


        double current_csection = 500.0;
        CrossSection* main_csection = nullptr;
        Container< double, CrossSection* > actives_csections;
        Container< double, CrossSection* > all_csections;



        RulesProcessor rules_processor;
        double csection_step;

};

#endif // CONTROLLER_H
