#include <iostream>


// Project headers
#include "MainWindow.h"

// QT libraries
#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    MainWindow w;
    w.show();

    return a.exec();
}

