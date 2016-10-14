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
    class SketchLib
    {
        public:

            using Natural = unsigned long long int;

             SketchLib ( ) = default;
            ~SketchLib ( ) = default;

            Curve2D getCurrentSketch() const;

            // Default Sketch Path Input
            void clear( );
            void reset( );
                void beginSketch( Curve2D& _curve );
                void beginSketch( );
                void undo( );
                void redo( );
            // Default LineString Input
                bool addStroke( const Curve2D& _curve );

            private:
                // Reduce the frequency of the input path
                void defaultFilter();

                Curve2D                 current_supporting_curve_;
                Curve2D                 swap_curve_;
                Curve2D                 previous_supporting_curve_;

    };

} /* namespace RRM */

#endif /* SRC_BASE_SKETCHLIB_SKETCHLIB_HPP_ */
