/*
 * SketchLib.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: felipe
 */

#include "SketchLib.hpp"

namespace RRM
{


    bool SketchLib::overSketching ( Curve2D& _curve , const Curve2D& _stroke)
    {
        Curve2D simplify;
        Curve2D stroke;
        Curve2D rest;

        if ( _stroke.size() < 7)
        {
            return false;
        }

        if ( _curve.size() == 0 )
        {
            _curve.join(_stroke);
        }
        else
        {
            stroke = _stroke;

            _curve = stroke.overSketch( _curve, rest, 1 , 16 );

            this->defaultFilter(_curve);
        }
       // std::cout << this->current_supporting_curve_.size() << std::endl;

        return true;
    }

    void SketchLib::defaultFilter ( Curve2D& _curve )
    {

        Curve2D tmp = _curve;
        tmp.douglasPeuckerSimplify(_curve, 1.0);

    }

} /* namespace RRM */

