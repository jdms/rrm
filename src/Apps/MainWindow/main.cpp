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

#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QIcon>


#include "MainWindow.h"


std::size_t Object::index = 0;
//size_t Stratigraphy::number_of_stratigraphies = 0;

int main( int argc, char *argv[] )
{


	QApplication app(argc, argv);

	QIcon appIcon;
	appIcon.addFile(":/logos/about/rrm.png");
	app.setWindowIcon(appIcon);
	

    app.setAttribute( Qt::AA_UseDesktopOpenGL );
    app.setAttribute( Qt::AA_ShareOpenGLContexts );


    bool testingNativeDriver = app.testAttribute( Qt::AA_UseDesktopOpenGL );
    if ( !testingNativeDriver )
    {
        std::cout << "Neither, your system do not have the hardware requirement " << std::endl
    			  << "nor the driver are not load properly" << std::endl;
    	return 0;
    }

    QSurfaceFormat format;
    format.setDepthBufferSize( 16 );
    format.setSamples( 8 );

    QSurfaceFormat::setDefaultFormat( format );

/*
    QOpenGLContext* ctx1 = new QOpenGLContext();
    ctx1->setFormat( format );
    ctx1->create();

    QOpenGLContext* ctx2 = new QOpenGLContext();
    ctx2->setFormat( ctx1->format() );
    ctx2->setShareContext( ctx1 );
    ctx2->create();

    bool are_ctx_shared = QOpenGLContext::areSharing( ctx1, ctx2 );

    std::cout << "Context are being shared? " << are_ctx_shared << "\n\n" << std::flush;
*/


    MainWindow w;
    w.show();

    w.run_app();


    return app.exec();
}

