cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( Sketching VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )



set( SKETCHING_HEADERS      sketchwindow.h )

set( SKETCHING_MOC_HEADERS  sketchwindow.h )

set( SKETCHING_SOURCES      sketchwindow.cpp )


## Q_OBJECT files
set( MOC_HEADERS ${SKETCHING_MOC_HEADERS} )
QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )


###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${SKETCHING_HEADERS} ${MOC_SOURCES} ${SKETCHING_SOURCES} )

