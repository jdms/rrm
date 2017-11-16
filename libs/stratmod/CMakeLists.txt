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

cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(StratMod CXX)

set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# Project options 
###############################################

# Set default build type (debug, release, etc.)

set(STRATMOD_BUILD_TYPE "Release" CACHE STRING
    "Set build type to: Debug, Release, RelWithDebInfo, or MinSizeRel")
set(CMAKE_BUILD_TYPE "${STRATMOD_BUILD_TYPE}" CACHE INTERNAL "" FORCE)


option(STRATMOD_BUILD_SHARED_LIBRARY "Build a shared library instead of static library" ON)
option(STRATMOD_BUILD_USING_OPENMP   "Use OpenMP for parallelization"                   ON)
option(STRATMOD_BUILD_USING_CEREAL   "Use Cereal library for serialization"             ON)
option(STRATMOD_BUILD_DOCS           "Build documentation with Doxygen"                 OFF)
option(STRATMOD_BUILD_TESTS          "Build with support for testing using Catch"       OFF)
option(STRATMOD_BUILD_EXAMPLE        "Build exmaples"                                   OFF) 

# regarding testing, see also: https://cmake.org/Wiki/CMake/Testing_With_CTest



###############################################
# Build in release mode
###############################################

# if(BUILD_FOR_RELEASE)
#     set(CMAKE_BUILD_TYPE RELEASE)
# endif()


###############################################
# Configure lib (shared or static)
###############################################

if(STRATMOD_BUILD_SHARED_LIBRARY)
    set(LIB_TYPE SHARED)
    set(PLANIN_FORCE_POSITION_INDEPENDENT_CODE ON)

else()
    set(LIB_TYPE STATIC)

endif()



###############################################
# Compiler flags (we support Clang (3.9+), 
# Gcc (5.4+) and VS2013 (revision 5+)). 
###############################################

if(MSVC)
    # Flag '/EHsc': the mere fact of including <iostream> yields warning C4530
    # in VS2013, which relates to exception handling.  The flag '/EHsc' seems
    # to be necessary to make VS2013 behave sanely. 

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3")

else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

endif()


###############################################
# Configure OpenMP for parallelization
###############################################

if(STRATMOD_BUILD_USING_OPENMP)
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP /openmp")

    else()
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Clang)")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP -fopenmp=libiomp5")

        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBUILD_USING_OPENMP -fopenmp")

        endif()
    endif()
endif()


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
# Use the Cereal library for serialization
###############################################

if(STRATMOD_BUILD_USING_CEREAL)
    add_definitions(-DBUILD_WITH_SERIALIZATION)

endif()


###############################################
# Auxiliary config files. 
###############################################

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include_directories("./")



###############################################
# Docs
###############################################

if (STRATMOD_BUILD_DOCS)
    include("${CMAKE_MODULE_PATH}/BuildDocumentation.cmake") 
endif(STRATMOD_BUILD_DOCS)



###############################################
# External libraries
###############################################

#  Eigen is mainly used as a wrapper for Lapack. 
# find_package(Eigen3 REQUIRED) 
# if (Eigen3_FOUND) 
#     include_directories(SYSTEM ${Eigen3_INCLUDE_DIRS})
# endif(Eigen3_FOUND) 



###############################################
# External projects
###############################################

include(ExternalProject)



###############################################
# PlanIn processes all modelling primitives
###############################################

set(PLANIN_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/planin)

set(PLANIN_CMAKE_ARGS 
    -DPLANIN_BUILD_DOCS=OFF 
    -DPLANIN_BUILD_TYPE=${STRATMOD_BUILD_TYPE}
    -DPLANIN_BUILD_USING_OPENMP=${STRATMOD_BUILD_USING_OPENMP}
    -DPLANIN_BUILD_USING_CEREAL=${STRATMOD_BUILD_USING_CEREAL}
    -DPLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE=${PLANIN_FORCE_POSITION_INDEPENDENT_CODE}
    )

if(STRATMOD_BUILD_USING_OPENMP AND APPLE)
    set(PLANIN_CMAKE_ARGS 
        -DPLANIN_BUILD_DOCS=OFF 
        -DPLANIN_BUILD_TYPE=${STRATMOD_BUILD_TYPE}
        -DPLANIN_BUILD_USING_OPENMP=ON 
        -DPLANIN_BUILD_USING_CEREAL=${STRATMOD_BUILD_USING_CEREAL}
        -DPLANIN_BUILD_FORCE_POSITION_INDEPENDENT_CODE=${PLANIN_FORCE_POSITION_INDEPENDENT_CODE}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_MODULE_PATH}/Darwin_Clang-OpenMP_Toolchain.cmake)
endif()

ExternalProject_Add(
    PlanIn
    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""
    SOURCE_DIR ${PLANIN_SOURCE}
    CMAKE_ARGS ${PLANIN_CMAKE_ARGS} 
    )


add_library(planin STATIC IMPORTED)
add_dependencies(planin PlanIn)
include_directories(${PLANIN_SOURCE})

set(PLANINLIB "${PLANIN_SOURCE}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}planin${CMAKE_STATIC_LIBRARY_SUFFIX}")
set_property(TARGET planin PROPERTY IMPORTED_LOCATION ${PLANINLIB})



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

    include_directories(${CATCH_SOURCE})

endif()



###############################################
# Main library
###############################################

include_directories(${CMAKE_SOURCE_DIR}/src)

set(HEADERS 
    src/win_dll_export_handler.hpp
    src/smodeller_primitives.hpp
    src/smodeller.hpp
    src/sutilities.hpp
    )

set(STRATMOD_LIB_SOURCES 
    src/smodeller_impl.cpp
    src/smodeller.cpp
    src/sutilities.cpp
    ) 


add_library(stratmod ${LIB_TYPE} ${STRATMOD_LIB_SOURCES} ${HEADERS})  
target_link_libraries(stratmod planin)
add_dependencies(stratmod planin)


set(INSTALL_DESTINATION ${CMAKE_SOURCE_DIR}/stratmod)

install(TARGETS stratmod
    ARCHIVE DESTINATION ${INSTALL_DESTINATION}/lib
    LIBRARY DESTINATION ${INSTALL_DESTINATION}/lib
    RUNTIME DESTINATION ${INSTALL_DESTINATION}/lib
    COMPONENT library
    )

install(FILES ${HEADERS}
    DESTINATION ${INSTALL_DESTINATION}/include
    )


###############################################
# Executables and utilities 
###############################################

if(${STRATMOD_BUILD_TESTS}) 
    set(TESTING_SOURCES src/main_tests.cpp src/first_test.cpp src/smodeller_impl.cpp)
    add_executable (run_tests ${TESTING_SOURCES} ${HEADERS})
    target_link_libraries(run_tests planin)
    add_dependencies(run_tests planin)
    add_dependencies(run_tests Catch-build)

    enable_testing(TRUE)
    add_test("Main" run_tests)

endif()


if(${STRATMOD_BUILD_EXAMPLE})
    set(SOURCES src/main.cpp)
    add_executable(run_me ${SOURCES} ${HEADERS})
    target_link_libraries(run_me stratmod)
    #    add_dependencies(run_me planin)

endif()

set(SOURCES src/test_boundary.cpp)
add_executable(test_boundary ${SOURCES} ${HEADERS})
target_link_libraries(test_boundary stratmod)

