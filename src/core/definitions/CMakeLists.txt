cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMBase )

#####################################################################
# Options 
#####################################################################


###############################################
# Main library
###############################################


set( BASE_HEADERS constants.hpp )
set( BASE_SOURCES constants.cpp )

add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${BASE_SOURCES} )

