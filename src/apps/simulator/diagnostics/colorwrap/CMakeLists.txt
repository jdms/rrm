# Copyright (c) 2018 Julio Daniel Machado Silva

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


###############################################
# Preamble  
###############################################

cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

project(ColorWrap CXX)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 9 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# For vim's YouCompleteMe plugin
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# Project options 
###############################################

# Set default build type (debug, release, etc.)

set(COLORWRAP_BUILD_TYPE "" CACHE STRING
    "Set build type to: Debug, Release, RelWithDebInfo, or MinSizeRel")

if(COLORWRAP_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${COLORWRAP_BUILD_TYPE}" CACHE INTERNAL "" FORCE)
endif()

option(COLORWRAP_BUILD_SHARED         "Build a shared library instead of a static library"          ON)
option(COLORWRAP_INTERPOLATE_COLORS   "Allow interpolating colours in RGB space -- requires Eigen3" ON)
option(COLORWRAP_BUILD_DOCS           "Build documentation with Doxygen"                            OFF)
option(COLORWRAP_BUILD_TESTS          "Build a test binary"                                         OFF)

# regarding testing, see also: https://cmake.org/Wiki/CMake/Testing_With_CTest



###############################################
# Auxiliary config files. 
###############################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


###############################################
# Configure lib (shared or static)
###############################################

if(COLORWRAP_BUILD_SHARED)
    set(LIB_TYPE SHARED)

else()
    set(LIB_TYPE STATIC)

endif()



###############################################
# Compiler flags (we support Clang (3.9+), 
# Gcc (5.4+) and VS2013 (revision 5+)). 
###############################################


###############################################
# Docs
###############################################

if (COLORWRAP_BUILD_DOCS)
    include(BuildDocumentation) 
endif(COLORWRAP_BUILD_DOCS)



###############################################
# External libraries
###############################################

#
#  Eigen is mainly used as a wrapper for Lapack. 
#

if(COLORWRAP_INTERPOLATE_COLORS)
    if(COLORWRAP_EIGEN3_INCLUDE_DIR)
        message(STATUS "Using Eigen3 include dir: ${COLORWRAP_EIGEN3_INCLUDE_DIR}")
        include_directories(SYSTEM ${COLORWRAP_EIGEN3_INCLUDE_DIR})

    else()
        find_package(Eigen3) 

        if(Eigen3_FOUND)
            set(GLOBAL_EIGEN3_INSTALL true CACHE BOOL "")
        endif()

        if(GLOBAL_EIGEN3_INSTALL) 
            include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

        else()
            set(EIGEN3_SOURCE ${CMAKE_SOURCE_DIR}/src/eigen3)
            include(ExternalProject)

            ExternalProject_Add(
                Eigen3
                GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror
                SOURCE_DIR ${EIGEN3_SOURCE}
                STEP_TARGETS build
                CONFIGURE_COMMAND ""
                BUILD_COMMAND ""
                INSTALL_COMMAND ""
                EXCLUDE_FROM_ALL TRUE
                )

            include_directories(SYSTEM ${EIGEN3_SOURCE})
            set(LOCAL_EIGEN3_INSTALL true CACHE BOOL "")

        endif() 

    endif() #if(COLORWRAP_EIGEN3_INCLUDE_DIR)
endif() #if(COLORWRAP_INTERPOLATE_COLORS)

    


###############################################
# Main library
###############################################

set(HEADERS 
    include/colorwrap_c.h
    include/colorwrap.hpp
    include/colorwrap_win_dll_export_handler.h
    )

set(COLORWRAP_DETAIL_HEADERS
    src/nspline.h
    src/cbrewer.h
    )

set(COLORWRAP_SOURCES 
    src/colorwrap_c_exports.cpp
    src/colorwrap.cpp
    ) 

add_library(colorwrap ${LIB_TYPE} ${COLORWRAP_SOURCES})  

target_include_directories(colorwrap
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(colorwrap PUBLIC cxx_std_11)

# MSVC (as recently as version 19.15.26729) throws hundreds of warnings from
# it's own implementation of the STL -- CMake's SYSTEM directive seems
# ineffective to prevent this behaviour.  Thus, using -Wall in MSVC is,
# unfortunately, not possible.  

if(NOT MSVC) 
    target_compile_options(colorwrap PRIVATE -Wall) 
endif()

if(COLORWRAP_INTERPOLATE_COLORS)
    target_compile_options(colorwrap PRIVATE -DCOLORWRAP_INTERPOLATE_COLORS)

    if(LOCAL_EIGEN3_INSTALL)
        add_dependencies(colorwrap Eigen3-build)
    endif()
endif()

if(COLORWRAP_BUILD_SHARED)
    target_compile_options(colorwrap PUBLIC -DCOLORWRAP_BUILD_SHARED)
endif()

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.

include(GNUInstallDirs)

# Modify installation path in cmake generation step by setting var: 
# CMAKE_INSTALL_PREFIX

#set(INSTALL_DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/colorwrap)

install(TARGETS colorwrap 
    EXPORT colorwrap-export
    #LIBRARY DESTINATION ${INSTALL_DESTINATION}/lib 
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #ARCHIVE DESTINATION ${INSTALL_DESTINATION}/lib 
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #RUNTIME DESTINATION ${INSTALL_DESTINATION}/lib
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT library # what is this COMPONENT reserved word for? 
)

install(FILES ${HEADERS}
    #DESTINATION ${INSTALL_DESTINATION}/include 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT colorwrap-export 
    FILE colorwrap-targets.cmake
    # NAMESPACE ColorWrap::
    #DESTINATION ${INSTALL_DESTINATION}/lib/cmake/colorwrap 
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/colorwrap
)

export(TARGETS colorwrap FILE colorwrap.cmake)

###############################################
# Executables and utilities 
###############################################

if(COLORWRAP_BUILD_TESTS)
    add_executable(main src/main.cpp)
    target_link_libraries(main colorwrap)
endif()
