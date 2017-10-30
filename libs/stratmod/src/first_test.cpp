#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "smodeller.hpp"
#include "sutilities.hpp"

#define TESTING
#include "testing_definitions.hpp" 
#include "smodeller.cpp" 
#include "sutilities.cpp"


/*
 * Helper functions, implemented below main().
 */


/* Get a surface (point data) created in matlab and load it into a vector of
 * double.
 *
 * Data is stored in an ascii file, for which each line contains the
 * coordinates (x, y, z) of a surface's vertex
 */
bool load_surface( std::string filename, std::vector<double> &point_data );


/* Write a processed stratigraphic surface into a file for visualization in
 * matlab.
 *
 * Data is stored in an ascii file, in a format suitable for being read by the
 * matlab script 'testing_model/view_model.m'
 */
bool write_surface( std::string filename, std::vector<double> &point_data,
        size_t numX, size_t numY ); 



/* In the code below, the macros:
 *
 * 'TEST_CASE(...)', 'INFO(...)', 'WARN(...)', 'FAIL(...)', and 'REQUIRE(...)'
 *
 * are defined by the catch/catch.hpp library for unit-testing, ignore them for
 * the sake of this example.
 */

TEST_CASE( "class StratigraphyModeller first use", "[modeller]" )
{
    bool status = true; 

    /************************************************/
    /* Load surface data from files.                */
    /************************************************/

    std::vector<double> top_surface, channel, interior_surface;

    INFO( "Trying to load surface_top.txt... (did you copy it to the current dir?)" );
    status &= load_surface("surface_top.txt", top_surface);
    REQUIRE( status == true );

    INFO( "Trying to load surface_channel.txt... (did you copy it to the current dir?)" );
    status &= load_surface("surface_channel.txt", channel);
    REQUIRE( status == true );

    INFO( "Trying to load surface_inside_channel.txt... (did you copy it to the current dir?)" );
    status &= load_surface("surface_inside_channel.txt", interior_surface); 
    REQUIRE( status == true );

    if ( status == false )
    {
        FAIL( "Error loading input files" );
    }



    /************************************************/
    /* Create modeller and apply geologic rules.    */
    /************************************************/

    StratigraphyModeller modeller; 

    /* A call to the tryChangeDiscretization(...) method will only return true if
     * the model is empty.
     */
    modeller.tryChangeDiscretization(128, 128);

    /* In the defaul coordinate systems, the axis are as follows: 
     *
     *        ^ z "height"
     *        |
     *        |
     *        |
     *        o-----> y "width"
     *       /
     *      /
     *     /
     *    x "depth"
     */
    modeller.useDefaultCoordinateSystem();

    modeller.setOrigin(0., 0., 0.);
    modeller.setLenght(1., 1., 1.);


    /* We begin the creation of the model by creating what should be the
     * surface inside the channel. 
     *
     * We (arbitrarily) attribute to this surface the number 0. 
     */
    INFO( "Trying to create surface 0" );
    status &= modeller.createSurface(0, interior_surface);
    REQUIRE( status == true );

    /* Since we just want what is inside the channel, we choose to put the
     * modeller in the removeBellow() state. This way, once we add the channel
     * surface it will correctly trim the interior_surface. 
     */
    modeller.removeBelow();

    /* We now create the channel, and declare it to be surface number 1.
     */
    INFO( "Trying to create surface 1" );
    status &= modeller.createSurface(1, channel);
    REQUIRE( status == true );

    /* The last surface we need to add to obtain the desired model is the
     * channel's top, which is here named top_surface.  To make the modeller
     * trim everything above the top_surface (when it is created in the next
     * step) we set the modeller into state removeAbove().
     */
    modeller.removeAbove();

    /* We finally create the channel's top, and allow the modeller to take care
     * of everything.  We choose to number the top_surface as surface number 2.
     */
    INFO( "Trying to create surface 2" );
    status &= modeller.createSurface(2, top_surface);
    REQUIRE( status == true );

    /* To get surface data from the modeller use the surface number you've
     * chosen when the surface was created.
     *
     * Here, the processed vertex lists will be saved in the same vectors that
     * were used for the input data, for simplicity.
     */
    INFO( "Trying to get surface 0 vertex list" );
    status &= modeller.getVertexList(0, interior_surface); 
    REQUIRE( status == true );

    INFO( "Trying to get surface 1 vertex list" );
    status &= modeller.getVertexList(1, channel); 
    REQUIRE( status == true );

    INFO( "Trying to get surface 2 vertex list" );
    status &= modeller.getVertexList(2, top_surface); 
    REQUIRE( status == true );

    if ( status == false )
    {
        FAIL( "Error using class StratigraphyModeller" );
    }



    /************************************************/
    /* Write processed surface data to files.       */
    /************************************************/

    size_t numX, numY; 
    StratigraphyUtilities modeller_util(modeller);

    numX = modeller_util.getNumVerticesWidth();
    numY = modeller_util.getNumVerticesDepth();

    INFO( "Trying to write surface 0 vertex list" );
    status &= write_surface( "output_surface_inside_channel.txt", interior_surface, numX, numY );
    REQUIRE( status == true );

    INFO( "Trying to write surface 1 vertex list" );
    status &= write_surface( "output_surface_channel.txt", channel, numX, numY ); 
    REQUIRE( status == true );

    INFO( "Trying to write surface 2 vertex list" );
    status &= write_surface( "output_surface_top.txt", top_surface, numX, numY );
    REQUIRE( status == true );

    if ( status == false )
    {
        FAIL( "Error writing output files" );
    }

} // TEST_CASE( "class StratigraphyModeller main functions", "[modeller]" )



/************************************************/
/* Implementations.                             */
/************************************************/

bool load_surface( std::string filename, std::vector<double> &point_data )
{
    std::ifstream file; 
    file.open( filename ); 
    if ( !file.good() )
    {
        std::cerr <<  "\nERROR: file " << filename << " could not be oppended. Have you copied it to the current dir?\n\n" << std::flush;
        return false; 
    }

    point_data.clear(); 

    std::istringstream ss; 
    std::string line; 
    double val;

    /* The input file is just a succession of ascii lines containing 3 doubles each */
    while( std::getline(file, line) )
    {
        try 
        {
            ss.clear(); 
            ss.str(line);

            ss >> val; point_data.push_back(val);
            ss >> val; point_data.push_back(val);
            ss >> val; point_data.push_back(val);
        }

        catch( const std::exception &e )
        {
            std::cerr << "\nERROR exception caught parsing file: << " << filename << "\n\n" << std::flush; 
            return false; 
        }
    }

    return true;
}



bool write_surface( std::string filename, std::vector<double> &point_data, size_t numX, size_t numY )
{
    /* point_data should contain numX*numY 3D points */ 
    if ( point_data.size() != 3 * numX*numY )
    {
        std::cerr << "\nERROR: point_data.size() does not match discretization size.\n\n" << std::flush;
        return false;
    }

    std::ofstream file;
    file.open( filename );
    if ( !file.good() )
    {
        std::cerr <<  "\nERROR: file " << filename << " could not be oppended.\n\n" << std::flush;
        return false; 
    }

    /* Given a pair ( 0 <= i < numX, 0 <= j < numY ) of a vertex's linear coordinates, 
     * compute the index such vertex should have.  
     *
     * This code is based on class PlanarSurface implementation. 
     * */
    auto getIndex = [ numX, numY ]( size_t i, size_t j ) -> size_t
    {
        return ( j + i*numY );
    };

    size_t index = 0;

    /* Let the matlab script know that we will write 3, numX*numY, matrices */ 
    file  << 3 << " " << numX << " " << numY << " ";

    if ( numY > 3 )
    {
        for ( size_t i = 0; i < numY -3; ++i ) 
        {
            file << "0." << " "; 
        }
    }
    file << std::endl;


    /* Write the x coordinates */
    for ( size_t i = 0; i < numX; ++i )
    {
        for ( size_t j = 0; j < numY; ++j )
        {
            index = getIndex(i,j); 
            try
            {
                file << point_data.at( 3*index + 0 ) << " ";
            }
            catch( const std::exception &e )
            {
                std::cerr << "\nERROR writting to file: << " << filename << "\n" << std::flush; 
                std::cerr << "while trying to access point_data[ " << 3*index + 0 << " ] " << "\n\n" << std::flush; 
                return false; 
            }
        }
        file << std::endl; 
    }

    /* Write the y coordinates */
    for ( size_t i = 0; i < numX; ++i )
    {
        for ( size_t j = 0; j < numY; ++j )
        {
            index = getIndex(i,j); 
            try
            {
                file << point_data.at( 3*index + 1 ) << " ";
            }
            catch( const std::exception &e )
            {
                std::cerr << "\nERROR writting to file: << " << filename << "\n" << std::flush; 
                std::cerr << "while trying to access point_data[ " << 3*index + 1 << " ] " << "\n\n" << std::flush; 
                return false; 
            }
        }
        file << std::endl; 
    }

    /* Write the z coordinates */
    for ( size_t i = 0; i < numX; ++i )
    {
        for ( size_t j = 0; j < numY; ++j )
        {
            index = getIndex(i,j); 
            try
            {
                file << point_data.at( 3*index + 2 ) << " ";
            }
            catch( const std::exception &e )
            {
                std::cerr << "\nERROR writing to file: << " << filename << "\n" << std::flush; 
                std::cerr << "\nTrying to access point_data[ " << 3*index + 2 << " ] " << "\n\n" << std::flush; 
                return false; 
            }
        }
        file << std::endl; 
    }

    return true;
}

