cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeology )

#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE Debug )


###############################################
# Main library
###############################################
     
set( GEOLOGY_HEADERS Models/Boundary.hpp 
					 Models/BoundaryRenderable.h 
					 Models/CrossSection.hpp 
					 Models/Stratigraphy.hpp 
					 Models/StratigraphyRenderable.hpp )
set( GEOLOGY_SOURCES  )

						
#add_library( ${PROJECT_NAME} STATIC ${GEOLOGY_SOURCES} ${GEOLOGY_HEADERS} )
