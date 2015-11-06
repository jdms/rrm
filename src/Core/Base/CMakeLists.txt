Project(RRMBase)


file(
  GLOB GEOMETRY_HEADERS
  "*.hpp"
  )

file(
  GLOB GEOMETRY_SOURCES
  "*.cpp"
  )


add_library( ${PROJECT_NAME} STATIC Constants.cpp Constants.hpp )