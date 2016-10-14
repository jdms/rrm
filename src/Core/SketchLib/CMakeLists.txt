Project(SketchLib)

file(
  GLOB SKETCHLIB_HEADERS
  "*.hpp"
  )

file(
  GLOB SKETCHLIB_SOURCES
  "*.cpp"
  )

add_library( ${PROJECT_NAME} STATIC ${SKETCHLIB_HEADERS} ${SKETCHLIB_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Constants  )