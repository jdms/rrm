cmake_minimum_required( VERSION 3.3 FATAL_ERROR )

project( RRMModel VERSION 3.1 LANGUAGES CXX )

find_package (Qt5Widgets)


#####################################################################
# Options
#####################################################################

include_directories( SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/colormap)


ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)



set( MODELS_HEADERS       controller.h
                          object.h
                          regions.h
                          volume.h
                          polycurve.h
                          surface.h
                          crosssection.h
                          scene.h
                          shader.h
                          stratigraphy.h
                          rules_processor.hpp
                          qt_popup.hpp
                          stratmod_wrapper.hpp
                          irap_grid_exporter.hpp
                          )

set( MODELS_SOURCES       controller.cpp
                          object.cpp
                          regions.cpp
                          volume.cpp
                          crosssection.cpp
                          stratigraphy.cpp
                          rules_processor.cpp
                          qt_popup.cpp
                          irap_grid_exporter.cpp 
                          )


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${MODELS_HEADERS} ${MODELS_SOURCES} )
target_link_libraries( ${PROJECT_NAME} colorwrap )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets  )
