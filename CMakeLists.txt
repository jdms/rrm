cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRM )


#####################################################################
# Options
#####################################################################

set( CMAKE_BUILD_TYPE RELEASE )
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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fopenmp")

ELSE()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()



###############################################
# Output CMake
###############################################

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )



################################################
## External libraries
################################################


include_directories( ${CMAKE_SOURCE_DIR}/libs )
include_directories( "./" )


OPTION( OWN_LIBRARIES "Use predefined libraries directories" ON )

if( OWN_LIBRARIES AND WIN32 )

    set( Qt5_DIR "C:/Qt/5.6/msvc2013/lib/cmake/Qt5" )
    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
    include_directories( SYSTEM ${Qt5_DIR} )
    include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )


    set( GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/GLEW/include )
    set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/x32/glew32.lib )
    find_package( GLEW REQUIRED )
    include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )


    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
    find_package( EIGEN3 REQUIRED )
    include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
    add_definitions( -DEIGEN_DONT_ALIGN )

        find_library( SAMG_LIBRARY samg_XP_6_2005 ${CMAKE_SOURCE_DIR}/libs/SAMG )



elseif( OWN_LIBRARIES AND UNIX )


    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
    find_package( Eigen3 REQUIRED )
    if( EIGEN3_FOUND )
        include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
        add_definitions( -DEIGEN_DONT_ALIGN )
    endif( EIGEN3_FOUND )

    find_package( GLEW REQUIRED )
    if( GLEW_FOUND )
        include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
    endif( GLEW_FOUND )


    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
    if( Qt5COMPONENTS_FOUND )
        include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )
    endif( Qt5COMPONENTS_FOUND )




else( OWN_LIBRARIES )


    find_package( Eigen3 REQUIRED )
    if( Eigen3_FOUND )
        include_directories( SYSTEM ${Eigen3_INCLUDE_DIR} )
        #add_definitions( -DEIGEN_DONT_ALIGN )
    endif( Eigen3_FOUND )

    find_package( GLEW REQUIRED )
    if( GLEW_FOUND )
        include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
    endif( GLEW_FOUND )


    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
    if( Qt5COMPONENTS_FOUND )
        include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
        include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )
    endif( Qt5COMPONENTS_FOUND )


endif( OWN_LIBRARIES AND WIN32 )





find_package( OpenGL REQUIRED )
if( OpenGL_FOUND )
    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
endif( OpenGL_FOUND )





add_subdirectory( libs/PlanInLib EXCLUDE_FROM_ALL )
add_subdirectory( libs/SketchLibrary EXCLUDE_FROM_ALL )
add_subdirectory( libs/Arrangement2D EXCLUDE_FROM_ALL )



################################################
## Preliminary support for stratmod library
################################################

## include_directories(/usr/include/openmpi)
## set(HypreLibs HYPRE HYPRE_sstruct_ls HYPRE_parcsr_ls HYPRE_IJ_mv mpi mpi++)

# include(ExternalProject)  ## needed

#set(BUILD_USING_OPENMP ON)

#set(STRATMOD_SOURCE ${CMAKE_SOURCE_DIR}/libs/stratmod)

#set(STRATMOD_CMAKE_ARGS
#    -DBUILD_USING_OPENMP=${BUILD_USING_OPENMP}
#    -DBUILD_SHARED_LIBRARY=OFF
#    )

#if(BUILD_USING_OPENMP AND APPLE)
#    set(STRATMOD_CMAKE_ARGS -DBUILD_USING_OPENMP=ON -DCMAKE_TOOLCHAIN_FILE=${CMAKE_MODULE_PATH}/Darwin_Clang-OpenMP_Toolchain.cmake)
#endif()

#ExternalProject_Add(
#    StratModLibBuild
#    DOWNLOAD_COMMAND git submodule update --init
#    UPDATE_COMMAND git submodule update -f
#    SOURCE_DIR ${STRATMOD_SOURCE}
#    CMAKE_ARGS ${STRATMOD_CMAKE_ARGS}
#    )

#if(MSVC)
#    set(STRATMODLIB ${STRATMOD_SOURCE}/lib/stratmod.lib)
#else()
#    set(STRATMODLIB ${STRATMOD_SOURCE}/lib/libstratmod.a)
#endif()

#add_library(stratmod STATIC IMPORTED)
#set_property(TARGET stratmod PROPERTY IMPORTED_LOCATION ${STRATMODLIB})
#add_dependencies(stratmod StratModLibBuild)
#include_directories(${STRATMOD_SOURCE})


#message(STATUS "STRATMOD PATH:" ${STRATMODLIB})

###############################################
# Main source
###############################################

add_subdirectory( src )



#cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

#project( RRM )


######################################################################
## Options
######################################################################

#set( CMAKE_BUILD_TYPE DEBUG )
#OPTION( BUILD_FULL_SUITE "Build RapidReservoir Suite" ON )


######################################################################
## Compiler flags ( we support Clang (3.7+), Gcc (4.8+) and
## VS2013 (revision 5+) ).
######################################################################

#IF( WIN32 )

#    if( MSVC )
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /openmp /EHsc")
#    endif()

#ELSEIF( UNIX )
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2 -fopenmp")

#ELSE()
#  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
#ENDIF()



################################################
## Output CMake
################################################

#set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )
#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )



#################################################
### External libraries
#################################################


#include_directories( ${CMAKE_SOURCE_DIR}/libs )
#include_directories( "./" )


#OPTION( OWN_LIBRARIES "Use predefined libraries directories" ON )

#if( OWN_LIBRARIES AND WIN32 )

#    set( Qt5_DIR "C:/Qt/5.6/msvc2013/lib/cmake/Qt5" )
#    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
#    include_directories( SYSTEM ${Qt5_DIR} )
#    include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )


#    set( GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/GLEW/include )
#    set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/x32/glew32.lib )
#    find_package( GLEW REQUIRED )
#    include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )


#    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
#    find_package( EIGEN3 REQUIRED )
#    include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
#    add_definitions( -DEIGEN_DONT_ALIGN )



#elseif( OWN_LIBRARIES AND UNIX )


#    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
#    find_package( Eigen3 REQUIRED )
#    if( EIGEN3_FOUND )
#        include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
#        add_definitions( -DEIGEN_DONT_ALIGN )
#    endif( EIGEN3_FOUND )

#    find_package( GLEW REQUIRED )
#    if( GLEW_FOUND )
#        include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
#    endif( GLEW_FOUND )


#    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
#    if( Qt5COMPONENTS_FOUND )
#        include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )
#    endif( Qt5COMPONENTS_FOUND )




#else( OWN_LIBRARIES )


#    find_package( Eigen3 REQUIRED )
#    if( Eigen3_FOUND )
#        include_directories( SYSTEM ${Eigen3_INCLUDE_DIR} )
#        add_definitions( -DEIGEN_DONT_ALIGN )
#    endif( Eigen3_FOUND )

#    find_package( GLEW REQUIRED )
#    if( GLEW_FOUND )
#        include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
#    endif( GLEW_FOUND )


#    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
#    if( Qt5COMPONENTS_FOUND )
#        include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
#        include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )
#    endif( Qt5COMPONENTS_FOUND )


#endif( OWN_LIBRARIES AND WIN32 )





#find_package( OpenGL REQUIRED )
#if( OpenGL_FOUND )
#    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
#endif( OpenGL_FOUND )





#add_subdirectory( libs/PlanInLib EXCLUDE_FROM_ALL )
#add_subdirectory( libs/SketchLibrary EXCLUDE_FROM_ALL )
#add_subdirectory( libs/Arrangement2D EXCLUDE_FROM_ALL )



################################################
## Main source
################################################

#add_subdirectory( src )






