cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( 3dViewApp VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )



set( 3DVIEW_HEADERS      canvas3d.h
                         scene3d.h )

set( 3DVIEW_MOC_HEADERS  canvas3d.h
                         scene3d.h )


set( 3DVIEW_SOURCES      canvas3d.cpp
                         scene3d.cpp )



## Q_OBJECT files
set( MOC_HEADERS ${3DVIEW_MOC_HEADERS} )
QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )


###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${3DVIEW_HEADERS} ${MOC_SOURCES} ${3DVIEW_SOURCES} )

