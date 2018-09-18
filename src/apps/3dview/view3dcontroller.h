#ifndef VIEW3DCONTROLLER_H
#define VIEW3DCONTROLLER_H

#include <memory>

#include "./core/models/controller.h"


class Scene3d;
class Canvas3d;

class View3dController
{

    public:

        View3dController();

        void setMainWindow( const std::shared_ptr< Canvas3d >& window_);
        void setController( const std::shared_ptr< Controller >& controller_ );

        void init();
        void setupScene();

        void setMainViewCrossSection();
        void setTopViewCrossSection();


        void updateVolume();
        void updateMainCrossSection();
        void updateTopViewCrossSection();

        void addStratigraphy( const ObjectPtr& obj_ );

        void updateObjects();
        void updateScene();


        void addRegions();
        void updateRegions();
        void clearRegions();

        void clear();

        void setVerticalExaggeration( double scale_ );

    protected:

        std::shared_ptr< Scene3d > scene;
        std::shared_ptr< Canvas3d > window;
        std::shared_ptr< Controller > controller;

};

#endif // VIEW3DCONTROLLER_H
