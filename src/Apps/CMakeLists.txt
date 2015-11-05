project(RRM)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


## Show all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

## Grab Controller Source Files

set( CONTROLLER_HEADERS Controller/MainWindow.h
                        
   )

set( CONTROLLER_SOURCES Controller/main.cpp
                        Controller/MainWindow.cpp
                        Controller/MainSimulator.cpp
                        Controller/MainModeller.cpp                       
   )

## Grab Modeller Source Files
set( MODELLER_HEADERS   Modeller/Canvas2D.h
                        Modeller/SketchingScene.h
						Modeller/BoundaryController.h
                        Modeller/HorizonController.h
                        Modeller/InputSketch.h   
                        Modeller/ExtrusionWidget/OpenGLWidget.hpp
									
   )
   
set( MODELLER_MOC_HEARDS Modeller/Canvas2D.h
                        Modeller/SketchingScene.h
						Modeller/ExtrusionWidget/OpenGLWidget.hpp
						)

set( MODELLER_SOURCES Modeller/Canvas2D.cpp
                        Modeller/BoundaryController.cpp
                        Modeller/HorizonController.cpp
                        Modeller/InputSketch.cpp
                        Modeller/SketchingScene.cpp
                        Modeller/ExtrusionWidget/OpenGLWidget.cpp
    )
        
## Grab Simulator Source Files

## Q_OBJECT files
set( MOC_HEADERS ${CONTROLLER_HEADERS} ${MODELLER_MOC_HEARDS}   
   )
## Qt Designer Ui files                 
set( UI_SOURCES  )
## Resources
set(RCC_SOURCES Controller/Resources/images.qrc)

QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS})
QT5_WRAP_UI (UI_HEADERS ${UI_SOURCES})
QT5_ADD_RESOURCES(RCC_HEADERS ${RCC_SOURCES})

# @see http://www.cmake.org/Wiki/CMake/Tutorials/Qt
IF(WIN32) # Check if we are on Windows
    if(MSVC) # Check if we are using the Visual Studio compiler
        add_definitions(/arch:SSE2)
        message(STATUS "- MSVC: Enabled SSE2 support")
        add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS} ${MODELLER_SOURCES} ${MODELLER_HEADERS} ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})
		target_link_libraries(${PROJECT_NAME} Qt5::WinMain Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Svg RRMGeometry ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${OPENGL_LIBRARIES}  ${GLEW_LIBRARY} )
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
    elseif(CMAKE_COMPILER_IS_GNUCXX)  # MinGW on Windows
        message (FATAL_ERROR "GCC are not support on Windows !")
    else() # Nor MSVC or GCC (MinGW)
        message(FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
    endif()
ELSEIF(UNIX)
    # Nothing special required for Linux
    message(STATUS "Build on Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS} ${MODELLER_SOURCES} ${MODELLER_HEADERS} ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})	
    target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg RRMGeometry  ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )
ELSE() # Not Windows or Unix
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()


