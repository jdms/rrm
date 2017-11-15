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

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <unordered_map>

#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QIcon>


#include "mainwindow.h"
#include "./core/base/models/object.h"

std::size_t Object::number_of_objects = 0;
std::size_t CrossSection::number_of_csections;


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
    using method_void = std::function< void( TI args ) >;
    using method2 = std::function< TO( CommonObjects obj_, TI args ) >;
};


template < typename TI, typename TI1, typename TO = void > struct FunctionType1
{
    using method = std::function< TO( CommonObjects obj_, TI args, TI1 args1 ) >;
    using method_void = std::function< void( CommonObjects obj_, TI args, TI1 args1 ) >;
};



template < class TI, typename TI1, class TO = void >
TO run_method( CommonObjects obj_, TI args1, TI1 args )
{

}


using method_void = std::function< void(void) >;


int main( int argc, char *argv[] )
{


	QApplication app(argc, argv);
    app.setAttribute( Qt::AA_UseDesktopOpenGL );
    app.setAttribute( Qt::AA_ShareOpenGLContexts );
    app.setAttribute( Qt::AA_EnableHighDpiScaling );

    bool testingNativeDriver = app.testAttribute( Qt::AA_UseDesktopOpenGL );
    if ( !testingNativeDriver )
    {
        std::cout << "Neither, your system do not have the hardware requirement " << std::endl
                  << "nor the driver are not load properly" << std::endl;
        return 0;
    }

    QSurfaceFormat format;
    format.setDepthBufferSize( 16 );
    format.setSamples( 16 );

    QSurfaceFormat::setDefaultFormat( format );


    MainWindow w;
    w.show();


    std::unordered_map< AppsCommands, method_void > runs;
    runs[ AppsCommands::NEW ] = [](void)->void{};
    runs[ AppsCommands::SAVE ] = [](void)->void{};
    runs[ AppsCommands::LOAD ] = [](void)->void{};
    runs[ AppsCommands::UNDO ] = [](void)->void{};
    runs[ AppsCommands::REDO ] = [](void)->void{};



    using method1_sizet = FunctionType< std::size_t >::method_void;
    using method1_double = FunctionType< double >::method_void;
    std::unordered_map< CommonObjects, method1_sizet > adds, remove;
    std::unordered_map< CommonObjects, method1_double > adds_double, remove_double;



    adds[ CommonObjects::VOLUME ] = []( std::size_t )->void{};
    adds[ CommonObjects::STRATIGRAPHY ] = []( std::size_t )->void{};
    adds[ CommonObjects::REGION ] = []( std::size_t )->void{};
    adds_double[ CommonObjects::CROSS_SECTION ] = []( double )->void{};


    remove[ CommonObjects::VOLUME ] = []( std::size_t )->void{};
    remove[ CommonObjects::STRATIGRAPHY ] = []( std::size_t )->void{};
    remove[ CommonObjects::REGION ] = []( std::size_t )->void{};
    remove_double[ CommonObjects::CROSS_SECTION ] = []( double )->void{};


    using method_string = FunctionType1< std::size_t, std::string >::method_void;
    using method_bool = FunctionType1< std::size_t, bool >::method_void;
    using method_color = FunctionType1< std::size_t, Color >::method;

    std::unordered_map< CommonProperties, method_string > setters_string;
    std::unordered_map< CommonProperties, method_bool > setters_bool;
    std::unordered_map< CommonProperties, method_color > setters_color;



    setters_string[ CommonProperties::NAME ] = []( CommonObjects obj_, std::size_t, std::string )->void{};
    setters_color[ CommonProperties::COLOR ] = []( CommonObjects obj_, std::size_t, Color )->void{};
    setters_bool[ CommonProperties::VISIBLE ] = []( CommonObjects obj_, std::size_t, bool )->void{};
    setters_bool[ CommonProperties::SELECTABLE ] = []( CommonObjects obj_, std::size_t, bool )->void{};
    setters_bool[ CommonProperties::SELECTED] = []( CommonObjects obj_, std::size_t, bool )->void{};
    setters_bool[ CommonProperties::ACTIVE ] = []( CommonObjects obj_, std::size_t, bool )->void{};
    setters_bool[ CommonProperties::CURRENT ] = []( CommonObjects obj_, std::size_t, bool )->void{};
    setters_bool[ CommonProperties::INDEX ] = []( CommonObjects obj_, std::size_t, bool )->void{};



    using method_string1 = FunctionType< std::size_t, std::string >::method2;
    using method_bool1 = FunctionType< std::size_t, bool >::method2;
    using method_color1 = FunctionType< std::size_t, Color >::method2;

    std::unordered_map< CommonProperties, method_string1 > getters_string;
    std::unordered_map< CommonProperties, method_bool1 > getters_bool;
    std::unordered_map< CommonProperties, method_color1 > getters_color;

    getters_string[ CommonProperties::NAME ] = []( CommonObjects obj_, std::size_t )->std::string{ return std::string(); };
    getters_color[ CommonProperties::COLOR ] = []( CommonObjects obj_, std::size_t )->Color{ return Color(); };
    getters_bool[ CommonProperties::VISIBLE ] = []( CommonObjects obj_, std::size_t )->bool{ return true;};
    getters_bool[ CommonProperties::SELECTABLE ] = []( CommonObjects obj_, std::size_t )->bool{ return true;};
    getters_bool[ CommonProperties::SELECTED]  = []( CommonObjects obj_, std::size_t )->bool{ return true;};
    getters_bool[ CommonProperties::ACTIVE ] = []( CommonObjects obj_, std::size_t )->bool{ return true;};
    getters_bool[ CommonProperties::CURRENT ] = []( CommonObjects obj_, std::size_t )->bool{ return true; };
    getters_bool[ CommonProperties::INDEX ] = []( CommonObjects obj_, std::size_t )->bool{return true; };



//    FunctionType<int>::settters Funcp = run_setters<int>;
//    Funcp( "index", "stratigraphy", 2 );


    return app.exec();
}

