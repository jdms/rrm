#****************************************************************************
#                                                                            
# This file is part of the "Planar Interpolation Library" (PlanInLib)        
# Copyright (C) 2016, Julio Daniel Machado Silva.                            
#                                                                            
# PlanInLib is free software; you can redistribute it and/or                 
# modify it under the terms of the GNU Lesser General Public                 
# License as published by the Free Software Foundation; either               
# version 2.1 of the License, or (at your option) any later version.         
#                                                                            
# PlanInLib is distributed in the hope that it will be useful,               
# but WITHOUT ANY WARRANTY; without even the implied warranty of             
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          
# Lesser General Public License for more details.                            
#                                                                            
# You should have received a copy of the GNU Lesser General Public           
# License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, 
# or write to the Free Software Foundation, Inc., 51 Franklin Street,        
# Fifth Floor, Boston, MA  02110-1301  USA.                                  
#                                                                            
#****************************************************************************

#####################################################################
# Preliminaries  
#####################################################################

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(PlanInLib CXX)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 2 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#####################################################################
# Objects and options 
#####################################################################

###############################################
# Set default build type (debug, release, etc.)
###############################################

set(PLANIN_BUILD_TYPE "" CACHE STRING
    "Leave empty or set to any of: Debug, Release, RelWithDebInfo, or MinSizeRel")

if(PLANIN_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "${PLANIN_BUILD_TYPE}" CACHE INTERNAL "" FORCE)
endif()


###############################################
# Build forcing position independent code
###############################################
option(PLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE "" ON) 


###############################################
# Use OpenMP
###############################################
option(PLANIN_BUILD_USING_OPENMP "" OFF) 


###############################################
# Use Cereal for serialization
###############################################
option(PLANIN_BUILD_USING_CEREAL "" OFF) 


###############################################
# Allow doxygen to build documentation.  
###############################################
option(PLANIN_BUILD_DOCS "" OFF) 


###############################################
# Build a test program.  
###############################################
option(PLANIN_BUILD_MAIN "" OFF)


###############################################
# Build a tetrahedral meshing test program. 
###############################################
option(PLANIN_BUILD_TEST_MESH "" OFF)


###############################################
# Use Backward-cpp
###############################################
option(PLANIN_BUILD_USING_BACKWARD_CPP "" ON)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(PLANIN_BUILD_USING_BACKWARD_CPP AND LINUX)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/backward-cpp")
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/backward-cpp")
    find_package(Backward)
endif()


###############################################
# Tools.  
###############################################
option(PLANIN_BUILD_UTILITY_UPDATE_MESH "" OFF)
option(PLANIN_BUILD_UTILITY_BUILD_MESH "" OFF)
option(PLANIN_BUILD_UTILITY_REMOVE_ABOVE "" OFF)
option(PLANIN_BUILD_UTILITY_REMOVE_BELOW "" OFF)


###############################################
# Examples and tests. 
###############################################
option(PLANIN_BUILD_DAISY_EXAMPLE_2A "" OFF)
option(PLANIN_BUILD_DAISY_EXAMPLE_2B "" OFF)

option(PLANIN_BUILD_DAISY_EXAMPLE_3A "" OFF)
option(PLANIN_BUILD_DAISY_EXAMPLE_3B "" OFF)

option(PLANIN_BUILD_DAISY_EXAMPLE_4A "" OFF)
option(PLANIN_BUILD_DAISY_EXAMPLE_4B "" OFF)

option(PLANIN_BUILD_DAISY_EXAMPLE_5A "" OFF)
option(PLANIN_BUILD_DAISY_EXAMPLE_5B "" OFF)


###############################################
# Auxiliary config files. 
###############################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# include_directories("./")


###############################################
# Compiler flags (we support Clang (3.7+), 
# Gcc (4.8+) and VS2013 (revision 5+)). 
###############################################

# if(MSVC)
    # Flag '/EHsc': the mere fact of including <iostream> yields warning C4530
    # in VS2013, which relates to exception handling.  The flag '/EHsc' seems
    # to be necessary to make VS2013 behave sanely. 

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3")

# else()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wno-unknown-pragmas")

# endif()

if(PLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

endif()


# Finally a way to use OpenMP with AppleClang!  (It's still a hack, though...)
# Let's keep the old hack commented out in case this breaks.  

if(PLANIN_BUILD_USING_OPENMP OR STRATMOD_BUILD_USING_OPENMP)
    include(${CMAKE_MODULE_PATH}/PatchOpenMPApple.cmake)
    find_package(OpenMP REQUIRED)
endif()


#if(PLANIN_BUILD_USING_OPENMP)
#    if (MSVC)
#        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp")
#
#    else()
#        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang)")
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#
#        else()
#            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
#
#        endif()
#    endif()
#endif()


###############################################
# Docs
###############################################

if (PLANIN_BUILD_DOCS OR STRATMOD_BUILD_DOCS)
    include("${CMAKE_MODULE_PATH}/BuildDocumentation.cmake") 
endif()

###############################################
# External libraries
###############################################

# To allow managing external projects
include(ExternalProject)


# Eigen is mainly used as a wrapper for Lapack. 
if(PLANIN_EIGEN3_INCLUDE_DIR)
    message(STATUS "Using customized eigen3 include dir: ${PLANIN_EIGEN3_INCLUDE_DIR}")

    # include_directories(SYSTEM ${PLANIN_EIGEN3_INCLUDE_DIR})
    set(EIGEN3_INCLUDE_DIR ${PLANIN_EIGEN3_INCLUDE_DIR})

else()
    find_package(Eigen3) 

    if(Eigen3_FOUND)
        set(GLOBAL_EIGEN3_INSTALL true CACHE BOOL "")
    endif()

    if(GLOBAL_EIGEN3_INSTALL) 
        # include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

    else()
        set(EIGEN3_SOURCE ${CMAKE_SOURCE_DIR}/eigen3)

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

        # include_directories(SYSTEM ${EIGEN3_SOURCE})
        set(EIGEN3_INCLUDE_DIR ${EIGEN3_SOURCE})
        set(LOCAL_EIGEN3_INSTALL true CACHE BOOL "")
    endif() 
endif()


# Cereal is used to serialize objects
if(PLANIN_BUILD_USING_CEREAL OR STRATMOD_BUILD_USING_CEREAL)
    #    set(CEREAL_SOURCE ${CMAKE_SOURCE_DIR}/cereal )
    #    ExternalProject_Add(
    #        CerealLib
    #        GIT_REPOSITORY https://github.com/USCiLab/cereal
    #        SOURCE_DIR ${CEREAL_SOURCE}
    #        STEP_TARGETS build
    #        CONFIGURE_COMMAND ""
    #        BUILD_COMMAND ""
    #        INSTALL_COMMAND ""
    #        EXCLUDE_FROM_ALL TRUE
    #    )

    #    include_directories(${CEREAL_SOURCE}/include)

    # include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/cereal)
    set(CEREAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cereal)
    add_definitions(-DBUILD_WITH_SERIALIZATION)
endif()


###############################################
# Main library
###############################################

set(HEADERS 
  use_openmp.hpp
  interpolant_2d.hpp
  interpolated_graph.hpp
  planar_surface.hpp 
  polynomial.hpp
  core.hpp
  kernels.hpp
  thinplatespline22.hpp
  thinplatespline23.hpp
  thinplatespline24.hpp
  wendland23.hpp
  basis_function_2d.hpp
  srules.hpp
  mesh/polyhedra.hpp
  mesh/tetrahedral_mesh_builder.hpp
  mesh/linear_complexes.hpp
  )

set(PLANIN_LIB_SOURCES 
    interpolant_2d.cpp
    interpolated_graph.cpp
    planar_surface.cpp
    srules.cpp
    triangle_soup_wrapper.cpp
    mesh/tetrahedral_mesh_builder.cpp
    mesh/linear_complexes.cpp
    ) 

add_library(planin STATIC ${PLANIN_LIB_SOURCES} ${HEADERS})  

target_include_directories(planin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mesh>
)

target_include_directories(planin SYSTEM
    PUBLIC
        $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${CEREAL_INCLUDE_DIR}>
)

target_link_libraries(planin)

target_compile_features(planin PUBLIC cxx_std_11)

if(MSVC)
    target_compile_options(planin PRIVATE -EHsc -W3)
else()
    target_compile_options(planin PRIVATE -Wall -Wno-unknown-pragmas)
endif()

if( ${PLANIN_BUILD_USING_OPENMP} )
    target_link_libraries(planin PUBLIC OpenMP::OpenMP_CXX)
endif()

if(LINUX)
    if(${Backward_FOUND})
        target_link_libraries(planin PUBLIC Backward::Backward)
    else()
        message(STATUS "Backward-cpp won't be used in this build")
    endif()
endif()

if(LOCAL_EIGEN3_INSTALL)
    add_dependencies(planin Eigen3-build)
endif()

# install(TARGETS planin
    # ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib)


###############################################
# Utilities 
###############################################

if(${PLANIN_BUILD_MAIN}) 
    set(SOURCES main.cpp)
    add_executable (main ${SOURCES} ${HEADERS})
    target_link_libraries(main planin)
endif()

if(${PLANIN_BUILD_TEST_MESH})
    set(SOURCES mesh/test_mesh.cpp)
    add_executable(test_mesh ${SOURCES} ${HEADER})
    target_link_libraries(test_mesh planin)
endif()

if(${PLANIN_BUILD_UTILITY_UPDATE_MESH})
    set(UPDATE_MESH util/update_mesh.cpp) 
    add_executable (update_mesh ${UPDATE_MESH} ${HEADERS})
    target_link_libraries(update_mesh planin)
endif()

if(${PLANIN_BUILD_UTILITY_BUILD_MESH})
    set(BUILD_MESH util/build_mesh.cpp) 
    add_executable (build_mesh ${BUILD_MESH} ${HEADERS})
    target_link_libraries(build_mesh planin)
endif()

if(${PLANIN_BUILD_UTILITY_REMOVE_ABOVE})
    set(REMOVE_ABOVE util/remove_above.cpp) 
    add_executable (remove_above ${REMOVE_ABOVE} ${HEADERS})
    target_link_libraries(remove_above planin)
endif()

if(${PLANIN_BUILD_UTILITY_REMOVE_BELOW})
    set(REMOVE_BELOW util/remove_below.cpp) 
    add_executable (remove_below ${REMOVE_BELOW} ${HEADERS})
    target_link_libraries(remove_below planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_2A})
    set(DAISY_EXAMPLE_2A examples/daisy_ex2a.cpp) 
    add_executable (daisy_ex2a ${DAISY_EXAMPLE_2A} ${HEADERS})
    target_link_libraries(daisy_ex2a planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_2B})
    set(DAISY_EXAMPLE_2B examples/daisy_ex2b.cpp) 
    add_executable (daisy_ex2b ${DAISY_EXAMPLE_2B} ${HEADERS})
    target_link_libraries(daisy_ex2b planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_3A})
    set(DAISY_EXAMPLE_3A examples/daisy_ex3a.cpp) 
    add_executable (daisy_ex3a ${DAISY_EXAMPLE_3A} ${HEADERS})
    target_link_libraries(daisy_ex3a planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_3B})
    set(DAISY_EXAMPLE_3B examples/daisy_ex3b.cpp) 
    add_executable (daisy_ex3b ${DAISY_EXAMPLE_3B} ${HEADERS})
    target_link_libraries(daisy_ex3b planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_4A})
    set(DAISY_EXAMPLE_4A examples/daisy_ex4a.cpp) 
    add_executable (daisy_ex4a ${DAISY_EXAMPLE_4A} ${HEADERS})
    target_link_libraries(daisy_ex4a planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_4B})
    set(DAISY_EXAMPLE_4B examples/daisy_ex4b.cpp) 
    add_executable (daisy_ex4b ${DAISY_EXAMPLE_4B} ${HEADERS})
    target_link_libraries(daisy_ex4b planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_5A})
    set(DAISY_EXAMPLE_5A examples/daisy_ex5a.cpp) 
    add_executable (daisy_ex5a ${DAISY_EXAMPLE_5A} ${HEADERS})
    target_link_libraries(daisy_ex5a planin)
endif()

if(${PLANIN_BUILD_DAISY_EXAMPLE_5B})
    set(DAISY_EXAMPLE_5B examples/daisy_ex5b.cpp) 
    add_executable (daisy_ex5b ${DAISY_EXAMPLE_5B} ${HEADERS})
    target_link_libraries(daisy_ex5b planin)
endif()


