#include <iostream>

#include <QApplication>

#include "FlowWindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    FlowWindow main_window;
    main_window.show();

    return app.exec();
}
