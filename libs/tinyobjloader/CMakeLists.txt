Project(TinyObjLoader)

file(
  GLOB TINYOBJLOADER_HEADERS
  "*.h"
  )

file(
  GLOB TINYOBJLOADER_SOURCES
  "*.cc"
  )

add_library( ${PROJECT_NAME} STATIC ${TINYOBJLOADER_HEADERS} ${TINYOBJLOADER_SOURCES} )
#target_link_libraries( ${PROJECT_NAME} RRMBase  )
