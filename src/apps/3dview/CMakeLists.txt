cmake_minimum_required( VERSION 3.1 FATAL_ERROR )

project( 3dViewApp VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt5 COMPONENTS Widgets REQUIRED )


set( 3DVIEW_HEADERS         canvas3d.h
                            scene3d.h
                            view3dcontroller.h
                            coordinate_axes_3d.h
                            models/planeshader.h
                            models/surfaceshader.h
                            models/volumeshader.h
                            models/regionshader.h )

set( 3DVIEW_MOC_HEADERS     canvas3d.h
                            scene3d.h )

set( 3DVIEW_SOURCES         canvas3d.cpp
                            scene3d.cpp
                            view3dcontroller.cpp
                            coordinate_axes_3d.cpp
                            models/planeshader.cpp
                            models/surfaceshader.cpp
                            models/volumeshader.cpp
                            models/regionshader.cpp )


#qt5_wrap_cpp( 3DVIEW_MOC_SRCS ${3DVIEW_MOC_HEADERS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${3DVIEW_MOC_SRCS} ${3DVIEW_SOURCES} ${3DVIEW_HEADERS} )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )

if(WIN32)
    target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
    target_link_libraries( ${PROJECT_NAME} ${OPENGL_LIBRARY} )
else()
    target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
    target_link_libraries( ${PROJECT_NAME} OpenGL::GL )
endif()

