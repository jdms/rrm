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
  
set( MAINWINDOW_HEADERS mainwindow/mainwindow.h
                        mainwindow/controller.h
                        mainwindow/rules_processor.hpp
                        mainwindow/widgets/color_picker.h
                        mainwindow/widgets/pages_stack.h
                        mainwindow/widgets/real_slider.h
                        mainwindow/exporter/csp3_exporter.hpp
                        mainwindow/exporter/irap_grid_exporter.hpp
                        mainwindow/object_tree/object_tree_item.h
                        mainwindow/object_tree/object_tree.h
                        mainwindow/help/help_dialog.h
                        mainwindow/about/about_widget.hpp )


set( MAINWINDOW_MOC_HEADERS mainwindow/mainwindow.h
                            mainwindow/widgets/color_picker.h
                            mainwindow/widgets/pages_stack.h
                            mainwindow/widgets/real_slider.h
                            mainwindow/object_tree/object_tree.h
                            mainwindow/help/help_dialog.h
                            mainwindow/about/about_widget.hpp )


set( MAINWINDOW_UI_SOURCES  mainwindow/about/about_widget.ui
                            mainwindow/help/help_dialog.ui
                            mainwindow/widgets/volume_resize_widget.ui )


set( MAINWINDOW_SOURCES mainwindow/main.cpp
                        mainwindow/mainwindow.cpp
                        mainwindow/widgets/color_picker.cpp
                        mainwindow/widgets/pages_stack.cpp
                        mainwindow/widgets/real_slider.cpp
                        mainwindow/controller.cpp
                        mainwindow/rules_processor.cpp
                        mainwindow/exporter/csp3_exporter.cpp
                        mainwindow/exporter/irap_grid_exporter.cpp
                        mainwindow/object_tree/object_tree_item.cpp
                        mainwindow/object_tree/object_tree.cpp
                        mainwindow/help/help_dialog.cpp
                        mainwindow/about/about_widget.cpp )




###############################################
# Main source -- Sketching SubDirectory
###############################################

set( SKETCHING_HEADERS        sketching/sketch_window.h
                              sketching/input_sketch.h
                              sketching/sketch_scene.h
                              sketching/csection_scene.h
                              sketching/topview_scene.h
                              sketching/item_wrappers/coordinate_axes_2d.h
                              sketching/item_wrappers/object_item_wrapper.h
                              sketching/item_wrappers/trajectory_item_wrapper.h
                              sketching/item_wrappers/volume_item_wrapper.h
                              sketching/item_wrappers/crosssection_item_wrapper.h
                              sketching/item_wrappers/region_item.hpp
                              sketching/polyqt_utils.hpp )


set( SKETCHING_MOC_HEADERS    sketching/sketch_window.h
                              sketching/topview_scene.h
                              sketching/sketch_scene.h
                              sketching/csection_scene.h )


set( SKETCHING_SOURCES        sketching/sketch_window.cpp
                              sketching/input_sketch.cpp
                              sketching/topview_scene.cpp
                              sketching/sketch_scene.cpp
                              sketching/csection_scene.cpp
                              sketching/item_wrappers/coordinate_axes_2d.cpp
                              sketching/item_wrappers/object_item_wrapper.cpp
                              sketching/item_wrappers/trajectory_item_wrapper.cpp
                              sketching/item_wrappers/volume_item_wrapper.cpp
                              sketching/item_wrappers/crosssection_item_wrapper.cpp
                              sketching/item_wrappers/region_item.cpp
                              sketching/polyqt_utils.cpp )


###############################################
# Main source -- 3dview SubDirectory
###############################################

set( 3DVIEW_HEADERS    3dview/canvas3d.h
                       3dview/model3d_utils.hpp
                       3dview/scene3d.h
                       3dview/opengl_wrappers/coordinate_axes_3d.h
                       3dview/opengl_wrappers/object_opengl_wrapper.h
                       3dview/opengl_wrappers/volume_opengl_wrapper.h
                       3dview/opengl_wrappers/crosssection_opengl_wrapper.h
                       3dview/gradient_background_shader.hpp
                       )


set( 3DVIEW_MOC_HEADERS  3dview/canvas3d.h
                         3dview/scene3d.h )



set( 3DVIEW_SOURCES     3dview/canvas3d.cpp
                        3dview/scene3d.cpp
                        3dview/model3d_utils.cpp
                        3dview/opengl_wrappers/coordinate_axes_3d.cpp
                        3dview/opengl_wrappers/object_opengl_wrapper.cpp
                        3dview/opengl_wrappers/volume_opengl_wrapper.cpp
                        3dview/opengl_wrappers/crosssection_opengl_wrapper.cpp
                        3dview/gradient_background_shader.cpp
                        )



###############################################
# Main source -- FlowDiagnostics SubDirectory
###############################################					
					

set( SIMULATOR_HEADERS
#                 simulator/tinyobjloader/tiny_obj_loader.h
                 simulator/colormap.h
                 simulator/coordinate_axes.h
                 simulator/flow_crosssection.h
                 simulator/flow_visualization_canvas.h
                 simulator/flow_visualization_controller.h
                 simulator/flow_window.h
                 simulator/mesh.h
                 simulator/normal_movable_crosssection_flow.h
                 simulator/colorbar.h
                 simulator/flow_diagnostics_interface.hpp
                 simulator/flow_computation/bsurface.h
                 simulator/flow_computation/cpgelement.h
                 simulator/flow_computation/curve.h
                 simulator/flow_computation/edge.h
                 simulator/flow_computation/facet.h
                 simulator/flow_computation/geosurface.h
                 simulator/flow_computation/graph.h
                 simulator/flow_computation/graphedge.h
                 simulator/flow_computation/graphnode.h
                 simulator/flow_computation/linearfunc.h
                 simulator/flow_computation/mathzz.h
                 simulator/flow_computation/matrix.h
                 simulator/flow_computation/meshinfo.h
                 simulator/flow_computation/modelsurface.h
                 simulator/flow_computation/node.h
                 simulator/flow_computation/parasurface.h
                 simulator/flow_computation/propertyarea.h
                 simulator/flow_computation/region.h
                 simulator/flow_computation/samg.h
                 simulator/flow_computation/segment.h
                 simulator/flow_computation/skeleton.h
                 simulator/flow_computation/tetgen.h
                 simulator/flow_computation/tetrahedron.h
                 simulator/flow_computation/tofb.h
                 simulator/flow_computation/tracerb.h
                 simulator/flow_computation/triangle.h
                 simulator/flow_computation/trifacet.h
                 simulator/flow_computation/usermanual.h
                 simulator/flow_computation/uvline.h
                 simulator/flow_computation/uvnode.h
                 simulator/flow_computation/well.h
                 simulator/flow_computation/cpgface.h
                 simulator/flow_computation/flowb.h
                 simulator/flow_computation/row.h
                 simulator/flow_computation/pillar.h
                 # Flow Model
                 simulator/flow_model/flow_model.hpp
                 simulator/flow_model/property_profile.hpp
                 # Well Module
                 simulator/well_module/well_canvas.hpp
                 simulator/well_module/well_scene.hpp
                 simulator/well_module/well_item.hpp
                 simulator/well_module/well_widget.hpp
                 # Region Module
                 simulator/region_module/region_widget.hpp
                 # Fluid Module
                 simulator/fluid_module/fluid_widget.hpp)

set ( SIMULATOR_MOC_HEADERS
                simulator/flow_visualization_canvas.h
                simulator/flow_visualization_controller.h
                simulator/flow_window.h
                simulator/normal_movable_crosssection_flow.h
                simulator/colorbar.h
                # Well Module
                simulator/well_module/well_canvas.hpp
                simulator/well_module/well_scene.hpp
                simulator/well_module/well_item.hpp
                simulator/well_module/well_widget.hpp
                # Region Module
                simulator/region_module/region_widget.hpp
                # Fluid Module
                simulator/fluid_module/fluid_widget.hpp)

set ( SIMULATOR_UI_SOURCES
                simulator/normal_movable_crosssection_flow.ui
                # Well Module
                 simulator/well_module/well_widget_form.ui
                 # Region Module
                 simulator/region_module/region_widget_form.ui
                 # Fluid Module
                 simulator/fluid_module/fluid_widget_form.ui
                 )

set( SIMULATOR_SOURCES
#                simulator/tinyobjloader/tiny_obj_loader.cc
                simulator/colormap.cpp
                simulator/coordinate_axes.cpp
                simulator/flow_crosssection.cpp
                simulator/flow_visualization_canvas.cpp
                simulator/flow_visualization_controller.cpp
                simulator/flow_window.cpp
                simulator/mesh.cpp
                simulator/normal_movable_crosssection_flow.cpp
                simulator/colorbar.cpp
                simulator/flow_diagnostics_interface.cpp
                simulator/flow_computation/bsurface.cpp
                simulator/flow_computation/cpgelement.cpp
                simulator/flow_computation/curve.cpp
                simulator/flow_computation/edge.cpp
                simulator/flow_computation/facet.cpp
                simulator/flow_computation/geosurface.cpp
                simulator/flow_computation/graph.cpp
                simulator/flow_computation/graphedge.cpp
                simulator/flow_computation/graphnode.cpp
                simulator/flow_computation/linearfunc.cpp
                simulator/flow_computation/mathzz.cpp
                simulator/flow_computation/matrix.cpp
                simulator/flow_computation/meshinfo.cpp
                simulator/flow_computation/modelsurface.cpp
                simulator/flow_computation/node.cpp
                simulator/flow_computation/predicates.cxx
                simulator/flow_computation/parasurface.cpp
                simulator/flow_computation/propertyarea.cpp
                simulator/flow_computation/region.cpp
                simulator/flow_computation/segment.cpp
                simulator/flow_computation/skeleton.cpp
                simulator/flow_computation/tetgen.cxx
                simulator/flow_computation/tetrahedron.cpp
                simulator/flow_computation/tofb.cpp
                simulator/flow_computation/tracerb.cpp
                simulator/flow_computation/triangle.c
                simulator/flow_computation/trifacet.cpp
                simulator/flow_computation/uvline.cpp
                simulator/flow_computation/uvnode.cpp
                simulator/flow_computation/well.cpp
                simulator/flow_computation/cpgface.cpp
                simulator/flow_computation/flowb.cpp
                simulator/flow_computation/row.cpp
                simulator/flow_computation/pillar.cpp
                # Flow Model
                simulator/flow_model/flow_model.cpp
                simulator/flow_model/property_profile.cpp
                # Well Module
                simulator/well_module/well_canvas.cpp
                simulator/well_module/well_scene.cpp
                simulator/well_module/well_item.cpp
                simulator/well_module/well_widget.cpp
                # Region Module
                simulator/region_module/region_widget.cpp
                # Fluid Module
                simulator/fluid_module/fluid_widget.cpp)


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
set( RCC_SOURCES resources/images.qrc )


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
                               ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} resources/rrm.rc)


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

1