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

        void updateVolume();


        void addObject();
        void addObjectToInterface();
        void showObjectInCrossSection( std::size_t id );

        bool isValidObject( std::size_t id ) const ;


        inline void setObjectType( const Object_Refactored::Type& type ) {
                                    setObjectType( current_object, type ); }

        inline Object_Refactored::Type getObjectType(){
                                    return getObjectType( current_object ); }

        void setObjectType( std::size_t id, const Object_Refactored::Type& type );
        Object_Refactored::Type getObjectType( std::size_t id );




        inline void setObjectName( const std::string& name ){
                                    setObjectName( current_object, name ) ;}
        inline std::string getObjectName(){
                                    return getObjectName( current_object ); }

        void setObjectName( std::size_t id, const std::string& name );
        std::string getObjectName( std::size_t id );


        inline void setObjectColor( int r, int g, int b ){
                                    setObjectColor( current_object, r, g, b ); }
        inline void getObjectColor( int& r, int& g, int& b ){
                                    getObjectColor( current_object, r, g, b ); }

        void setObjectColor( std::size_t id, int r, int g, int b );
        void getObjectColor( std::size_t id, int& r, int& g, int& b );


        inline void setObjectVisibility( bool status ){
                                    setObjectVisibility( current_object, status ); }
        inline bool getObjectVisibility(){
                                    return getObjectVisibility( current_object ); }

        void setObjectVisibility( std::size_t id,  bool status );
        bool getObjectVisibility( std::size_t id );


        void addCurveToObject( const Curve2D& curve );
        void addTrajectoryToObject( const Curve2D& curve );


        void removeCurveFromObject( double depth );
        bool createObjectSurface();

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


        void setCurrentColor( int r, int g, int b );
        void getCurrentColor( int& r, int& g, int& b ) const ;


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


        std::tuple< int, int, int > current_color;
};

#endif // CONTROLLER_REFACTORED_H
