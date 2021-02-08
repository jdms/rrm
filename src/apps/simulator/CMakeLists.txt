cmake_minimum_required( VERSION 3.1 FATAL_ERROR )


project( FlowDiagnosticsApp VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt5Widgets REQUIRED )
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)


################################################################
## TODO: research how cmake interacts with Qt

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# Is the following required? 
# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)

################################################################


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )

# The following code lists all directories already included in the project
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
#   message(STATUS "dir='${dir}'")
# endforeach()


set( SIMULATOR_HEADERS
                flow_window.h)

set ( SIMULATOR_MOC_HEADERS
                flow_window.h)

set ( SIMULATOR_UI_SOURCES
                )

set( SIMULATOR_SOURCES
               flow_window.cpp)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################
# ADD TO LIBRARY AND LINK
###############################################

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

add_library( ${PROJECT_NAME} STATIC ${SIMULATOR_HEADERS} ${SIMULATOR_MOC_HEADERS} ${SIMULATOR_UI_SOURCES} ${SIMULATOR_SOURCES} )
target_link_libraries( ${PROJECT_NAME} RRMModel )
if(WIN32)
    target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
else()
    target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
    target_link_libraries( ${PROJECT_NAME} OpenGL::GL GLEW::GLEW )
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
