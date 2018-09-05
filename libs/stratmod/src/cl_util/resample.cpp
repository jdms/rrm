#include <fstream>
#include <chrono>
#include <string>

#include "smodeller.hpp"
#include "sutilities.hpp"
#include "detail/planin/planin.hpp"

enum callerCodes { 
    CALLED_WITHOUT_ARGUMENTS = 1,
    CALLED_WITHOUT_OUTPUT_FILE = 2, 
    CALLED_WITH_INPUT_AND_OUTPUT_FILES = 3, 
    WRONG_USAGE = 4 
}; 

class SModellerWrap : public SModeller {
    public:
        bool changeDiscretization( size_t numX, size_t numY ) {
            return SModeller::changeDiscretization(numX, numY);
        }
};

int main( int argc, char **argv )
{

    size_t numX = 128;
    size_t numY = 128;

    std::string file_sufix = "_" + std::to_string(numX) + "x"  + std::to_string(numY) + ".rrm";

    std::string progname = argv[0];
    std::string input, output;

    if( argc == CALLED_WITHOUT_ARGUMENTS ) {
        argc = WRONG_USAGE;
    }

    if( argc >= WRONG_USAGE ) { 
        std::cout << "\nUsage: " + progname + " input[.rrm] [output], where [output] is optional." << std::endl << std::endl;
        
        return WRONG_USAGE;
    } 
    else if( argc == CALLED_WITHOUT_OUTPUT_FILE ) { 
        input = argv[1];
        output = input + file_sufix; 
    }
    else if( argc == CALLED_WITH_INPUT_AND_OUTPUT_FILES ) {
        input = argv[1];
        output = argv[2];
    }

    SModellerWrap model;
    bool status = model.loadBinary(input) || model.loadJSON(input);
    if ( !status )
    {
        std::cout << "Failed to load input file\n";
        return 11;
    }

    model.changeDiscretization(numX, numY);
    model.saveBinary(output);

    if ( !status )
    {
        std::cout << "Failed to resample model\n";
        return 12;
    }

    return 0;
}
/* int exportCurves( std::vector<std::vector<double>> &vlists, std::string filename ); */

/* int main() */
/* { */
/*     SModeller model; */

/*     model.loadBinary("model.rrm"); */
/*     SUtilities u(model); */

/*     std::vector<std::vector<double>> vlists; */
/*     std::vector<std::vector<size_t>> elists; */

/*     u.getFrontBoundary3DCurves(vlists, elists); */
/*     exportCurves(vlists, "front"); */

/*     u.getBackBoundary3DCurves(vlists, elists); */
/*     exportCurves(vlists, "back"); */

/*     u.getLeftBoundary3DCurves(vlists, elists); */
/*     exportCurves(vlists, "left"); */

/*     u.getRightBoundary3DCurves(vlists, elists); */
/*     exportCurves(vlists, "right"); */

/*     return 0; */
/* } */

/* int exportCurves( std::vector<std::vector<double>> &vlists, std::string filename ) */
/* { */
/*     std::ofstream ofile(filename + "_interior.mat"); */
/*     if ( !ofile.good() ) */
/*     { */
/*         std::cout << "Error: could not open export file.\n"; */
/*         return -1; */
/*     } */

/*     for ( size_t i = 0; i < vlists.size() -2; ++i ) */
/*     { */
/*         std::cout << "Exporting curve: " << i << ", curve size is: " << vlists[i].size() << "\n"; */
/*         for ( size_t j = 0; j < vlists[i].size(); ++j ) */
/*         { */
/*             ofile << vlists[i].operator[](j) << " "; */
/*         } */
/*         ofile <<  "\n"; */
/*     } */

/*     ofile = std::ofstream(filename + "_lateral.mat"); */
/*     if ( !ofile.good() ) */
/*     { */
/*         std::cout << "Error: could not open export file.\n"; */
/*         return -1; */
/*     } */

/*     for ( size_t i = vlists.size()-2; i < vlists.size(); ++i ) */
/*     { */
/*         std::cout << "Exporting curve: " << i << ", curve size is: " << vlists[i].size() << "\n"; */
/*         for ( size_t j = 0; j < vlists[i].size(); ++j ) */
/*         { */
/*             ofile << vlists[i].operator[](j) << " "; */
/*         } */
/*         ofile <<  "\n"; */
/*     } */

/*     return 0; */
/* } */

