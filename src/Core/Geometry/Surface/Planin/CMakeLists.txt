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


cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(PlanInLib)

option(BUILD_MAIN "" OFF)
option(BUILD_UTILITY_BUILD_MESH "" OFF)
option(BUILD_UTILITY_REMOVE_ABOVE "" OFF)
option(BUILD_UTILITY_REMOVE_BELOW "" OFF)

option(BUILD_DAISY_EXAMPLE_2A "" OFF)
option(BUILD_DAISY_EXAMPLE_2B "" OFF)

option(BUILD_DAISY_EXAMPLE_3A "" OFF)
option(BUILD_DAISY_EXAMPLE_3B "" OFF)

option(BUILD_DAISY_EXAMPLE_4A "" OFF)
option(BUILD_DAISY_EXAMPLE_4B "" OFF)

option(BUILD_DAISY_EXAMPLE_5A "" OFF)
option(BUILD_DAISY_EXAMPLE_5B "" OFF)


###############################################
# Compiler flags (we support Clang (3.7+), 
# Gcc (4.8+) and VS2013 (revision 5+)). 
###############################################

if(MSVC)
# Flag '/EHsc': the mere fact of including <iostream> yields warning C4530 in VS2013, which relates to exception handling. 
# The flag '/EHsc' seems to be necessary to make VS2013 behave sanely. 
    set(CMAKE_CXX_FLAGS "/Od /openmp /EHsc")
else()
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O2 -fopenmp")
endif()


###############################################
# Auxiliary config files. 
###############################################

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include_directories("./")


###############################################
# External libraries
###############################################

# Eigen is mainly used as a wrapper for Lapack. 
find_package(Eigen3 REQUIRED) 
if (Eigen3_FOUND) 
    include_directories(system ${Eigen3_INCLUDE_DIRS})
endif (Eigen3_FOUND) 


###############################################
# Main library
###############################################

set(HEADERS 
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
  )

set(PLANIN_LIB_SOURCES 
    interpolant_2d.cpp
    interpolated_graph.cpp
    planar_surface.cpp
    ) 

add_library(planin STATIC ${PLANIN_LIB_SOURCES} ${HEADERS})  
target_link_libraries(planin ${})


###############################################
# Utilities 
###############################################

if(${BUILD_MAIN}) 
    set(SOURCES main.cpp)
    add_executable (main ${SOURCES} ${HEADERS})
    target_link_libraries(main planin)
endif()

if(${BUILD_UTILITY_BUILD_MESH})
    set(BUILD_MESH util/build_mesh.cpp) 
    add_executable (build_mesh ${BUILD_MESH} ${HEADERS})
    target_link_libraries(build_mesh planin)
endif()

if(${BUILD_UTILITY_REMOVE_ABOVE})
    set(REMOVE_ABOVE util/remove_above.cpp) 
    add_executable (remove_above ${REMOVE_ABOVE} ${HEADERS})
    target_link_libraries(remove_above planin)
endif()

if(${BUILD_UTILITY_REMOVE_BELOW})
    set(REMOVE_BELOW util/remove_below.cpp) 
    add_executable (remove_below ${REMOVE_BELOW} ${HEADERS})
    target_link_libraries(remove_below planin)
endif()

if(${BUILD_DAISY_EXAMPLE_2A})
    set(DAISY_EXAMPLE_2A examples/daisy_ex2a.cpp) 
    add_executable (daisy_ex2a ${DAISY_EXAMPLE_2A} ${HEADERS})
    target_link_libraries(daisy_ex2a planin)
endif()

if(${BUILD_DAISY_EXAMPLE_2B})
    set(DAISY_EXAMPLE_2B examples/daisy_ex2b.cpp) 
    add_executable (daisy_ex2b ${DAISY_EXAMPLE_2B} ${HEADERS})
    target_link_libraries(daisy_ex2b planin)
endif()

if(${BUILD_DAISY_EXAMPLE_3A})
    set(DAISY_EXAMPLE_3A examples/daisy_ex3a.cpp) 
    add_executable (daisy_ex3a ${DAISY_EXAMPLE_3A} ${HEADERS})
    target_link_libraries(daisy_ex3a planin)
endif()

if(${BUILD_DAISY_EXAMPLE_3B})
    set(DAISY_EXAMPLE_3B examples/daisy_ex3b.cpp) 
    add_executable (daisy_ex3b ${DAISY_EXAMPLE_3B} ${HEADERS})
    target_link_libraries(daisy_ex3b planin)
endif()

if(${BUILD_DAISY_EXAMPLE_4A})
    set(DAISY_EXAMPLE_4A examples/daisy_ex4a.cpp) 
    add_executable (daisy_ex4a ${DAISY_EXAMPLE_4A} ${HEADERS})
    target_link_libraries(daisy_ex4a planin)
endif()

if(${BUILD_DAISY_EXAMPLE_4B})
    set(DAISY_EXAMPLE_4B examples/daisy_ex4b.cpp) 
    add_executable (daisy_ex4b ${DAISY_EXAMPLE_4B} ${HEADERS})
    target_link_libraries(daisy_ex4b planin)
endif()

if(${BUILD_DAISY_EXAMPLE_5A})
    set(DAISY_EXAMPLE_5A examples/daisy_ex5a.cpp) 
    add_executable (daisy_ex5a ${DAISY_EXAMPLE_5A} ${HEADERS})
    target_link_libraries(daisy_ex5a planin)
endif()

if(${BUILD_DAISY_EXAMPLE_5B})
    set(DAISY_EXAMPLE_5B examples/daisy_ex5b.cpp) 
    add_executable (daisy_ex5b ${DAISY_EXAMPLE_5B} ${HEADERS})
    target_link_libraries(daisy_ex5b planin)
endif()


