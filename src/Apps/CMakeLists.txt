project(RRM)

###############################################
# Main source -- Apps Directory
###############################################

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )
    
ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)

###############################################
# Main source -- MainWindow SubDirectory
###############################################
  
set( MAINWINDOW_HEADERS MainWindow/mainwindow.h
                        MainWindow/controller.h
                        MainWindow/rules_processor.hpp
                        MainWindow/Widgets/color_picker.h
                        MainWindow/Widgets/pages_stack.h
                        MainWindow/Widgets/real_slider.h
                        MainWindow/Exporter/csp3_exporter.hpp
                        MainWindow/Exporter/irap_grid_exporter.hpp
                        MainWindow/Object_Tree/object_tree_item.h
                        MainWindow/Object_Tree/object_tree.h
                        MainWindow/Help/help_dialog.h
                        MainWindow/About/about_widget.hpp )


set( MAINWINDOW_MOC_HEADERS MainWindow/mainwindow.h
                            MainWindow/Widgets/color_picker.h
                            MainWindow/Widgets/pages_stack.h
                            MainWindow/Widgets/real_slider.h
                            MainWindow/Object_Tree/object_tree.h
                            MainWindow/Help/help_dialog.h
                            MainWindow/About/about_widget.hpp )


set( MAINWINDOW_UI_SOURCES  MainWindow/About/about_widget.ui
                            MainWindow/Help/help_dialog.ui
                            MainWindow/Widgets/volume_resize_widget.ui )


set( MAINWINDOW_SOURCES MainWindow/main.cpp
                        MainWindow/mainwindow.cpp
                        MainWindow/Widgets/color_picker.cpp
                        MainWindow/Widgets/pages_stack.cpp
                        MainWindow/Widgets/real_slider.cpp
                        MainWindow/controller.cpp
                        MainWindow/rules_processor.cpp
                        MainWindow/Exporter/csp3_exporter.cpp
                        MainWindow/Exporter/irap_grid_exporter.cpp
                        MainWindow/Object_Tree/object_tree_item.cpp
                        MainWindow/Object_Tree/object_tree.cpp
                        MainWindow/Help/help_dialog.cpp
                        MainWindow/About/about_widget.cpp )




###############################################
# Main source -- Sketching SubDirectory
###############################################

set( SKETCHING_HEADERS        Sketching/sketch_window.h
                              Sketching/input_sketch.h
                              Sketching/sketch_scene.h
                              Sketching/csection_scene.h
                              Sketching/topview_scene.h
                              Sketching/ItemWrappers/coordinate_axes_2d.h
                              Sketching/ItemWrappers/object_item_wrapper.h
                              Sketching/ItemWrappers/trajectory_item_wrapper.h
                              Sketching/ItemWrappers/volume_item_wrapper.h
                              Sketching/ItemWrappers/crosssection_item_wrapper.h
                              Sketching/ItemWrappers/region_item.hpp
                              Sketching/polyqt_utils.hpp )


set( SKETCHING_MOC_HEADERS    Sketching/sketch_window.h
                              Sketching/topview_scene.h
                              Sketching/sketch_scene.h
                              Sketching/csection_scene.h )


set( SKETCHING_SOURCES        Sketching/sketch_window.cpp
                              Sketching/input_sketch.cpp
                              Sketching/topview_scene.cpp
                              Sketching/sketch_scene.cpp
                              Sketching/csection_scene.cpp
                              Sketching/ItemWrappers/coordinate_axes_2d.cpp
                              Sketching/ItemWrappers/object_item_wrapper.cpp
                              Sketching/ItemWrappers/trajectory_item_wrapper.cpp
                              Sketching/ItemWrappers/volume_item_wrapper.cpp
                              Sketching/ItemWrappers/crosssection_item_wrapper.cpp
                              Sketching/ItemWrappers/region_item.cpp
                              Sketching/polyqt_utils.cpp )


###############################################
# Main source -- 3dView SubDirectory
###############################################

set( 3DVIEW_HEADERS    3dView/canvas3d.h
                       3dView/model3d_utils.hpp
                       3dView/scene3d.h
                       3dView/OpenGLWrappers/coordinate_axes_3d.h
                       3dView/OpenGLWrappers/object_opengl_wrapper.h
                       3dView/OpenGLWrappers/volume_opengl_wrapper.h
                       3dView/OpenGLWrappers/crosssection_opengl_wrapper.h
                       3dView/gradient_background_shader.hpp
                       )


set( 3DVIEW_MOC_HEADERS  3dView/canvas3d.h
                         3dView/scene3d.h )



set( 3DVIEW_SOURCES     3dView/canvas3d.cpp
                        3dView/scene3d.cpp
                        3dView/model3d_utils.cpp
                        3dView/OpenGLWrappers/coordinate_axes_3d.cpp
                        3dView/OpenGLWrappers/object_opengl_wrapper.cpp
                        3dView/OpenGLWrappers/volume_opengl_wrapper.cpp
                        3dView/OpenGLWrappers/crosssection_opengl_wrapper.cpp
                        3dView/gradient_background_shader.cpp
                        )



###############################################
# Main source -- FlowDiagnostics SubDirectory
###############################################					
					

set( SIMULATOR_HEADERS
#                 Simulator/tinyobjloader/tiny_obj_loader.h
                 Simulator/ColorMap.h
                 Simulator/CoordinateAxes.h
                 Simulator/FlowCrossSection.h
                 Simulator/FlowVisualizationCanvas.h
                 Simulator/FlowVisualizationController.h
                 Simulator/FlowWindow.h
                 Simulator/Mesh.h
                 Simulator/NormalMovableCrossSectionFlow.h
                 Simulator/ProgressCounter.h
                 Simulator/ColorBar.h
                 Simulator/FlowDiagnosticsInterface.hpp
                 Simulator/FlowComputation/bsurface.h
                 Simulator/FlowComputation/cpgelement.h
                 Simulator/FlowComputation/curve.h
                 Simulator/FlowComputation/edge.h
                 Simulator/FlowComputation/facet.h
                 Simulator/FlowComputation/geosurface.h
                 Simulator/FlowComputation/graph.h
                 Simulator/FlowComputation/graphedge.h
                 Simulator/FlowComputation/graphnode.h
                 Simulator/FlowComputation/linearfunc.h
                 Simulator/FlowComputation/mathzz.h
                 Simulator/FlowComputation/matrix.h
                 Simulator/FlowComputation/meshinfo.h
                 Simulator/FlowComputation/modelsurface.h
                 Simulator/FlowComputation/node.h
                 Simulator/FlowComputation/parasurface.h
                 Simulator/FlowComputation/propertyarea.h
                 Simulator/FlowComputation/region.h
                 Simulator/FlowComputation/samg.h
                 Simulator/FlowComputation/segment.h
                 Simulator/FlowComputation/skeleton.h
                 Simulator/FlowComputation/tetgen.h
                 Simulator/FlowComputation/tetrahedron.h
                 Simulator/FlowComputation/tofb.h
                 Simulator/FlowComputation/tracerb.h
                 Simulator/FlowComputation/triangle.h
                 Simulator/FlowComputation/trifacet.h
                 Simulator/FlowComputation/usermanual.h
                 Simulator/FlowComputation/uvline.h
                 Simulator/FlowComputation/uvnode.h
                 Simulator/FlowComputation/well.h
                 Simulator/FlowComputation/cpgface.h
                 Simulator/FlowComputation/flowb.h
                 Simulator/FlowComputation/row.h
                                 Simulator/FlowComputation/pillar.h
                 # Flow Model
                 Simulator/FlowModel/FlowModel.hpp
                 Simulator/FlowModel/PropertyProfile.hpp
                 # Well Module
                 Simulator/WellModule/WellCanvas.hpp
                 Simulator/WellModule/WellScene.hpp
                 Simulator/WellModule/WellItem.hpp
                 Simulator/WellModule/WellWidget.hpp
                 # Region Module
                 Simulator/RegionModule/RegionWidget.hpp
                 # Fluid Module
                 Simulator/FluidModule/FluidWidget.hpp)

set ( SIMULATOR_MOC_HEADERS
                Simulator/FlowVisualizationCanvas.h
                Simulator/FlowVisualizationController.h
                Simulator/FlowWindow.h
                Simulator/NormalMovableCrossSectionFlow.h
                Simulator/ProgressCounter.h
                Simulator/ColorBar.h
                # Well Module
                Simulator/WellModule/WellCanvas.hpp
                Simulator/WellModule/WellScene.hpp
                Simulator/WellModule/WellItem.hpp
                Simulator/WellModule/WellWidget.hpp
                # Region Module
                Simulator/RegionModule/RegionWidget.hpp
                # Fluid Module
                Simulator/FluidModule/FluidWidget.hpp)

set ( SIMULATOR_UI_SOURCES
                Simulator/NormalMovableCrossSectionFlow.ui
                # Well Module
                 Simulator/WellModule/WellWidgetForm.ui
                 # Region Module
                 Simulator/RegionModule/RegionWidgetForm.ui
                 # Fluid Module
                 Simulator/FluidModule/FluidWidgetForm.ui
                 )

set( SIMULATOR_SOURCES
#                Simulator/tinyobjloader/tiny_obj_loader.cc
                Simulator/ColorMap.cpp
                Simulator/CoordinateAxes.cpp
                Simulator/FlowCrossSection.cpp
                Simulator/FlowVisualizationCanvas.cpp
                Simulator/FlowVisualizationController.cpp
                Simulator/FlowWindow.cpp
                Simulator/Mesh.cpp
                Simulator/NormalMovableCrossSectionFlow.cpp
                Simulator/ColorBar.cpp
                Simulator/FlowDiagnosticsInterface.cpp
                Simulator/FlowComputation/bsurface.cpp
                Simulator/FlowComputation/cpgelement.cpp
                Simulator/FlowComputation/curve.cpp
                Simulator/FlowComputation/edge.cpp
                Simulator/FlowComputation/facet.cpp
                Simulator/FlowComputation/geosurface.cpp
                Simulator/FlowComputation/graph.cpp
                Simulator/FlowComputation/graphedge.cpp
                Simulator/FlowComputation/graphnode.cpp
                Simulator/FlowComputation/linearfunc.cpp
                Simulator/FlowComputation/mathzz.cpp
                Simulator/FlowComputation/matrix.cpp
                Simulator/FlowComputation/meshinfo.cpp
                Simulator/FlowComputation/modelsurface.cpp
                Simulator/FlowComputation/node.cpp
                Simulator/FlowComputation/predicates.cxx
                Simulator/FlowComputation/parasurface.cpp
                Simulator/FlowComputation/propertyarea.cpp
                Simulator/FlowComputation/region.cpp
                Simulator/FlowComputation/segment.cpp
                Simulator/FlowComputation/skeleton.cpp
                Simulator/FlowComputation/tetgen.cxx
                Simulator/FlowComputation/tetrahedron.cpp
                Simulator/FlowComputation/tofb.cpp
                Simulator/FlowComputation/tracerb.cpp
                Simulator/FlowComputation/triangle.c
                Simulator/FlowComputation/trifacet.cpp
                Simulator/FlowComputation/uvline.cpp
                Simulator/FlowComputation/uvnode.cpp
                Simulator/FlowComputation/well.cpp
                Simulator/FlowComputation/cpgface.cpp
                Simulator/FlowComputation/flowb.cpp
                Simulator/FlowComputation/row.cpp
                            Simulator/FlowComputation/pillar.cpp
                # Flow Model
                Simulator/FlowModel/FlowModel.cpp
                Simulator/FlowModel/PropertyProfile.cpp
                # Well Module
                Simulator/WellModule/WellCanvas.cpp
                Simulator/WellModule/WellScene.cpp
                Simulator/WellModule/WellItem.cpp
                Simulator/WellModule/WellWidget.cpp
                # Region Module
                Simulator/RegionModule/RegionWidget.cpp
                # Fluid Module
                Simulator/FluidModule/FluidWidget.cpp)


###############################################
# Wrapping Qt Elements
###############################################

## Q_OBJECT files
#set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS})
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} ${SIMULATOR_MOC_HEADERS} )

## Qt Designer Ui files
#set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} ${SIMULATOR_UI_SOURCES} )


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
                               ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS}
                               ${GEOLOGY_SOURCES} ${GEOLOGY_HEADERS}
                               ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} Resources/rrm.rc)


target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg ${HYPRE} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${SAMG_LIBRARY}
                                        planin stratmod RRMGeometry RRMGeology Arrangement_2D SketchLib TinyObjLoader RRMMesh ${OPENVOLUMEMESH_LIBRARY} )


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

