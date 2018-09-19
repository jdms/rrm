project(trimesh2 CXX C)
cmake_minimum_required(VERSION 2.6.2)



include_directories( src/include  )


set(HEADERS 
    src/include/Box.h
    src/include/bsphere.h
    src/include/Color.h
    src/include/endianutil.h
    src/include/ICP.h
    src/include/KDtree.h
    src/include/lineqn.h
    src/include/mathcompat.h
    src/include/mathutil.h
    src/include/noise3d.h
    src/include/strutil.h
    src/include/timestamp.h
    src/include/TriMesh_algo.h
    src/include/TriMesh.h
    src/include/Vec.h
    src/include/XForm.h
    )

set(LIB_SOURCES 
    src/libsrc/conn_comps.cc
    src/libsrc/diffuse.cc
    src/libsrc/edgeflip.cc
    src/libsrc/faceflip.cc
    src/libsrc/filter.cc
    src/libsrc/ICP.cc
    src/libsrc/KDtree.cc
    src/libsrc/make.cc
    src/libsrc/merge.cc
    src/libsrc/overlap.cc
    src/libsrc/remove.cc
    src/libsrc/reorder_verts.cc
    src/libsrc/subdiv.cc
    src/libsrc/TriMesh_bounding.cc
    src/libsrc/TriMesh_connectivity.cc
    src/libsrc/TriMesh_curvature.cc
    src/libsrc/TriMesh_grid.cc
    src/libsrc/TriMesh_io.cc
    src/libsrc/TriMesh_normals.cc
    src/libsrc/TriMesh_pointareas.cc
    src/libsrc/TriMesh_stats.cc
    src/libsrc/TriMesh_tstrips.cc
    src/libsrc/umbrella.cc
    ) 

add_library(${PROJECT_NAME} STATIC
    ${LIB_SOURCES} ${HEADERS}
)

set(INSTALL_DESTINATION ${CMAKE_SOURCE_DIR}/install)

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${INSTALL_DESTINATION}/lib
    LIBRARY DESTINATION ${INSTALL_DESTINATION}/lib
    RUNTIME DESTINATION ${INSTALL_DESTINATION}/lib
    COMPONENT library
    )

install(FILES ${HEADERS}
    DESTINATION ${INSTALL_DESTINATION}/include
    )