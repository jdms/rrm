message(STATUS "Core Lbirary ")

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(Base)
add_subdirectory(Geometry)

add_subdirectory(Geometry/Surface/Planin)



