find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

find_library(GLEW glew32 "${CMAKE_SOURCE_DIR}/libs/GLEW/glew-1.13.0/lib/Release/Win32")
set( GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/GLEW/glew-1.13.0/include" )
set( GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/libs/GLEW/glew-1.13.0/lib/Release/Win32/glew32.lib" )
message( "Glew Include DIR: " ${GLEW_INCLUDE_DIR} )
message( "Glew Library: " ${GLEW_LIBRARY} )

## It asks for Qt5_DIR
find_package(Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL)

IF(UNIX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -O0")
ELSEIF(WIN32)
    if(MSVC)
        FIND_LIBRARY(SAMG_LIBRARY samg_XP_6_2005 ${CMAKE_SOURCE_DIR}/libs/SAMG )
        message("SAMG Library." ${SAMG_LIBRARY})
    endif(MSVC)
ENDIF()    

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /openmp /EHsc /FS" CACHE STRING "" FORCE)
message("Current compiler flags: " ${CMAKE_CXX_FLAGS})

include_directories(
    #Eigen3
    ${EIGEN3_INCLUDE_DIR}
    ## Tucano Library
    ${CMAKE_SOURCE_DIR}/libs
    ## - OpenGL 
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ## - Qt5
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5OpenGL_INCLUDE_DIRS}
    ## - CGAL-4.7.1
    #${CGAL_INCLUDE_DIRS}
    ## - Find Boost
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} 
)
		
## Basic Libraries and Algorithms	
add_subdirectory(Core)
if (BUILD_FULL_SUITE)
    add_subdirectory(Apps)
endif(BUILD_FULL_SUITE)
	
