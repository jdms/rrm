cmake_minimum_required( VERSION 3.3 FATAL_ERROR )


project( MainApp VERSION 3.1 LANGUAGES CXX )



include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )



set( MAINWINDOW_HEADERS     mainwindow.h
                            rrmapplication.h )


set( MAINWINDOW_MOC_HEADERS mainwindow.h )




set( MAINWINDOW_SOURCES     main.cpp
                            mainwindow.cpp
                            rrmapplication.cpp )


add_subdirectory( ../3dview ${CMAKE_BINARY_DIR}/3dview )
include_directories( ${3DVIEW_HEADERS} )


add_subdirectory( ../sketching ${CMAKE_BINARY_DIR}/sketching )
include_directories( ${SKETCHING_HEADERS} )


#add_subdirectory( ../simulator ${CMAKE_BINARY_DIR}/simulator )
#include_directories( ${SIMULATOR_HEADERS} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################
# ADD TO LIBRARY AND LINK
###############################################

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


add_library( ${PROJECT_NAME} STATIC ${MAINWINDOW_HEADERS} ${MAINWINDOW_MOC_SOURCES} ${MAINWINDOW_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg
                                       ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} RRMBase RRMModel RRMWidgets 3dViewApp
                                       Sketching ${STRATMODLIB}  ${HYPRE} ${OPENVOLUMEMESH_LIBRARY} TinyObjLoader )
