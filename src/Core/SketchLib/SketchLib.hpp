/*
 * SketchLib.hpp
 *
 *  Created on: Oct 11, 2016
 *      Author: felipe
 */

#ifndef _SKETCHLIB_HPP_
#define _SKETCHLIB_HPP_

#include "Geometry/PolygonalCurve/PolygonalCurve2D.hpp"
#include <vector>

namespace RRM
{
    class SketchLib
    {
        public:

            using Natural = unsigned long long int;

            SketchLib  ( );
            ~SketchLib ( ) = default;

            // Default Sketch Path Input
            void clear( );
                void beginSketch( const Curve2D& curve );
                void undo( );
                void redo( );
                void endSketch( );
            // Default LineString Input
                void addStroke( const Curve2D& _point, double _tolerance );

                // Using Douglas Peucker Simplify
                void pointRedution(const Curve2D& _curve,const double tolerance);
                void subDivision();

            private:
                // Reduce the frequency of the input path
                void defaultFilter();

                Curve2D                 main_sketch_curve_;
                std::vector<Curve2D>    main_sketch_subcurves_;
                Natural                 last_sub_curves_;
                Natural                 current_sub_curves_;
                Curve2D                 stroke_path_;
    };

} /* namespace RRM */

#endif /* SRC_BASE_SKETCHLIB_SKETCHLIB_HPP_ */
