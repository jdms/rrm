#ifndef RRMAPPLICATION_H
#define RRMAPPLICATION_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <unordered_map>

#include <QObject>


#include "3dview/canvas3d.h"
#include "sketching/sketchwindow.h"
#include "widgets/realfeaturedslider.h"
#include "widgets/objecttree.h"
#include "widgets/pages_stack.h"


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

        enum class AxesDirection{ X, Y, Z };


        RRMApplication() = default;
        RRMApplication( MainWindow* mw_ );
        ~RRMApplication(){}


        void init();
        void setRRMDefaultValuesOnInterface();

        void changeDimensionVolume( const AxesDirection& dir_, double value_ );
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


        void initSketchingApp();


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

        const CrossSection::Direction DEFAULT_CROSSSECTION_DIRECTION = CrossSection::Direction::Z;
        const double DEFAULT_CROSSSECTION_DEPTHX = 0;
        const double DEFAULT_CROSSSECTION_DEPTHY = 0;
        const double DEFAULT_CROSSSECTION_DEPTHZ = 0;


        std::size_t discretization = 1;
        double step_csection = 1.0;



};



#endif // RRMAPPLICATION_H
