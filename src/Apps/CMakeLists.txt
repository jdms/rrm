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


## Grab MainWindow Files

set( MAINWINDOW_HEADERS MainWindow/MainWindow.h
                        MainWindow/Scene.h
                        MainWindow/Controller.hpp
                        MainWindow/ExtrusionRulesProcessor.hpp
                        MainWindow/HelpDialog.h
                        MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_MOC_HEADERS MainWindow/MainWindow.h
                            MainWindow/Scene.h
                            MainWindow/Controller.hpp
                            MainWindow/HelpDialog.h
                            MainWindow/About/AboutWidget.hpp )


set( MAINWINDOW_UI_SOURCES  MainWindow/About/AboutWidget.ui
                            MainWindow/HelpDialog.ui )


set( MAINWINDOW_SOURCES MainWindow/main.cpp
                        MainWindow/MainWindow.cpp
                        MainWindow/Scene.cpp
                        MainWindow/Controller.cpp
                        MainWindow/ExtrusionRulesProcessor.cpp
                        MainWindow/HelpDialog.cpp
                        MainWindow/About/AboutWidget.cpp )




## Grab Sketching Files
set( SKETCHING_HEADERS Sketching/StratigraphicItem.hpp
                       Sketching/InputSketch.h
                       Sketching/PolyQtUtils.hpp
                       Sketching/BoundaryItem2D.hpp
                       Sketching/SketchingWindow.h
                       Sketching/SketchingCanvas.h )


set( SKETCHING_MOC_HEADERS  Sketching/SketchingWindow.h
                            Sketching/SketchingCanvas.h )



set( SKETCHING_SOURCES  Sketching/StratigraphicItem.cpp
                        Sketching/InputSketch.cpp
                        Sketching/PolyQtUtils.cpp
                        Sketching/BoundaryItem2D.cpp
                        Sketching/SketchingWindow.cpp
                        Sketching/SketchingCanvas.cpp )


## Grab 3DView Files
set( 3DVIEW_HEADERS 3dView/BoundingBox3D.h
                       3dView/CrossPlane.h
                       3dView/View3DCanvas.hpp
                       3dView/Surface.h
                       3dView/View3DWindow.hpp
                       3dView/Model3DUtils.hpp
                       3dView/GradientBackgroundShader.cpp )


set( 3DVIEW_MOC_HEADERS  3dView/View3DCanvas.hpp
                         3dView/View3DWindow.hpp )



set( 3DVIEW_SOURCES 3dView/BoundingBox3D.cpp
                    3dView/CrossPlane.cpp
                    3dView/View3DCanvas.cpp
                    3dView/Surface.cpp
                    3dView/View3DWindow.cpp
                    3dView/Model3DUtils.cpp
                    3dView/GradientBackgroundShader.cpp )





## Grab 3DView Files
set( MODEL_HEADERS model_new/Boundary.hpp
                   model_new/BoundaryRenderable.h
                   model_new/CrossSection.hpp
                   model_new/StratigraphyRenderable.hpp
                   model_new/Intersection.hpp
                   model_new/Stratigraphy.hpp )




set( MODEL_SOURCES  )





### Grab Controller Source Files
#set( CONTROLLER_HEADERS Controller/MainWindow.h
#                        Controller/Boundary.hpp
#                        Controller/Controller.hpp
#                        Controller/CrossSection.hpp
#                        Controller/Stratigraphy.hpp
#                        Controller/StratigraphicItem.hpp
#                        Controller/About/AboutWidget.hpp
#   )
   


#set( CONTROLLER_MOC_HEADERS Controller/MainWindow.h
#                            Controller/About/AboutWidget.hpp
  
#   )


   
#set ( CONTROLLER_UI_SOURCES  Controller/About/AboutWidget.ui )

#set( CONTROLLER_SOURCES Controller/main.cpp
#                        Controller/MainWindow.cpp
#                        Controller/Controller.cpp
#                        Controller/StratigraphicItem.cpp
#                        Controller/About/AboutWidget.cpp
#   )

## todo , SBIM will be a huge module with all possible scenarios of sketch, 
## aka: black screen, seismic, point cloud ...



## Grab SBIM Sources Files
#set ( SBIM_HEADERS SBIM/SeismicSlice.hpp
#                   SBIM/SeismicVolume.hpp
#                   SBIM/SBIMSeismicController.hpp
#                   SBIM/SketchSeismicModule.hpp
#                   # View
#                   SBIM/View/SeismicView.hpp
#                   SBIM/View/SeismicWindow.hpp
#                   SBIM/View/SeismicThumbnailItem.hpp
#                   )
                   
#set ( SBIM_MOC_HEADERS  SBIM/View/SeismicView.hpp
#                        SBIM/View/SeismicWindow.hpp
#                        SBIM/SBIMSeismicController.hpp
#                        )
                        
#set ( SBIM_UI_SOURCES  SBIM/View/SeismicWindow.ui)

#set ( SBIM_SOURCES SBIM/SeismicSlice.cpp
#                   SBIM/SeismicVolume.cpp
#                   SBIM/SBIMSeismicController.cpp
#                   SBIM/SketchSeismicModule.cpp
#                   SBIM/View/SeismicView.cpp
#                   SBIM/View/SeismicWindow.cpp
#                   SBIM/View/SeismicThumbnailItem.cpp)



### Grab Modeller Source Files
#set( MODELLER_HEADERS   Modeller/Sketching2DModule.h
#                        Modeller/InputSketch.h
#                        Modeller/StratigraphyItem.hpp
#                        Modeller/BoundaryItem.h
#                        Modeller/SketchController.hpp
#                        Modeller/ExtrusionWidget/View3DCanvas.hpp
#                        Modeller/ExtrusionWidget/ExtrusionController.hpp
#                        Modeller/ExtrusionWidget/View3DWindow.hpp
#                        Modeller/ExtrusionWidget/BoundingBox3D.h
#                        Modeller/ExtrusionWidget/BoundaryItem2D.hpp
#                        Modeller/ExtrusionWidget/CrossPlane.h
#                        Modeller/ExtrusionWidget/Surface.h
#                        Modeller/SketchBoardWidget/SketchBoard.hpp
#                        Modeller/SketchingCanvas.h
#                        Modeller/Scene.h
#                        #Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp
#                        Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp
#   )
   
#set( MODELLER_MOC_HEADERS Modeller/Sketching2DModule.h
#                          Modeller/SketchController.hpp
#                          Modeller/ExtrusionWidget/View3DCanvas.hpp
#                          Modeller/ExtrusionWidget/ExtrusionController.hpp
#                          Modeller/ExtrusionWidget/View3DWindow.hpp
#                          Modeller/SketchBoardWidget/SketchBoard.hpp
#                          Modeller/SketchingCanvas.h
#                          Modeller/Scene.h
#                          #Modeller/SketchBoardWidget/SketchSession/SketchSession.hpp
#                          Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.hpp
#						)

#set( MODELLER_SOURCES   Modeller/Sketching2DModule.cpp
#                        Modeller/InputSketch.cpp
#                        Modeller/StratigraphyItem.cpp
#                        Modeller/BoundaryItem.cpp
#                        Modeller/SketchController.cpp
#                        Modeller/ExtrusionWidget/OpenGLWidget.cpp
#                        Modeller/ExtrusionWidget/ExtrusionController.cpp
#                        Modeller/ExtrusionWidget/View3DWindow.cpp
#                        Modeller/ExtrusionWidget/BoundingBox3D.cpp
#                        Modeller/ExtrusionWidget/CrossPlane.cpp
#                        Modeller/ExtrusionWidget/BoundaryItem2D.cpp
#                        Modeller/ExtrusionWidget/Surface.cpp
#                        Modeller/SketchBoardWidget/SketchBoard.cpp
#                        Modeller/SketchingCanvas.cpp
#                        Modeller/Scene.cpp
#                        #Modeller/SketchBoardWidget/SketchSession/SketchSession.cpp
#                        Modeller/SketchBoardWidget/SketchSession/SketchSessionTesting.cpp
#   )




### Grab Simulator Source Files
#set( SIMULATOR_HEADERS
#	 Simulator/Ply/ply.h
#     Simulator/auxiliar.h
#     Simulator/ColorMap.h
#     Simulator/CoordinateAxes.h
#     Simulator/FlowCrossSection.h
#     Simulator/FlowParametersBar.h
#     Simulator/FlowRenderingOptionsMenu.h
#     Simulator/FlowVisualizationCanvas.h
#     Simulator/FlowVisualizationController.h
#     Simulator/FlowWindow.h
#     Simulator/FormPropertyValues.h
#     Simulator/Mesh.h
#     Simulator/NormalMovableCrossSectionFlow.h
#     Simulator/OpenFlowFilesBar.h
#     Simulator/DialogMeshVisualizationParameters.h
#     Simulator/ProgressCounter.h
#     Simulator/ColorBar.h
#     Simulator/PoreVolumeResultsForm.h
#     Simulator/FlowComputation/bsurface.h
#     Simulator/FlowComputation/cpgelement.h
#     Simulator/FlowComputation/curve.h
#     Simulator/FlowComputation/edge.h
#     Simulator/FlowComputation/facet.h
#     Simulator/FlowComputation/geosurface.h
#     Simulator/FlowComputation/graph.h
#     Simulator/FlowComputation/graphedge.h
#     Simulator/FlowComputation/graphnode.h
#     Simulator/FlowComputation/linearfunc.h
#     Simulator/FlowComputation/mathzz.h
#     Simulator/FlowComputation/matrix.h
#     Simulator/FlowComputation/model.h
#     Simulator/FlowComputation/meshinfo.h
#     Simulator/FlowComputation/modelsurface.h
#     Simulator/FlowComputation/node.h
#     Simulator/FlowComputation/parasurface.h
#     Simulator/FlowComputation/propertyarea.h
#     Simulator/FlowComputation/region.h
#     Simulator/FlowComputation/samg.h
#     Simulator/FlowComputation/segment.h
#     Simulator/FlowComputation/skeleton.h
#     Simulator/FlowComputation/tetgen.h
#     Simulator/FlowComputation/tetrahedron.h
#     Simulator/FlowComputation/tofb.h
#     Simulator/FlowComputation/tracerb.h
#     Simulator/FlowComputation/triangle.h
#     Simulator/FlowComputation/trifacet.h
#     Simulator/FlowComputation/usermanual.h
#     Simulator/FlowComputation/uvline.h
#     Simulator/FlowComputation/uvnode.h
#     Simulator/FlowComputation/well.h
#    )

#set ( SIMULATOR_MOC_HEADERS
#     Simulator/FlowParametersBar.h
#     Simulator/FlowRenderingOptionsMenu.h
#     Simulator/FlowVisualizationCanvas.h
#     Simulator/FlowVisualizationController.h
#     Simulator/FlowWindow.h
#     Simulator/FormPropertyValues.h
#     Simulator/NormalMovableCrossSectionFlow.h
#     Simulator/OpenFlowFilesBar.h
#     Simulator/ProgressCounter.h
#     Simulator/DialogMeshVisualizationParameters.h
#     Simulator/ColorBar.h
#     Simulator/PoreVolumeResultsForm.h
#)

#set ( SIMULATOR_UI_SOURCES
#    Simulator/FlowParametersBar.ui
#    Simulator/NormalMovableCrossSectionFlow.ui
#    Simulator/OpenFlowFilesBar.ui
#    Simulator/DialogMeshVisualizationParameters.ui
#)

#set( SIMULATOR_SOURCES
#	 Simulator/Ply/ply.c
#     Simulator/ColorMap.cpp
#     Simulator/CoordinateAxes.cpp
#     Simulator/FlowCrossSection.cpp
#     Simulator/FlowParametersBar.cpp
#     Simulator/FlowRenderingOptionsMenu.cpp
#     Simulator/FlowVisualizationCanvas.cpp
#     Simulator/FlowVisualizationController.cpp
#     Simulator/FlowWindow.cpp
#     Simulator/FormPropertyValues.cpp
#     Simulator/Mesh.cpp
#     Simulator/NormalMovableCrossSectionFlow.cpp
#     Simulator/OpenFlowFilesBar.cpp
#     Simulator/DialogMeshVisualizationParameters.cpp
#     Simulator/ColorBar.cpp
#     Simulator/PoreVolumeResultsForm.cpp
#     Simulator/FlowComputation/bsurface.cpp
#     Simulator/FlowComputation/cpgelement.cpp
#     Simulator/FlowComputation/curve.cpp
#     Simulator/FlowComputation/edge.cpp
#     Simulator/FlowComputation/facet.cpp
#     Simulator/FlowComputation/geosurface.cpp
#     Simulator/FlowComputation/graph.cpp
#     Simulator/FlowComputation/graphedge.cpp
#     Simulator/FlowComputation/graphnode.cpp
#     Simulator/FlowComputation/linearfunc.cpp
#     Simulator/FlowComputation/mathzz.cpp
#     Simulator/FlowComputation/matrix.cpp
#     Simulator/FlowComputation/model.cpp
#     Simulator/FlowComputation/meshinfo.cpp
#     Simulator/FlowComputation/modelsurface.cpp
#     Simulator/FlowComputation/node.cpp
#     Simulator/FlowComputation/predicates.cxx
#     Simulator/FlowComputation/parasurface.cpp
#     Simulator/FlowComputation/propertyarea.cpp
#     Simulator/FlowComputation/region.cpp
#     Simulator/FlowComputation/segment.cpp
#     Simulator/FlowComputation/skeleton.cpp
#     Simulator/FlowComputation/tetgen.cxx
#     Simulator/FlowComputation/tetrahedron.cpp
#     Simulator/FlowComputation/tofb.cpp
#     Simulator/FlowComputation/tracerb.cpp
#     Simulator/FlowComputation/triangle.c
#     Simulator/FlowComputation/trifacet.cpp
#     Simulator/FlowComputation/uvline.cpp
#     Simulator/FlowComputation/uvnode.cpp
#     Simulator/FlowComputation/well.cpp
#    )





## Q_OBJECT files
set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} )
#set( MOC_HEADERS ${MAINWINDOW_MOC_HEADERS} ${SBIM_MOC_HEADERS} ${SKETCHING_MOC_HEADERS} ${3DVIEW_MOC_HEADERS} ${SIMULATOR_MOC_HEADERS} )


## Qt Designer Ui files
set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} )
#set( UI_SOURCES ${MAINWINDOW_UI_SOURCES} ${SBIM_UI_SOURCES} ${SIMULATOR_UI_SOURCES} )


## Resources
set( RCC_SOURCES Resources/images.qrc )



QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS})
QT5_WRAP_UI (UI_HEADERS ${UI_SOURCES})
QT5_ADD_RESOURCES(RCC_HEADERS ${RCC_SOURCES})



IF(WIN32) # Check if we are on Windows

    if(MSVC) # Check if we are using the Visual Studio compiler

        add_definitions(/arch:SSE2)

        message(STATUS "- MSVC: Enabled SSE2 support")


        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
                                       ${MODEL_SOURCES} ${MODEL_HEADERS}
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )


#        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
#                                       ${SBIM_SOURCES} ${SBIM_HEADERS}
#                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
#                                       ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS}
#                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
#                                       ${MODEL_SOURCES} ${MODEL_HEADERS}
#                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )

        target_link_libraries(${PROJECT_NAME} Qt5::WinMain Qt5::Gui Qt5::Core Qt5::Help Qt5::OpenGL Qt5::Widgets Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SAMG_LIBRARY} )



    elseif(CMAKE_COMPILER_IS_GNUCXX)  # MinGW on Windows
        message (FATAL_ERROR "GCC are not support on Windows !")

    else() # Nor MSVC or GCC (MinGW)
        message(FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")

    endif()


ELSEIF(UNIX)

    # Nothing special required for Linux
    message(STATUS "Build on Linux")

        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )

#        add_executable(${PROJECT_NAME} ${MAINWINDOW_SOURCES} ${MAINWINDOW_HEADERS}
#                                       ${SBIM_SOURCES} ${SBIM_HEADERS}
#                                       ${SKETCHING_SOURCES} ${SKETCHING_HEADERS}
#                                       ${3DVIEW_SOURCES} ${3DVIEW_HEADERS}
#                                       ${MODEL_SOURCES} ${MODEL_HEADERS}
#                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS} )

        target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )


ELSE() # Not Windows or Unix
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")


ENDIF()





### Q_OBJECT files
#set( MOC_HEADERS ${CONTROLLER_MOC_HEADERS} ${SBIM_MOC_HEADERS} ${MODELLER_MOC_HEADERS}
#    ${SIMULATOR_MOC_HEADERS}
#   )
### Qt Designer Ui files
#set( UI_SOURCES ${CONTROLLER_UI_SOURCES} ${SBIM_UI_SOURCES}
#    ${SIMULATOR_UI_SOURCES}
#    )
### Resources
#set(RCC_SOURCES Controller/Resources/images.qrc
#				Modeller/ExtrusionWidget/shaderresources.qrc
#				)

#QT5_WRAP_CPP( MOC_SOURCES ${MOC_HEADERS})
#QT5_WRAP_UI (UI_HEADERS ${UI_SOURCES})
#QT5_ADD_RESOURCES(RCC_HEADERS ${RCC_SOURCES})

## @see http://www.cmake.org/Wiki/CMake/Tutorials/Qt
#IF(WIN32) # Check if we are on Windows
#    if(MSVC) # Check if we are using the Visual Studio compiler
#        add_definitions(/arch:SSE2)
#        message(STATUS "- MSVC: Enabled SSE2 support")
        
#       	# /SUBSYSTEM:CONSOLE DEBUG VERSION
#		# /SUBSYSTEM:WINDOWN RELEASE VERSION
#        #set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
#		#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
        
#        add_executable(${PROJECT_NAME}
#        							   ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS}
#                                       ${SBIM_SOURCES} ${SBIM_HEADERS}
#                                       ${MODELLER_SOURCES} ${MODELLER_HEADERS}
#                                       ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS}
#                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})
#		target_link_libraries(${PROJECT_NAME} Qt5::WinMain Qt5::Gui Qt5::Core Qt5::Help Qt5::OpenGL Qt5::Widgets Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} ${SAMG_LIBRARY} )

#    elseif(CMAKE_COMPILER_IS_GNUCXX)  # MinGW on Windows
#        message (FATAL_ERROR "GCC are not support on Windows !")
#    else() # Nor MSVC or GCC (MinGW)
#        message(FATAL_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
#    endif()
#ELSEIF(UNIX)
#    # Nothing special required for Linux
#    message(STATUS "Build on Linux")
#        add_executable(${PROJECT_NAME} ${CONTROLLER_SOURCES} ${CONTROLLER_HEADERS}
#                                       ${SBIM_SOURCES} ${SBIM_HEADERS}
#                                       ${MODELLER_SOURCES} ${MODELLER_HEADERS}
#                                       #${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS}
#                                       ${MOC_SOURCES} ${UI_HEADERS} ${RCC_HEADERS})
#    target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::OpenGL Qt5::Widgets Qt5::Core Qt5::Svg planin RRMGeometry ${OPENGL_LIBRARIES} ${GLEW_LIBRARY} )
#ELSE() # Not Windows or Unix
#  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
#ENDIF()



