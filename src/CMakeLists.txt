find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

## It asks for Qt5_DIR
find_package(Qt5 COMPONENTS Core Gui Widgets OpenGL)

## Find CGAL 4.7
find_package(CGAL REQUIRED COMPONENTS Core Qt5 )

if(CGAL_FOUND AND CGAL_Qt5_FOUND)
    message("Found CGAL.")
    include(${CGAL_USE_FILE})
   
    message("CGAL_CXX_FLAGS_INIT: ${CGAL_CXX_FLAGS_INIT}")
    add_definitions(${CGAL_CXX_GLAFS_INIT})

    message("CGAL_LIBRARY: ${CGAL_LIBRARY}")
    message("CGAL_LIBRARIES: ${CGAL_LIBRARIES}")
    message("CGAL_ImageIO_LIBRARY: ${CGAL_ImageIO_LIBRARY}")
    message("CGAL_3RD_PARTY_LIBRARIES: ${CGAL_3RD_PARTY_LIBRARIES}")
    
else()
    message(FATAL_ERROR "CGAL is required.")
endif()

IF(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSEIF(WIN32)
    if(MSVC)
        FIND_LIBRARY(SMAG_LIBRARY samg_XP_6_2005 ${CMAKE_SOURCE_DIR}/libs/SAMG )
        message("CGAL is required." ${SMAG_LIBRARY})
    endif(MSVC)
ENDIF()    



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
    ${CGAL_INCLUDE_DIRS}
    ## - Find Boost
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} 
)
		
## Basic Libraries and Algorithms	
add_subdirectory(Core)
if (BUILD_FULL_SUITE)
    add_subdirectory(Apps)
endif(BUILD_FULL_SUITE)
	