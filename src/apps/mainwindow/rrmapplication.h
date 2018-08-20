/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef RRMAPPLICATION_H
#define RRMAPPLICATION_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <unordered_map>
#include <random>


#include <QObject>


#include "./apps/3dview/canvas3d.h"
#include "./apps/sketching/sketchwindow.h"

#include "./core/definitions/constants.hpp"
#include "./core/widgets/realfeaturedslider.h"

//#include "./core/widgets/pages_stack.h"
//#include "./apps/simulator/flow_window.h"
//#include "./core/widgets/objecttree.h"


class MainWindow;
class Controller;


class RRMApplication: public QObject
{
    Q_OBJECT

public:


    RRMApplication() = default;
    RRMApplication(const RRMApplication & app_);
    RRMApplication & operator=(const RRMApplication & app_);
    ~RRMApplication(){}

    void setMainWindow( MainWindow* const& window_ );
    void setController(/* Controller* const& controller_ */);
    void init();




public slots:


    void setVolumeName( const std::string& name_ );
    void setVolumeVisible( bool status_ );


    void setVolumeDimensions( const  Settings::CrossSection::CrossSectionDirections& dir_, double width_, double height_ );

    void setDiscretization( const Settings::CrossSection::CrossSectionDirections& dir_ );

    void changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ );
    void moveMainCrossSection( double depth_ );

    void addFixedCrossSection( double depth_, QColor color_ );
    void removeFixedCrossSection( double depth_ );

    void setImageToCrossSection( const std::string& file_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_, double ox_, double oy_, double w_, double h_  );

    void clearImageInCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


    void setObjectVisible( std::size_t index_, bool status_ );
    void setObjectName( std::size_t index_, const std::string& name_ );
    void setObjectColor( std::size_t index_, int red_, int green_, int blue_ );


    void setCurrentObjectType( const Settings::Objects::ObjectType& type_ );

    void addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
    void addTrajectoryToObject( const PolyCurve& curve_ );
    void previewLastTrajectory();

    void createObjectSurface();

    void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );

    void setSketchAbove( bool status_ );
    void setSketchBelow( bool status_ );
    void setSketchRegion( bool status_ );


    void defineCurrentColor( int red_, int green_, int blue_ );

    void undo();
    void redo();
    void checkUndoRedo();
    void checkPreserveStatus();

    void save( const std::string& filename_ );
    void load( const std::string& filename_ );

    void loadObjectTree();
    void updateObjectTree();


    void reset();


    void setObjectSelectedAsBoundering( const std::size_t& index_ );
    void getRegionByPointAsBoundering( float px_, float py_, double depth_, const Settings::CrossSection::CrossSectionDirections& dir_ );


    void defineRandomColor();


signals:

    void addObject( const std::shared_ptr<Object>& obj_ );
    void addObjectinObjectTree( const std::shared_ptr<Object>& obj_ );

    void updateVolume();
    void updateObjects();
    void updateTrajectories();

    void updateMainCrossSection();
    void updateTopViewCrossSection();

    void changeToTopViewDirection();
    void changeToCrossSectionDirection();


    void addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );

    void addFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_/*, QColor color_*/ );
    void removeFixedCrossSectionWindow( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_/*, QColor color_*/ );

    void updateDiscretization( const std::size_t& disc_ );
    void updateRange( double min_, double max_, bool inverted_ = true );

    void startApplication();
    void resetApplication();

    void disableVolumeResizing();
    void enableVolumeResizing();

    void lockDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
    void unlockDirections();

    void selectEnabled( const std::string& status_ );
    void setCurrentColor( int r_, int g_, int b_ );




protected:


    MainWindow* window;
    Controller* controller;

};



#endif // RRMAPPLICATION_H
