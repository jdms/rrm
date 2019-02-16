#include "linear_complexes.hpp"

std::vector<double> vlist = {0, 0, 0, 1, 0, 1};
Segment<std::vector<double>> sa(vlist, 0, 1);
Segment<std::vector<double>> sb(vlist, 0, 1);

using MySegment = Segment<std::vector<double>>;
double t = computeVerticalIntersection(sa, sb);
