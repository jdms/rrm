cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMModel )


#####################################################################
# Options
#####################################################################

set(CMAKE_AUTOMOC ON)



###############################################
# Main library
###############################################

set( MODELS_HEADERS     container.h
                        controller.h
                        object.h
                        region.h
                        volume.h
                        polycurve.h
                        surface.h
                        crosssection.h
                        scene.h
                        shader.h
                        well.h
                        rules_processor.hpp
                        stratmod_wrapper.hpp )

set( MODELS_SOURCES     controller.cpp
                        object.cpp
                        region.cpp
                        volume.cpp
                        crosssection.cpp
                        well.cpp
                        rules_processor.cpp )



add_library( ${PROJECT_NAME} STATIC ${MODELS_HEADERS} ${MODELS_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets )
