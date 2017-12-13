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


#include "3dview/canvas3d.h"
#include "sketching/sketchwindow.h"
#include "simulator/flow_window.h"
#include "./core/definitions/constants.hpp"
#include "./core/widgets/realfeaturedslider.h"
#include "./core/widgets/objecttree.h"
#include "./core/widgets/pages_stack.h"


enum class AppsCommands{ NEW, SAVE, LOAD, UNDO, REDO };
enum class CommonCommands{ ADD, REMOVE, SET, GET };
enum class CommonProperties{ NAME, COLOR, INDEX, CURRENT, VISIBLE, SELECTABLE, SELECTED, ACTIVE};
enum class CommonObjects{ VOLUME, STRATIGRAPHY, CROSS_SECTION, REGION };



struct Color
{
    int r = 255;
    int b = 0;
    int g = 0;
};


template < typename TI, typename TO = void > struct FunctionType
{
    using method = std::function< TO( TI args ) >;
    using method_objs = std::function< TO( CommonObjects obj_, TI args ) >;
};


template < typename TI, typename TI1, typename TO = void > struct FunctionType1
{
    using method = std::function< void( TI args, TI1 args1 ) >;
    using method_objs = std::function< void( CommonObjects obj_, TI args, TI1 args1 ) >;

};


using simple_method = std::function< void(void) >;
using singleparams_sizet = FunctionType< std::size_t >::method_objs;
using singleparams_double = FunctionType< double >::method_objs;

using inputs_string = FunctionType1< std::size_t, std::string >::method_objs;
using inputs_bool = FunctionType1< std::size_t, bool >::method_objs;
using inputs_color = FunctionType1< std::size_t, Color >::method_objs;

using output_string = FunctionType< std::size_t, std::string >::method_objs;
using output_bool = FunctionType< std::size_t, bool >::method_objs;
using output_color = FunctionType< std::size_t, Color >::method_objs;



class MainWindow;


class RRMApplication
{


    public:


        RRMApplication() = default;
        RRMApplication( MainWindow* mw_ );
        ~RRMApplication(){}


        void init();


        void setSiderBarVisibility( bool status_ );
        void setDefaultRule( Settings::Stratigraphy::StratigraphicRules rule_ );
        void setDefaultSketchingRegion( Settings::Objects::BounderingRegion sketching_region_ );
        void setDefaultSiderBarValues();
        void setRRMDefaultValuesOnInterface();


        void changeVolumeDimension( const Settings::CrossSection::CrossSectionDirections& dir_, double value_ );
        void changeVolumeDimensions( const Settings::CrossSection::CrossSectionDirections& dir_, double dim1_, double dim2_ );

        void setVolumeOriginToController( double ox_, double oy_, double oz_ );
        void setVolumeDimensionsToController(  double width_, double height_, double length_ );
        void getVolumeDimensionsFromController() const;


        void setVolumeName( std::size_t index_, const std::string& name_ );
        void setVolumeVisible( std::size_t index_, bool status_ );



        void setCurrentCrossSection( double depth_ );



        void setObjectName( std::size_t index_, const std::string& name_ );
        void setObjectVisible( std::size_t index_, bool status_ );
        void setObjectColor( std::size_t index_, int r_, int g_, int b_ );

        void saveObjectInformation( const std::string & text_ );
        void getObjectInformation( QTreeWidgetItem* const& item_ ) const;


        void setRegionName( std::size_t index_, const std::string& name_ );
        void setRegionVisible( std::size_t index_, bool status_ );
        void setRegionColor( std::size_t index_, int r_, int g_, int b_ );



        void setStratigraphicRule( const Settings::Stratigraphy::StratigraphicRules& rules_ );
        void setSketchAbove( bool status_ );
        void setSketchBelow( bool status_ );
        void setObjectAsBoundering( std::size_t index_ );


        void initSketchingApp();
        void updateSketchingCanvas();

        void acceptSketchingCurve( const PolyCurve& curve_, double depth_ );
        void removeCurveFromObject(  double depth_, std::size_t index_ );
        void acceptSketchingTrajectory( const PolyCurve& curve_ );
        void createObjectSurface();


        void setModeSelecting();
        void setModeSketching();


        void save( const std::string& filename_ );
        void load( const std::string& filename_ );


        void undo();
        void redo();
        void checkUndoRedo();
        void checkSketchStatus();


        void setCurrentColor( int r_, int g_, int b_ );
        void defineRandomColor();


        void clearInterface();
        void clear();
        void restart();


        void addCrossSectionCanvas( double depth_ );
        void removeCrossSectionCanvas( double depth_ );


        void setImageToCrossSection( double depth_, std::string file_, double ox_, double oy_, double x_, double y_ );
        void removeImageFromCrossSection( double depth_ );


        void setImageToTopView( std::string file_, double ox_, double oy_, double x_, double y_ );
        void removeImageFromTopView();
        void getHeightMapTopView();

        void startFlowDiagnostics();
        void getLegacyMeshes( std::vector<double> &points, std::vector<size_t> &nu, std::vector<size_t> &nv, size_t num_extrusion_steps );
        void getSurfacesMeshes( std::vector< FlowWindow::TriangleMesh >& triangles_meshes, std::vector< FlowWindow::CurveMesh>& left_curves,
                                std::vector< FlowWindow::CurveMesh >& right_curves, std::vector< FlowWindow::CurveMesh > & front_curves,
                                std::vector< FlowWindow::CurveMesh >& back_curves );






    protected:


        std::unordered_map< AppsCommands, simple_method > runs;
        std::unordered_map< CommonObjects, singleparams_sizet > adds, remove;
        std::unordered_map< CommonObjects, singleparams_double > adds_double, remove_double;

        std::unordered_map< CommonProperties, output_string > getters_string;
        std::unordered_map< CommonProperties, output_bool > getters_bool;
        std::unordered_map< CommonProperties, output_color > getters_color;


        MainWindow* mainwindow = nullptr;


        const double VOLUME_WIDTH = 500;
        const double VOLUME_HEIGHT = 500;
        const double VOLUME_LENGTH = 500;


        std::size_t discretization = 1;
        double step_csection = 1.0;



};



#endif // RRMAPPLICATION_H
