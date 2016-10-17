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

            // Default LineString Input
            private:
                // Standard smooth filter
                void defaultFilter(Curve2D& _curve);

    };

} /* namespace RRM */

#endif /* SRC_BASE_SKETCHLIB_SKETCHLIB_HPP_ */
