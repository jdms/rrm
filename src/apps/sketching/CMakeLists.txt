cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( Sketching VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )



set( SKETCHING_HEADERS      sketchwindow.h
                            canvasstack.h
                            sketchscene.h
                            inputsketch.h
                            item_wrappers/volume_item_wrapper.h
                            item_wrappers/object_item_wrapper.h
                            item_wrappers/crosssection_item_wrapper.h
                            item_wrappers/trajectoryitemwrapper.h
                            item_wrappers/coordinate_axes_2d.h
                            item_wrappers/image_item_wrapper.h )

set( SKETCHING_MOC_HEADERS  sketchwindow.h
                            canvasstack.h
                            sketchscene.h )


set( SKETCHING_SOURCES      sketchwindow.cpp
                            canvasstack.cpp
                            sketchscene.cpp
                            inputsketch.cpp
                            item_wrappers/volume_item_wrapper.cpp
                            item_wrappers/object_item_wrapper.cpp
                            item_wrappers/crosssection_item_wrapper.cpp
                            item_wrappers/trajectoryitemwrapper.cpp
                            item_wrappers/coordinate_axes_2d.cpp
                            item_wrappers/image_item_wrapper.cpp )



## Q_OBJECT files
set( MOC_HEADERS ${SKETCHING_MOC_HEADERS} )
QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )


###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${SKETCHING_HEADERS} ${MOC_SOURCES} ${SKETCHING_SOURCES} )

