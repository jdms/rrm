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
include_directories("./Models")

set( GEOLOGY_HEADERS Models/object.h
                     Models/cross_section.h
                     Models/Region1.h
                     Models/volume.h )

set( GEOLOGY_SOURCES  Models/object.cpp )

						

add_library( ${PROJECT_NAME} STATIC ${GEOLOGY_SOURCES} ${GEOLOGY_HEADERS} )
set_target_properties( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX )
