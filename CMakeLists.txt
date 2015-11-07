cmake_minimum_required ( VERSION 3.0 )

project (RRM)

MESSAGE(STATUS "Platform : " ${CMAKE_SYSTEM})

# where cmake will search for FINDME.cmake 
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/Files/CMake)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib/ )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/lib/ )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/lib/ )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin/ )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/build/bin/ )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/build/bin/ )

set(CMAKE_BUILD_TYPE Debug)
# From FreeCAD CMakeLists.txt 
OPTION(BUILD_FULL_SUITE "Build RapidReservoir Suite" ON)

#@see http://stackoverflow.com/questions/23878590/turn-off-eigen-alignment-in-the-pcl-build
add_definitions(-DEIGEN_DONT_ALIGN)

IF(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(WIN32)
    if(MSVC)
    endif(MSVC)
ENDIF()    

## - Examples using and Qt
add_subdirectory(src)
