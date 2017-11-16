cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( MainApp VERSION 3.1 LANGUAGES CXX )



include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )



set( MAINWINDOW_HEADERS     mainwindow.h
                            rrmapplication.h
                            widgets/realfeaturedslider.h
                            widgets/objecttree.h
                            widgets/objecttreeitem.h
                            widgets/color_picker.h
                            widgets/pages_stack.h )


set( MAINWINDOW_MOC_HEADERS mainwindow.h
                            widgets/realfeaturedslider.h
                            widgets/objecttree.h
                            widgets/color_picker.h
                            widgets/pages_stack.h )

set( MAINWINDOW_UI_SOURCES  widgets/volume_resize_widget.ui
                            widgets/object_properties.ui )


set( MAINWINDOW_SOURCES     main.cpp
                            mainwindow.cpp
                            rrmapplication.cpp
                            widgets/realfeaturedslider.cpp
                            widgets/objecttree.cpp
                            widgets/objecttreeitem.cpp
                            widgets/color_picker.cpp
                            widgets/pages_stack.cpp )


add_subdirectory( ../3dview ${CMAKE_BINARY_DIR}/3dview )
include_directories( ${3DVIEW_HEADERS} )


add_subdirectory( ../sketching ${CMAKE_BINARY_DIR}/sketching )
include_directories( ${SKETCHING_HEADERS} )



## Q_OBJECT files
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} )
QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )
QT5_WRAP_UI( UI_HEADERS ${UI_SOURCES} )


###############################################
# ADD TO LIBRARY AND LINK
###############################################

add_library( ${PROJECT_NAME} STATIC ${MAINWINDOW_HEADERS} ${MOC_SOURCES} ${UI_SOURCES} ${MAINWINDOW_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg
                                       ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} RRMBase 3dViewApp
                                       Sketching )