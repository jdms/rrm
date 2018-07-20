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

        void createTopViewCrossSection();
        const CrossSectionPtr& getTopViewCrossSection() const;
        void moveTopViewCrossSection( double depth_ );

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

        bool createObjectSurface();
        bool createObjectSurfaceDirectionX();
        bool createObjectSurfaceDirectionZ();




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
        void setVolumeDiscretization();
        std::size_t getCurrentDiscretization() const;
        void getCurrentRange( double& min_, double& max_ ) const;

        std::size_t indexCrossSectionX( double value_ ) const;
        std::size_t indexCrossSectionZ( double value_ ) const;

        void setRemoveAbove();
        void setRemoveAboveIntersection();
        void setRemoveBelow();
        void setRemoveBelowIntersection();
        void applyStratigraphicRule();

        void setMeshResolution( const Controller::MeshResolution& resolution_ );



    protected:


        void getCurvesFromRulesProcessorDirectionX( const std::size_t& index_, bool is_preview_ = false );
        void getCurvesFromRulesProcessorDirectionZ( const std::size_t& index_, bool is_preview_ = false );



    protected:


        RRMApplication* app = nullptr;
        CrossSectionPtr csection;
        CrossSectionPtr topview;

        struct Model
        {
            VolumePtr volume;

            std::map< double, CrossSectionPtr > csectionsX;
            std::map< double, CrossSectionPtr > csectionsY;
            std::map< double, CrossSectionPtr > csectionsZ;

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


};

#endif // CONTROLLER_H
