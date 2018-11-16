cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRM )


###############################################
# CMake Options
###############################################

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
#set( CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build )


###############################################
# Options
###############################################

set(RRM_BUILD_TYPE "Debug" CACHE STRING
    "Set build type to: Debug, Release, RelWithDebInfo, or MinSizeRel")
set(CMAKE_BUILD_TYPE "${RRM_BUILD_TYPE}" CACHE INTERNAL "" FORCE)

OPTION( BUILD_FULL_SUITE "Build RapidReservoir Suite" ON )

OPTION( BUILD_SHARED_STRATMODLIB "Build a shared library from stratmod" ON )

OPTION( BUILD_IN_PREFIXED_BINARY_DIR "To simplify cleaning in Windows" ON)

if(BUILD_IN_PREFIXED_BINARY_DIR)
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )
endif()

###############################################
# Compiler flags (we support Clang (3.7+),
# Gcc (4.8+) and VS2013 (revision 5+)).
###############################################

# BUG: the following was commented out because the compiler flags were 
#      creating havoc in windows.  Moreover, such flags are redundant since
#      we are setting the build type directly in cmake.

IF( WIN32 )

    if( MSVC )
        #        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /openmp /EHsc")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp /EHsc")
    endif()

ELSEIF( UNIX )
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2"  )
    #            -fopenmp")

ELSE()
    message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()


################################################
## External libraries
################################################

include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets )
include_directories( ${CMAKE_BINARY_DIR}/src/apps/simulator)
include_directories( ${CMAKE_BINARY_DIR}/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_BINARY_DIR}/simulator/FlowDiagnosticsApp_autogen/include_Debug)
include_directories( ${CMAKE_BINARY_DIR}/simulator/FlowDiagnosticsApp_autogen/include_Release)
include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets/RRMWidgets_autogen/include )
include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets/RRMWidgets_autogen/include_Debug )
include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets/RRMWidgets_autogen/include_Release )

include_directories( ${CMAKE_SOURCE_DIR}/libs )
include_directories( "./" )

# The following code prints all directories currently included in the project
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
# endforeach()

OPTION( RRM_DIRECTORY_LIBRARIES "Setup automatically libraries" ON )

if( RRM_DIRECTORY_LIBRARIES )


    set( Qt5_DIR "/opt/Qt/5.12.2/gcc_64/lib/cmake/Qt5" )
    #set( Qt5_DIR "C:/Qt/5.8/msvc2017_64/lib/cmake/Qt5" )
    #set( Qt5_DIR "C:/Qt/5.10.1/msvc2017_64/lib/cmake/Qt5" )
#    set( Qt5_DIR "C:/Qt/5.8/msvc2013/lib/cmake/Qt5" )
    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )

endif()


find_package( OpenGL REQUIRED )
if( OpenGL_FOUND )
    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
endif( OpenGL_FOUND )

if(WIN32)
    set( GLEW_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs/GLEW/include )
    set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/x64/glew32.lib )
    #set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/x32/glew32.lib )
    set( GLEW_LIBRARIES ${GLEW_LIBRARY} )
else()
    find_package( GLEW REQUIRED )
endif()
include_directories( SYSTEM ${GLEW_INCLUDE_DIRS} )

find_package( Eigen3 REQUIRED )
include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
add_definitions( -DEIGEN_DONT_ALIGN )

find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL REQUIRED )
include_directories( SYSTEM ${Qt5_DIR} )
include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )

set( TUCANO_DIR ${CMAKE_SOURCE_DIR}/libs/Tucano )
include_directories( SYSTEM ${TUCANO_DIR} TUCANO_DIR)

add_subdirectory( ${CMAKE_SOURCE_DIR}/libs/PlanInLib EXCLUDE_FROM_ALL )


if(WIN32)
    set( HYPRE_DIR ${CMAKE_SOURCE_DIR}/libs/hypre64 )
    include_directories( HYPRE_INCLUDE_DIR ${HYPRE_DIR}/include )
    find_library( HYPRE HYPRE ${HYPRE_DIR}/lib )
else()
    find_package(OpenMP REQUIRED)
    set( HYPRE_DIR ${CMAKE_SOURCE_DIR}/libs/hypre64 )
    include_directories( HYPRE_INCLUDE_DIR ${HYPRE_DIR}/include )
    find_library( HYPRE HYPRE ${HYPRE_DIR}/lib )
endif()

add_subdirectory( ${CMAKE_SOURCE_DIR}/libs/tinyobjloader EXCLUDE_FROM_ALL )

if(WIN32)
    set(COLORWRAP_BUILD_SHARED OFF) 
else()
    set(COLORWRAP_BUILD_SHARED ON) 
endif()
set(COLORWRAP_EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3)
add_subdirectory( ${CMAKE_SOURCE_DIR}/libs/colorwrap EXCLUDE_FROM_ALL )
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libs/colorwrap/include)

###############################################
# Preliminary support for stratmod library
###############################################

include(ExternalProject)  ## needed

set(BUILD_USING_OPENMP ON)

set(STRATMOD_SOURCE ${CMAKE_SOURCE_DIR}/libs/stratmod)

set(STRATMOD_CMAKE_ARGS 
    -DSTRATMOD_EIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}
    -DSTRATMOD_BUILD_USING_OPENMP=${BUILD_USING_OPENMP}
    -DSTRATMOD_BUILD_SHARED_LIBRARY=ON
    -DSTRATMOD_BUILD_TYPE=${RRM_BUILD_TYPE}
    )

if(BUILD_USING_OPENMP AND APPLE)
    set(STRATMOD_CMAKE_ARGS 
        -DSTRATMOD_EIGEN3_INCLUDE_DIR=${EIGEN3_INCLUDE_DIR}
        -DSTRATMOD_BUILD_USING_OPENMP=ON 
        -DSTRATMOD_BUILD_SHARED_LIBRARY=${BUILD_SHARED_STRATMODLIB}
        -DSTRATMOD_BUILD_TYPE=${RRM_BUILD_TYPE}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_MODULE_PATH}/Darwin_Clang-OpenMP_Toolchain.cmake
        )
endif()

ExternalProject_Add(
    StratModLibBuild
    DOWNLOAD_COMMAND git submodule update --init
    UPDATE_COMMAND git submodule update -f
    SOURCE_DIR ${STRATMOD_SOURCE}
    CMAKE_ARGS ${STRATMOD_CMAKE_ARGS} 
    )

if(MSVC)
    set(STRATMODLIB ${STRATMOD_SOURCE}/stratmod/lib/stratmod.lib)
else()
    set(STRATMODLIB "${STRATMOD_SOURCE}/stratmod/lib/${CMAKE_SHARED_LIBRARY_PREFIX}stratmod${CMAKE_SHARED_LIBRARY_SUFFIX}")
    #set(STRATMODLIB ${STRATMOD_SOURCE}/lib/libstr  atmod.a)
endif()


include_directories(${STRATMOD_SOURCE}/stratmod/include)

#message(WARNING stratmod)

################################################
## Trimesh2
################################################
## For Libraies without find*.cmake
#set (  TRIMESH2_INCLUDE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/libs/trimesh2/install/include/")
#set (  TRIMESH2_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/trimesh2/install/lib/trimesh2.lib")

###############################################
# Main source
###############################################


add_subdirectory( src )

