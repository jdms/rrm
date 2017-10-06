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
                  models/canvasstack.h
                  models/container.h
                  models/controller.h
                  models/object.h
                  models/cross_section.h
                  models/region.h
                  models/volume.h
                  models/crosssection.h
                  models/planeshader.h
                  models/scene.h
                  models/shader.h
                  models/surfaceshader.h
                  models/volumeshader.h
                  models/well.h)
set( BASE_SOURCES constants/constants.cpp
                  models/canvasstack.cpp
                  models/controller.cpp
                  models/object.cpp
                  models/region.cpp
                  models/volume.cpp
                  models/crosssection.cpp
                  models/planeshader.cpp
                  models/surfaceshader.cpp
                  models/volumeshader.cpp
                  models/well.cpp)
						
						
add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${BASE_SOURCES} )  

