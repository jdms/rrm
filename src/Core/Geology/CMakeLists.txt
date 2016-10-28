cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMGeology )


#####################################################################
# Options
#####################################################################

set( CMAKE_BUILD_TYPE Debug )


###############################################
# Main library
###############################################
     

include_directories("./")
include_directories("./Models")

set( GEOLOGY_HEADERS Models/Boundary.hpp
                     Models/BoundaryRenderable.h
                     Models/CrossSection.hpp
                     Models/Stratigraphy.hpp
                     Models/StratigraphyRenderable.hpp )
set( GEOLOGY_SOURCES  )

						

add_library( ${PROJECT_NAME} STATIC ${GEOLOGY_HEADERS} )
set_target_properties( ${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C )
