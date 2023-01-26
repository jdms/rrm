cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(diagnostics LANGUAGES CXX)


###############################################
# Create stub library interface
###############################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rrm-fd-ui/src/rrm/fd)

set(RRM_DIAGNOSTICS_SOURCES
    fd_definitions.cpp
    fd_definitions.h
    fd_interface.cpp
    fd_interface.hpp
    model/metadata_access.h
    )

# if (NOT TARGET diagnostics) 
    # add_library(diagnostics INTERFACE)
# endif()

add_library(diagnostics ${RRM_DIAGNOSTICS_SOURCES})

target_include_directories(diagnostics PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(diagnostics PRIVATE cxx_std_17)
target_link_libraries(diagnostics PRIVATE Qt5::Gui Qt5::Widgets stratmod colorwrap rrm_flow_diagnostics_lib)
