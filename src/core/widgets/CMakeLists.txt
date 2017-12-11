cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMWidgets )




include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

#####################################################################
# Options
#####################################################################

set( WIDGETS_HEADERS realfeaturedslider.h
                     objecttree.h
                     objecttreeitem.h
                     color_picker.h
                     pages_stack.h
                     canvasstack.h
                     slider.h )


set( WIDGETS_MOC_HEADERS realfeaturedslider.h
                         objecttree.h
                         color_picker.h
                         pages_stack.h
                         canvasstack.h
                         slider.h )


set( WIDGETS_UI_SOURCES  volume_resize_widget.ui
                         object_properties.ui )


set( WIDGETS_SOURCES realfeaturedslider.cpp
                     objecttree.cpp
                     objecttreeitem.cpp
                     color_picker.cpp
                     pages_stack.cpp
                     canvasstack.cpp
                     slider.cpp )


#set(CMAKE_AUTOMOC ON)


#qt5_wrap_ui
QT5_WRAP_CPP( WIDGETS_MOC_SOURCES ${WIDGETS_MOC_HEADERS} )
QT5_WRAP_UI( WIDGETS_MOC_UI ${WIDGETS_UI_SOURCES} )

add_library( ${PROJECT_NAME} STATIC ${WIDGETS_HEADERS} ${WIDGETS_MOC_SOURCES} ${WIDGETS_MOC_UI} ${WIDGETS_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
