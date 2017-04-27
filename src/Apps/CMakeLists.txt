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
                        MainWindow/Controller.hpp
                        MainWindow/RulesProcessor.hpp
                        MainWindow/Exporter/CPS3Exporter.hpp
                        MainWindow/Exporter/IrapGridExporter.hpp
                        MainWindow/Object_Tree/ObjectTreeItem.h
                        MainWindow/Object_Tree/ObjectTree.h
                        MainWindow/Help/HelpDialog.h
                        MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_MOC_HEADERS MainWindow/MainWindow.h
                            MainWindow/Controller.hpp
                            MainWindow/Object_Tree/ObjectTree.h
                            MainWindow/Help/HelpDialog.h
                            MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_UI_SOURCES  MainWindow/About/AboutWidget.ui
                            MainWindow/Help/HelpDialog.ui
                            MainWindow/MainWindow.ui )


set( MAINWINDOW_SOURCES MainWindow/main.cpp
                        MainWindow/MainWindow.cpp
                        MainWindow/Controller.cpp
                       MainWindow/RulesProcessor.cpp
                        MainWindow/Exporter/CPS3Exporter.cpp
                        MainWindow/Exporter/IrapGridExporter.cpp
                        MainWindow/Object_Tree/ObjectTreeItem.cpp
                        MainWindow/Object_Tree/ObjectTree.cpp
                        MainWindow/Help/HelpDialog.cpp
                        MainWindow/About/AboutWidget.cpp )




###############################################
# Main source -- Sketching SubDirectory
###############################################

set( SKETCHING_HEADERS        Sketching/Inputsketch.h
                              Sketching/CanvasPath.h
                              Sketching/PathScene.h
                              Sketching/SketchCanvas.h
                              Sketching/SketchScene.h
                              Sketching/ItemWrappers/ObjectItemWrap.h
                              Sketching/ItemWrappers/VolumeItemWrapper.h
                              Sketching/ItemWrappers/CrossSectionItemWrapper.h
                              Sketching/ItemWrappers/RegionItem.hpp
                              Sketching/PolyQtUtils.hpp )


set( SKETCHING_MOC_HEADERS    Sketching/CanvasPath.h
                              Sketching/PathScene.h
                              Sketching/SketchCanvas.h
                              Sketching/SketchScene.h )



set( SKETCHING_SOURCES        Sketching/Inputsketch.cpp
                              Sketching/CanvasPath.cpp
                              Sketching/PathScene.cpp
                              Sketching/SketchCanvas.cpp
                              Sketching/SketchScene.cpp
                              Sketching/ItemWrappers/ObjectItemWrap.cpp
                              Sketching/ItemWrappers/VolumeItemWrapper.cpp
                              Sketching/ItemWrappers/RegionItem.cpp
                              Sketching/PolyQtUtils.cpp )


###############################################
# Main source -- 3dView SubDirectory
###############################################

set( 3DVIEW_HEADERS    3dView/Canvas3D.h
                       3dView/Scene3D.h
                       3dView/Model3DUtils.hpp
                       3dView/OpenGLWrappers/ObjectOpenGLWrapper.h
                       3dView/OpenGLWrappers/VolumeOpenGLWrap.h
                       Sketching/ItemWrappers/CrossSectionItemWrapper.cpp
                       3dView/GradientBackgroundShader.hpp )


set( 3DVIEW_MOC_HEADERS  3dView/Canvas3D.h
                         3dView/Scene3D.h )



set( 3DVIEW_SOURCES     3dView/Canvas3D.cpp
                        3dView/Scene3D.cpp
                        3dView/Model3DUtils.cpp
                        3dView/OpenGLWrappers/ObjectOpenGLWrapper.cpp
                        3dView/OpenGLWrappers/VolumeOpenGLWrap.cpp
                        3dView/GradientBackgroundShader.cpp )


###############################################
# Main source -- FlowDiagnostics SubDirectory
###############################################					
					
					
#set( SIMULATOR_HEADERS
#                 Simulator/tinyobjloader/tiny_obj_loader.h
#                 Simulator/ColorMap.h
#                 Simulator/CoordinateAxes.h
#                 Simulator/FlowCrossSection.h
#                 Simulator/FlowParametersBar.h
#                 Simulator/FlowRenderingOptionsMenu.h
#                 Simulator/FlowVisualizationCanvas.h
#                 Simulator/FlowVisualizationController.h
#                 Simulator/FlowWindow.h
#                 Simulator/FormPropertyValues.h
#                 Simulator/Mesh.h
#                 Simulator/NormalMovableCrossSectionFlow.h
#                 Simulator/ProgressCounter.h
#                 Simulator/ColorBar.h
#                 Simulator/FlowDiagnosticsInterface.hpp
#                 Simulator/PoreVolumeResultsForm.h
#                 Simulator/FlowComputation/bsurface.h
#                 Simulator/FlowComputation/cpgelement.h
#                 Simulator/FlowComputation/curve.h
#                 Simulator/FlowComputation/edge.h
#                 Simulator/FlowComputation/facet.h
#                 Simulator/FlowComputation/geosurface.h
#                 Simulator/FlowComputation/graph.h
#                 Simulator/FlowComputation/graphedge.h
#                 Simulator/FlowComputation/graphnode.h
#                 Simulator/FlowComputation/linearfunc.h
#                 Simulator/FlowComputation/mathzz.h
#                 Simulator/FlowComputation/matrix.h
#                 Simulator/FlowComputation/model.h
#                 Simulator/FlowComputation/meshinfo.h
#                 Simulator/FlowComputation/modelsurface.h
#                 Simulator/FlowComputation/node.h
#                 Simulator/FlowComputation/parasurface.h
#                 Simulator/FlowComputation/propertyarea.h
#                 Simulator/FlowComputation/region.h
#                 Simulator/FlowComputation/samg.h
#                 Simulator/FlowComputation/segment.h
#                 Simulator/FlowComputation/skeleton.h
#                 Simulator/FlowComputation/tetgen.h
#                 Simulator/FlowComputation/tetrahedron.h
#                 Simulator/FlowComputation/tofb.h
#                 Simulator/FlowComputation/tracerb.h
#                 Simulator/FlowComputation/triangle.h
#                 Simulator/FlowComputation/trifacet.h
#                 Simulator/FlowComputation/usermanual.h
#                 Simulator/FlowComputation/uvline.h
#                 Simulator/FlowComputation/uvnode.h
#                 Simulator/FlowComputation/well.h
#                 Simulator/FlowComputation/cpgface.h
#                 Simulator/FlowComputation/flowb.h
#                 Simulator/FlowComputation/row.h )

#set ( SIMULATOR_MOC_HEADERS
#                Simulator/FlowParametersBar.h
#                Simulator/FlowRenderingOptionsMenu.h
#                Simulator/FlowVisualizationCanvas.h
#                Simulator/FlowVisualizationController.h
#                Simulator/FlowWindow.h
#                Simulator/FormPropertyValues.h
#                Simulator/NormalMovableCrossSectionFlow.h
#                Simulator/ProgressCounter.h
#                Simulator/ColorBar.h
#                Simulator/PoreVolumeResultsForm.h )

#set ( SIMULATOR_UI_SOURCES
#                Simulator/FormPropertyValues.ui
#                Simulator/NormalMovableCrossSectionFlow.ui )

#set( SIMULATOR_SOURCES
#                Simulator/tinyobjloader/tiny_obj_loader.cc
#                Simulator/ColorMap.cpp
#                Simulator/CoordinateAxes.cpp
#                Simulator/FlowCrossSection.cpp
#                Simulator/FlowParametersBar.cpp
#                Simulator/FlowRenderingOptionsMenu.cpp
#                Simulator/FlowVisualizationCanvas.cpp
#                Simulator/FlowVisualizationController.cpp
#                Simulator/FlowWindow.cpp
#                Simulator/FormPropertyValues.cpp
#                Simulator/Mesh.cpp
#                Simulator/NormalMovableCrossSectionFlow.cpp
#                Simulator/ColorBar.cpp
#                Simulator/PoreVolumeResultsForm.cpp
#                Simulator/FlowDiagnosticsInterface.cpp
#                Simulator/FlowComputation/bsurface.cpp
#                Simulator/FlowComputation/cpgelement.cpp
#                Simulator/FlowComputation/curve.cpp
#                Simulator/FlowComputation/edge.cpp
#                Simulator/FlowComputation/facet.cpp
#                Simulator/FlowComputation/geosurface.cpp
#                Simulator/FlowComputation/graph.cpp
#                Simulator/FlowComputation/graphedge.cpp
#                Simulator/FlowComputation/graphnode.cpp
#                Simulator/FlowComputation/linearfunc.cpp
#                Simulator/FlowComputation/mathzz.cpp
#                Simulator/FlowComputation/matrix.cpp
#                Simulator/FlowComputation/model.cpp
#                Simulator/FlowComputation/meshinfo.cpp
#                Simulator/FlowComputation/modelsurface.cpp
#                Simulator/FlowComputation/node.cpp
#                Simulator/FlowComputation/predicates.cxx
#                Simulator/FlowComputation/parasurface.cpp
#                Simulator/FlowComputation/propertyarea.cpp
#                Simulator/FlowComputation/region.cpp
#                Simulator/FlowComputation/segment.cpp
#                Simulator/FlowComputation/skeleton.cpp
#                Simulator/FlowComputation/tetgen.cxx
#                Simulator/FlowComputation/tetrahedron.cpp
#                Simulator/FlowComputation/tofb.cpp
#                Simulator/FlowComputation/tracerb.cpp
#                Simulator/FlowComputation/triangle.c
#                Simulator/FlowComputation/trifacet.cpp
#                Simulator/FlowComputation/uvline.cpp
#                Simulator/FlowComputation/uvnode.cpp
#                Simulator/FlowComputation/well.cpp
#                Simulator/FlowComputation/cpgface.cpp
#                Simulator/FlowComputation/flowb.cpp
#                Simulator/FlowComputation/row.cpp )
					
					

###############################################
# Wrapping Qt Elements
###############################################

## Q_OBJECT files
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS})
#set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} ${SIMULATOR_MOC_HEADERS} )

## Qt Designer Ui files
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )
#set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} ${SIMULATOR_UI_SOURCES} )


## Resources
set( RCC_SOURCES Resources/images.qrc )


QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )
QT5_WRAP_UI( UI_HEADERS ${UI_SOURCES} )
QT5_ADD_RESOURCES( RCC_HEADERS ${RCC_SOURCES} )


				   
###############################################
# ADD TO EXECUTABLE AND LINK
###############################################

add_executable(${PROJECT_NAME} WIN32 ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                               ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                               ${3DVIEW_SOURCES} ${3DVIEW_HEADERS} 
#                               ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS}
                               ${GEOLOGY_HEADERS}
                               ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} Resources/rrm.rc)


target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SAMG_LIBRARY}
                                        planin stratmod RRMGeometry Arrangement_2D SketchLib )


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

