cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeometry )

#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE Debug )


###############################################
# Main library
###############################################
     
set( GEOMETRY_HEADERS PolygonalCurve/PolygonalCurve2D.hpp )
set( GEOMETRY_SOURCES PolygonalCurve/PolygonalCurve2D.cpp )					
						
						
add_library( ${PROJECT_NAME} STATIC ${GEOMETRY_HEADERS} ${GEOMETRY_SOURCES} )  
target_link_libraries( ${PROJECT_NAME} RRMBase )
