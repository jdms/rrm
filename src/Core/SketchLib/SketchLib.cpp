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

    /// \brief Ensure x_monotonicity of the curve
    /// Modify the curve, thus is is granted to be one x monotone curve
    /// \param Curve to be filters
    void SketchLib::ensure_x_monotonicity ( Curve2D& _curve ) const
    {
        std::map<std::size_t,std::vector<std::size_t> > x_monotone_subcurves;

        std::vector<std::size_t> lr;
        std::vector<std::size_t> rl;

        this->is_x_monotonic_curve(_curve,x_monotone_subcurves,lr,rl);

        // The total amount of subcurves which is oriented left to right
        std::size_t left_to_right = 0;
        // The total amount of subcurves which is oriented right to left
        std::size_t right_to_left = 0;
        // The result Curve
        Curve2D     result_curve;
        // All points of the subsequent sub cuves have to be larger/smaller then
        // the target_point.x()
        Point2D     target_point;
        // Use to update the target_point
        std::size_t current_size = 0;


        // Find the large orientation of the curve
        for ( auto subcurve : x_monotone_subcurves )
        {
            if ( _curve[subcurve.second[1]].x() > _curve[subcurve.second[0]].x()  )
            {
               left_to_right += subcurve.second.size();
            }else
            {
               right_to_left += subcurve.second.size();
            }
        }

        target_point      =  _curve[x_monotone_subcurves.begin()->second.front()];
        current_size      = result_curve.size();

        // Grab all left to right segments
        if ( left_to_right > right_to_left )
        {
            for ( auto subcurve : x_monotone_subcurves )
            {
                if ( _curve[subcurve.second[1]].x() > _curve[subcurve.second[0]].x()  )
                {
                    for(auto it : subcurve.second)
                    {
                        if ( _curve[it].x() > target_point.x() )
                        {
                            result_curve.add((_curve[it]));
                        }
                    }

                    if ( result_curve.size() > current_size )
                    {
                        target_point =  _curve[subcurve.second.back()];
                        current_size += result_curve.size();
                    }
                }

            }

        }
        // Grab all right to left segments
        else
        {
            for ( auto subcurve : x_monotone_subcurves )
            {
                if ( ! (_curve[subcurve.second[1]].x() > _curve[subcurve.second[0]].x())  )
                {
                    for(auto it : subcurve.second)
                    {
                        if ( !(_curve[it].x() > target_point.x()) )
                        {
                            result_curve.add((_curve[it]));
                        }
                    }

                    if ( result_curve.size() > current_size )
                    {
                        target_point =  _curve[subcurve.second.back()];
                        current_size += result_curve.size();
                    }
                }

            }
        }

        _curve = result_curve;

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
            if (  ( _curve[it].x( ) >= _curve[it - 1].x( ) ) ^ ( is_current_segment ) )
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



