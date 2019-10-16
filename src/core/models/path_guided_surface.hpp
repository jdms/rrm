/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RRM_CORE_MODELS_PATH_GUIDED_SURFACE
#define RRM_CORE_MODELS_PATH_GUIDED_SURFACE

#include <algorithm>
#include <array>
#include <cmath>
#include <tuple>

#include "ode_solver_2d.hpp"

class PathGuidedSurface {
   public:
    void setOrigin(double origin_width, double origin_length)
    {
        solver_.setDomainOrigin(origin_width, origin_length);
    }

    bool setSize(double size_width, double size_length)
    {
        bool success = solver_.setDomainSize(size_width, size_length);
        if (!success) {
            return false;
        }

        double support_size =
            4 * std::sqrt(size_width * size_width + size_length * size_length);
        solver_.setInterpolantBasisScale(support_size);

        return success;
    }

    bool addGuidingPathTangentVectors(const std::vector<double>& curve,
                                      double samples_per_unit_length,
                                      std::size_t max_num_samples)
    {
        if (samples_per_unit_length <= 0) {
            return false;
        }

        auto [curve_w, curve_l] = convertCurve(curve);
        std::size_t num_samples = std::min(
            static_cast<std::size_t>(
                std::round(getCurveLength(curve_w, curve_l) / samples_per_unit_length)),
            max_num_samples);

        if (num_samples < 1) {
            return false;
        }

        return solver_.inputVectors(
            solver_.sampleCurveTangents(curve_w, curve_l, num_samples));
    }

    bool setGuidingPaths()
    {
        bool success = solver_.interpolateVectorField();
        /* std::cout << "Interpolate guiding path = " << success << "\n" << std::flush; */

        return success;
    }

    bool addOrbitToSurfaceSamples(double width,
                                  double length,
                                  double height,
                                  double samples_per_unit_length,
                                  std::size_t max_num_samples)
    {
        /* std::cout << "::: Processing orbit at: (" << width << ", " << length << "),
         * height = " << height; */
        if (samples_per_unit_length <= 0) {
            /* std::cout << "-- samples per unit length <= 0\n"; */
            return false;
        }

        auto [curve_w, curve_l] = solver_.getOrbit(width, length);
        std::size_t num_samples = std::min(
            static_cast<std::size_t>(
                std::round(getCurveLength(curve_w, curve_l) / samples_per_unit_length)),
            max_num_samples);

        std::tie(curve_w, curve_l) =
            odeSolver2D::sampleCurve(curve_w, curve_l, num_samples);
        auto num_points = static_cast<std::size_t>(curve_w.size());

        /* std::cout << "samples/length = " << samples_per_unit_length << ", num_samples =
         * " << num_samples << ", actual num_points = " << num_points << '\n'; */
        if (num_points < 1) {
            /* std::cout << "-- num_points < 1\n"; */
            return false;
        }

        if (curve_w.size() != curve_l.size()) {
            /* std::cout << "-- curve_w.size() != curve_l.size()\n"; */
            return false;
        }

        for (size_t j = 0; j < curve_l.size(); ++j) {
            surface_points_.push_back(curve_w[j]);
            surface_points_.push_back(height);
            surface_points_.push_back(curve_l[j]);
        }
        /* std::cout << "Added " << curve_l.size() << " points to surface.\n"; */

        return true;
    }

    std::vector<double> getSurfaceSamples()
    {
        /* std::cout << "Surface contains " << surface_points_.size() / 3.0 << " points.\n"; */
        return surface_points_;
    }

    odeSolver2D& getSolver() { return solver_; }

   private:
    odeSolver2D solver_;
    std::vector<double> surface_points_;

    odeSolver2D::Curve convertCurve(const std::vector<double>& input_curve)
    {
        size_t num_points = input_curve.size() / 2;
        std::vector<double> curve_x(num_points);
        std::vector<double> curve_y(num_points);

        for (size_t i = 0; i < num_points; ++i) {
            curve_x[i] = input_curve[2 * i + 0];
            curve_y[i] = input_curve[2 * i + 1];
            /* std::cout << "::: Adding point: (" << path_w[i] << ", " << path_l[i] <<
             * ")\n"; */
        }

        return std::make_tuple(curve_x, curve_y);
    }

    double getCurveLength(const odeSolver2D::Curve& curve)
    {
        auto [xcoords, ycoords] = curve;
        return getCurveLength(xcoords, ycoords);
    }

    double getCurveLength(const std::vector<double>& xcoords,
                          const std::vector<double>& ycoords)
    {
        double len = 0.0;
        auto numel = xcoords.size();
        if ((numel < 2) || (xcoords.size() != ycoords.size())) {
            return len;
        }

        auto dist = [](const odeSolver2D::XCoordinates& xcoords,
                       const odeSolver2D::YCoordinates& ycoords,
                       size_t i0,
                       size_t i1) -> double {
            double x = xcoords[i0] - xcoords[i1];
            double y = ycoords[i0] - ycoords[i1];
            double d = std::sqrt(x * x + y * y);

            return d;
        };

        for (size_t i = 0; i < numel - 1; ++i) {
            len += dist(xcoords, ycoords, i, i + 1);
        }

        return len;
    }
};

#endif
