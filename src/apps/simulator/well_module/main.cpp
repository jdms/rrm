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



#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>

#include <iostream>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{

    QApplication app ( argc , argv );
    /// Selecting the Desktop Driver instead the software one
    /// @see https://forum.qt.io/topic/51574/opengl-dynamic-selected-driver/3
    app.setAttribute ( Qt::AA_UseDesktopOpenGL );
    app.setAttribute ( Qt::AA_DontCreateNativeWidgetSiblings );

    bool testingNativeDriver = app.testAttribute ( Qt::AA_UseDesktopOpenGL );

    if ( !testingNativeDriver )
    {
        std::cout << "Neither, your system do not have the hardware requirement (OpenGL 4.3+) " << std::endl << "nor the driver are not load properly" << std::endl;
        return 0;
    }

    // FIXME For some how, systems with dual GPU don't work with this line.
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType ( QSurfaceFormat::OpenGL );
    format.setSamples ( 16 );
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat ( format );

    MainWindow form;
    form.show();

    return app.exec();
}
