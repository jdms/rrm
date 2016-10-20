/*
 * SketchLib.hpp
 *
 *  Created on: Oct 11, 2016
 *      Author: felipe
 */

#ifndef _SKETCHLIB_HPP_
#define _SKETCHLIB_HPP_

#include "Core/Geometry/PolygonalCurve/PolygonalCurve2D.hpp"
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

            // \brief Naive implementation of over sketch
            // Edit a curve which is result of the over sketch with a given stroke.
            // \param _curve  Curve to be edit. If the curve is empty, append the stroke to it.
            // \param _stroke The piece of curve used to over sketch.
            bool overSketching( Curve2D& _curve, const Curve2D& _stroke );

            bool ensure_x_monotonicity ( Curve2D& _curve );

            bool is_x_monotonic_curve ( const Curve2D&              _curve,
                   std::map<std::size_t,std::vector<std::size_t> >& _x_monotone_subcurves,
                   std::vector<std::size_t>&                        _left_to_right_x_monotone_subcurves,
                   std::vector<std::size_t>&                        _right_to_left_x_monotone_subcurves
                   ) const;

            // Default LineString Input
            private:
                // Standard smooth filter
                void defaultFilter(Curve2D& _curve);

    };

} /* namespace RRM */

#endif /* SRC_BASE_SKETCHLIB_SKETCHLIB_HPP_ */
