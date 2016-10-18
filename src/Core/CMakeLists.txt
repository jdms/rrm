project(RRM)

###############################################
# Main source -- Core Directory
###############################################

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )
  
#add_subdirectory( Geology )
add_subdirectory( Geometry )
add_subdirectory( Base )
#@Felipe
add_subdirectory(Arrangement_2D)