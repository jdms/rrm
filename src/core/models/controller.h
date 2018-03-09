#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <vector>
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



class Controller
{
    public:

        /* struct TriangleMesh */
        /* { */
        /*     std::vector<double> vertex_list; */
        /*     std::vector<std::size_t> face_list; */
        /* }; */

        /* struct CurveMesh */
        /* { */
        /*     std::vector<double> vertex_list; */
        /*     std::vector<std::size_t> edge_list; */
        /* }; */

        using TriangleMesh = RulesProcessor::TriangleMesh;
        using CurveMesh = RulesProcessor::CurveMesh;


        enum class MeshResolution{ LOW, MEDIUM, HIGH };


        Controller() = default;


        void setScene3d( Scene3d* const& sc_ );
        void setObjectTree( ObjectTree* const& ot_ );

        void setCurrentColor( int r, int g, int b );
        void getCurrentColor( int& r, int& g, int& b ) const ;


        void init();


        void addVolume();

        void setVolumeOrigin( double ox_, double oy_, double oz_ );
        void getVolumeOrigin( double& ox_, double& oy_, double& oz_ ) const;

        void setVolumeDimensions( const double& width_, const double& height_, const double& length_ );
        void getVolumeDimensions( double& width_, double& height_, double& length_ ) const;

        void setVolumeName( const std::string& name_ );
        const std::string& getVolumeName() const ;

        bool isVolumeResizable() const;

        void setVolumeVisibility( bool status_ );
        bool getVolumeVisibility() const;
        void setupCrossSectionDiscretization( std::size_t& disc_, double& step_ );


        void addMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );
        CrossSection* getMainCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_ ) const;

        void addTopViewCrossSection( double depth_ );
        CrossSection* getTopViewCrossSection() const;


        void setCurrentCrossSection( double depth_ );
        void updateCurrentCrossSection();


        void setImageCrossSection( double depth_, const std::string& path_, double ox_, double oy_, double x_, double y_ );
        bool hasImageCrossSection( double depth_ );
        bool clearImageCrossSection( double depth_ );
        bool getImageCrossSection( double depth_, std::string& path_, double& ox_, double& oy_, double& x_, double& y_ );


        void setTopViewImage( std::string file_, double ox_, double oy_, double x_, double y_ );
        void removeTopViewImage();


        CrossSection* getCrossSection( const double& depth_ ) const;
        CrossSection* getCurrentCrossSection() const;



        bool addObject( std::size_t index_ = 9999 );
        Object* getObject( std::size_t index_ ) const;
        Object* getCurrentObject() const;

        void setObjectName( std::size_t index_, const std::string& name_ );
        std::string getObjectName( std::size_t index_) const;

        void setObjectVisibility( std::size_t index_, bool status_ );
        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );

        void saveObjectInformation( std::size_t index_, const std::string & text_ );
        const std::string& getObjectInformation( std::size_t index_ ) const;


        bool addObjectCurve( PolyCurve curve_, double depth_ );
        bool removeObjectCurve( std::size_t index_, double depth_ );
        bool removeObjectCurve( double depth_ );

        bool addObjectTrajectory( PolyCurve curve_ );
        void removeObjectTrajectory();


        bool createPreviewSurface();
        bool createObjectSurface();


        void setActiveAllObjects( bool status_ );


        void setRegionName( std::size_t index_, const std::string& name_ );
        void setRegionVisibility( std::size_t index_, bool status_ );
        void setRegionColor( std::size_t index_, int r_, int g_, int b_ );
        void getRegionColor( std::size_t index_, int& r_, int& g_, int& b_ );


        void initRulesProcessor();
        void updateBoundingBoxRulesProcessor();
        void getCurveFromRulesProcessor( Object* obj_, double csection_depth_ );


        void updateObjectCurveFromCrossSection( std::size_t object_id_, double csection_depth_ );
        void updatePreviewCurves( Object* obj_, double csection_depth_ );
        void updateObjectSurfaces( std::size_t object_id_ );
        void updateObjectInFixedCrossSections( std::size_t id_ );
        void updateModel();


        void setRemoveAbove();
        void setRemoveAboveIntersection();
        void setRemoveBelow();
        void setRemoveBelowIntersection();
        void setTruncate();
        void applyStratigraphicRule();



        bool enableCreateAbove( bool status_ );
        void stopCreateAbove();
        bool requestCreateAbove();
        bool isDefineAboveActive();

        bool enableCreateBelow( bool status_ );
        void stopCreateBelow();
        bool requestCreateBelow();
        bool isDefineBelowActive();


        bool isDefineAboveObjectSelected();
        bool isDefineBelowObjectSelected();


        void setObjectAsBoundering( std::size_t index_ );
        void setObjectsAsSelectable( std::vector< std::size_t >& indexes_, bool status_ );
        void setObjectAsSelected( std::size_t index_, bool status_ );


        void saveFile( const std::string& filename );
        void loadFile( const std::string& filename, Controller::MeshResolution& resol_ );
        void loadObjects( const std::string& filename, Controller::MeshResolution& resol_ );


        bool undo();
        bool canUndo();

        bool redo();
        bool canRedo();


        void getOutputVolume( std::map< std::size_t, Volume::Color >& regions_map_ );


        void clear();


        bool addFixedCrossSection( double depth_ );
        bool removeFixedCrossSection( double depth_ );


        void setSurfacesMeshes( std::vector< TriangleMesh >& triangles_meshes, std::vector< CurveMesh >& left_curves, std::vector< CurveMesh >& right_curves,
                                std::vector< CurveMesh > & front_curves, std::vector< CurveMesh >& back_curves );

        inline void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps )
        {
            rules_processor.getLegacyMeshes( points, nu, nv, num_extrusion_steps );
        }

        inline void getVolumeDimension(double& width, double& height, double&  depth)
        {
            rules_processor.getLenght( width, height, depth );
        }


        std::size_t indexCrossSection( double value_ ) const;
        double depthCrossSection( std::size_t index_ ) const;

        std::vector<int> getTetrahedronsRegions( const std::vector< float >& vertices, const std::vector< unsigned int >& edges, const std::vector< unsigned int >& faces );


        void hideRegions();

        bool saveObjectsMetaData( const std::string& filename );
        void loadObjectMetaDatas( QFile& load_file/*const std::string& filename*/ );
        void loadObjectNoMetaDatas();

        void setMeshResolution( const Controller::MeshResolution& resolution_ );

        void setPreviewEnabled( bool status_ );
        bool isPreviewEnabled() const;

        void exportToIrapGrid();

    protected:


        Scene3d* scene3d = nullptr;
        ObjectTree* object_tree = nullptr;


        Volume* volume = nullptr;


        struct Color
        {
            int r = 255;
            int g = 0;
            int b = 0;
        } current_color;

        std::size_t current_object = 0;
        Container< std::size_t, Object* > objects;
        Container< std::size_t, Regions* > regions;


        double current_csection = 500.0;
        CrossSection* main_csection = nullptr;
        CrossSection* topview_csection = nullptr;
        Container< double, CrossSection* > fixed_csections;
        Container< double, CrossSection* > all_csections;


        RulesProcessor rules_processor;
        double csection_step = 1.0;

        Settings::Stratigraphy::StratigraphicRules current_rule = Settings::Stratigraphy::DEFAULT_STRAT_RULES;
        std::vector< std::size_t > selectable_upper;
        std::vector< std::size_t > selectable_bottom;

        Settings::Objects::BounderingRegion boundering_region;
        std::size_t index_upper_boundary;
        std::size_t index_bottom_boundary;

        struct ImageData
        {
            std::string file;
            double ox;
            double oy;
            double x;
            double y;
        };


        std::map< double, ImageData > csections_background;
        bool preview_enabled = true;
};

#endif // CONTROLLER_H
