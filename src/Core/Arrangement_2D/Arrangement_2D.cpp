/*
 * Arrangement2D.cpp
 *
 *  Created on: Oct 4, 2016
 *      Author: felipe
 */

#include "Arrangement_2D.hpp"


namespace RRM{

    void Arrangement_2D::setBoundary(Real x, Real y, Real width, Real height)
    {
        Real  sampling = 7.0;

        this->anchor_[0] = x;
        this->anchor_[1] = y;
        this->extension_[0] = width;
        this->extension_[1] = height;

        // New boundary means a new Arrangement
        this->curve_cache.clear();

        Curve_cache  segment_1;
            segment_1.supporting_curve.add(Point2D(this->anchor_[0], this->extension_[1]));
            segment_1.supporting_curve.add(Point2D(this->extension_[0], this->extension_[1]));
            segment_1.supporting_curve.add(Point2D(this->extension_[0], this->anchor_[1]));
            segment_1.supporting_curve.lineFilter(sampling, 3);
            segment_1.is_boundary_ = true;
            segment_1.subcurves.push_back(segment_1.supporting_curve);
            segment_1.index = std::numeric_limits<Natural>::max()-1;

            this->curve_cache[segment_1.index] = segment_1;

         Curve_cache  segment_2;
            segment_2.supporting_curve.add(Point2D(this->extension_[0], this->anchor_[1]));
            segment_2.supporting_curve.add(Point2D(this->anchor_[0], this->anchor_[1]));
            segment_2.supporting_curve.add(Point2D(this->anchor_[0], this->extension_[1]));
            segment_2.supporting_curve.lineFilter(sampling, 3);
            segment_2.is_boundary_ = true;
            segment_2.subcurves.push_back(segment_2.supporting_curve);
            segment_2.index = std::numeric_limits<Natural>::max()-2;

            this->curve_cache[segment_2.index] = segment_2;


            std::cout << "Boundary Created" <<  curve_cache.size() << std::endl;

            std::cout << "Boundary Created X" <<  x << std::endl;
            std::cout << "Boundary Created Y" <<  y << std::endl;
            std::cout << "Boundary Created W" <<  width << std::endl;
            std::cout << "Boundary Created H" <<  height << std::endl;

    }

    bool Arrangement_2D::insert ( const Curve2D& _c , Natural id )
    {
        Curve_cache new_curve;
        new_curve.index = id;
        new_curve.supporting_curve.setPoints ( _c.getPoints ( ) );
        new_curve.subcurves.push_back(new_curve.supporting_curve);

        std::vector<std::size_t> arr_curve_index;
        std::vector<Real>        arr_curve_alphas;
        std::vector<std::size_t> input_curve_index;
        std::vector<Real>        input_curve_alphas;
        std::vector<Point2D>     points;

        std::vector<Intersection_cache>      intersection_points;
        intersection_points.clear();

        std::map<std::size_t,std::vector<std::size_t> > curves;

        if ( !new_curve.supporting_curve.is_x_monotonic_curve(_c,curves) )
        {
            //std::cout << "Not x monotonic curve" << std::endl;
            return false;
        }

        // New Curve to be analysed
        // The Curve Arrangement
        for ( auto curve : curve_cache )
        {
            // Given on curve, which sub curve the input has intersect
            for ( std::size_t s_it = 0; s_it < curve.second.subcurves.size(); s_it++ )
            {
                // Grab all intersection and save into the intersection list
                if ( curve.second.subcurves[s_it].intersections ( new_curve.supporting_curve , arr_curve_index , arr_curve_alphas , input_curve_index , input_curve_alphas , points ) )
                {
                    for ( std::size_t it = 0; it < input_curve_index.size ( ); it++ )
                    {
                        if ( isAboveCurve ( curve.second.subcurves[s_it][arr_curve_index[it]] , curve.second.subcurves[s_it][arr_curve_index[it] + 1] , new_curve.supporting_curve[input_curve_index[it]] ) )
                        {
                            intersection_points.push_back(Intersection_cache(
                                    points[it],
                                    new_curve,
                                    input_curve_index[it],
                                    curve.second,
                                    s_it,
                                    arr_curve_index[it],
                                    RELATIVE_POSITION::Above
                            ));
                        }
                        else
                        {
                            intersection_points.push_back(Intersection_cache(
                                    points[it],
                                    new_curve,
                                    input_curve_index[it],
                                    curve.second,
                                    s_it,
                                    arr_curve_index[it],
                                    RELATIVE_POSITION::Below
                            ));

                        }
                    }
                }

                arr_curve_index.clear ( );;
                arr_curve_alphas.clear ( );;
                input_curve_index.clear ( );;
                input_curve_alphas.clear ( );;
                points.clear ( );
            }
        }

        if ( intersection_points.size ( ) == 1 )
        {
            std::cout << "One Intersection" << std::endl;
            return false;
        }

        std::sort(intersection_points.begin(),intersection_points.end());

        if ( intersection_points.size ( ) >= 2 )
        {
    //        std::cout << "-- intersection_points -- " << intersection_points.size() << std::endl;
    //
    //        for( auto point: intersection_points)
    //        {
    //            std::cout << "-- Points --" << std::endl;
    //            std::cout << "--: " <<  point.point_.x()      << std::endl;
    //            std::cout << "--: " <<  point.input_curve_.index   << std::endl;
    //            std::cout << "--: " <<  point.input_curve_index_   << std::endl;
    //            std::cout << "--: " <<  point.arr_subcurve_index_  << std::endl;
    //            if (point.where_ == RELATIVE_POSITION::Above)
    //            {
    //                std::cout << "Above" << std::endl;
    //            }else
    //            {
    //                std::cout << "Below" << std::endl;
    //            }
    //            std::cout << "------------" << std::endl;
    //        }

            upper_bound_.clear();
            lower_bound_.clear();


                 // Out/In  --- In/Out
                 // The desired segment lies In<->In

            if (intersection_points[0].arr_curve_.index < std::numeric_limits<Natural>::max()-2)
            {
                if ( (intersection_points[0].where_ == RELATIVE_POSITION::Above)  )
                {
                    lower_bound_.push_back(intersection_points[0].arr_curve_.index);
                }else
                {
                    upper_bound_.push_back(intersection_points[0].arr_curve_.index);
                }
            }
            if (intersection_points[1].arr_curve_.index < std::numeric_limits<Natural>::max()-2)
            {
                if ( intersection_points[1].where_ == RELATIVE_POSITION::Above )
                {
                    upper_bound_.push_back(intersection_points[1].arr_curve_.index);
                }else
                {
                    lower_bound_.push_back(intersection_points[1].arr_curve_.index);
                }
            }


        }

        last_curve_ = new_curve;

        this->curve_cache[new_curve.index] = new_curve;

        std::cout << " Last Curve Log " << std::endl;
        std::cout << " Index::   " <<  last_curve_.index << std::endl;
        std::cout << " Below   " <<  lower_bound_.size() << std::endl;
        for(auto c: lower_bound_)
        {
            std::cout << c << std::endl;
        }
        std::cout << " Above   " <<  upper_bound_.size() << std::endl;
        for(auto c: upper_bound_)
        {
                std::cout << c << std::endl;
        }

        return true;
    }

    void Arrangement_2D::clear()
    {
        this->curve_cache.clear();
        this->setBoundary(this->anchor_[0],this->anchor_[1],this->extension_[0],this->extension_[1]);
    }

    void Arrangement_2D::updateSubcurves(const std::map<Natural,std::vector<Curve2D>>& _updated_curves)
    {
        this->clear();

        for( auto& new_curves: _updated_curves )
        {
            for ( auto curve : new_curves.second )
            {
                this->curve_cache[new_curves.first].subcurves.push_back(PolyCurve2D(curve));
                this->curve_cache[new_curves.first].index = new_curves.first;
            }
        }

        this->log();
    }
    // RRM interface
    std::vector<Natural>Arrangement_2D::getLastCurveLowerBound() const
    {
        return lower_bound_;
    }

    std::vector<Natural>Arrangement_2D::getLastCurveUpperBound() const
    {
        return upper_bound_;
    }

    bool Arrangement_2D::isAboveCurve ( Point2D _p1 , Point2D _p2 , Point2D _q1 )
    {
        PolyCurve2D p;
        return (p.CCW(_p1,_p2,_q1) >= 0 );
    }

    void Arrangement_2D::log ( )
    {
        std::cout << " -- Arrangement Log -- " << std::endl;
        std::cout << " -- size -- " << curve_cache.size() << std::endl;
    }

}


