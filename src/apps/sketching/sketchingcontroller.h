#ifndef SKETCHINGCONTROLLER_H
#define SKETCHINGCONTROLLER_H

#include <memory>
#include <map>


#include "./core/models/controller.h"
#include "./models/volumeitem.h"


class SketchScene;
class SketchWindow;

class SketchingController
{
    public:

        SketchingController();

        void setMainWindow( const std::shared_ptr< SketchWindow >& window_);
        void setController( const std::shared_ptr< Controller >& controller_ );


        void createMainCrossSection();
        void updateMainCrossSection();

        void viewCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_/*, bool fixed_ = false*/ );

        void addStratigraphy( const std::size_t& index_ );
        void updateStratigraphy( const std::size_t& index_ );

        void removeWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

        void removeWindowsDirectionX();
        void removeWindowsDirectionY();
        void removeWindowsDirectionZ();



    protected:


        void setObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ );
        void updateObjectsToScene( const CrossSectionPtr& csection_ , const std::shared_ptr< SketchScene >& scene_ );




    protected:

        std::shared_ptr< SketchScene > main_scene;
        std::shared_ptr< SketchWindow > window;
        std::map< double, std::shared_ptr< SketchScene > > scenesX;
        std::map< double, std::shared_ptr< SketchScene > > scenesY;
        std::map< double, std::shared_ptr< SketchScene > > scenesZ;
        std::shared_ptr< Controller > controller;



};

#endif // SKETCHINGCONTROLLER_H
