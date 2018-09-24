cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( 3dViewApp VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR} )
#                     ${TRIMESH2_INCLUDE_DIR} )



set( 3DVIEW_HEADERS      canvas3d.h
                         scene3d.h
                         models/planeshader.h
                         models/surfaceshader.h
                         models/volumeshader.h
                         models/regionshader.h )

set( 3DVIEW_MOC_HEADERS  canvas3d.h
                         scene3d.h )


set( 3DVIEW_SOURCES      canvas3d.cpp
                         scene3d.cpp
                         models/planeshader.cpp
                         models/surfaceshader.cpp
                         models/volumeshader.cpp
                         models/regionshader.cpp )


#QT5_WRAP_CPP( 3DVIEW_MOC_SOURCES ${3DVIEW_MOC_HEADERS} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################
# ADD TO LIBRARY AND LINK
###############################################


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


add_library( ${PROJECT_NAME} STATIC ${3DVIEW_HEADERS} ${3DVIEW_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
  #${TRIMESH2_LIB})

#add_library( ${PROJECT_NAME} STATIC ${3DVIEW_HEADERS} ${3DVIEW_MOC_SOURCES} ${3DVIEW_SOURCES} )
#target_link_libraries( ${PROJECT_NAME} Qt5::Widgets ${TRIMESH2_LIB})

