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



#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "./core/definitions/constants.hpp"
#include "./core/models/container.h"
#include "./core/widgets/objecttree.h"
#include "./core/models/irap_grid_exporter.hpp"
#include "./apps/3dview/scene3d.h"
#include "crosssection.h"
#include "regions.h"
#include "rules_processor.hpp"


class Volume;
class Object;
class RRMApplication;


using VolumePtr = std::shared_ptr< Volume >;
using CrossSectionPtr = std::shared_ptr< CrossSection >;
using ObjectPtr = std::shared_ptr< Object >;
using RegionsPtr = std::shared_ptr< Regions >;
const std::size_t UNDEFINED_INDEX = 9999;

class Controller
{
    public:

        using TriangleMesh = RulesProcessor::TriangleMesh;
        using CurveMesh = RulesProcessor::CurveMesh;


        enum class MeshResolution{ LOW, MEDIUM, HIGH };


        Controller();
        Controller(const Controller & cont_);
        Controller & operator=(const Controller & cont_);
        ~Controller();

        void setApplication( RRMApplication* const& app_ );

       ///==========================================================================



        void init();

        void createVolume();
        void resizeVolume( double width_, double height_, double depth_ );

        void setVolumeName( const std::string& name_ );
        void setVolumeVisibility( bool status_ );

        void createMainCrossSection();
        void changeMainCrossSectionDirection( const Settings::CrossSection::CrossSectionDirections& dir_ );
        void moveMainCrossSection( double depth_ );
        const CrossSectionPtr& getMainCrossSection() const;

        void addCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        bool getCrossSection( const Settings::CrossSection::CrossSectionDirections & dir_, double depth_, CrossSectionPtr& csection_ );
        void removeCrossSection( const Settings::CrossSection::CrossSectionDirections & dir_, double depth_ );

        //TODO: addObject should be protected
        bool addObject( std::size_t index_ = UNDEFINED_INDEX );

        const ObjectPtr& getObject( std::size_t index_ ) const;
        const ObjectPtr& getCurrentObject() const;
        const std::map< std::size_t, ObjectPtr >& getObjects();


        void setObjectName( std::size_t index_, const std::string& name_ );
        std::string getObjectName( std::size_t index_) const;

        void setObjectVisibility( std::size_t index_, bool status_ );
        void setObjectsVisibility( bool status_ );

        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );

        void setObjectsActive( bool status_ );
        void setObjectActive( std::size_t index_, bool status_ );
        bool isObjectActive( std::size_t index_ ) const;

        void setObjectSelectable( std::size_t index_, bool status_ );
        bool isObjectSelectable( std::size_t index_ ) const;

        void setObjectSelected( std::size_t index_, bool status_ );
        bool isObjectSelected( std::size_t index_ ) const;


        bool addCurveToObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_, const PolyCurve& curve_ );
        bool removeCurveFromObject( Settings::CrossSection::CrossSectionDirections dir_, double depth_ );

        void addTrajectoryToObject( const PolyCurve& curve_ );
        void removeTrajectoryFromObject();


        void updateModel();
        void updateObjectSurface( const std::size_t& index_ );
        void updateObjectCurves( const std::size_t& index_ );
        void updatePreviewSurface();


        void updateObjectCurveInCrossSection( const std::size_t& index_, double depth_ );
        void updateObjectsCurvesInCrossSection( double depth_ );
        void updateCrossSectionsX();
        void updateCrossSectionsZ();
        void clearAndSetCurveinCrossSectionFromRulesProcessor( const std::size_t& index_ , double depth_ );


        void defineRegions();

        void setRegionsVisible(bool status_);
        void setRegionVisible(std::size_t index_, bool status_);
        bool isRegionVisible(std::size_t index_) const;

        void setRegionColor( std::size_t index_, int r_, int g_, int b_ );
        void getRegionColor( std::size_t index_, int& r_, int& g_, int& b_ ) const ;

        void setRegionsActive(bool status_);
        void setRegionActive(std::size_t index_, bool status_);
        bool isRegionActive(std::size_t index_) const;

        void setRegionSelectable(std::size_t index_, bool status_);
        bool isRegionSelectable(std::size_t index_) const;

        void setRegionSelected(std::size_t index_, bool status_);
        bool isRegionSelected(std::size_t index_) const;


        void createDomain(std::set<std::size_t> indexes_ = std::set< std::size_t >() );
        void addRegionToDomain(std::size_t region_id_, std::size_t domain_id_);
        void removeRegionFromDomain(std::size_t region_id_, std::size_t domain_id_);
        std::set<std::size_t> getRegionsFromDomain(std::size_t domain_id_) const;


        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();
        void setVolumeDiscretization( std::size_t& width_disc_, std::size_t& lenght_disc_ );
        std::size_t indexCrossSectionX( double value_ ) const;
        std::size_t indexCrossSectionZ( double value_ ) const;

        void setRemoveAbove();
        void setRemoveAboveIntersection();
        void setRemoveBelow();
        void setRemoveBelowIntersection();
        void applyStratigraphicRule();



        ///==========================================================================





//        void setScene3d( Scene3d* const& sc_ );
//        void setObjectTree( ObjectTree* const& ot_ );

//        void setCurrentColor( int r, int g, int b );
//        void getCurrentColor( int& r, int& g, int& b ) const ;

//        void addVolume();

//        void setVolumeOrigin( double ox_, double oy_, double oz_ );
//        void getVolumeOrigin( double& ox_, double& oy_, double& oz_ ) const;

//        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
//        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;


//        const std::string& getVolumeName() const ;

//        bool isVolumeResizable() const;


//        bool getVolumeVisibility() const;
//        void setupCrossSectionDiscretization( std::size_t& disc_, double& step_ );


//        void addMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
//        CrossSection* getMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_ ) const;

//        void addTopViewCrossSection( double depth_ );
//        CrossSection* getTopViewCrossSection() const;


//        void setCurrentCrossSection( double depth_ );
//        void updateCurrentCrossSection();


//        void setImageCrossSection( double depth_, const std::string& path_, double ox_, double oy_, double x_, double y_ );
//        bool hasImageCrossSection( double depth_ );
//        bool clearImageCrossSection( double depth_ );
//        bool getImageCrossSection( double depth_, std::string& path_, double& ox_, double& oy_, double& x_, double& y_ );


//        void setTopViewImage( std::string file_, double ox_, double oy_, double x_, double y_ );
//        void removeTopViewImage();


//        CrossSection* getCrossSection( const double& depth_ ) const;
//        CrossSection* getCurrentCrossSection() const;



////        bool addObject( std::size_t index_ = 9999 );
////        Object* getObject( std::size_t index_ ) const;
////        Object* getCurrentObject() const;






//        void saveObjectInformation( std::size_t index_, const std::string & text_ );
//        const std::string& getObjectInformation( std::size_t index_ ) const;


//        bool addObjectCurve( PolyCurve curve_, double depth_ ){ return false; }
//        bool removeObjectCurve( std::size_t index_, double depth_ );
//        bool removeObjectCurve( double depth_ );

//        bool addObjectTrajectory( PolyCurve curve_ ){ return false; }
//        void removeObjectTrajectory(){}


//        bool createPreviewSurface(){ return false; }
//        bool createObjectSurface();


//        void setActiveAllObjects( bool status_ );


//        void setRegionName( std::size_t index_, const std::string& name_ );
//        void setRegionVisibility( std::size_t index_, bool status_ );

//        void getCurveFromRulesProcessor( Object* obj_, double csection_depth_ );


//        void updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_depth_ ){}
//        void updatePreviewCurves( Object* obj_, double csection_depth_ ){}
//        void updateObjectSurfaces( std::size_t object_id_ ){}
//        void updateObjectInFixedCrossSections( std::size_t id_ );



//        void setTruncate();



//        bool enableCreateAbove( bool status_ );
//        void stopCreateAbove();
//        bool requestCreateAbove();
//        bool isDefineAboveActive();

//        bool enableCreateBelow( bool status_ );
//        void stopCreateBelow();
//        bool requestCreateBelow();
//        bool isDefineBelowActive();


//        bool isDefineAboveObjectSelected();
//        bool isDefineBelowObjectSelected();


//        void setObjectAsBoundering( std::size_t index_ );
//        void setObjectsAsSelectable( std::vector< std::size_t >& indexes_, bool status_ );
//        void setObjectAsSelected( std::size_t index_, bool status_ );


//        void saveFile( const std::string& filename );
//        void loadFile( const std::string& filename, Controller::MeshResolution& resol_ );
//        void loadObjects( const std::string& filename, Controller::MeshResolution& resol_ );


//        bool undo();
//        bool canUndo();

//        bool redo();
//        bool canRedo();


//        void getOutputVolume( std::map< std::size_t, Volume::Color >& regions_map_ );


//        void clear();


//        bool addFixedCrossSection( double depth_ );
//        bool removeFixedCrossSection( double depth_ );


//        void setSurfacesMeshes( std::vector< TriangleMesh >& triangles_meshes, std::vector< CurveMesh >& left_curves, std::vector< CurveMesh >& right_curves,
//                                std::vector< CurveMesh > & front_curves, std::vector< CurveMesh >& back_curves );

//        inline void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
//        {
//            rules_processor.getLegacyMeshes( points, nu, nv, num_extrusion_steps );
//        }

//        inline void getVolumeDimension(double& width, double& height, double&  depth)
//        {
//            rules_processor.getLenght( width, height, depth );
//        }




//        std::vector<int> getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces );


//        void hideRegions();

//        bool saveObjectsMetaData( const std::string& filename );
//        void loadObjectMetaDatas( QFile& load_file/*const std::string& filename*/ );
//        void loadObjectNoMetaDatas();

        void setMeshResolution( const Controller::MeshResolution& resolution_ );

//        void setPreviewEnabled( bool status_ );
//        bool isPreviewEnabled() const;

//        void exportToIrapGrid();


    protected:


        void getCurvesFromRulesProcessorDirectionX( const std::size_t& index_, bool is_preview_ = false );
        void getCurvesFromRulesProcessorDirectionZ( const std::size_t& index_, bool is_preview_ = false );



    protected:


        RRMApplication* app = nullptr;
        CrossSectionPtr csection;

        struct Model
        {
            VolumePtr volume;

            std::map< double, CrossSectionPtr > csectionsX;
            std::map< double, CrossSectionPtr > csectionsY;
            std::map< double, CrossSectionPtr > csectionsZ;

            std::vector< double > used_csectionsX;
            std::vector< double > used_csectionsY;
            std::vector< double > used_csectionsZ;

            std::map< std::size_t, ObjectPtr > objects;
            std::map< std::size_t, RegionsPtr > regions;
            std::map< std::size_t, Domain > domains;

        } model;

        RulesProcessor rules_processor;

        std::size_t current_object = 0;
        bool object_defined = false;

        Settings::Stratigraphy::StratigraphicRules current_rule = Settings::Stratigraphy::DEFAULT_STRAT_RULES;

        double csection_stepx = 1.0;
        double csection_stepz = 1.0;

        ///=========================================================


//        Scene3d* scene3d = nullptr;
//        ObjectTree* object_tree = nullptr;



//        Volume* volume = nullptr;


//        struct Color
//        {
//            int r = 255;
//            int g = 0;
//            int b = 0;
//        } current_color;

//        Container< std::size_t, Object* > objects;
//        Container< std::size_t, Regions* > regions;


//        double current_csection = 500.0;
//        CrossSection* main_csection = nullptr;
//        CrossSection* topview_csection = nullptr;
//        Container< double, CrossSection* > fixed_csections;
//        Container< double, CrossSection* > all_csections;


//

//
//        std::vector< std::size_t > selectable_upper;
//        std::vector< std::size_t > selectable_bottom;

//        Settings::Objects::BounderingRegion boundering_region;
//        std::size_t index_upper_boundary;
//        std::size_t index_bottom_boundary;

//        struct ImageData
//        {
//            std::string file;
//            double ox;
//            double oy;
//            double x;
//            double y;
//        };


//        std::map< double, ImageData > csections_background;
//        bool preview_enabled = true;
};

#endif // CONTROLLER_H
