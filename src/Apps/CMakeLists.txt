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
						Controller/About/AboutWidget.hpp
   )
   
set( CONTROLLER_MOC_HEADERS Controller/MainWindow.h
							Controller/About/AboutWidget.hpp
  
   )
   
set ( CONTROLLER_UI_SOURCES  Controller/About/AboutWidget.ui )  

set( CONTROLLER_SOURCES Controller/main.cpp
                        Controller/MainWindow.cpp
                        Controller/About/AboutWidget.cpp
                        Controller/MainModeller.cpp
                        Controller/MainFlow.cpp
   )

## todo , SBIM will be a huge module with all possible scenarios of sketch, 
## aka: black screen, seismic, point cloud ...

## Grab SBIM Sources Files
set ( SBIM_HEADERS SBIM/SeismicSlice.hpp
                   SBIM/SeismicVolume.hpp
                   SBIM/SBIMSeismicController.hpp
                   SBIM/SketchSeismicModule.hpp 
                   # View
                   SBIM/View/SeismicView.hpp
                   SBIM/View/SeismicWindow.hpp
                   SBIM/View/SeismicThumbnailItem.hpp
                   )
                   
set ( SBIM_MOC_HEADERS  SBIM/View/SeismicView.hpp
                        SBIM/View/SeismicWindow.hpp
                        SBIM/SBIMSeismicController.hpp
                        )
                        
set ( SBIM_UI_SOURCES  SBIM/View/SeismicWindow.ui)                       

set ( SBIM_SOURCES SBIM/SeismicSlice.cpp
                   SBIM/SeismicVolume.cpp
                   SBIM/SBIMSeismicController.cpp
                   SBIM/SketchSeismicModule.cpp
                   # View
                   SBIM/View/SeismicView.cpp
                   SBIM/View/SeismicWindow.cpp 
                   SBIM/View/SeismicThumbnailItem.cpp)

## Grab Modeller Source Files
set( MODELLER_HEADERS   Modeller/Sketching2DModule.h
                        Modeller/InputSketch.h
                        Modeller/StratigraphyItem.hpp
                        Modeller/BoundaryItem.h
                        Modeller/SketchController.hpp   
                        Modeller/ExtrusionWidget/OpenGLWidget.hpp
                        Modeller/ExtrusionWidget/ExtrusionController.hpp
                        Modeller/ExtrusionWidget/View3DModule.hpp
                        Modeller/SketchBoardWidget/SketchBoard.hpp
                        #Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp
                        Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp
   )
   
set( MODELLER_MOC_HEADERS Modeller/Sketching2DModule.h
                          Modeller/SketchController.hpp
						  Modeller/ExtrusionWidget/OpenGLWidget.hpp
						  Modeller/ExtrusionWidget/ExtrusionController.hpp
						  Modeller/ExtrusionWidget/View3DModule.hpp
					      Modeller/SketchBoardWidget/SketchBoard.hpp
                          #Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp
                          Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp
						)

set( MODELLER_SOURCES   Modeller/Sketching2DModule.cpp
                        Modeller/InputSketch.cpp
                        Modeller/StratigraphyItem.cpp
                        Modeller/BoundaryItem.cpp
                        Modeller/SketchController.cpp
                        Modeller/ExtrusionWidget/OpenGLWidget.cpp
                        Modeller/ExtrusionWidget/ExtrusionController.cpp
                        Modeller/ExtrusionWidget/View3DModule.cpp
                        Modeller/SketchBoardWidget/SketchBoard.cpp
                        #Modeller/SketchBoardWidget/SketchSession/SketchSession.cpp
                        Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.cpp
   )
## Grab Simulator Source Files
set( SIMULATOR_HEADERS
	 Simulator/Ply/ply.h
     Simulator/auxiliar.h
     Simulator/ColorMap.h
     Simulator/CoordinateAxes.h
     Simulator/FlowCrossSection.h
     Simulator/FlowParametersBar.h
     Simulator/FlowRenderingOptionsMenu.h
     Simulator/FlowVisualizationCanvas.h
     Simulator/FlowVisualizationController.h   
     Simulator/FlowWindow.h
     Simulator/FormPropertyValues.h
     Simulator/Mesh.h
     Simulator/NormalMovableCrossSectionFlow.h
     Simulator/OpenFlowFilesBar.h
     Simulator/ProgressCounter.h
     Simulator/FlowComputation/bsurface.h
     Simulator/FlowComputation/cpgelement.h
     Simulator/FlowComputation/curve.h
     Simulator/FlowComputation/edge.h
     Simulator/FlowComputation/facet.h
     Simulator/FlowComputation/geosurface.h
     Simulator/FlowComputation/graph.h
     Simulator/FlowComputation/graphedge.h
     Simulator/FlowComputation/graphnode.h
     Simulator/FlowComputation/linearfunc.h
     Simulator/FlowComputation/mathzz.h
     Simulator/FlowComputation/matrix.h
     Simulator/FlowComputation/meshinfo.h
     Simulator/FlowComputation/modelsurface.h
     Simulator/FlowComputation/node.h
     Simulator/FlowComputation/parasurface.h
     Simulator/FlowComputation/region.h
     Simulator/FlowComputation/samg.h
     Simulator/FlowComputation/segment.h
     Simulator/FlowComputation/skeleton.h
     Simulator/FlowComputation/tetgen.h
     Simulator/FlowComputation/tetrahedron.h
     Simulator/FlowComputation/tofb.h
     Simulator/FlowComputation/tracerb.h
     Simulator/FlowComputation/triangle.h
     Simulator/FlowComputation/trifacet.h
     Simulator/FlowComputation/usermanual.h
     Simulator/FlowComputation/uvline.h
     Simulator/FlowComputation/uvnode.h
     Simulator/FlowComputation/well.h
    )

set ( SIMULATOR_MOC_HEADERS
     Simulator/FlowParametersBar.h
     Simulator/FlowRenderingOptionsMenu.h
     Simulator/FlowVisualizationCanvas.h
     Simulator/FlowVisualizationController.h   
     Simulator/FlowWindow.h
     Simulator/FormPropertyValues.h
     Simulator/NormalMovableCrossSectionFlow.h
     Simulator/OpenFlowFilesBar.h
     Simulator/ProgressCounter.h
)

set ( SIMULATOR_UI_SOURCES
    Simulator/FlowParametersBar.ui                       
    Simulator/NormalMovableCrossSectionFlow.ui
    Simulator/OpenFlowFilesBar.ui
)

set( SIMULATOR_SOURCES    
	 Simulator/Ply/ply.c
     Simulator/ColorMap.cpp
     Simulator/CoordinateAxes.cpp
     Simulator/FlowCrossSection.cpp
     Simulator/FlowParametersBar.cpp
     Simulator/FlowRenderingOptionsMenu.cpp
     Simulator/FlowVisualizationCanvas.cpp
     Simulator/FlowVisualizationController.cpp   
     Simulator/FlowWindow.cpp
     Simulator/FormPropertyValues.cpp
     Simulator/Mesh.cpp
     Simulator/NormalMovableCrossSectionFlow.cpp
     Simulator/OpenFlowFilesBar.cpp
     Simulator/FlowComputation/bsurface.cpp
     Simulator/FlowComputation/cpgelement.cpp
     Simulator/FlowComputation/curve.cpp
     Simulator/FlowComputation/edge.cpp
     Simulator/FlowComputation/facet.cpp
     Simulator/FlowComputation/geosurface.cpp
     Simulator/FlowComputation/graph.cpp
     Simulator/FlowComputation/graphedge.cpp
     Simulator/FlowComputation/graphnode.cpp
     Simulator/FlowComputation/linearfunc.cpp
     Simulator/FlowComputation/mathzz.cpp
     Simulator/FlowComputation/matrix.cpp
     Simulator/FlowComputation/meshinfo.cpp
     Simulator/FlowComputation/modelsurface.cpp
     Simulator/FlowComputation/node.cpp
	 Simulator/FlowComputation/predicates.cxx
     Simulator/FlowComputation/parasurface.cpp
     Simulator/FlowComputation/region.cpp
     Simulator/FlowComputation/segment.cpp
     Simulator/FlowComputation/skeleton.cpp
     Simulator/FlowComputation/tetgen.cxx
     Simulator/FlowComputation/tetrahedron.cpp
     Simulator/FlowComputation/tofb.cpp
     Simulator/FlowComputation/tracerb.cpp
     Simulator/FlowComputation/triangle.c
     Simulator/FlowComputation/trifacet.cpp
     Simulator/FlowComputation/uvline.cpp
     Simulator/FlowComputation/uvnode.cpp
     Simulator/FlowComputation/well.cpp
    )


## Q_OBJECT files
set( MOC_HEADERS ${CONTROLLER_MOC_HEADERS} ${SBIM_MOC_HEADERS} ${MODELLER_MOC_HEADERS} ${SIMULATOR_MOC_HEADERS}  
#${SIMULATOR_MOC_HEADERS} 
   )
## Qt Designer Ui files                 
set( UI_SOURCES ${CONTROLLER_UI_SOURCES} ${SBIM_UI_SOURCES} ${SIMULATOR_UI_SOURCES} )
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
        add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS}
                                       ${SBIM_SOURCES} ${SBIM_HEADERS}      
                                       ${MODELLER_SOURCES} ${MODELLER_HEADERS} 
                                       ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS} 
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})
		target_link_libraries(${PROJECT_NAME} Qt5::WinMain Qt5::Gui Qt5::OpenGL Qt5::Widgets planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SAMG_LIBRARY} )
		# /SUBSYSTEM:CONSOLE DEBUG VERSION
		# /SUBSYSTEM:WINDOWN RELEASE VERSION
        #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    elseif(CMAKE_COMPILER_IS_GNUCXX)  # MinGW on Windows
        message (FATAL_ERROR "GCC are not support on Windows !")
    else() # Nor MSVC or GCC (MinGW)
        message(FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
    endif()
ELSEIF(UNIX)
    # Nothing special required for Linux
    message(STATUS "Build on Linux")
        add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS}
                                       ${SBIM_SOURCES} ${SBIM_HEADERS}      
                                       ${MODELLER_SOURCES} ${MODELLER_HEADERS} 
                                       ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS} 
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})	
    target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )
ELSE() # Not Windows or Unix
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()



