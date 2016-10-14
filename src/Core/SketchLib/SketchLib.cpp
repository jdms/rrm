/*
 * SketchLib.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: felipe
 */

#include "SketchLib.hpp"

namespace RRM
{

    Curve2D SketchLib::getCurrentSketch( ) const
    {
        return this->current_supporting_curve_;
    }

    void SketchLib::clear ( )
    {
        this->current_supporting_curve_.clear();
        this->previous_supporting_curve_.clear();
        this->swap_curve_.clear();
    }

    void SketchLib::reset ( )
    {
        this->current_supporting_curve_.clear();
    }

    void SketchLib::undo ( )
    {
        this->swap_curve_ = this->current_supporting_curve_;
        this->current_supporting_curve_ = previous_supporting_curve_;
    }

    void SketchLib::redo ( )
    {
        this->current_supporting_curve_ = this->swap_curve_;
    }

    void SketchLib::beginSketch ( Curve2D& _curve )
    {
        this->reset();
        this->current_supporting_curve_ = _curve;
    }

    void SketchLib::beginSketch (  )
    {
        this->reset();
    }

    bool SketchLib::addStroke ( const Curve2D& _curve )
    {
        Curve2D simplify;
        Curve2D stroke;
        Curve2D rest;

        if ( this->current_supporting_curve_.size() == 0 )
        {
            this->current_supporting_curve_.join(_curve);
        }
        else
        {
            stroke = _curve;

            previous_supporting_curve_ = current_supporting_curve_;

            current_supporting_curve_ = stroke.overSketch( current_supporting_curve_, rest, 1 , 16 );

            current_supporting_curve_.douglasPeuckerSimplify(simplify, 1.0);

            current_supporting_curve_ = simplify;

        }

        std::cout << this->current_supporting_curve_.size() << std::endl;

        return true;
    }

    void SketchLib::defaultFilter ( )
    {

    }

} /* namespace RRM */

