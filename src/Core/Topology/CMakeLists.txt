project(RRMTopology)

file(
  GLOB TOPOLOGY_HEADERS
  "*.hpp"
  )

file(
  GLOB TOPOLOGY_SOURCES
  "*.cpp"
  )

add_library( RRMTopology STATIC ${TOPOLOGY_SOURCES} ${TOPOLOGY_HEADERS}  )

target_link_libraries(RRMTopology ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )


