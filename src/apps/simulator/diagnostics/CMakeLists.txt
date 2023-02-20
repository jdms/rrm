cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(diagnostics LANGUAGES CXX)


###############################################
# Create stub library interface
###############################################

find_package(pugixml QUIET)

# The files in the resources folder are not being copied to the right place
# set(FD_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dp-69/src/rrm/fd)

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
add_custom_command(TARGET diagnostics
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/dp-69/src/rrm/fd/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/_config
    )

if(WIN32)
    add_custom_command(TARGET diagnostics
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/dp-69/src/rrm/fd/resources ${CMAKE_BINARY_DIR}/src/apps/_config
        )
endif()

target_include_directories(diagnostics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(diagnostics PUBLIC cxx_std_17)
target_link_libraries(diagnostics PRIVATE Qt5::Gui Qt5::Widgets stratmod colorwrap rrm_flow_diagnostics_lib)
