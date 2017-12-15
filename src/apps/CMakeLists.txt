project(RRM)


###############################################
# Main source -- Apps Directory
###############################################


include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR} )

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )
#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator/FlowDiagnosticsApp_autogen/include)
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets/RRMWidgets_autogen/include )

ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)

###############################################
# Main source -- Subprojects
###############################################

add_subdirectory( mainwindow )
#add_subdirectory( simulator )



###############################################
# Wrapping Qt Elements
###############################################
#find_package(Qt5Widgets REQUIRED)
#QT5_WRAP_CPP( SIMULATOR_MOC_SOURCES ${SIMULATOR_MOC_HEADERS} )
#QT5_WRAP_UI( SIMULATOR_MOC_UI ${SIMULATOR_UI_SOURCES} )


### Resources
set( RCC_SOURCES resources/images.qrc )
#QT5_ADD_RESOURCES( RCC_HEADERS ${RCC_SOURCES} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#SET(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)


###############################################
# ADD TO EXECUTABLE AND LINK
###############################################

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


add_executable(${PROJECT_NAME} WIN32 ${RCC_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg
    ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} MainApp ${STRATMODLIB} )

#add_executable(${PROJECT_NAME} WIN32 ${RCC_HEADERS} resources/rrm.rc ${SIMULATOR_HEADERS} ${SIMULATOR_MOC_SOURCES} ${SIMULATOR_MOC_UI} ${SIMULATOR_SOURCES} )
#target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg
#    ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} MainApp RRMModel ${STRATMODLIB}  ${HYPRE} ${OPENVOLUMEMESH_LIBRARY} TinyObjLoader )

######################################################
# POST BUILD COMMANDS: COPYING FILES TO EXEC DIRECTORY
######################################################


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/manual ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/manual )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/inputs ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/inputs )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/models ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/models )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy
    ${CMAKE_SOURCE_DIR}/libs/stratmod/stratmod/lib/stratmod.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

if( EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )

    add_custom_command( TARGET ${PROJECT_NAME} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files/dlls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

endif(  EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )
