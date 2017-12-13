project(RRM)


###############################################
# Main source -- Apps Directory
###############################################


include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/src/core/widgets )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/build/simulator )

###############################################
# Main source -- Subprojects
###############################################

add_subdirectory( mainwindow )
#add_subdirectory( 3dview )


###############################################
# Wrapping Qt Elements
###############################################

## Resources
set( RCC_SOURCES resources/images.qrc )
QT5_ADD_RESOURCES( RCC_HEADERS ${RCC_SOURCES} )




###############################################
# ADD TO EXECUTABLE AND LINK
###############################################

add_executable(${PROJECT_NAME} WIN32 ${RCC_HEADERS} resources/rrm.rc )
target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg
    ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} MainApp  ${STRATMODLIB}  )
#stratmod planin)



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

#add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy
#    libs/stratmod/stratmod/lib/stratmod.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy
    ${CMAKE_SOURCE_DIR}/libs/stratmod/stratmod/lib/stratmod.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

if( EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )

    add_custom_command( TARGET ${PROJECT_NAME} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files/dlls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

endif(  EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )
