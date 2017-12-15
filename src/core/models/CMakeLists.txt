cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRMModel )


#####################################################################
# Options
#####################################################################


include_directories( SYSTEM ${OPENVOLUMEMESH_INCLUDE_DIR} )

ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)


###############################################
# Main library
###############################################

set( MODELS_HEADERS     container.h
                        controller.h
                        object.h
                        regions.h
                        volume.h
                        polycurve.h
                        surface.h
                        crosssection.h
                        scene.h
                        shader.h
                        well.h
                        rules_processor.hpp
                        stratmod_wrapper.hpp
                        mesh_generator.hpp
                        openvolumemesh_defines.hpp )

set( MODELS_SOURCES     controller.cpp
                        object.cpp
                        regions.cpp
                        volume.cpp
                        crosssection.cpp
                        well.cpp
                        rules_processor.cpp
                        mesh_generator.cpp
                        openvolumemesh_defines.cpp)

add_library( ${PROJECT_NAME} STATIC ${MODELS_HEADERS} ${MODELS_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets ${OPENVOLUMEMESH_LIBRARY} )
