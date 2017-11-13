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
                  models/container.h
                  models/slider.h
                  models/controller.h
                  models/object.h
                  models/region.h
                  models/volume.h
                  models/polycurve.h
                  models/surface.h
                  models/crosssection.h
                  models/planeshader.h
                  models/scene.h
                  models/shader.h
                  models/surfaceshader.h
                  models/volumeshader.h
                  models/well.h
                  models/rules_processor.hpp
                  models/stratmod_wrapper.hpp )

set( BASE_MOC_HEADERS models/slider.h )


set( BASE_SOURCES constants/constants.cpp
                  models/slider.cpp
                  models/controller.cpp
                  models/object.cpp
                  models/region.cpp
                  models/volume.cpp
                  models/crosssection.cpp
                  models/planeshader.cpp
                  models/surfaceshader.cpp
                  models/volumeshader.cpp
                  models/well.cpp
                  models/rules_processor.cpp )

set( MOC_HEADERS ${BASE_MOC_HEADERS} )
QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS} )

						
add_library( ${PROJECT_NAME} STATIC ${BASE_HEADERS} ${MOC_SOURCES} ${BASE_SOURCES} )

