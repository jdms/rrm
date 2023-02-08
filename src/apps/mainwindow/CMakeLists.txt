cmake_minimum_required( VERSION 3.1 FATAL_ERROR )

project( MainApp LANGUAGES CXX )

find_package( Qt5Widgets REQUIRED )



set( MAINWINDOW_HEADERS      mainwindow.h
                             sketchinterface.h
                             view3dinterface.h
                             diagnosticsinterface.h
                             rrmapplication.h
                             # flow_window.h
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

# Use cmake to automatically update RRM version and git
# repository info in the about window.
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/about)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/about)

add_subdirectory( ../3dview ${CMAKE_BINARY_DIR}/3dview )
include_directories( ${3DVIEW_HEADERS} )

add_subdirectory( ../sketching ${CMAKE_BINARY_DIR}/sketching )
include_directories( ${SKETCHING_HEADERS} )


# add_subdirectory( ../simulator ${CMAKE_BINARY_DIR}/simulator )
include_directories( ${SIMULATOR_HEADERS} )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${MAINWINDOW_SOURCES} ${MAINWINDOW_MOC_SRCS} ${MAINWINDOW_HEADERS} )

target_include_directories(${PROJECT_NAME} PUBLIC CMAKE_CURRENT_SOURCE_DIR)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/about)
target_include_directories(${PROJECT_NAME} PUBLIC ${3DVIEW_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SKETCHING_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SIMULATOR_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/MainApp_autogen/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/MainApp_autogen/include_Debug)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/MainApp_autogen/include_Release)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/MainApp_autogen/include_RelWithDebInfo)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR}/MainApp_autogen/include_MinSizeRel)


#target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
# target_link_libraries( ${PROJECT_NAME} RRMBase RRMModel RRMWidgets 3dViewApp Sketching FlowDiagnosticsApp )
target_link_libraries( ${PROJECT_NAME} RRMBase RRMModel RRMWidgets 3dViewApp Sketching RRMAppsIncludes )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg ${OPENGL_LIBRARIES} ) #${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )
if(WIN32)
    #target_link_libraries( ${PROJECT_NAME} stratmod TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
else()
    #target_link_libraries( ${PROJECT_NAME} stratmod OpenMP::OpenMP_CXX TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} TinyObjLoader )
    target_link_libraries( ${PROJECT_NAME} GLEW::GLEW )
endif()

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)



