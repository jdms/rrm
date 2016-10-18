Project(Arrangement_2D)


file(
  GLOB ARRANGEMENT_HEADERS
  "*.hpp"
  )

file(
  GLOB ARRANGEMENT_SOURCES
  "*.cpp"
  )

message(${ARRANGEMENT_SOURCES})

add_library( ${PROJECT_NAME} STATIC ${ARRANGEMENT_HEADERS} ${ARRANGEMENT_SOURCES} )
target_link_libraries( ${PROJECT_NAME} RRMBase  )