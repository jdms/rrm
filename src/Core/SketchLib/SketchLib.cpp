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

    bool SketchLib::ensure_x_monotonicity ( Curve2D& _curve )
    {
        std::vector<std::size_t> left_to_right_x_monotone_subcurves;
        std::vector<std::size_t> right_to_left_x_monotone_subcurves;
        std::map<std::size_t,std::vector<std::size_t> > x_monotone_subcurves;

        bool result = this->is_x_monotonic_curve(_curve,
                                                 x_monotone_subcurves,
                                                 left_to_right_x_monotone_subcurves,
                                                 right_to_left_x_monotone_subcurves
                                                 );

        Curve2D tmp;

        for (auto it: left_to_right_x_monotone_subcurves)
        {
            tmp.add(_curve[it]);
        }

        _curve = tmp;

        return result;

    }

    bool SketchLib::is_x_monotonic_curve ( const Curve2D&   _curve,
           std::map<std::size_t,std::vector<std::size_t> >& _x_monotone_subcurves,
           std::vector<std::size_t>&                        _left_to_right_x_monotone_subcurves,
           std::vector<std::size_t>&                        _right_to_left_x_monotone_subcurves ) const
    {
        std::size_t current_index = 0;

        _x_monotone_subcurves.clear();
        _left_to_right_x_monotone_subcurves.clear();
        _right_to_left_x_monotone_subcurves.clear();

        if ( _curve.size() <= 2)
        {
            return false;
        }

        // is oriented left to right
        bool is_current_segment = _curve[1].x() >= _curve[0].x();
        is_current_segment      = !is_current_segment;

        // Help in alternate between x_monotone_segments orientation
        for ( std::size_t it = 1; it < _curve.size ( ); it++ )
        {
            if (  ( _curve[it].x( ) >= _curve[it - 1].x( ) )   xor ( is_current_segment ) )
            {
                _x_monotone_subcurves[current_index].push_back (it - 1);

                if ( _curve[it].x( ) >= _curve[it - 1].x( ) )
                {
                    _left_to_right_x_monotone_subcurves.push_back(it - 1);
                }else
                {
                    _right_to_left_x_monotone_subcurves.push_back(it - 1);
                }

            }
            else
            {

                _left_to_right_x_monotone_subcurves.push_back(it-1);
                _right_to_left_x_monotone_subcurves.push_back(it-1);

                _x_monotone_subcurves[current_index].push_back (it-1);

                is_current_segment = !is_current_segment;
                current_index = it;
            }
        }
        // current_index == 0 means the curve is x_monotone
        return ( current_index == 0 );
    }

} /* namespace RRM */



