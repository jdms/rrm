#include <fstream>
#include <chrono>

#include "smodeller.hpp"
#include "sutilities.hpp"
#include "detail/planin/planin.hpp"


enum callerCodes { 
    CALLED_WITHOUT_ARGUMENTS = 1,
    CALLED_WITHOUT_OUTPUT_FILE = 2, 
    CALLED_WITH_INPUT_AND_OUTPUT_FILES = 3, 
    WRONG_USAGE = 4 
}; 

enum fileErrorCodes 
{ 
    FAILED_TO_SAVE_BINARY_FILE = 11, 
    FAILED_TO_LOAD_BINARY_FILE = 12,
    FAILED_TO_SAVE_JSON_FILE = 13, 
    FAILED_TO_LOAD_JSON_FILE = 14 
}; 

int main( int argc , char* argv[] )
{
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
        output = input + ".json";
    }
    else if( argc == CALLED_WITH_INPUT_AND_OUTPUT_FILES ) {
        input = argv[1];
        output = argv[2];
    }

    SModeller model;
    bool success = model.loadBinary(input);

    if( success ) {
        std::cout << std::endl << "The binary file was successfully read." << std::endl << std::endl;
    } 
    else {
        return FAILED_TO_LOAD_BINARY_FILE;
    }


    success = model.saveJSON(output);
    if ( success ) {
        std::cout << std::endl << "The JSON file was successfully written." << std::endl << std::endl;
    }
    else { 
        return FAILED_TO_SAVE_JSON_FILE; 
    }


    return 0;
}
