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

#ifndef RRM_CORE_MODELS_ODE_SOLVER_2D
#define RRM_CORE_MODELS_ODE_SOLVER_2D

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <tuple>

#include "odeint_wrapper/boost_odeint_wrapper.hpp"

#include "nspline/nspline.h"
#include "planin/vector_field/div_free_vector_field_2d.hpp"

class odeSolver2D {
   public:
    using Coordinates = std::vector<double>;
    using XCoordinates = Coordinates;
    using YCoordinates = Coordinates;

    using Curve = std::tuple<XCoordinates, YCoordinates>;
    using Tangents = std::tuple<XCoordinates, YCoordinates>;

    odeSolver2D() = default;
    ~odeSolver2D() = default;
    odeSolver2D(odeSolver2D&&) = default;
    odeSolver2D& operator=(odeSolver2D&&) = default;

    void setDomainOrigin(double origin_x, double origin_y)
    {
        origin_x_ = origin_x;
        origin_y_ = origin_y;
    }

    bool setDomainSize(double size_x, double size_y)
    {
        constexpr double min_tol = 1E-10;
        if (size_x <= min_tol || size_y <= min_tol) {
            return false;
        }

        size_x_ = size_x;
        size_y_ = size_y;

        return true;
    }

    bool setInterpolantBasisScale(double scale)
    {
        if (scale <= 0) {
            return false;
        }

        scale_ = scale;
        flux_ptr_->setBasisFunctionScale(scale_);
        /* flux_ptr_ = std::make_unique<DivFreeVectorField2D>(VFKernel(scale_), 4); */

        return true;
    }

    bool inputVectors(const std::tuple<Curve, Tangents>& input)
    {
        return inputVectors(const_cast<Curve&>(std::get<0>(input)),
                            const_cast<Tangents&>(std::get<1>(input)));
    }

    bool inputVectors(const Curve& curve, const Tangents& tangents)
    {
        return inputVectors(const_cast<XCoordinates&>(std::get<0>(curve)),
                            const_cast<YCoordinates&>(std::get<1>(curve)),
                            const_cast<XCoordinates&>(std::get<0>(tangents)),
                            const_cast<YCoordinates&>(std::get<1>(tangents)));
    }

    bool inputVectors(const XCoordinates& curve_x,
                      const YCoordinates& curve_y,
                      const XCoordinates& curve_dx,
                      const YCoordinates& curve_dy)
    {
        // Number of points must match
        if (curve_x.size() != curve_y.size()) {
            return false;
        }

        if (curve_x.size() != curve_dx.size()) {
            return false;
        }

        if (curve_dx.size() != curve_dy.size()) {
            return false;
        }

        /* std::cout << "#curve_x = " << curve_x.size() << "\n" << std::flush; */

        // Expects at least one sample
        auto num_points = static_cast<long int>(curve_x.size());
        if (num_points < 1) {
            return false;
        }

        // Try to add points and tangents to interpolant
        double ti, xi, yi, dxi, dyi;
        /* double tolerance = 1E-4; */
        bool success = false;
        double ratio =
            static_cast<double>(num_points - 1) / static_cast<double>(num_points - 1);

        for (long int i = 0; i < num_points; ++i) {
            xi = curve_x[i];
            yi = curve_y[i];
            dxi = curve_dx[i];
            dyi = curve_dy[i];

            success |= flux_ptr_->addVectorEvaluation({xi, yi}, {dxi, dyi});
        }

        /* std::cout << "Got tangents = " << success << "\n" << std::flush; */
        return success;
    }

    bool inputVectors(const XCoordinates& curve_x,
                      const YCoordinates& curve_y,
                      long int num_samples)
    {
        // Number of points must match
        if (curve_x.size() != curve_y.size()) {
            return false;
        }

        // Needs at least 2 points to define a vector
        auto num_points = curve_x.size();
        if (num_points < 2) {
            return false;
        }

        // If num_samples < 1 then get as many samples as there are points in curve_x
        if (num_samples < 1) {
            num_samples = num_points;
        }

        // Linearly interpolate parameter
        std::vector<double> time;
        time.resize(num_points);
        for (size_t i = 0; i < num_points; ++i) {
            time[i] = static_cast<double>(i);
        }

        // Interpolate coordinates with natural splines
        // Domain of curves is [0, num_points-1]
        NSpline x(time, curve_x);
        NSpline y(time, curve_y);

        // Get tangents from interpolant
        // Samples are drawn from (0, num_points-1)
        double ti, xi, yi, dxi, dyi, norm_ddt;
        double tolerance = 1E-4;
        bool success = false;
        double ratio =
            static_cast<double>(num_points - 1) / static_cast<double>(num_samples - 1);

        for (long int i = 0; i < num_samples; ++i) {
            ti = static_cast<double>(i) * ratio;
            xi = x(ti);
            yi = y(ti);
            dxi = x.D(ti);
            dyi = y.D(ti);

            norm_ddt = norm(dxi, dyi);
            if (norm_ddt > tolerance) {
                dxi = dxi / norm_ddt;
                dyi = dyi / norm_ddt;
                success |= flux_ptr_->addVectorEvaluation({xi, yi}, {dxi, dyi});
            }
        }

        return success;
    }

    bool interpolateVectorField()
    {
        bool success = flux_ptr_->interpolate();

        return success;
    }

    Curve getOrbit(double x0, double y0)
    {
        /* std::cout << "::: Processing orbit at: (" << x0 << ", " << y0 << ")"; */
        if (!flux_ptr_->interpolantIsSet()) {
            /* std::cout << " -- interpolant is not set\n"; */
            return std::make_tuple(XCoordinates(), YCoordinates());
        }

        if (!lieInDomain({x0, y0})) {
            /* std::cout << "-- point lies outside of domain\n"; */
            return std::make_tuple(XCoordinates(), YCoordinates());
        }

        XCoordinates curve_x;
        YCoordinates curve_y;
        /* std::cout << "::: Getting orbit at: (" << x0 << ", " << y0 << ")"; */

        odeintWrapper<DivFreeVectorField2D, State> solver(*flux_ptr_);
        State p0 = {x0, y0};
        double time0 = 0;
        double time1 = scale_;

        // Integrate from time0 to -time1
        {
            auto [states, times] = solver.integrate(p0, time0, -time1);
            truncateOrbit(states, p0);
            size_t num_states = states.size();
            for (size_t i = 0; i < states.size(); ++i) {
                auto cur_state = states.size() - i - 1;
                /* if (!lieInDomain(states[num_states - i - 1])) */
                /*     continue; */

                curve_x.push_back(states[cur_state][0]);
                curve_y.push_back(states[cur_state][1]);

                if (i == states.size() - 2) {
                    /* std::cout << " <-- " << "(" << curve_x.back() << ", " <<
                     * curve_y.back() << ")"; */
                }
            }
        }

        // Integrate from time0 to time1
        {
            auto [states, times] = solver.integrate(p0, time0, time1);
            truncateOrbit(states, p0);
            for (size_t i = 1; i < states.size(); ++i) {
                curve_x.push_back(states[i][0]);
                curve_y.push_back(states[i][1]);

                if (i == 1) {
                    /* std::cout << " --> " << "(" << curve_x.back() << ", " <<
                     * curve_y.back() << ")"; */
                }

                /* if (!lieInDomain(states[i])) */
                /*     break; */
            }
        }

        /* std::cout << "-- got " << curve_x.size() << "points\n"; */

        return std::make_tuple(curve_x, curve_y);
    }

    static Curve sampleCurve(const Curve& curve, long int num_samples)
    {
        return sampleCurve(const_cast<XCoordinates&>(std::get<0>(curve)),
                           const_cast<YCoordinates&>(std::get<1>(curve)),
                           num_samples);
    }

    static Curve sampleCurve(const XCoordinates& curve_x,
                             YCoordinates& curve_y,
                             long int num_samples)
    {
        if (curve_x.empty()) {
            return std::make_tuple(XCoordinates(), YCoordinates());
        }

        if (curve_x.size() != curve_y.size()) {
            return std::make_tuple(XCoordinates(), YCoordinates());
        }

        if (num_samples < 1) {
            return std::make_tuple(XCoordinates(), YCoordinates());
        }

        if (num_samples == 1) {
            return std::make_tuple(XCoordinates({curve_x.front()}),
                                   YCoordinates({curve_y.front()}));
        }

        // Interpolate coordinates with natural splines
        // Domain of curves is [0, num_points-1]
        size_t num_points = curve_x.size();
        std::vector<double> t(num_points);
        double n = 0;
        for (size_t i = 0; i < num_points; ++i) {
            t[i] = static_cast<double>(i);
        }

        NSpline x(t, curve_x);
        NSpline y(t, curve_y);

        // Resample orbit
        // Samples are drawn from [0, num_points-1]
        XCoordinates xcoords(num_samples);
        YCoordinates ycoords(num_samples);
        double ratio =
            static_cast<double>(num_points - 1) / static_cast<double>(num_samples - 1);

        for (long int i = 0; i < num_samples; ++i) {
            double ti = static_cast<double>(i) * ratio;
            xcoords[i] = x(ti);
            ycoords[i] = y(ti);
        }

        return std::make_tuple(xcoords, ycoords);
    }

    static std::tuple<Curve, Tangents> sampleCurveTangents(const Curve& curve,
                                                           long int num_samples,
                                                           double tolerance = 1E-4)
    {
        return sampleCurveTangents(const_cast<XCoordinates&>(std::get<0>(curve)),
                                   const_cast<YCoordinates&>(std::get<1>(curve)),
                                   num_samples,
                                   tolerance);
    }

    static std::tuple<Curve, Tangents> sampleCurveTangents(const XCoordinates& curve_x,
                                                           YCoordinates& curve_y,
                                                           long int num_samples,
                                                           double tolerance = 1E-4)
    {
        // Output curve and tangents coordinates
        Coordinates xcoords, ycoords, dxcoords, dycoords;

        // Number of points must match
        if (curve_x.size() != curve_y.size()) {
            return std::make_tuple(std::make_tuple(xcoords, ycoords),
                                   std::make_tuple(dxcoords, dycoords));
        }

        // Needs at least 2 points to define a vector
        auto num_points = curve_x.size();
        if (num_points < 2) {
            return std::make_tuple(std::make_tuple(xcoords, ycoords),
                                   std::make_tuple(dxcoords, dycoords));
        }

        xcoords.reserve(num_samples);
        ycoords.reserve(num_samples);
        dxcoords.reserve(num_samples);
        dycoords.reserve(num_samples);

        // If num_samples < 1 then get as many samples as there are points in curve_x
        if (num_samples < 1) {
            num_samples = num_points;
        }

        // Linearly interpolate parameter
        std::vector<double> time;
        time.resize(num_points);
        for (size_t i = 0; i < num_points; ++i) {
            time[i] = static_cast<double>(i);
        }

        // Interpolate coordinates with natural splines
        // Domain of curves is [0, num_points-1]
        NSpline x(time, curve_x);
        NSpline y(time, curve_y);

        // Get tangents from interpolant
        // Samples are drawn from (0, num_points-1)
        double ti, xi, yi, dxi, dyi, norm_ddt;
        /* double tolerance = 1E-4; */
        bool success = false;
        double ratio =
            static_cast<double>(num_points - 1) / static_cast<double>(num_samples - 1);

        for (long int i = 0; i < num_samples; ++i) {
            ti = static_cast<double>(i) * ratio;
            xi = x(ti);
            yi = y(ti);
            dxi = x.D(ti);
            dyi = y.D(ti);

            norm_ddt = norm(dxi, dyi);
            if (norm_ddt > tolerance) {
                dxi = dxi / norm_ddt;
                dyi = dyi / norm_ddt;
                xcoords.push_back(xi);
                ycoords.push_back(yi);
                dxcoords.push_back(dxi);
                dycoords.push_back(dyi);
                /* std::cout << "::: Adding at point (i = " << ti << "): (" << xi <<
                   ", " << yi << "), tangent: " */
                /* << dxi << ", " << dyi << ")\n"; */
                /* success |= flux_.addVectorEvaluation( {xi, yi}, {dxi, dyi} ); */
            }
        }

        return std::make_tuple(std::make_tuple(xcoords, ycoords),
                               std::make_tuple(dxcoords, dycoords));
    }

   private:
    using State = std::array<double, 2>;
    using OrbitStates =
        typename odeintWrapper<DivFreeVectorField2D, State>::ObservedStates;

    std::unique_ptr<DivFreeVectorField2D> flux_ptr_ = std::make_unique<DivFreeVectorField2D>(VFKernel(), 4);
    /* DivFreeVectorField2D flux_; */
    double scale_ = 1.0;
    double origin_x_ = 0.0;
    double origin_y_ = 0.0;
    double size_x_ = 1.0;
    double size_y_ = 1.0;

    static double dist(const State& s0, const State& s1)
    {
        auto sqr = [](double t) -> double { return t * t; };
        return std::sqrt(sqr(s1[0] - s0[0]) + sqr(s1[1] - s0[1]));
    }

    static double norm(const State& s) { return std::sqrt(s[0] * s[0] + s[1] * s[1]); }

    static double norm(double x, double y) { return std::sqrt(x * x + y * y); }

    bool lieInDomain(double x, double y)
    {
        if ((x < origin_x_) || (x > origin_x_ + size_x_)) {
            return false;
        }

        if ((y < origin_y_) || (y > origin_y_ + size_y_)) {
            return false;
        }

        return true;
    }

    bool lieInDomain(const State& p)
    {
        if ((p[0] < origin_x_) || (p[0] > origin_x_ + size_x_)) {
            return false;
        }

        if ((p[1] < origin_y_) || (p[1] > origin_y_ + size_y_)) {
            return false;
        }

        return true;
    }

    bool lieInCircumscribingEllipsoid(const State& p)
    {
        double centre_x = origin_x_ + size_x_ / 2;
        double centre_y = origin_y_ + size_y_ / 2;
        double a = std::sqrt(2) * size_x_ / 2.0;
        double b = std::sqrt(2) * size_y_ / 2.0;

        double val = (p[0] - centre_x) * (p[0] - centre_x) / (a * a) +
                     (p[1] - centre_y) * (p[1] - centre_y) / (b * b);
        if (val > 1) {
            return false;
        }

        return true;
    }

    void truncateOrbit(OrbitStates& states, State& /* s0 */)
    {
        if (states.size() <= 2) {
            return;
        }

        /* double avg_dist, d, acc_dist = 0.0; */
        for (size_t i = 1; i < states.size(); ++i) {
            if (!lieInDomain(states[i])) {
                states.resize(i + 1);
                break;
            }

            /*     d = dist(states[i], s0); */
            /*     acc_dist += dist(states[i], s0); */
            /*     avg_dist = acc_dist/static_cast<double>(i); */
            /*     if (?) */
            /*     { */
            /* } */
        }

        return;
    }
};

#endif
