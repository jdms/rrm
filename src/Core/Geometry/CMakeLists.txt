Project(RRMGeometry)


file(
  GLOB GEOMETRY_HEADERS
  "*.hpp"
  )

file(
  GLOB GEOMETRY_SOURCES
  "*.cpp"
  )

add_library( ${PROJECT_NAME} STATIC ${GEOMETRY_HEADERS} ${GEOMETRY_SOURCES} )

target_link_libraries( ${PROJECT_NAME} RRMBase )
