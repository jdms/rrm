cmake_minimum_required( VERSION 3.1 FATAL_ERROR )

project( RRMWidgets VERSION 3.1 LANGUAGES CXX )

find_package( Qt5 COMPONENTS Widgets REQUIRED )


set( WIDGETS_HEADERS        realfeaturedslider.h
                            objecttree.h
                            objecttreeitem.h
                            color_picker.h
                            canvasstack.h
                            pages_stack.h
                            slider.h )

set( WIDGETS_MOC_HEADERS    realfeaturedslider.h
                            objecttree.h
                            objecttreeitem.h
                            color_picker.h
                            canvasstack.h
                            pages_stack.h
                            slider.h )


set( WIDGETS_UI_SOURCES     volume_resize_widget.ui
                            object_properties.ui )


set( WIDGETS_SOURCES        realfeaturedslider.cpp
                            objecttree.cpp
                            objecttreeitem.cpp
                            color_picker.cpp
                            pages_stack.cpp
                            canvasstack.cpp
                            slider.cpp )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )

add_library( ${PROJECT_NAME} STATIC ${WIDGETS_HEADERS} ${WIDGETS_UI_SOURCES} ${WIDGETS_SOURCES} )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )

#qt5_wrap_cpp( WIDGETS_MOC_SRCS ${WIDGETS_MOC_HEADERS} )
#qt5_wrap_ui ( WIDGETS_UI_HDRS ${WIDGETS_UI_SOURCES} )


#include_directories (${PROJECT_SOURCE_DIR})
#include_directories (${PROJECT_BINARY_DIR})


################################################
## ADD TO LIBRARY AND LINK
################################################


#add_library( ${PROJECT_NAME} STATIC ${WIDGETS_SOURCES} ${WIDGETS_MOC_SRCS} ${WIDGETS_UI_HDRS} ${WIDGETS_HEADERS} )






