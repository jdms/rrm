cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

project(RRM_Suite CXX)

set(rrm_version_major 1)
set(rrm_version_minor 0)
set(rrm_version_patch 1)
mark_as_advanced(rrm_version_major rrm_version_minor rrm_version_patch)

set(rrm_version "${rrm_version_major}.${rrm_version_minor}.${rrm_version_patch}")
mark_as_advanced(rrm_version)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# Options
###############################################

# Leave empty to use the (platform dependent) default build type
set(RRM_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Leave empty ot set build type to: Debug, Release, RelWithDebInfo, or MinSizeRel")

OPTION(RRM_BUILD_SHARED_STRATMODLIB "Build a shared library from stratmod"  ON)

OPTION(RRM_BUILD_IN_PREFIXED_BINARY_DIR "To simplify sharing in Windows"    ON)

OPTION(RRM_BUILD_DEPLOY_QT
    "Automatically copy Qt resources to build dir (Windows only)"          OFF)

OPTION(RRM_BUILD_FLOW_DIAGNOSTICS "Build flow diagnostics"                 ON)

### Execute options
if(RRM_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${RRM_BUILD_TYPE}" CACHE INTERNAL "" FORCE)
endif()

if(RRM_BUILD_SHARED_STRATMODLIB)
    set(STRATMOD_BUILD_SHARED_LIBRARY ON)
else()
    set(STRATMOD_BUILD_SHARED_LIBRARY OFF)
endif()

if(RRM_BUILD_IN_PREFIXED_BINARY_DIR)
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ CACHE STRING "" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ CACHE STRING "" )
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()


################################################
## Configure libraries
################################################

### OpenGL settings
find_package( OpenGL REQUIRED )
if( OpenGL_FOUND )
    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
endif( OpenGL_FOUND )

### GLEW settings
find_package(GLEW)
if(NOT GLEW_FOUND)
    if(WIN32)
        # DEPRECATED: use vcpkg's glew instead
        # A script in `libs/` will download an old copy of GLEW and
        # set it up for use by cmake.
        add_subdirectory(${CMAKE_SOURCE_DIR}/libs)
    else()
        message(FATAL "Could not find GLEW")
    endif()
endif()
include_directories( SYSTEM ${GLEW_INCLUDE_DIRS} )

### Eigen3 settings
find_package( Eigen3 REQUIRED )
include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
add_definitions( -DEIGEN_DONT_ALIGN ) # Why is this necessary?

### Qt settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(QT_USE_QTMAIN TRUE)

# Setup the cmake prefix path so Qt can be found. Use the QTDIR variable if defined
if((NOT Qt5_DIR) AND (NOT QTDIR) AND (NOT DEFINED ENV{QTDIR}))
    MESSAGE(STATUS
   "Info: QTDIR (environment) variable not set.  If cmake can't find Qt5,
   set QTDIR (either as an environment or cmake cache variable) to the Qt
   root install location."
    )
endif()

if((DEFINED ENV{QTDIR}) AND (NOT QTDIR))
    set(QTDIR $ENV{QTDIR} CACHE STRING "Qt install path")
endif()

if(QTDIR AND (NOT DEFINED Qt5_DIR))
    list(APPEND CMAKE_PREFIX_PATH ${QTDIR})
endif()

find_package(Qt5 COMPONENTS Core Gui Svg Widgets OpenGL REQUIRED)

### Tucano settings
set( TUCANO_DIR ${CMAKE_SOURCE_DIR}/libs/Tucano )
include_directories( SYSTEM ${TUCANO_DIR} )

### tinyobjloader settings
add_subdirectory( ${CMAKE_SOURCE_DIR}/libs/tinyobjloader EXCLUDE_FROM_ALL )

### colorwrap settings
set(COLORWRAP_EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/colorwrap)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/libs/colorwrap/include)

### stratmod settings
set(STRATMOD_EIGEN3_INCLUDE_DIR ${EIGEN3_INCLUDE_DIR})
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/stratmod)
set(STRATMOD_SOURCE ${CMAKE_SOURCE_DIR}/libs/stratmod)
include_directories(${STRATMOD_SOURCE}/include)


###############################################
# Main source
###############################################

# FIXME: legacy includes
include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets/RRMWidgets_autogen/include )
include_directories( ${CMAKE_BINARY_DIR}/src/core/widgets/RRMWidgets_autogen/include_${CMAKE_CFG_INTDIR} )
include_directories( ${CMAKE_SOURCE_DIR}/libs )
include_directories( ${CMAKE_SOURCE_DIR} )

add_subdirectory( src )

# # Useful for debugging: the following code prints all directories currently
# # included in the project
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# message(STATUS "List of directories manually included in the project this far:")
# foreach(dir ${dirs})
#     message(STATUS ":: '${dir}'")
# endforeach()
