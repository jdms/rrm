project( RRM )

###############################################
# Main source -- Source Directory
###############################################

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

add_subdirectory( apps )
add_subdirectory( core )
