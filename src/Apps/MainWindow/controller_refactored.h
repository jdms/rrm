#ifndef CONTROLLER_REFACTORED_H
#define CONTROLLER_REFACTORED_H

class CSectionScene;
class TopViewScene;
class Scene3d_refactored;

class Volume;


class Controller_Refactored
{

    public:

        Controller_Refactored();

        void init();

        void setScene3d( Scene3d_refactored* const& sc );
        void setCSectionScene( CSectionScene* const& sc );
        void setTopViewScene( TopViewScene* const& sc );
        inline void setObjectTree(){}

        void addVolume();
        void addVolumeToInterface();

        inline void setVolumeWidth(){}
        inline void setVolumeHeight(){}
        inline void setVolumeDepth(){}

        inline void getVolumeWidth(){}
        inline void getVolumeHeight(){}
        inline void getVolumeDepth(){}

        inline void setVolumeVisibility(){}
        inline void getVolumeVisibility(){}


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


    private:

        CSectionScene* csection_scene;
        TopViewScene* topview_scene;
        Scene3d_refactored* scene3d;

        Volume* volume;
};

#endif // CONTROLLER_REFACTORED_H
