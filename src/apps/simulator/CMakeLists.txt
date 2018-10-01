cmake_minimum_required( VERSION 3.1 FATAL_ERROR )


project( FlowDiagnosticsApp VERSION 3.1 LANGUAGES CXX )


include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

find_package( Qt5Widgets REQUIRED )

include_directories( SYSTEM ${OPENVOLUMEMESH_INCLUDE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )

# The following code lists all directories already included in the project
# get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
# foreach(dir ${dirs})
#   message(STATUS "dir='${dir}'")
# endforeach()


set( SIMULATOR_HEADERS
                 colormap.h
                 coordinate_axes.h
                 flow_crosssection.h
                 flow_visualization_canvas.h
                 flow_visualization_controller.h
                 flow_window.h
                 mesh.h
                 normal_movable_crosssection_flow.h
                 colorbar.h
                 flow_diagnostics_interface.hpp
                 flow_computation/bsurface.h
                 flow_computation/cpgelement.h
                 flow_computation/curve.h
                 flow_computation/edge.h
                 flow_computation/facet.h
                 flow_computation/geosurface.h
                 flow_computation/graph.h
                 flow_computation/graphedge.h
                 flow_computation/graphnode.h
                 flow_computation/linearfunc.h
                 flow_computation/mathzz.h
                 flow_computation/matrix.h
                 flow_computation/meshinfo.h
                 flow_computation/modelsurface.h
                 flow_computation/node.h
                 flow_computation/parasurface.h
                 flow_computation/propertyarea.h
                 flow_computation/region.h
                 flow_computation/samg.h
                 flow_computation/segment.h
                 flow_computation/skeleton.h
                 flow_computation/tetgen.h
                 flow_computation/tetrahedron.h
                 flow_computation/tofb.h
                 flow_computation/tracerb.h
                 flow_computation/triangle.h
                 flow_computation/trifacet.h
                 flow_computation/usermanual.h
                 flow_computation/uvline.h
                 flow_computation/uvnode.h
                 flow_computation/well.h
                 flow_computation/cpgface.h
                 flow_computation/flowb.h
                 flow_computation/row.h
                 flow_computation/pillar.h
                 flow_model/flow_model.hpp
                 flow_model/property_profile.hpp
                 well_module/well_canvas.hpp
                 well_module/well_scene.hpp
                 well_module/well_item.hpp
                 well_module/well_widget.hpp
                 region_module/region_widget.hpp
                 fluid_module/fluid_widget.hpp
                 widgets/spanslider/qxtspanslider.h
                 widgets/spanslider/qxtspanslider_p.h)

set ( SIMULATOR_MOC_HEADERS
                flow_visualization_canvas.h
                flow_visualization_controller.h
                flow_window.h
                normal_movable_crosssection_flow.h
                colorbar.h
                well_module/well_canvas.hpp
                well_module/well_scene.hpp
                well_module/well_item.hpp
                well_module/well_widget.hpp
                region_module/region_widget.hpp
                fluid_module/fluid_widget.hpp
                widgets/spanslider/qxtspanslider.h
                widgets/spanslider/qxtspanslider_p.h)

set ( SIMULATOR_UI_SOURCES
                normal_movable_crosssection_flow.ui
                well_module/well_widget_form.ui
                region_module/region_widget_form.ui
                fluid_module/fluid_widget_form.ui )

set( SIMULATOR_SOURCES
               colormap.cpp
               coordinate_axes.cpp
               flow_crosssection.cpp
               flow_visualization_canvas.cpp
               flow_visualization_controller.cpp
               flow_window.cpp
               mesh.cpp
               normal_movable_crosssection_flow.cpp
               colorbar.cpp
               flow_diagnostics_interface.cpp
               flow_computation/bsurface.cpp
               flow_computation/cpgelement.cpp
               flow_computation/curve.cpp
               flow_computation/edge.cpp
               flow_computation/facet.cpp
               flow_computation/geosurface.cpp
               flow_computation/graph.cpp
               flow_computation/graphedge.cpp
               flow_computation/graphnode.cpp
               flow_computation/linearfunc.cpp
               flow_computation/mathzz.cpp
               flow_computation/matrix.cpp
               flow_computation/meshinfo.cpp
               flow_computation/modelsurface.cpp
               flow_computation/node.cpp
               flow_computation/predicates.cxx
               flow_computation/parasurface.cpp
               flow_computation/propertyarea.cpp
               flow_computation/region.cpp
               flow_computation/segment.cpp
               flow_computation/skeleton.cpp
               flow_computation/tetgen.cxx
               flow_computation/tetrahedron.cpp
               flow_computation/tofb.cpp
               flow_computation/tracerb.cpp
               flow_computation/triangle.c
               flow_computation/trifacet.cpp
               flow_computation/uvline.cpp
               flow_computation/uvnode.cpp
               flow_computation/well.cpp
               flow_computation/cpgface.cpp
               flow_computation/flowb.cpp
               flow_computation/row.cpp
               flow_computation/pillar.cpp
               flow_model/flow_model.cpp
               flow_model/property_profile.cpp
               well_module/well_canvas.cpp
               well_module/well_scene.cpp
               well_module/well_item.cpp
               well_module/well_widget.cpp
               region_module/region_widget.cpp
               fluid_module/fluid_widget.cpp
                widgets/spanslider/qxtspanslider.cpp )


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

###############################################
# ADD TO LIBRARY AND LINK
###############################################

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


add_library( ${PROJECT_NAME} STATIC ${SIMULATOR_HEADERS} ${SIMULATOR_MOC_HEADERS} ${SIMULATOR_UI_SOURCES} ${SIMULATOR_SOURCES} )
#target_link_libraries( ${PROJECT_NAME} Qt5::Widgets ${HYPRE} )

