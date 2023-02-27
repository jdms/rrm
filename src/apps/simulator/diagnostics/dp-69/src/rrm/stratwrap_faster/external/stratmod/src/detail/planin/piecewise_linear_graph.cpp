#include "planin/piecewise_linear_graph.hpp"
#include "planin/core.hpp"

#include <Eigen/Dense>

template class PiecewiseLinearGraph<std::array<double, 2>>;

template class PiecewiseLinearGraph<Point2>;

template class PiecewiseLinearGraph<Eigen::Vector2d>;
