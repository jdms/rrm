cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMBase )

#####################################################################
# Options 
#####################################################################


###############################################
# Main library
###############################################
     
set( BASE_HEADERS Constants/constants.hpp )
set( BASE_SOURCES Constants/constants.cpp )
						
						
add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${BASE_SOURCES} )  

