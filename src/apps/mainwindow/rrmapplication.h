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
//#include "./apps/simulator/flow_window.h"
#include "./core/definitions/constants.hpp"
#include "./core/widgets/realfeaturedslider.h"
#include "./core/widgets/objecttree.h"
#include "./core/widgets/pages_stack.h"




//class MainWindow;
class Controller;


class RRMApplication: public QObject
{
    Q_OBJECT

    public:


        RRMApplication() = default;
        RRMApplication(const RRMApplication & app_);
        RRMApplication & operator=(const RRMApplication & app_);
        ~RRMApplication(){}

        void setController( Controller* const& controller_ );
        void init();


        void setDiscretization();


    public slots:

        void changeCrossSectionDirection( Settings::CrossSection::CrossSectionDirections dir_ );
        void moveMainCrossSection( double depth_ );


        void addCurveToObject( const PolyCurve& curve_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        void addTrajectoryToObject( const PolyCurve& curve_ );
        void createObjectSurface();

        void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );


    signals:

        void addObject( const std::size_t& index_ );

        void updateVolume();
        void updateObjects();
        void updateMainCrossSection();
        void updateTopViewCrossSection();

        void updateDiscretization( const std::size_t& disc_ );
        void updateRange( double min_, double max_ );


///================================================================================


//        RRMApplication() = default;
//        RRMApplication( MainWindow* mw_ );
//        ~RRMApplication(){}




//        void setSiderBarVisibility( bool status_ );
//        void setDefaultRule( Settings::Stratigraphy::StratigraphicRules rule_ );
//        void setDefaultSketchingRegion( Settings::Objects::BounderingRegion sketching_region_ );
//        void setDefaultSiderBarValues();
//        void setRRMDefaultValuesOnInterface();


//        void changeVolumeDimension( const Settings::CrossSection::CrossSectionDirections& dir_, double value_ );
//        void changeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double dim1_, double dim2_ );

//        void setVolumeOriginToController( double ox_, double oy_, double oz_ );
//        void setVolumeDimensionsToController(  double width_, double height_, double length_ );
//        void getVolumeDimensionsFromController() const;


//        void setVolumeName( std::size_t index_, const std::string& name_ );
//        void setVolumeVisible( std::size_t index_, bool status_ );



//        void setCurrentCrossSection( double depth_ );



//        void setObjectName( std::size_t index_, const std::string& name_ );
//        void setObjectVisible( std::size_t index_, bool status_ );
//        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );

//        void saveObjectInformation( const std::string & text_ );
//        void getObjectInformation( QTreeWidgetItem* const& item_ ) const;


//        void setRegionName( std::size_t index_, const std::string& name_ );
//        void setRegionVisible( std::size_t index_, bool status_ );
//        void setRegionColor( std::size_t index_, int r_, int g_, int b_ );



//        void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );
//        void setSketchAbove( bool status_ );
//        void setSketchBelow( bool status_ );
//        void setObjectAsBoundering( std::size_t index_ );


//
//        void updateSketchingCanvas();

//        void acceptSketchingCurve( const PolyCurve& curve_, double depth_ );
//        void removeCurveFromObject(  double depth_, std::size_t index_ );
//        void acceptSketchingTrajectory( const PolyCurve& curve_ );
//


//        void setModeSelecting();
//        void setModeSketching();


//        void save( const std::string& filename_ );
//        void load( const std::string& filename_ );


//        void undo();
//        void redo();
//        void checkUndoRedo();
//        void checkSketchStatus();


//        void setCurrentColor( int r_, int g_, int b_ );
//        void defineRandomColor();

//        void screenshot();

//        void clearInterface();
//        void clear();
//        void restart();


//        void addCrossSectionCanvas( double depth_, QColor color_ );
//        void removeCrossSectionCanvas( double depth_ );


//        void setImageToCrossSection( double depth_, std::string file_, double ox_, double oy_, double x_, double y_ );
//        void removeImageFromCrossSection( double depth_ );


//        void setImageToTopView( std::string file_, double ox_, double oy_, double x_, double y_ );
//        void removeImageFromTopView();
//        void getHeightMapTopView();

//        void startFlowDiagnostics();
//        void closeFlowDiagnostics();

//        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
//        void getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes, std::vector< FlowWindow::CurveMesh>& left_curves,
//                                std::vector< FlowWindow::CurveMesh >& right_curves, std::vector< FlowWindow::CurveMesh > & front_curves,
//                                std::vector< FlowWindow::CurveMesh >& back_curves );


//        void getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces );

//        void setLowResolution();
//        void setMediumResolution();
//        void setHighResolution();

//        void enablePreview( bool status_ );

//        void exportToIRAP();



    protected:


        Controller* controller;

        ///================================================================================



//        MainWindow* mainwindow = nullptr;


//        const double VOLUME_WIDTH = 500;
//        const double VOLUME_HEIGHT = 500;
//        const double VOLUME_LENGTH = 500;


//        std::size_t discretization = 1;
//        double step_csection = 1.0;



};



#endif // RRMAPPLICATION_H
