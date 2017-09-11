cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeometry )

#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE Release )


###############################################
# Main library
###############################################
     
set( GEOMETRY_HEADERS PolygonalCurve/polygonal_curve_2d.hpp )
set( GEOMETRY_SOURCES PolygonalCurve/polygonal_curve_2d.cpp )
						
						
add_library( ${PROJECT_NAME} STATIC ${GEOMETRY_HEADERS} ${GEOMETRY_SOURCES} )  
target_link_libraries( ${PROJECT_NAME} RRMBase )
