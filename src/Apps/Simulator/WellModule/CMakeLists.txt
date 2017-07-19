# Target name
set(target qt5-wells_plugin)

#include(${CMAKE_MODULE_PATH}/cmake-init/CompileOptions.cmake)

find_package(Eigen3 REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)

# Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND)
    message(STATUS "Example ${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "Example ${target}")
endif()
 
# 
# Sources
#
 
 
set(sources    
    MainWindow.hpp
    MainWindow.cpp
    main.cpp
    AboutWidget/AboutWidget.cpp
    AboutWidget/AboutWidget.hpp
    WellScene.hpp
    WellScene.cpp
    WellCanvas.hpp
    WellCanvas.cpp
    WellItem.cpp
    WellItem.hpp
    WidgetWells.hpp
    WidgetWells.cpp
)
# Build executable
add_executable(${target}
    ${sources}
    icons.qrc
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "Plugins/Wells"
    CXX_STANDARD 11
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    SYSTEM
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${EIGEN3_INCLUDE_DIR}	
    ${CMAKE_SOURCE_DIR}/libs # ${CMAKE_SOURCE_DIR} CMake Root Directory
)

message(${CMAKE_SOURCE_DIR}/libs)

# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}    
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets    	
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}   
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}   
)
