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
#include "./core/models/object.h"


int main( int argc, char *argv[] )
{

	QApplication app(argc, argv);
    app.setAttribute( Qt::AA_UseDesktopOpenGL );
    app.setAttribute( Qt::AA_ShareOpenGLContexts );
    //app.setAttribute( Qt::AA_EnableHighDpiScaling );

    QSurfaceFormat format; 
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setMajorVersion( 4 );
    format.setMinorVersion( 1 );
    /* format.setDepthBufferSize( 16 ); */
    format.setSamples( 4 );

    QSurfaceFormat::setDefaultFormat( format );

    bool testingNativeDriver = app.testAttribute( Qt::AA_UseDesktopOpenGL );
    if ( !testingNativeDriver )
    {
        std::cout << "Failure to initialize OpenGL: either your system does not have the hardware requirements, or " << std::endl
                  << "the GPU driver was not loaded properly" << std::endl;
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}

