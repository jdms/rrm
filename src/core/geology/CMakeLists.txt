cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeology )


#####################################################################
# Options
#####################################################################

set( CMAKE_BUILD_TYPE Release )


###############################################
# Main library
###############################################
     

include_directories("./")
#include_directories("./models")

set( GEOLOGY_HEADERS  )

set( GEOLOGY_SOURCES   )

						

add_library( ${PROJECT_NAME} STATIC ${GEOLOGY_SOURCES} ${GEOLOGY_HEADERS} )
set_target_properties( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX )
