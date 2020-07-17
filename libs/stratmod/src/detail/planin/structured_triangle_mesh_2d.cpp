#include "planin/structured_triangle_mesh_2d.hpp"
#include "planin/core.hpp"

#include <Eigen/Dense>

template class StructuredTriangleMesh2D<std::array<double, 2>>;

template class StructuredTriangleMesh2D<Point2>;

template class StructuredTriangleMesh2D<Eigen::Vector2d>;
