cmake_minimum_required( VERSION 3.1 FATAL_ERROR )

project( MainApp VERSION 3.1 LANGUAGES CXX )

find_package( Qt5Widgets REQUIRED )



set( MAINWINDOW_HEADERS      mainwindow.h
                             sketchinterface.h
                             view3dinterface.h
                             diagnosticsinterface.h
                             rrmapplication.h
                             about/about_widget.hpp  )



set( MAINWINDOW_MOC_HEADERS  mainwindow.h
                             rrmapplication.h
                             sketchinterface.h
                             diagnosticsinterface.h
                             view3dinterface.h )

set( MAINWINDOW_SOURCES     about/about_widget.ui )


set( MAINWINDOW_SOURCES      main.cpp
                             mainwindow.cpp
                             sketchinterface.cpp
                             view3dinterface.cpp
                             diagnosticsinterface.cpp
                             rrmapplication.cpp
                             about/about_widget.cpp  )



#qt5_wrap_cpp( MAINWINDOW_MOC_SRCS ${MAINWINDOW_MOC_HEADERS} )


add_subdirectory( ../3dview ${CMAKE_BINARY_DIR}/3dview )
include_directories( ${3DVIEW_HEADERS} )

add_subdirectory( ../sketching ${CMAKE_BINARY_DIR}/sketching )
include_directories( ${SKETCHING_HEADERS} )


add_subdirectory( ../simulator ${CMAKE_BINARY_DIR}/simulator )
include_directories( ${SIMULATOR_HEADERS} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${MAINWINDOW_SOURCES} ${MAINWINDOW_MOC_SRCS} ${MAINWINDOW_HEADERS} )

#target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
target_link_libraries( ${PROJECT_NAME} RRMBase RRMModel RRMWidgets 3dViewApp Sketching FlowDiagnosticsApp )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg ${OPENGL_LIBRARIES} ) #${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )
target_link_libraries( ${PROJECT_NAME} ${HYPRE} )
if(WIN32)
    #target_link_libraries( ${PROJECT_NAME} stratmod TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} ${GLEW_LIBRARY} )
else()
    #target_link_libraries( ${PROJECT_NAME} stratmod OpenMP::OpenMP_CXX TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} OpenMP::OpenMP_CXX TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
endif()



