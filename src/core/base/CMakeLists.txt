cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMBase )

#####################################################################
# Options 
#####################################################################


###############################################
# Main library
###############################################

include_directories("./models")

set( BASE_HEADERS constants/constants.hpp 
                  models/object.h
                  models/cross_section.h
                  models/region1.h
                  models/volume.h)
set( BASE_SOURCES constants/constants.cpp
                  models/object.cpp)
						
						
add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${BASE_SOURCES} )  

