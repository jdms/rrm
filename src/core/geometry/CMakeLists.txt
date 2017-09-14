cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeometry )

#####################################################################
# Options 
#####################################################################

ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


###############################################
# Main library
###############################################
     
set( MESH_HEADERS mesh_generator.hpp openvolumemesh_defines.hpp)
set( MESH_SOURCES mesh_generator.cpp openvolumemesh_defines.cpp)
					
add_library( ${PROJECT_NAME} STATIC ${MESH_HEADERS} ${MESH_SOURCES} )  
target_link_libraries( ${PROJECT_NAME} ${OPENVOLUMEMESH_LIBRARY} )
