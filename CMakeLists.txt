cmake_minimum_required ( VERSION 3.3 FATAL_ERROR )

project( RRM )


#####################################################################
# Options 
#####################################################################

set( CMAKE_BUILD_TYPE DEBUG )
OPTION( BUILD_FULL_SUITE "Build RapidReservoir Suite" ON )


###############################################
# Compiler flags (we support Clang (3.7+), 
# Gcc (4.8+) and VS2013 (revision 5+)). 
###############################################

IF( WIN32 ) 
    
    if( MSVC )
        elseif( CMAKE_COMPILER_IS_GNUCXX )  # MinGW on Windows
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /openmp /EHsc " CACHE STRING "" )
	else() # Nor MSVC or GCC (MinGW)
        message( FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)" )
    endif()
	
ELSEIF( UNIX )    
 	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g3 -O0 -fopenmp" CACHE STRING "" )

ELSE() 
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()



###############################################
# Output CMake 
###############################################

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )



################################################
## External libraries
################################################


include_directories( ${CMAKE_SOURCE_DIR}/libs )
include_directories( "./" )


OPTION( OWN_LIBRARIES "Use predefined libraies directories" ON )

if( OWN_LIBRARIES )

    set( GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/GLEW/include )
    set( GLEW_LIBRARY ${CMAKE_SOURCE_DIR}/libs/GLEW/libs/win32/glew32.lib )
    find_package( GLEW REQUIRED )
    include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )


    set( EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/Eigen3 )
    find_package( EIGEN3 REQUIRED )
    include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
    add_definitions( -DEIGEN_DONT_ALIGN )


#    set( Qt5_DIR "C:/Qt/5.6/msvc2013/lib/cmake/Qt5" )
#    find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
#    include_directories( SYSTEM ${Qt5_DIR} )
#    include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
#    include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )

    
else()

    find_package( EIGEN3 REQUIRED )
    if( EIGEN3_FOUND )
        include_directories( SYSTEM ${EIGEN3_INCLUDE_DIR} )
        add_definitions( -DEIGEN_DONT_ALIGN )
    endif( EIGEN3_FOUND )

    find_package( GLEW REQUIRED )
    if( GLEW_FOUND )
        include_directories( SYSTEM ${GLEW_INCLUDE_DIR} )
    endif( GLEW_FOUND )


endif( OWN_LIBRARIES )


find_package( OpenGL REQUIRED )
if( OpenGL_FOUND )
    include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
endif( OpenGL_FOUND )


find_package( Qt5 COMPONENTS Core Help Gui Svg Widgets OpenGL )
if( Qt5COMPONENTS_FOUND )
    include_directories( SYSTEM ${Qt5Core_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5Widgets_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5Gui_INCLUDE_DIRS} )
    include_directories( SYSTEM ${Qt5OpenGL_INCLUDE_DIRS} )
endif( Qt5COMPONENTS_FOUND )


add_subdirectory( libs/PlanInLib EXCLUDE_FROM_ALL )


###############################################
# Main source
###############################################

add_subdirectory( src )

