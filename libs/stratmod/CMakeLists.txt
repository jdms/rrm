#****************************************************************************
#                                                                            
# This file is part of the ""Stratigraphy Modeller Library" (StratModLib)    
# Copyright (C) 2016, Julio Daniel Machado Silva.                            
#                                                                            
# StratMod is free software; you can redistribute it and/or                  
# modify it under the terms of the GNU Lesser General Public                 
# License as published by the Free Software Foundation; either               
# version 3 of the License, or (at your option) any later version.           
#                                                                            
# StratMod is distributed in the hope that it will be useful,                
# but WITHOUT ANY WARRANTY; without even the implied warranty of             
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          
# Lesser General Public License for more details.                            
#                                                                            
# You should have received a copy of the GNU Lesser General Public           
# License along with StratMod.  If not, see <http://www.gnu.org/licenses/>,  
# or write to the Free Software Foundation, Inc., 51 Franklin Street,        
# Fifth Floor, Boston, MA  02110-1301  USA.                                  
#                                                                            
#****************************************************************************


###############################################
# Preliminaries  
###############################################

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(StratMod CXX)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(STRATMOD_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
mark_as_advanced(STRATMOD_VERSION)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# Project options 
###############################################

# Set default build type (debug, release, etc.), 
# or leave empty to use the (platform specific) 
# default

set(STRATMOD_BUILD_TYPE "" CACHE STRING
    "Leave empty or set to any of: Debug, Release, RelWithDebInfo, or MinSizeRel")

if(STRATMOD_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${STRATMOD_BUILD_TYPE}" CACHE INTERNAL "" FORCE)
endif()


if(NOT DEFINED STRATMOD_BUILD_SHARED_LIBRARY)
    option(STRATMOD_BUILD_SHARED_LIBRARY "Build a shared library instead of static library" ON)
endif()

if(NOT DEFINED STRATMOD_BUILD_USING_OPENMP)
    option(STRATMOD_BUILD_USING_OPENMP   "Use OpenMP for parallelization"                   ON)
endif()

if(NOT DEFINED STRATMOD_BUILD_USING_LAPACK)
    option(STRATMOD_BUILD_USING_LAPACK   "Use lapack to accelerate linear algebra"          ON)
endif()

if(NOT DEFINED STRATMOD_BUILD_USING_CEREAL)
    option(STRATMOD_BUILD_USING_CEREAL   "Use Cereal library for serialization"             ON)
endif()

if(NOT DEFINED STRATMOD_BUILD_VTK_EXPORTER_UTILITY)
    option(STRATMOD_BUILD_VTK_EXPORTER_UTILITY   "Build utility to export models to vtk"    ON)
endif()

# # Detects whether this is a top-level project
# get_directory_property(STRATMOD_PARENT_DIR PARENT_DIRECTORY)
# if(NOT STRATMOD_PARENT_DIR)
#     set(STRATMOD_TOP_LEVEL_PROJECT ON)
# else()
#     set(STRATMOD_TOP_LEVEL_PROJECT OFF)
# endif()

set(STRATMOD_TOP_LEVEL_PROJECT OFF)

option(STRATMOD_BUILD_DOCS           "Build documentation with Doxygen"                 ${STRATMOD_TOP_LEVEL_PROJECT})
option(STRATMOD_BUILD_TESTS          "Build with support for testing using Catch"       ${STRATMOD_TOP_LEVEL_PROJECT})
option(STRATMOD_BUILD_EXAMPLE        "Build examples"                                   ${STRATMOD_TOP_LEVEL_PROJECT}) 
option(STRATMOD_BUILD_UTILITIES      "Build command line utilities"                     ${STRATMOD_TOP_LEVEL_PROJECT}) 

# regarding testing, see also: https://cmake.org/Wiki/CMake/Testing_With_CTest



###############################################
# Auxiliary config files. 
###############################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# include_directories("./")



###############################################
# Configure lib (shared or static)
###############################################

if(STRATMOD_BUILD_SHARED_LIBRARY)
    add_definitions(-DSTRATMOD_BUILD_SHARED)
    set(LIB_TYPE SHARED)
    # set(PLANIN_FORCE_POSITION_INDEPENDENT_CODE ON)
    # set(PLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE ON)

else()
    set(LIB_TYPE STATIC)

endif()



###############################################
# Compiler flags (we support Clang (3.9+), 
# Gcc (5.4+) and VS2013 (revision 5+)). 
###############################################

# if(MSVC)
    # Flag '/EHsc': the mere fact of including <iostream> yields warning C4530
    # in VS2013, which relates to exception handling.  The flag '/EHsc' seems
    # to be necessary to make VS2013 behave sanely. 

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3")

# else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

# endif()


###############################################
# Configure OpenMP for parallelization
###############################################

if(STRATMOD_BUILD_USING_OPENMP)
    # Looks like new versions of FindOpenMP.cmake have macos sorted out already
    # include(PatchOpenMPApple)
    find_package(OpenMP REQUIRED)
    set(PLANIN_BUILD_USING_OPENMP "" ON) 
endif()

#if(STRATMOD_BUILD_USING_OPENMP)
#    if (MSVC)
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP /openmp")
#
#    else()
#        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang)")
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP -fopenmp=libiomp5")
#
#        else()
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP -fopenmp")
#
#        endif()
#    endif()
#endif()


# Waiting for macOS to support OpenMP natively to adopt the following solution:

#if(BUILD_USING_OPENMP)
#  find_package(OpenMP REQUIRED)
#
#  if(OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#
#  endif()
#
#endif()


###############################################
# Docs
###############################################

if (STRATMOD_BUILD_DOCS)
    include(BuildDocumentation) 
    set(PLANIN_BUILD_DOCS OFF) 
endif(STRATMOD_BUILD_DOCS)



###############################################
# External libraries
###############################################

#  Eigen is mainly used as a wrapper for Lapack. 
# find_package(Eigen3 REQUIRED) 
# if (Eigen3_FOUND) 
#     include_directories(SYSTEM ${Eigen3_INCLUDE_DIRS})
# endif(Eigen3_FOUND) 

if(STRATMOD_EIGEN3_INCLUDE_DIR)
    message(STATUS "Using eigen3 include dir: ${STRATMOD_EIGEN3_INCLUDE_DIR}")
    set(PLANIN_EIGEN3_INCLUDE_DIR ${STRATMOD_EIGEN3_INCLUDE_DIR})
endif()

if(STRATMOD_BUILD_USING_LAPACK)
    set(PLANIN_BUILD_USE_LAPACK ON)
endif()

###############################################
# External projects
###############################################

include(ExternalProject)



###############################################
# PlanIn processes all modelling primitives
###############################################

set(PLANIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/planin)
add_subdirectory(${PLANIN_PATH})

if(STRATMOD_BUILD_SHARED_LIBRARY)
    set_property(TARGET planin PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# set(PLANIN_CMAKE_ARGS 
#     -DPLANIN_EIGEN3_INCLUDE_DIR=${STRATMOD_EIGEN3_INCLUDE_DIR}
#     -DPLANIN_BUILD_DOCS=OFF 
#     -DPLANIN_BUILD_TYPE=${STRATMOD_BUILD_TYPE}
#     -DPLANIN_BUILD_USING_OPENMP=${STRATMOD_BUILD_USING_OPENMP}
#     -DPLANIN_BUILD_USING_CEREAL=${STRATMOD_BUILD_USING_CEREAL}
#     -DPLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE=${PLANIN_FORCE_POSITION_INDEPENDENT_CODE}
#     )

# if(STRATMOD_BUILD_USING_OPENMP AND APPLE)
#     set(PLANIN_CMAKE_ARGS 
#         -DPLANIN_EIGEN_INCLUDE_DIR=${STRATMOD_EIGEN_INCLUDE_DIR}
#         -DPLANIN_BUILD_DOCS=OFF 
#         -DPLANIN_BUILD_TYPE=${STRATMOD_BUILD_TYPE}
#         -DPLANIN_BUILD_USING_OPENMP=ON 
#         -DPLANIN_BUILD_USING_CEREAL=${STRATMOD_BUILD_USING_CEREAL}
#         -DPLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE=${PLANIN_FORCE_POSITION_INDEPENDENT_CODE}
#     )
#         #-DCMAKE_TOOLCHAIN_FILE=${CMAKE_MODULE_PATH}/Darwin_Clang-OpenMP_Toolchain.cmake)
# endif()

# ExternalProject_Add(
#     PlanIn
#     DOWNLOAD_COMMAND ""
#     UPDATE_COMMAND ""
#     SOURCE_DIR ${PLANIN_SOURCE}
#     CMAKE_ARGS ${PLANIN_CMAKE_ARGS} 
#     )


# add_library(planin STATIC IMPORTED)
# add_dependencies(planin PlanIn)
# include_directories(SYSTEM ${PLANIN_SOURCE})

# set(PLANINLIB "${PLANIN_SOURCE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}planin${CMAKE_STATIC_LIBRARY_SUFFIX}")
# set_property(TARGET planin PROPERTY IMPORTED_LOCATION ${PLANINLIB})



###############################################
# Use the Cereal library for serialization
###############################################

if(STRATMOD_BUILD_USING_CEREAL)
    add_definitions(-DBUILD_WITH_SERIALIZATION)
    set(PLANIN_BUILD_USING_CEREAL "" ON) 
endif()



###############################################
# Use catch for unit-testing and logging
###############################################

if(STRATMOD_BUILD_TESTS)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTESTING")

    set(CATCH_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/catch)
    set(CATCH_REMOTE https://raw.githubusercontent.com/philsquared/Catch/master/single_include/catch.hpp)
    set(CATCH_FILENAME catch.hpp)

    ExternalProject_Add(
        Catch
        DOWNLOAD_DIR ${CATCH_SOURCE}
        DOWNLOAD_COMMAND curl -s -o ${CATCH_FILENAME} ${CATCH_REMOTE}
        STEP_TARGETS build
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        EXCLUDE_FROM_ALL TRUE
        )

    # include_directories(${CATCH_SOURCE})

endif()



###############################################
# Main library
###############################################

# include_directories(${CMAKE_SOURCE_DIR}/src)

set(STRATMOD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/stratmod)
set(STRATMOD_LIB_HEADERS 
    ${STRATMOD_INCLUDE_DIR}/stratmod_win_dll_export_handler.hpp
    ${STRATMOD_INCLUDE_DIR}/smodeller_primitives.hpp
    ${STRATMOD_INCLUDE_DIR}/smodeller.hpp
    ${STRATMOD_INCLUDE_DIR}/sutilities.hpp
    )

set(STRATMOD_LIB_SOURCES 
    src/detail/smodeller_impl.cpp
    src/smodeller.cpp
    src/sutilities.cpp
    src/use_smodeller.cpp
    src/use_sutilities.cpp
    ) 


add_library(stratmod ${LIB_TYPE} ${STRATMOD_LIB_SOURCES} ${STRATMOD_LIB_HEADERS})  
add_library(stratmod::stratmod ALIAS stratmod)

target_compile_features(stratmod PUBLIC cxx_std_11)

if(MSVC)
    target_compile_options(stratmod PRIVATE -EHsc -W3)
else()
    target_compile_options(stratmod PRIVATE -Wall -Wextra -Wno-unknown-pragmas)
endif()

if(STRATMOD_BUILD_SHARED_LIBRARY)
    target_compile_options(stratmod PUBLIC -DSTRATMOD_BUILD_SHARED_LIBRARY)
endif()

target_link_libraries(stratmod PRIVATE planin)

if( ${STRATMOD_BUILD_USING_OPENMP} )
    target_link_libraries(stratmod PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(stratmod
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_include_directories(stratmod SYSTEM
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/detail/planin>
        $<BUILD_INTERFACE:${CATCH_SOURCE}>
)
add_dependencies(stratmod planin)


# Prepare exports
if(STRATMOD_BUILD_SHARED_LIBRARY)
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        stratmodConfigVersion.cmake
        VERSION ${STRATMOD_VERSION}
        COMPATIBILITY AnyNewerVersion
        )

    # set(INSTALL_DESTINATION ${CMAKE_BINARY_DIR}/stratmod)

    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "libs/stratmod" CACHE PATH "..." FORCE)
    endif()

    install(TARGETS stratmod
        EXPORT stratmodTargets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION ${INSTALL_DESTINATION}/include
        # COMPONENT library
        )

    install(EXPORT stratmodTargets
        FILE stratmodTargets.cmake
        NAMESPACE stratmod::
        DESTINATION ${INSTALL_DESTINATION}/cmake
        )

    # install(DIRECTORY ${STRATMOD_INCLUDE_DIR}
    #     DESTINATION ${INSTALL_DESTINATION}/include
    #     )

    configure_file(stratmodConfig.cmake.in stratmodConfig.cmake @ONLY)
    install(
        FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/stratmodConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/stratmodConfigVersion.cmake"
        DESTINATION 
        cmake
        )
endif() #if(STRATMOD_BUILD_SHARED_LIBRARY)

###############################################
# Executables and utilities 
###############################################

if(${STRATMOD_BUILD_TESTS}) 
    set(TESTING_SOURCES src/test/main_tests.cpp src/test/first_test.cpp src/detail/smodeller_impl.cpp)
    add_executable (run_tests ${TESTING_SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(run_tests planin OpenMP::OpenMP_CXX)
    add_dependencies(run_tests planin)
    add_dependencies(run_tests Catch-build)

    enable_testing(TRUE)
    add_test("Main" run_tests)

endif()


if(${STRATMOD_BUILD_EXAMPLE})
    set(SOURCES src/main.cpp)
    add_executable(run_me ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(run_me stratmod)
    #    add_dependencies(run_me planin)

endif()

if(${STRATMOD_BUILD_UTILITIES})
    set(SOURCES src/test/test_boundary.cpp)
    add_executable(test_boundary ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(test_boundary stratmod)

    set(SOURCES src/test/test_mesh.cpp)
    add_executable(test_mesh ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(test_mesh stratmod)

    set(SOURCES src/test/test_regions.cpp)
    add_executable(test_regions ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(test_regions stratmod)

    set(SOURCES src/cl_util/binary2json.cpp)
    add_executable(binary2json ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(binary2json stratmod)

    set(SOURCES src/cl_util/json2binary.cpp)
    add_executable(json2binary ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(json2binary stratmod)

    set(SOURCES src/cl_util/resample.cpp)
    add_executable(resample ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(resample stratmod)

    set(SOURCES src/cl_util/export2VTK.cpp)
    add_executable(export2VTK ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(export2VTK stratmod)
endif()

if(STRATMOD_BUILD_VTK_EXPORTER_UTILITY)
    set(SOURCES src/cl_util/rrm2vtk.cpp)
    add_executable(rrm2vtk ${SOURCES} ${STRATMOD_LIB_HEADERS})
    target_link_libraries(rrm2vtk stratmod planin)
endif()
