#include <fstream>

#include "smodeller.hpp"
#include "sutilities.hpp"

bool exportCurves( std::vector<std::vector<double>> &vlists, std::string filename );

int main()
{
    SModeller model;

    model.loadBinary("model.rrm");
    SUtilities u(model);

    std::vector<std::vector<double>> vlists;
    std::vector<std::vector<size_t>> elists;

    u.getFrontBoundary3DCurves(vlists, elists);
    exportCurves(vlists, "front");

    u.getBackBoundary3DCurves(vlists, elists);
    exportCurves(vlists, "back");

    u.getLeftBoundary3DCurves(vlists, elists);
    exportCurves(vlists, "left");

    u.getRightBoundary3DCurves(vlists, elists);
    exportCurves(vlists, "right");

    return 0;
}

bool exportCurves( std::vector<std::vector<double>> &vlists, std::string filename )
{
    std::ofstream ofile(filename + "_interior.mat");
    if ( !ofile.good() )
    {
        std::cout << "Error: could not open export file.\n";
        return -1;
    }

    for ( size_t i = 0; i < vlists.size() -2; ++i )
    {
        std::cout << "Exporting curve: " << i << ", curve size is: " << vlists[i].size() << "\n";
        for ( size_t j = 0; j < vlists[i].size(); ++j )
        {
            ofile << vlists[i].operator[](j) << " ";
        }
        ofile <<  "\n";
    }

    ofile = std::ofstream(filename + "_lateral.mat");
    if ( !ofile.good() )
    {
        std::cout << "Error: could not open export file.\n";
        return -1;
    }

    for ( size_t i = vlists.size()-2; i < vlists.size(); ++i )
    {
        std::cout << "Exporting curve: " << i << ", curve size is: " << vlists[i].size() << "\n";
        for ( size_t j = 0; j < vlists[i].size(); ++j )
        {
            ofile << vlists[i].operator[](j) << " ";
        }
        ofile <<  "\n";
    }

    return 0;
}

