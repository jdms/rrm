cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(diagnostics LANGUAGES CXX)


###############################################
# Create stub library interface
###############################################

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/rrm-fd-ui/src/rrm/fd)

if (NOT TARGET diagnostics) 
    # add_library(diagnostics INTERFACE)
    add_library(diagnostics ALIAS rrm_flow_diagnostics_lib)
endif()
