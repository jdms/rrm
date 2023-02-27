project(RRM)


###############################################
# Main source -- Apps Directory
###############################################
find_package( Qt5Widgets REQUIRED )

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}
                      ${CMAKE_CURRENT_BINARY_DIR} )


ADD_DEFINITIONS(-DINCLUDE_TEMPLATES)

###############################################
# Main source -- Subprojects
###############################################

add_library(RRMAppsIncludes INTERFACE)
target_include_directories(RRMAppsIncludes
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/simulator>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/mainwindow>
)
add_subdirectory( mainwindow )
add_subdirectory( simulator )

target_link_libraries(MainApp FlowDiagnosticsApp)

###############################################
# Wrapping Qt Elements
###############################################

set( RCC_SOURCES resources/images.qrc )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


###############################################
# ADD TO EXECUTABLE AND LINK
###############################################

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})


add_executable(${PROJECT_NAME} WIN32 ${RCC_SOURCES} )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

if(WIN32)
    # target_link_libraries( ${PROJECT_NAME} MainApp stratmod colorwrap )
    target_link_libraries( ${PROJECT_NAME} MainApp colorwrap )
else()
    # target_link_libraries( ${PROJECT_NAME} MainApp stratmod OpenMP::OpenMP_CXX colorwrap )
    target_link_libraries( ${PROJECT_NAME} MainApp colorwrap )
endif()
# target_link_libraries( ${PROJECT_NAME} ${STRATMODLIB} colorwrap )
# target_link_libraries( ${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg )
# target_link_libraries( ${PROJECT_NAME} OpenGL::GL )
# target_link_libraries( ${PROJECT_NAME} ${GLEW_LIBRARY} ${OPENGL_LIBRARY} )

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

######################################################
# POST BUILD COMMANDS: COPYING FILES TO EXEC DIRECTORY
######################################################


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/shaders )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/manual ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/manual )


add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/files/objects ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/objects )

if(WIN32)
    # add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E copy
    # ${CMAKE_SOURCE_DIR}/libs/stratmod/stratmod/lib/stratmod.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    if(GLEW_RUNTIME)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_RUNTIME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/
            )
    endif()

    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders )


    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files/manual ${CMAKE_CURRENT_BINARY_DIR}/manual )


    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/files/objects ${CMAKE_CURRENT_BINARY_DIR}/objects )

    if( EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )

        add_custom_command( TARGET ${PROJECT_NAME} PRE_LINK COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/files/dlls ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

    endif(  EXISTS ${CMAKE_SOURCE_DIR}/files/dlls )
endif(WIN32)

if(WIN32 AND RRM_BUILD_DEPLOY_QT)

    # Find windeployqt.exe
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    # Run windeployqt
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
        # --verbose 0
        # --no-compiler-runtime
        \"$<TARGET_FILE:RRM>\"
        COMMENT "Deploying Qt..."
        )

endif()

