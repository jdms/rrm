#include <fstream>
#include <chrono>

#include "stratmod/smodeller.hpp"
#include "stratmod/sutilities.hpp"
#include "planin/planin.hpp"

enum callerCodes { 
    CALLED_WITHOUT_ARGUMENTS = 1,
    CALLED_WITHOUT_OUTPUT_FILE = 2, 
    CALLED_WITH_INPUT_AND_OUTPUT_FILES = 3, 
    WRONG_USAGE = 4 
}; 

int main( int argc, char **argv )
{

    size_t numX = 256;
    size_t numY = 256;

    // A file suffix is not needed as SModeller::exportToVTK() adds it automatically
    std::string file_sufix = ""; // ".vtk";

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

    SModeller model;
    bool status = model.loadBinary(input) || model.loadJSON(input);
    if ( !status )
    {
        std::cout << "Failed to load input file\n";
        return 11;
    }

    // Actual logic of utility
    model.changeDiscretization(numX, numY);
    SUtilities u(model);

    auto time1 = std::chrono::high_resolution_clock::now();

    status = u.exportToVTK(output);

    auto time2 = std::chrono::high_resolution_clock::now();

    // Final tests
    if ( !status )
    {
        std::cout << "Failed to export mesh\n";
        return 12;
    }

    std::cout << "Time to build mesh and export file: " << std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count() << "\n";

    return 0;
}

