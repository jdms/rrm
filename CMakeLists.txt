cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRM )


###############################################
# CMake Options
###############################################

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
set( CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/ )



###############################################
# Options
###############################################

set(RRM_BUILD_TYPE "Debug" CACHE STRING
    "Set build type to: Debug, Release, RelWithDebInfo, or MinSizeRel")
set(CMAKE_BUILD_TYPE "${RRM_BUILD_TYPE}" CACHE INTERNAL "" FORCE)

OPTION( BUILD_FULL_SUITE "Build RapidReservoir Suite" ON )



###############################################
# Compiler flags (we support Clang (3.7+),
# Gcc (4.8+) and VS2013 (revision 5+)).
###############################################

IF( WIN32 )

    if( MSVC )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /openmp /EHsc")
    endif()

ELSEIF( UNIX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2"  )
#            -fopenmp")

ELSE()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()


################################################
## External libraries
################################################


include_directories( ${CMAKE_SOURCE_DIR}/libs )
include_directories( "./" )

OPTION( RRM_DIRECTORY_LIBRARIES "Setup automatically libraries" ON )

if( RRM_DIRECTORY_LIBRARIES )

    set( Qt5_DIR "C:/Qt/5.8/msvc2013/lib/cmake/Qt5" )
    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
    set( GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/GLEW/include )
    set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/x32/glew32.lib )

endif()


find_package( OpenGL REQUIRED )
if( OpenGL_FOUND )
    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
endif( OpenGL_FOUND )

find_package( GLEW REQUIRED )
include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )

find_package( EIGEN3 REQUIRED )
include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
add_definitions( -DEIGEN_DONT_ALIGN )

find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL REQUIRED )
include_directories( SYSTEM ${Qt5_DIR} )
include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )

#set( TUCANO_DIR ${CMAKE_SOURCE_DIR}/libs/Tucano )
#INCLUDE_DIRECTORIES( SYSTEM ${TUCANO_DIR} TUCANO_DIR)

#add_subdirectory( ${CMAKE_SOURCE_DIR}/libs/PlanInLib EXCLUDE_FROM_ALL )


################################################
## Preliminary support for stratmod library
################################################

#include(ExternalProject)  ## needed

#set(BUILD_USING_OPENMP ON)

#set(STRATMOD_SOURCE ${CMAKE_SOURCE_DIR}/libs/stratmod)

#set(STRATMOD_CMAKE_ARGS
#    -DBUILD_USING_OPENMP=${BUILD_USING_OPENMP}
#    -DBUILD_SHARED_LIBRARY=OFF
#    -DSTRATMOD_BUILD_TYPE=${RRM_BUILD_TYPE}
#    )

#if(BUILD_USING_OPENMP AND APPLE)
#    set(STRATMOD_CMAKE_ARGS
#        -DBUILD_USING_OPENMP=ON
#        -DBUILD_SHARED_LIBRARY=OFF
#        -DSTRATMOD_BUILD_TYPE=${RRM_BUILD_TYPE}
#        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_MODULE_PATH}/Darwin_Clang-OpenMP_Toolchain.cmake
#        )
#endif()

#ExternalProject_Add(
#    StratModLibBuild
#    DOWNLOAD_COMMAND git submodule update --init
#    UPDATE_COMMAND git submodule update -f
#    SOURCE_DIR ${STRATMOD_SOURCE}
#    CMAKE_ARGS ${STRATMOD_CMAKE_ARGS}
#    )


#set(STRATMODLIB "${STRATMOD_SOURCE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}stratmod${CMAKE_STATIC_LIBRARY_SUFFIX}")

#message(WARNING ${STRATMODLIB})

#add_library(stratmod STATIC IMPORTED)
#set_target_properties(stratmod PROPERTIES IMPORTED_LOCATION ${STRATMODLIB})
#add_dependencies(stratmod StratModLibBuild)
#include_directories(${STRATMOD_SOURCE})

#message(WARNING stratmod)



###############################################
# Main source
###############################################

add_subdirectory( src )

