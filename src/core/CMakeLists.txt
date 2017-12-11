project(RRM)

###############################################
# Main source -- Core Directory
###############################################

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR} )

  
include_directories("./definitions")
include_directories("./widgets")
include_directories("./models")

add_subdirectory( definitions )
add_subdirectory( widgets )
add_subdirectory( models )


