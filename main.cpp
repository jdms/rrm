#include <iostream>


// QT libraries
#include <QApplication>

// Project headers
#include "MainWindow.h"
#include "VTKData.h"



int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );

    MainWindow w;
    w.show();

    VTKData *data = new VTKData();

//    std::string filename = std::string( "/Users/Clarissa/Downloads/results_example9coarse.vtk" );
//    data->readUnstructuredGridFile(  filename );

    return a.exec();
}

