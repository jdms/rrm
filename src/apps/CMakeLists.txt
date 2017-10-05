project(RRM)


###############################################
# Main source -- Apps Directory
###############################################


include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR} )


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
                                       ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} MainApp 3dViewApp )



