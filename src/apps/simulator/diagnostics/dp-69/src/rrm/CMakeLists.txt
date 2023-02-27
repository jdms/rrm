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

cmake_minimum_required (VERSION 3.21)

if(MSVC)
  add_compile_options("/w")
else()
  add_compile_options("-w")
endif()

add_subdirectory("stratwrap_faster/external/stratmod")

if(UNIX)
  set(FD_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
else()
  set(FD_OUTPUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()
add_subdirectory(fd)

add_executable(rrmfd # WIN32
  "main.cpp"  
)

target_compile_features(rrmfd PUBLIC cxx_std_17)
#target_compile_features(rrmfd PUBLIC cxx_std_20)

target_link_libraries(rrmfd PUBLIC rrm_flow_diagnostics_lib)

find_package(nlohmann_json)
target_link_libraries(rrmfd PUBLIC nlohmann_json::nlohmann_json)

foreach(FILE_EXT "fdsettings.json" "rrm.json" "rrm")
  if(NOT EXISTS "${FD_OUTPUT_DIR}/_config/test_model.${FILE_EXT}")
    file(COPY_FILE
      "${CMAKE_CURRENT_SOURCE_DIR}/resources/test_model.${FILE_EXT}"
      "${FD_OUTPUT_DIR}/_config/test_model.${FILE_EXT}")
  endif()
endforeach()
