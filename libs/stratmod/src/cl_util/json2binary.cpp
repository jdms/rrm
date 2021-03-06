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
    /* int i = 0; */
    /* std::cout << i++ << "\n"; */ 
    std::string progname = argv[0];
    std::string input, output;

    if( argc == CALLED_WITHOUT_ARGUMENTS ) {
        argc = WRONG_USAGE;
    }

    /* std::cout << i++ << "\n"; */ 
    if( argc >= WRONG_USAGE ) { 
        std::cout << "\nUsage: " + progname + " input[.json] [output], where [output] is optional." << std::endl << std::endl;
        
        return WRONG_USAGE;
    } 
    else if( argc == CALLED_WITHOUT_OUTPUT_FILE ) { 
        input = argv[1];
        output = input + ".rrm"; 
    }
    else if( argc == CALLED_WITH_INPUT_AND_OUTPUT_FILES ) {
        input = argv[1];
        output = argv[2];
    }

    /* std::cout << i++ << "\n"; */ 

    SModeller model;
    bool success = model.loadJSON(input);
    if ( success ) {
        std::cout << std::endl << "The JSON file was successfully read." << std::endl << std::endl;
    }
    else { 
        std::cout << std::endl << "Failed to load JSON file." << std::endl << std::endl;
        return FAILED_TO_LOAD_JSON_FILE;
    }

    /* std::cout << i++ << "\n"; */ 

    success = model.saveBinary(output);
    if( success ) { 
        std::cout << std::endl << "The binary file was successfully written." << std::endl << std::endl;
    }
    else {
        std::cout << std::endl << "Failed to save binary file." << std::endl << std::endl;
        return FAILED_TO_SAVE_BINARY_FILE;
    }

    /* std::cout << i++ << "\n"; */ 
    
    return 0;
}

