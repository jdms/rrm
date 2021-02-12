cmake_minimum_required( VERSION 3.1 FATAL_ERROR )

project( Sketching VERSION 3.1 LANGUAGES CXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt5Widgets REQUIRED )

set( SKETCHING_HEADERS      sketchwindow.h
                            sketchscene.h
                            sketchingcanvas.h
                            sketchingcontroller.h
                            lateralbar.h
                            models/qtgeometries/curveitem.h
                            models/qtgeometries/lineitem.h
                            models/qtgeometries/planeitem.h
                            models/qtgeometries/polygonitem.h
                            models/coordinateaxes_2d.h
                            models/image_item_wrapper.h
                            models/dipanglepicture.h
                            models/crosssectionitem.h
                            models/regionitem.h
                            models/stratigraphyitem.h
                            models/volumeitem.h
                            models/anglepicture.h )



set( SKETCHING_MOC_HEADERS  sketchwindow.h
                            sketchscene.h
                            lateralbar.h )



set( SKETCHING_SOURCES      sketchwindow.cpp
                            sketchscene.cpp
                            sketchingcanvas.cpp
                            sketchingcontroller.cpp
                            lateralbar.cpp
                            models/qtgeometries/curveitem.cpp
                            models/qtgeometries/lineitem.cpp
                            models/qtgeometries/planeitem.cpp
                            models/qtgeometries/polygonitem.cpp
                            models/coordinateaxes_2d.cpp
                            models/image_item_wrapper.cpp
                            models/dipanglepicture.cpp
                            models/crosssectionitem.cpp
                            models/regionitem.cpp
                            models/stratigraphyitem.cpp
                            models/volumeitem.cpp
                            models/anglepicture.cpp )



#qt5_wrap_cpp( SKETCHING_MOC_SRCS ${SKETCHING_MOC_HEADERS})


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${SKETCHING_SOURCES} ${SKETCHING_MOC_SRCS} ${SKETCHING_HEADERS} )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)






