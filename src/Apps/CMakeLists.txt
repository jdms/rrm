project(RRM)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

## Show all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()


## Grab MainWindow Files

set( MAINWINDOW_HEADERS MainWindow/MainWindow.h
                        MainWindow/Scene.h
                        MainWindow/Controller.hpp
                        MainWindow/ExtrusionRulesProcessor.hpp
                        MainWindow/HelpDialog.h
                        MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_MOC_HEADERS MainWindow/MainWindow.h
                            MainWindow/Scene.h
                            MainWindow/Controller.hpp
                            MainWindow/HelpDialog.h
                            MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_UI_SOURCES  MainWindow/About/AboutWidget.ui
                            MainWindow/HelpDialog.ui )


set( MAINWINDOW_SOURCES MainWindow/main.cpp
                        MainWindow/MainWindow.cpp
                        MainWindow/Scene.cpp
                        MainWindow/Controller.cpp
                        MainWindow/ExtrusionRulesProcessor.cpp
                        MainWindow/HelpDialog.cpp
                        MainWindow/About/AboutWidget.cpp )




## Grab Sketching Files
set( SKETCHING_HEADERS Sketching/StratigraphicItem.hpp
                       Sketching/InputSketch.h
                       Sketching/PolyQtUtils.hpp
                       Sketching/BoundaryItem2D.hpp
                       Sketching/SketchingWindow.h
                       Sketching/SketchingCanvas.h )


set( SKETCHING_MOC_HEADERS  Sketching/SketchingWindow.h
                            Sketching/SketchingCanvas.h )



set( SKETCHING_SOURCES  Sketching/StratigraphicItem.cpp
                        Sketching/InputSketch.cpp
                        Sketching/PolyQtUtils.cpp
                        Sketching/BoundaryItem2D.cpp
                        Sketching/SketchingWindow.cpp
                        Sketching/SketchingCanvas.cpp )


## Grab 3DView Files
set( 3DVIEW_HEADERS 3dView/BoundingBox3D.h
                       3dView/CrossPlane.h
                       3dView/View3DCanvas.hpp
                       3dView/Surface.h
                       3dView/View3DWindow.hpp
                       3dView/Model3DUtils.hpp
                       3dView/GradientBackgroundShader.hpp )


set( 3DVIEW_MOC_HEADERS  3dView/View3DCanvas.hpp
                         3dView/View3DWindow.hpp )



set( 3DVIEW_SOURCES 3dView/BoundingBox3D.cpp
                    3dView/CrossPlane.cpp
                    3dView/View3DCanvas.cpp
                    3dView/Surface.cpp
                    3dView/View3DWindow.cpp
                    3dView/Model3DUtils.cpp
                    3dView/GradientBackgroundShader.cpp )





## Grab 3DView Files
set( MODEL_HEADERS Model/Boundary.hpp
                   Model/BoundaryRenderable.h
                   Model/CrossSection.hpp
                   Model/StratigraphyRenderable.hpp
                   Model/Intersection.hpp
                   Model/Stratigraphy.hpp )




set( MODEL_SOURCES  )




## Q_OBJECT files
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} )


## Qt Designer Ui files
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )


## Resources
set( RCC_SOURCES Resources/images.qrc )



QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS})
QT5_WRAP_UI (UI_HEADERS ${UI_SOURCES})
QT5_ADD_RESOURCES(RCC_HEADERS ${RCC_SOURCES})



IF(WIN32) # Check if we are on Windows

    if(MSVC) # Check if we are using the Visual Studio compiler

        add_definitions(/arch:SSE2)

        message(STATUS "- MSVC: Enabled SSE2 support")


        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
                                       ${MODEL_SOURCES} ${MODEL_HEADERS}
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )



        target_link_libraries(${PROJECT_NAME} Qt5::WinMain Qt5::Gui Qt5::Core Qt5::Help Qt5::OpenGL Qt5::Widgets Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SAMG_LIBRARY} )



    elseif(CMAKE_COMPILER_IS_GNUCXX)  # MinGW on Windows
        message (FATAL_ERROR "GCC are not support on Windows !")

    else() # Nor MSVC or GCC (MinGW)
        message(FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")

    endif()


ELSEIF(UNIX)

    # Nothing special required for Linux
    message(STATUS "Build on Linux")

        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )


        target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )


ELSE() # Not Windows or Unix
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")


ENDIF()





