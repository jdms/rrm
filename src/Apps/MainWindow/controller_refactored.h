#ifndef CONTROLLER_REFACTORED_H
#define CONTROLLER_REFACTORED_H

#include <vector>
#include <string>
#include <map>

#include "RulesProcessor.hpp"
#include "./Core/Geology/Models/object_refactored.h"


class CSectionScene;
class TopViewScene;
class Scene3d_refactored;
class ObjectTree;

class Volume;


class Controller_Refactored
{

    public:


        enum class StratigraphicRules : int {
            UNDEFINED = -1,
            NO_GEOLOGIC_RULE,
            REMOVE_ABOVE, // Remove above
            REMOVE_ABOVE_INTERSECTION, // Remove above intersection
            REMOVE_BELOW, // Remove below
            REMOVE_BELOW_INTERSECTION, // Remove below intersection
            DEFINE_ABOVE, // Define above
            DEFINE_BELOW, // Define below
            DEFINE_REGION // Define region
        };

        enum class RequestRegion { ABOVE, BELOW, NONE };

        Controller_Refactored();

        void init();

        void setScene3d( Scene3d_refactored* const& sc );
        void setCSectionScene( CSectionScene* const& sc );
        void setTopViewScene( TopViewScene* const& sc );
        void setObjectTree( ObjectTree* const& ot );

        void addVolume();
        void addVolumeToInterface();

        void setVolumeWidth( double width );
        void setVolumeHeight( double height );
        void setVolumeDepth( double depth );

        double getVolumeWidth() const ;
        double getVolumeHeight() const ;
        double getVolumeDepth() const ;

        void setVolumeVisibility( bool status );
        bool getVolumeVisibility() const ;

        bool isVolumeResizable() const;

        void updateVolume();


        void addObject();
        void addObject( std::size_t id );
        void addObjectToInterface();
        void showObjectInCrossSection( std::size_t id );

        bool isValidObject( std::size_t id ) const ;


        void setObjectType( const Object_Refactored::Type& type );
        void setObjectType( std::size_t id, const Object_Refactored::Type& type );

        Object_Refactored::Type getObjectType();
        Object_Refactored::Type getObjectType( std::size_t id );


        void setObjectName( const std::string& name );
        std::string getObjectName();

        void setObjectName( std::size_t id, const std::string& name );
        std::string getObjectName( std::size_t id );


        void setObjectColor( int r, int g, int b );
        void getObjectColor( int& r, int& g, int& b );

        void setObjectColor( std::size_t id, int r, int g, int b );
        void getObjectColor( std::size_t id, int& r, int& g, int& b );


        void setObjectVisibility( bool status );
        bool getObjectVisibility();

        void setObjectVisibility( std::size_t id,  bool status );
        bool getObjectVisibility( std::size_t id );


        void addCurveToObject( const Curve2D& curve );
        void addTrajectoryToObject( const Curve2D& curve );

        void removeCurveFromObject( double depth );
        void removeTrajectoryFromObject();

        void removeCurrentObject();

        void enableTrajectory( bool status );
        void enableCurve( bool status );

        void enableDeletingCurves( bool status  );

        bool createObjectSurface();
        bool testObjectSurface();

        void updateObject( std::size_t id );
        void updateActiveObjects();
        bool updateActiveCurve( std::size_t id );
        bool updateActiveSurface( std::size_t id );
        void desactiveObjects();


        void setCurrentCrossSection( double depth );
        double getCurrentCrossSection() const;
        bool isValidCrossSection( double depth ) const;


        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();


        std::size_t setupCrossSectionDiscretization();
        std::size_t indexCrossSection( double value ) const;
        double depthCrossSection( std::size_t index ) const;


        void setCurrentColor( int r, int g, int b );
        void getCurrentColor( int& r, int& g, int& b ) const ;


        Controller_Refactored::StratigraphicRules getCurrentRule() const;
        void setCurrentRule( const Controller_Refactored::StratigraphicRules& rule );


        void enableCreateAbove( bool status );
        void stopCreateAbove();
        void requestCreateAbove();

        void enableCreateBelow( bool status );
        void stopCreateBelow();
        void requestCreateBelow();


        void setObjectsAsSelectable( std::vector< std::size_t >& indexes,
                                                            bool status );
        void setObjectSelected( std::size_t id, bool status );
        void defineObjectSelected( std::size_t id );


        void saveFile( const std::string& filename );
        void loadFile( const std::string& filename );

        void loadObjects();
        void loadStatus();


        bool undo();
        bool canUndo();

        bool redo();
        bool canRedo();


        void clear();
        void clearScenes();
        void clearData();


        void setDefaultValues();


        std::vector< int > createVectorOfColors( std::size_t number_of_colors );


    private:

        CSectionScene* csection_scene;
        TopViewScene* topview_scene;
        Scene3d_refactored* scene3d;
        ObjectTree* object_tree;

        RulesProcessor rules_processor;

        Volume* volume;
        std::map< std::size_t, Object_Refactored* > objects;
        std::size_t current_object;


        double current_csection;
        double csection_step;

        std::tuple< int, int, int > current_color;

        StratigraphicRules current_rule;
        const StratigraphicRules RULE_DEFAULT = StratigraphicRules::REMOVE_ABOVE_INTERSECTION;


        std::vector< std::size_t > selectable_upper;
        std::vector< std::size_t > selectable_below;

        std::size_t boundering_above;
        std::size_t boundering_below;

        RequestRegion current_region;

};

#endif // CONTROLLER_REFACTORED_H
