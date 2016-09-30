cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeology )

#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE Debug )


###############################################
# Main library
###############################################
     
set( GEOLOGY_HEADERS )
set( GEOLOGY_SOURCES )					
						
						
add_library( ${PROJECT_NAME} STATIC ${GEOLOGY_HEADERS} ${GEOLOGY_SOURCES} )  

