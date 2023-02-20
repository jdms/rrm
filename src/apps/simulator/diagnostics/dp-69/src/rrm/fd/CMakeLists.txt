#
# This file is part of Rapid Reservoir Modelling Software.
#   | https://rapidreservoir.org/
#   | https://bitbucket.org/rapidreservoirmodelling/rrm/
#
# Copyright (c) 2022
#   | Dmytro Petrovskyy, PhD
#   | dmytro.petrovsky@gmail.com
#   | https://www.linkedin.com/in/dmytro-petrovskyy/
#
# RRM is free software: you can redistribute it and/or modify              
# it under the terms of the GNU General Public License as published by     
# the Free Software Foundation, either version 3 of the License, or        
# (at your option) any later version.                                      
#                                                                         
# RRM is distributed in the hope that it will be useful,                   
# but WITHOUT ANY WARRANTY; without even the implied warranty of           
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            
# GNU General Public License for more details.                             
#                                                                         
# You should have received a copy of the GNU General Public License        
# along with RRM. If not, see <http://www.gnu.org/licenses/>.
#
#

cmake_minimum_required (VERSION 3.3.0)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

find_package(VTK REQUIRED)

set(FLOW_DIAG_SOURCES 
  "FDWidget.cpp"
  "FDWidgetImpl.cpp"
  "FDQVTKWidget.cpp"
  "FDPropertyTree.cpp"

  "core/property_editor/Editors.hpp"
  "core/property_editor/PropertyEntities.hpp"
  "core/property_editor/QPropertyTreeView.hpp"
  "core/QExtendedPieChartView.hpp"
  
  "component/RecoveryMapTask.cpp"
  "component/Volumetrics.cpp"
  "component/LorenzPlot.hpp"
)
add_library(rrm_flow_diagnostics_lib ${FLOW_DIAG_SOURCES})


target_compile_features(rrm_flow_diagnostics_lib PUBLIC cxx_std_17)
#target_compile_features(rrm_flow_diagnostics_lib PUBLIC cxx_std_20)


target_include_directories(rrm_flow_diagnostics_lib PUBLIC "../..")
target_include_directories(rrm_flow_diagnostics_lib PUBLIC "../../rrm")
#target_include_directories(rrm_flow_diagnostics_lib PUBLIC "dpl")


target_link_libraries(rrm_flow_diagnostics_lib PUBLIC stratmod)


find_package(nlohmann_json)
target_link_libraries(rrm_flow_diagnostics_lib PUBLIC nlohmann_json::nlohmann_json)



if(UNIX)
  find_package(Qt5 COMPONENTS Core Gui Widgets Charts X11Extras REQUIRED)
else()
  find_package(Qt5 COMPONENTS Core Gui Widgets Charts REQUIRED)
endif()
message("Qt5_DIR: " ${Qt5_DIR})
message("Qt5_VERSION: " ${Qt5_VERSION})


if(UNIX)
  target_link_libraries(rrm_flow_diagnostics_lib PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Charts Qt5::X11Extras)
else()
  target_link_libraries(rrm_flow_diagnostics_lib PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Charts)
endif()

message("VTK_DIR: " ${VTK_DIR})
message("VTK_VERSION: " ${VTK_VERSION})

if(VTK_VERSION VERSION_LESS "8.90.0")
  message("VTK_USE_FILE: " ${VTK_USE_FILE})
  include(${VTK_USE_FILE})

  target_link_libraries(rrm_flow_diagnostics_lib PUBLIC ${VTK_LIBRARIES}
      vtkCommonColor
      vtkCommonCore
      vtkFiltersModeling
      vtkFiltersSources
      vtkChartsCore
      vtkImagingCore
      vtkIOImage
      vtkInteractionStyle
      vtkInteractionWidgets
      vtkRenderingAnnotation    
      vtkRenderingContextOpenGL2
      vtkRenderingCore
      vtkRenderingFreeType
      vtkRenderingGL2PSOpenGL2
      vtkRenderingOpenGL2
      vtkRenderingVolumeOpenGL2
      vtkGUISupportQt)
else()
  target_link_libraries(rrm_flow_diagnostics_lib PUBLIC ${VTK_LIBRARIES})
  vtk_module_autoinit(TARGETS rrm_flow_diagnostics_lib MODULES ${VTK_LIBRARIES})
endif()


find_package(Boost) # COMPONENTS filesystem
target_include_directories(rrm_flow_diagnostics_lib PUBLIC ${Boost_INCLUDE_DIRS})


find_package(HYPRE CONFIG QUIET)
if(HYPRE_FOUND)
  target_link_libraries(rrm_flow_diagnostics_lib PRIVATE HYPRE::HYPRE)
else()
  target_link_libraries(rrm_flow_diagnostics_lib
    PRIVATE ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/HYPRE.lib)
endif()


 



if (DEFINED FD_OUTPUT_DIR)
  file(MAKE_DIRECTORY "${FD_OUTPUT_DIR}/_config")

  foreach(FILE "droplet.png" "fdsettings.json" "startup.json")
    if(NOT EXISTS "${FD_OUTPUT_DIR}/_config/${FILE}")
      file(COPY_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/${FILE}"
        "${FD_OUTPUT_DIR}/_config/${FILE}")
    endif()
  endforeach()
endif()


 














