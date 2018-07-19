cmake_minimum_required( VERSION 3.3 FATAL_ERROR )

project( 3dViewApp VERSION 3.1 LANGUAGES CXX )

find_package (Qt5Widgets)


set( 3DVIEW_HEADERS         canvas3d.h
                            scene3d.h
                            view3dcontroller.h
                            models/planeshader.h
                            models/surfaceshader.h
                            models/volumeshader.h
                            models/regionshader.h )

set( 3DVIEW_MOC_HEADERS     canvas3d.h
                            scene3d.h )

set( 3DVIEW_SOURCES         canvas3d.cpp
                            scene3d.cpp
                            view3dcontroller.cpp
                            models/planeshader.cpp
                            models/surfaceshader.cpp
                            models/volumeshader.cpp
                            models/regionshader.cpp )


qt5_wrap_cpp( 3DVIEW_MOC_SRCS ${3DVIEW_MOC_HEADERS})


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${3DVIEW_MOC_SRCS} ${3DVIEW_SOURCES} ${3DVIEW_HEADERS} )






