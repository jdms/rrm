#include <iostream>


// Project headers
#include "MainWindow.h"

// QT libraries
#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    /// Selecting the Desktop Driver instead the software one
    /// @see https://forum.qt.io/topic/51574/opengl-dynamic-selected-driver/3
    app.setAttribute(Qt::AA_UseDesktopOpenGL);

    bool testingNativeDriver = app.testAttribute(Qt::AA_UseDesktopOpenGL);

    if (!testingNativeDriver)
    {
    	std::cout << "Neither, your system do not have the hardware requirement (OpenGL 4.3+) " << std::endl
    			  << "nor the driver are not load properly" << std::endl;
    	return 0;
    }


// fixme some how, systems with dual GPU don't work with this line.
    	    QSurfaceFormat format;
//    	    format.setDepthBufferSize(24);
//    	    format.setVersion(3, 3);
//    	    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    	    format.setRenderableType(QSurfaceFormat::OpenGL);
    	    format.setSamples(16);
    	    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.show();

    return app.exec();
}

