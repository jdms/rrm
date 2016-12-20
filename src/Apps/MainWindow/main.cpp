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
    app.setAttribute( Qt::AA_DontCreateNativeWidgetSiblings );

    bool testingNativeDriver = app.testAttribute( Qt::AA_UseDesktopOpenGL );
    if ( !testingNativeDriver )
    {
        std::cout << "Neither, your system do not have the hardware requirement " << std::endl
    			  << "nor the driver are not load properly" << std::endl;
    	return 0;
    }



    QSurfaceFormat format;
//    format.setDepthBufferSize( 24 );
//    format.setVersion( 4, 1 );
//    format.setProfile( QSurfaceFormat::CompatibilityProfile );
    format.setRenderableType( QSurfaceFormat::OpenGL );
    format.setSamples( 16 );
    QSurfaceFormat::setDefaultFormat( format );


    MainWindow w;
    w.show();

    return app.exec();
}

