cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMBase )

#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE Debug )


###############################################
# Main library
###############################################
     
set( BASE_HEADERS Constants/Constants.hpp )
set( BASE_SOURCES Constants/Constants.cpp )					
						
						
add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${BASE_SOURCES} )  

