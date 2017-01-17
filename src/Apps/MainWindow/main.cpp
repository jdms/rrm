#include <iostream>

#include <QtWidgets/QApplication>
#include <QtGui/QSurfaceFormat>
#include <QtGui/QIcon>


#include "MainWindow.h"



size_t Stratigraphy::number_of_stratigraphies = 0;

int main( int argc, char *argv[] )
{


	QApplication app(argc, argv);

	/// Added icon on main window
	QIcon appIcon;
	appIcon.addFile(":/logos/about/rrm.png");
	app.setWindowIcon(appIcon);
	

    app.setAttribute( Qt::AA_UseDesktopOpenGL );

    bool testingNativeDriver = app.testAttribute( Qt::AA_UseDesktopOpenGL );
    if ( !testingNativeDriver )
    {
        std::cout << "Neither, your system do not have the hardware requirement " << std::endl
    			  << "nor the driver are not load properly" << std::endl;
    	return 0;
    }


	/// Create a defaut OpenGL context to use toward the application
    QSurfaceFormat format;
    format.setDepthBufferSize( 16 );

//	format.setStencilBufferSize ( 8 );
//	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
//	format.setOptions(QSurfaceFormat::DeprecatedFunctions);
    //format.setVersion( 4, 1 );
    //format.setProfile( QSurfaceFormat::CompatibilityProfile );
//	format.setProfile(QSurfaceFormat::OpenGLContextProfile::CompatibilityProfile);
//    format.setRenderableType( QSurfaceFormat::OpenGL );

    format.setSamples( 8 );
	QSurfaceFormat::setDefaultFormat( format );

	/// Lauch the application
    MainWindow w;
    w.show();

    return app.exec();
}

