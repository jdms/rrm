project(RRM)

###############################################
# Main source -- Apps Directory
###############################################

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )
    
###############################################
# Main source -- MainWindow SubDirectory
###############################################
  
set( MAINWINDOW_HEADERS MainWindow/MainWindow.h
                        MainWindow/Scene.h
                        MainWindow/Controller.hpp
                        MainWindow/ExtrusionRulesProcessor.hpp
                        MainWindow/Exporter/CPS3Exporter.hpp
                        MainWindow/Exporter/IrapGridExporter.hpp
                        MainWindow/Help/HelpDialog.h
                        MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_MOC_HEADERS MainWindow/MainWindow.h
                            MainWindow/Scene.h
                            MainWindow/Controller.hpp
                            MainWindow/Help/HelpDialog.h
                            MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_UI_SOURCES  MainWindow/About/AboutWidget.ui
                            MainWindow/Help/HelpDialog.ui )


set( MAINWINDOW_SOURCES MainWindow/main.cpp
                        MainWindow/MainWindow.cpp
                        MainWindow/Scene.cpp
                        MainWindow/Controller.cpp
                        MainWindow/ExtrusionRulesProcessor.cpp
                        MainWindow/Exporter/CPS3Exporter.cpp
                        MainWindow/Exporter/IrapGridExporter.cpp
                        MainWindow/Help/HelpDialog.cpp
                        MainWindow/About/AboutWidget.cpp )




###############################################
# Main source -- Sketching SubDirectory
###############################################

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


###############################################
# Main source -- 3dView SubDirectory
###############################################

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



###############################################
# Wrapping Qt Elements
###############################################

## Q_OBJECT files
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} )

## Qt Designer Ui files
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )

## Resources
set( RCC_SOURCES Resources/images.qrc )


QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )
QT5_WRAP_UI( UI_HEADERS ${UI_SOURCES} )
QT5_ADD_RESOURCES( RCC_HEADERS ${RCC_SOURCES} )


				   
###############################################
# ADD TO EXECUTABLE AND LINK
###############################################


add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                               ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                               ${3DVIEW_SOURCES} ${3DVIEW_HEADERS} ${GEOLOGY_HEADERS}
                               ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )


target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets 
                                       Qt5::Core Qt5::Svg ${OPENGL_LIBRARIES} ${GLEW_LIBRARY}
                                        planin RRMGeometry Arrangement_2D SketchLib )

#target_include_directories( ${PROJECT_NAME} PUBLIC ${RRMGeology}/Models )

#add_custom_target( ${PROJECT_NAME} )

######################################################
# CUSTOM COMMANDS: COPYING AUX FILES TO EXECUTABLE DIR
######################################################


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/files/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/files/manual ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/manual )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/files/inputs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/inputs )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/files/models ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models )





if( EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )

#    if( x64 )

#        add_custom_command( TARGET ${PROJECT_NAME} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory
#                            ${CMAKE_SOURCE_DIR}/files/dlls/x64 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

#    else()

        add_custom_command( TARGET ${PROJECT_NAME} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/files/dlls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

#    endif( x64 )



endif(  EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )

