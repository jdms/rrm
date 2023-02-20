###############################################
# Preamble
###############################################

cmake_minimum_required(VERSION 3.10)

project(StratmodWrapper)

# For vim's YouCompleteMe plugin
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# Wrap stratmod library
###############################################

# set(STRATMOD_BUILD_USING_OPENMP OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/stratmod)

add_library(stratwrap INTERFACE)
target_compile_features(stratwrap INTERFACE cxx_std_17)
target_include_directories(stratwrap INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(stratwrap INTERFACE stratmod)


#######################################################
# Build dependencies in the same folder as upstream 
# project executable
#######################################################

set_target_properties(stratmod PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(stratmod PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)


#######################################################
# The following is for testing, not for external use
#######################################################

add_executable(stratwrap_test_build stratwrap.cpp)
target_link_libraries(stratwrap_test_build stratwrap)


