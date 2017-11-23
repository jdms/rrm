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

        void setVolumeOrigin( double ox_, double oy_, double oz_ );
        void getVolumeOrigin( double& ox_, double& oy_, double& oz_ ) const;

        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;

        void setVolumeName( const std::string& name_ );
        const std::string& getVolumeName() const ;

        bool isVolumeResizable() const;

        void setVolumeVisibility( bool status_ );
        bool getVolumeVisibility() const;
        void setupCrossSectionDiscretization( std::size_t& disc_, double& step_ );


        void addMainCrossSection( const CrossSection::Direction& dir_, double depth_ );
        CrossSection* getMainCrossSection( const CrossSection::Direction& dir_ ) const;

        void addTopViewCrossSection( double depth_ );
        CrossSection* getTopViewCrossSection() const;


        void setCurrentCrossSection( double depth_ );
        void updateCurrentCrossSection();


        CrossSection* getCrossSection( const double& depth_ ) const;
        CrossSection* getCurrentCrossSection() const;



        bool addObject();
        Object* getObject( std::size_t index_ ) const;
        Object* getCurrentObject() const;

        void setObjectName( std::size_t index_, const std::string& name_ );
        void setObjectVisibility( std::size_t index_, bool status_ );
        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );

        void saveObjectInformation( std::size_t index_, const std::string & text_ );
        const std::string& getObjectInformation( std::size_t index_ ) const;

        bool addObjectCurve( PolyCurve curve_ );

        bool addObjectTrajectory( PolyCurve curve_ );
        void removeObjectTrajectory();


        bool createPreviewSurface();
        bool createObjectSurface();


        void setActiveAllObjects( bool status_ );



        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();
        void getCurveFromRulesProcessor( Object* obj_, double csection_depth_ );


        void updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_depth_ );
        void updatePreviewCurves( Object* obj_, double csection_depth_ );
        void updateObjectSurfaces( std::size_t object_id_ );
        void updateModel();



//        bool setMainCrossSection( const CrossSection::Direction& dir_, double depth_ );
//        bool addCrossSection( const CrossSection::Direction& dir_, double depth_ );
//        bool removeCrossSection( const CrossSection::Direction& dir_, double depth_ );
//        CrossSection* getCrossSection( const double& depth_ );

//        void setCurrentCrossSection( const double& depth_ );
//        void updateCurrentCrossSection();
//        double getCurrentCrossSection();

//        void addTopViewCrossSection();
//        CrossSection* getTopViewCrossSection();



//
//        bool addObject( std::size_t index_ );
//        bool addObjectCurve( PolyCurve curve_ );
//        bool removeObjectCurve( double csection_ );

//        bool addObjectTrajectory( PolyCurve curve_ );
//        void removeObjectTrajectory();


//        Object* getCurrentObject();

//        void saveObjectInformation( std::size_t index_, const std::string & text_ );
//        const std::string& getObjectInformation( std::size_t index_ );
//        void clearObjectInformation( std::size_t index_ );

//        void setObjectColor( std::size_t index_, int r_, int g_, int b_);
//        void getObjectColor( std::size_t index_, int& r_, int& g_, int& b_);

//        void setObjectName( std::size_t index_, const std::string& name_ );
//        std::string getObjectName( std::size_t index_ );

//        void setObjectVisibility( std::size_t index_, bool status_ );
//        bool getObjectVisibility( std::size_t index_ );


//        void setObjectsAsSelectable( std::vector< std::size_t >& indexes_, bool status_ );
//        void setObjectAsSelected( std::size_t index_, bool status_ );


//        std::size_t getIndexCurrentObject() const;

//        bool createObjectSurface();
//        bool createPreviewSurface();


//        void getOutputVolume();


//        void initRulesProcessor();
//        void updateBoundingBoxRulesProcessor();

//        void setRemoveAbove();
//        void setRemoveAboveIntersection();
//        void setRemoveBelow();
//        void setRemoveBelowIntersection();
//        void applyStratigraphicRule();



        std::size_t indexCrossSection( double value_ ) const;
        double depthCrossSection( std::size_t index_ ) const;


//        void updateModel();
//        void updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_id_ );
//        void updateObjectSurfaces( std::size_t object_id_ );


//        bool enableCreateAbove( bool status_ );
//        void stopCreateAbove();
//        bool requestCreateAbove();

//        bool enableCreateBelow( bool status_ );
//        void stopCreateBelow();
//        bool requestCreateBelow();


//        void getObjectsAsUpperBoundering( std::vector< std::size_t >& indexes_);
//        void setObjectAsBoundering( std::size_t index_ );


//        bool undo();
//        bool canUndo();

//        bool redo();
//        bool canRedo();


//        void saveFile( const std::string& filename );
//        void loadFile( const std::string& filename );
//        void loadObjects();
//        std::vector< int > createVectorOfColors( std::size_t number_of_colors );


//        bool isDefineAboveActive();
//        bool isDefineBelowActive();


//        void clear();
//        void initializeData();

    protected:


        enum class StratigraphicRules : int {
            UNDEFINED = -1,
            NO_GEOLOGIC_RULE,
            REMOVE_ABOVE, // Remove above
            REMOVE_ABOVE_INTERSECTION, // Remove above intersection
            REMOVE_BELOW, // Remove below
            REMOVE_BELOW_INTERSECTION, // Remove below intersection
        };


        enum class BounderingRegion { ABOVE, BELOW };


        Scene3d* scene3d = nullptr;
        ObjectTree* object_tree = nullptr;


        Volume* volume = nullptr;


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
        CrossSection* topview_csection = nullptr;
        Container< double, CrossSection* > actives_csections;
        Container< double, CrossSection* > all_csections;


        RulesProcessor rules_processor;
        double csection_step = 1.0;

        StratigraphicRules current_rule = StratigraphicRules::REMOVE_ABOVE;
        std::vector< std::size_t > selectable_upper;
        std::vector< std::size_t > selectable_bottom;

        BounderingRegion boundering_region;
        std::size_t index_upper_boundary;
        std::size_t index_bottom_boundary;






};

#endif // CONTROLLER_H
