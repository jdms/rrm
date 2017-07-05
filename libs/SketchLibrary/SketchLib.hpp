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


/*
 * SketchLib.hpp
 *
 *  Created on: Oct 11, 2016
 *      Author: felipe
 */

#ifndef _SKETCHLIB_HPP_
#define _SKETCHLIB_HPP_

#include "./src/Core/Geometry/PolygonalCurve/polygonal_curve_2d.hpp"
#include <vector>


namespace RRM
{
    // \brief Tentative class to handle input sketches. Meanwhile, it provides functions
    // to perform naive over sketch and filters to smooth hand drawn strokes.
    class SketchLib
    {
        public:

            using Natural = unsigned long long int;

             SketchLib ( ) = default;
            ~SketchLib ( ) = default;

            // \brief Naive implementation of over sketching
            //  Edit a curve by over sketching it with a given stroke.
            // \param _curve  Curve to be edit. If the curve is empty, return the stroke itself.
            // \param _stroke The piece of curve used to over sketching.
            bool overSketching( Curve2D& _curve, const Curve2D& _stroke );

            /// \brief Return all x_monotone sub curve of the curve.
            /// Return all x-monotone sub-curve of the given curve.
            /// \param _curve. The curve to be analysis
            /// \param _x_monotone_subcurves. The list of all x_monotone sub curve.
            ///
            /// \return true  if the curve is x_monotone
            ///         false if the curve is not x_monotone and list of x_monotone sub curves
            bool is_x_monotonic_curve ( const Curve2D& _curve, std::map<std::size_t,std::vector<std::size_t> >& x_monotone_subcurves ) const;

            /// \brief Ensure x-monotonicity of a given curve.
            /// Since the could not be necessarily x-monotone, this function ensure
            /// a path that is x-monotone in the entire sketch.
            /// \param _curve. The curve to be analysis
            bool ensure_x_monotonicity ( Curve2D& _curve ) const;

            // Default LineString Input
            private:
                // Standard smooth filter
                void defaultFilter(Curve2D& _curve);

    };

} /* namespace RRM */

#endif /* SRC_BASE_SKETCHLIB_SKETCHLIB_HPP_ */
