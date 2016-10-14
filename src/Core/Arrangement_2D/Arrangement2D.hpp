/*
 * Arrangement2D.hpp
 *
 *  Created on: Oct 4, 2016
 *      Author: felipe
 */

#ifndef SRC_ARRANGEMENT2D_HPP_
#define SRC_ARRANGEMENT2D_HPP_

#include <set>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iterator>

#include "../Arrangement_2D/PolygonalCurve2.hpp"
#include "Geometry/PolygonalCurve/PolygonalCurve2D.hpp"




typedef unsigned long long Natural;
typedef double Real;
typedef RRM::PolygonalCurve2 PolyCurve2D;


enum class RELATIVE_POSITION
{
        Above,
        Below,
        Undefined
};

class Curve_cache
{
    public:

        Curve_cache()
        {
            this->index = 0;
            this->is_boundary_ = false;
        }

        Curve_cache( const Curve_cache& other)
        {
            this->index            = other.index;
            this->is_boundary_     = other.is_boundary_;
            this->supporting_curve = other.supporting_curve;
            this->subcurves        = other.subcurves;
        }

        /// Curve id, the point itself, the Relative Position
        bool operator<(const Curve_cache& other) const
        {
            return (this->index < other.index);
        }
        /// Lexicographic Order
        Natural                              index;
        bool                                 is_boundary_;
        PolyCurve2D                          supporting_curve;
        std::vector<PolyCurve2D>             subcurves;

};

class Intersection_cache
{
    public:
        Intersection_cache ( )
        {
            this->point_              = Point2D(0.0,0.0);
            this->input_curve_index_  = 0;
            this->arr_subcurve_index_ = 0;
            this->arr_index_          = 0;
            this->where_              = RELATIVE_POSITION::Undefined;
        }

        Intersection_cache(Point2D              _point,
                           Curve_cache          _input_curve,
                           std::size_t          _input_curve_index_,
                           Curve_cache          _arr_curve,
                           std::size_t          _arr_subcurve_index,
                           std::size_t          _arr_index,
                           RELATIVE_POSITION    _where )
        {
            this->point_              = _point;
            this->input_curve_        = _input_curve;
            this->input_curve_index_  = _input_curve_index_;
            this->arr_curve_          = _arr_curve;
            this->arr_subcurve_index_ = _arr_subcurve_index;
            this->arr_index_          = _arr_index;
            this->where_              = _where;
        }

        Intersection_cache(const Intersection_cache& other)
        {
            this->point_              = other.point_;
            this->input_curve_        = other.input_curve_;
            this->input_curve_index_  = other.input_curve_index_;
            this->arr_curve_          = other.arr_curve_;
            this->arr_subcurve_index_ = other.arr_subcurve_index_;
            this->arr_index_          = other.arr_index_;
            this->where_              = other.where_;
        }

        /// Sort based in the Input curve indices
        bool operator<(const Intersection_cache& other ) const
        {
            return  ( input_curve_index_ < other.input_curve_index_ );
        }

        /// Intersection Point
        Point2D point_;
        /// input curve;
        Curve_cache input_curve_;
            /// Point where the intersection event occur into input curve
            std::size_t input_curve_index_;
        /// curve into the arrangement
        Curve_cache arr_curve_;
            /// Index of the sub curve of the supporting curve in the arrangement
            std::size_t arr_subcurve_index_;
            /// Index of the point where the intersection event occur into the curve in the arrangement
            std::size_t arr_index_;
        /// Which side of the curve the intersection the event was triggered
        RELATIVE_POSITION where_;
};

class Arrangement2D
{
    public:

        Arrangement2D ( ) = default;

        ~Arrangement2D ( ) = default;

        void boundary( Real x, Real y, Real width, Real height);

        bool insert ( const Curve2D &c , Natural id );

        std::vector<Natural> getLastCurveUpperBound ( ) const;
        std::vector<Natural> getLastCurveLowerBound ( ) const;

        Natural lastInsertedCurve ( ) const;

        void updateSubcurves(const std::map<Natural,std::vector<Curve2D>>& _updated_curves);

    private:

        bool isAboveCurve(Point2D p1, Point2D p2, Point2D q1);

        Natural last_curve_;

        std::vector<Natural> upper_bound_;
        std::vector<Natural> lower_bound_;

        std::tuple<Natural,std::vector<Natural>,std::vector<Natural>> undo;
        std::tuple<Natural,std::vector<Natural>,std::vector<Natural>> redo;

        std::map<Natural,Curve_cache>        curve_cache;

        void log();
};

#endif /* SRC_ARRANGEMENT2D_HPP_ */

