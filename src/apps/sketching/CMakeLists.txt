cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( Sketching VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )


set( SKETCHING_HEADERS      sketchwindow.h
                            sketchscene.h
                            sketchingcanvas.h
                            sketchingcontroller.h
                            models/qtgeometries/curveitem.h
                            models/qtgeometries/lineitem.h
                            models/qtgeometries/planeitem.h
                            models/qtgeometries/polygonitem.h
                            models/crosssectionitem.h
                            models/regionitem.h
                            models/stratigraphyitem.h
                            models/volumeitem.h
                            models/inputsketch.h
                            models/volume_item_wrapper.h
                            models/object_item_wrapper.h
                            models/crosssection_item_wrapper.h
                            models/trajectoryitemwrapper.h
                            models/coordinate_axes_2d.h
                            models/image_item_wrapper.h )

set( SKETCHING_MOC_HEADERS  sketchwindow.h
                            sketchscene.h )


set( SKETCHING_SOURCES      sketchwindow.cpp
                            sketchscene.cpp
                            sketchingcanvas.cpp
                            sketchingcontroller.cpp
                            models/qtgeometries/curveitem.cpp
                            models/qtgeometries/lineitem.cpp
                            models/qtgeometries/planeitem.cpp
                            models/qtgeometries/polygonitem.cpp
                            models/crosssectionitem.cpp
                            models/regionitem.cpp
                            models/stratigraphyitem.cpp
                            models/volumeitem.cpp
                            models/inputsketch.cpp
                            models/volume_item_wrapper.cpp
                            models/object_item_wrapper.cpp
                            models/crosssection_item_wrapper.cpp
                            models/trajectoryitemwrapper.cpp
                            models/coordinate_axes_2d.cpp
                            models/image_item_wrapper.cpp )


#QT5_WRAP_CPP( SKETCHING_MOC_SOURCES ${SKETCHING_MOC_HEADERS} )
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${SKETCHING_SOURCES} ${SKETCHING_HEADERS} )

#add_library( ${PROJECT_NAME} STATIC ${SKETCHING_HEADERS} ${SKETCHING_MOC_SOURCES} ${SKETCHING_SOURCES} )

