#ifndef CONTROLLER_REFACTORED_H
#define CONTROLLER_REFACTORED_H

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


        inline void addObject(){}
        inline void addObjectToInterface(){}

        inline void setObjectType(){}
        inline void getObjectType(){}

        inline void setObjectName(){}
        inline void getObjectName(){}

        inline void setObjectColor(){}
        inline void getObjectColor(){}

        inline void setObjectVisibility(){}
        inline void getObjectVisibility(){}

        inline void addCurveToObject();
        inline void addTrajectoryToObject(){}

        inline void getObjectCurves(){}
        inline void getObjectTrajectory(){}

        inline void createObjectSurface(){}


        inline void setCurrentCrossSection(){}
        inline void getCurrentCrossSection(){}


        inline void updateActiveCurves(){}
        inline void updateActiveSurfaces(){}


        inline void initRulesProcessor(){}
        inline void updateBoundingBoxRulesProcessor( double width, double height, double depth ){}


    private:

        CSectionScene* csection_scene;
        TopViewScene* topview_scene;
        Scene3d_refactored* scene3d;
        ObjectTree* object_tree;

        Volume* volume;
};

#endif // CONTROLLER_REFACTORED_H
