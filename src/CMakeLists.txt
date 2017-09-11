project( RRM )

###############################################
# Main source -- Source Directory
###############################################

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


if( BUILD_FULL_SUITE )
    add_subdirectory( apps )
endif( BUILD_FULL_SUITE )

add_subdirectory( core )


	
