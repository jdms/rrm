cmake_minimum_required( VERSION 3.3 FATAL_ERROR )

project( MainApp VERSION 3.1 LANGUAGES CXX )

find_package (Qt5Widgets)



set( MAINWINDOW_HEADERS      mainwindow.h
                             mainwindow1.h
                             sketchinterface.h
                             view3dinterface.h
                             rrmapplication.h  )



set( MAINWINDOW_MOC_HEADERS  mainwindow.h
                             mainwindow1.h
                             rrmapplication.h
                             sketchinterface.h
                             view3dinterface.h )



set( MAINWINDOW_SOURCES      main.cpp
                             mainwindow.cpp
                             mainwindow1.cpp
                             sketchinterface.cpp
                             view3dinterface.cpp
                             rrmapplication.cpp  )



qt5_wrap_cpp( MAINWINDOW_MOC_SRCS ${MAINWINDOW_MOC_HEADERS} )


add_subdirectory( ../3dview ${CMAKE_BINARY_DIR}/3dview )
include_directories( ${3DVIEW_HEADERS} )

add_subdirectory( ../sketching ${CMAKE_BINARY_DIR}/sketching )
include_directories( ${SKETCHING_HEADERS} )


include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


###############################################
# ADD TO LIBRARY AND LINK
###############################################


add_library( ${PROJECT_NAME} STATIC ${MAINWINDOW_SOURCES} ${MAINWINDOW_MOC_SRCS} ${MAINWINDOW_HEADERS} )

target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} RRMBase RRMModel RRMWidgets 3dViewApp Sketching ${STRATMODLIB}  ${HYPRE} TinyObjLoader )



