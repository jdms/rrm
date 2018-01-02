#ifndef __TRUNCATE_HELPER__
#define __TRUNCATE_HELPER__

#include <iostream>
#include <vector>
#include <map>

#include "./libs/PolygonalCurve/CurveN.hpp"
#include "stratmod_wrapper.hpp"

template<typename CurveType>
class TruncateHelper 
{
    public:
        using SurfacesIndices = std::vector<std::size_t>;
        using CurveTuple = std::tuple<CurveType, double>;

        TruncateHelper( const CurveTuple &curve_tuple, SModellerWrapper &m, SurfacesIndices &isurf ) : in_curve_(std::get<0>(curve_tuple)), modeller_( m )
        {
            curve_ = in_curve_;

            cross_section_ = std::get<1>(curve_tuple);

            intersected_surfaces_ = isurf;

            first_cross_ = false;
            second_cross_ = false;
        }

        bool getFirstRegionCurveIntersects( SurfacesIndices &lb, SurfacesIndices &ub );

        bool canTruncate()
        {
            return first_cross_ && second_cross_;
        }

        bool truncateCurve();

        CurveType getTruncatedCurve()
        {
            if (truncated_curve_.size() > 0)
            {
                return truncated_curve_;
            }

            Point2D p;
            for (size_t i = 0; i < left_part_.size(); ++i)
            {
                p = left_part_[i];
                truncated_curve_.add(p);
            }

            for (size_t i = first_cross_index_; i <= second_cross_index_; ++i)
            {
                p = curve_[i];
                truncated_curve_.add(p);
            }

            for (size_t i = 0; i < right_part_.size(); ++i)
            {
                p = right_part_[i];
                truncated_curve_.add(p);
            }

            return truncated_curve_;
        }

        std::vector<double> getTruncatedPoints2D()
        {
            std::vector<double> out;
            if (truncated_curve_.size() == 0)
            {
                getTruncatedCurve();
            }

            for (size_t i = 0; i < truncated_curve_.size(); ++i)
            {
                out.push_back(truncated_curve_[i].x());
                out.push_back(truncated_curve_[i].y());
            }

            return out;
        }

        std::vector<double> getTruncatedPoints3D()
        {
            std::vector<double> out;

            for (size_t i = 0; i < truncated_curve_.size(); ++i)
            {
                out.push_back(truncated_curve_[i].x());
                out.push_back(truncated_curve_[i].y());
                out.push_back(cross_section_);
            }

            return out;
        }

    private:
        SModellerWrapper &modeller_;

        const CurveType &in_curve_;
        CurveType curve_;
        double cross_section_;

        CurveType upper_curve_;
        CurveType lower_curve_;

        CurveType truncated_curve_, left_part_, right_part_;

        std::vector<size_t> upper_boundary_;
        std::vector<size_t> lower_boundary_;

        SurfacesIndices intersected_surfaces_;

        size_t first_cross_index_, second_cross_index_;
        bool first_cross_, second_cross_;

        struct { double x, y, z;  } origin_, model_size_;

        enum class CrossedBoundary { UPPER, LOWER, BOUNDING_BOX, INVALID };

        enum class CrossingType { UP_DOWN, DOWN_UP, OUT_IN, IN_OUT, INVALID };


        //
        // Methods
        //

        template<typename Point>
        bool truncatePoint( Point &p, const Point &bound, CrossedBoundary b, CrossingType t )
        {
            bool success = false;
            double displacement_ = 1.;

            if ( b == CrossedBoundary::UPPER )
            {
                if ( t == CrossingType::UP_DOWN )
                {
                    p.y() += displacement_;
                    // p.y() = std::max(p.y(), bound.y());
                    success = true;
                }
                else if ( t == CrossingType::DOWN_UP )
                {
                    p.y() += displacement_;
                    // p.y() = std::max(p.y(), bound.y());
                    success = true;

                }
            }

            else if ( b == CrossedBoundary::LOWER )
            {
                if ( t == CrossingType::UP_DOWN )
                {
                    p.y() -= displacement_;
                    // p.y() = std::min(p.y(), bound.y());
                    success = true;
                }
                else if ( t == CrossingType::DOWN_UP )
                {
                    p.y() -= displacement_;
                    // p.y() = std::min(p.y(), bound.y());
                    success = true;
                }
            }

            return success;
        }
        
        bool liesInsideBoundingBox( double x, double y, double z )
        {
            struct { double x, y, z; } origin, model_size;

            modeller_.getOrigin(origin.x, origin.y, origin.z);
            modeller_.getSize(model_size.x, model_size.y, model_size.z);

            bool liesInside = true;

            liesInside &= (x >= origin.x);
            liesInside &= (y >= origin.y);
            liesInside &= (z >= origin.z);

            liesInside &= (x <= origin.x + model_size.x);
            liesInside &= (y <= origin.y + model_size.y);
            liesInside &= (z <= origin.z + model_size.z);

            return liesInside;
        }

        bool reverseCurve( CurveType &c )
        {
            if ( c.size() > 1 )
            {
                auto p = c.atBegin();
                auto q = c.atEnd();

                if ( p.x() > q.x() )
                {
                    c.reverse();
                    return true;
                }
            }

            return false;
        }

		void getIntersectedBoundaries(SurfacesIndices &lb, SurfacesIndices &ub)
        {
            std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), lower_boundary_.begin(), lower_boundary_.end(), std::back_inserter(lb));
            std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), upper_boundary_.begin(), upper_boundary_.end(), std::back_inserter(ub));
        }

        void convertToCurveType(const std::vector<double> &in_curve, CurveType &curve)
        {
            curve.clear();
            Point2D p; 

            for ( size_t i = 0; i < in_curve.size(); i += 2 )
            {
                p = { { { in_curve[i], in_curve[i + 1] } } };
                curve.add(p);
            }
        }

        //CrossingType getCrossingType(size_t index)
        //{
        //	//auto region = u.getSurfacesIndicesBelowPoint(curve_[index].x(), curve_[index].y(), cross_section_);

        //	auto prev_lb = u.getSurfacesIndicesBelowPoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
        //	auto lb = u.getSurfacesIndicesBelowPoint(curve_[index].x(), curve_[index].y(), cross_section_);

        //	/* auto pub = u.getSurfacesIndicesAbovePoint(in_curve[index-1].x(), in_curve[index-1].y(), in_curve_depth); */
        //	/* auto ub = u.getSurfacesIndicesAbovePoint(in_curve[index].x(), in_curve[index].y(), in_curve_depth); */

        //	std::vector<size_t> sym_diff = {};
        //	std::set_symmetric_difference(prev_lb.begin(), prev_lb.end(), lb.begin(), lb.end(), std::back_inserter(sym_diff));

        //	bool points_upwards = std::includes(lb.begin(), lb.end(), sym_diff.begin(), sym_diff.end());
        //	// bool at_upper_boundary = std::includes(ubound.begin(), ubound.end(), sym_diff.begin(), sym_diff.end());

        //	if (points_upwards)
        //	{
        //		return CrossingType::UPWARDS;
        //	}
        //	else //if (!points_upwards)
        //	{
        //		return CrossingType::DOWNWARDS;
        //	}
        //}

        CrossedBoundary getCrossedBoundary(size_t index)
        {

            if (!liesInsideBoundingBox(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_))
            {
                return CrossedBoundary::BOUNDING_BOX;
            }

            if (!liesInsideBoundingBox(curve_[index].x(), curve_[index].y(), cross_section_))
            {
                return CrossedBoundary::BOUNDING_BOX;
            }

            std::vector<size_t> sym_diff = {};

            SUtilitiesWrapper u(modeller_);

            auto prev_ub = u.getSurfacesIndicesAbovePoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            auto ub		 = u.getSurfacesIndicesAbovePoint(curve_[index    ].x(), curve_[index    ].y(), cross_section_);
            std::set_symmetric_difference(prev_ub.begin(), prev_ub.end(), ub.begin(), ub.end(), std::back_inserter(sym_diff));

            bool at_upper_boundary = !sym_diff.empty() && std::includes(upper_boundary_.begin(), upper_boundary_.end(), sym_diff.begin(), sym_diff.end());

            if (at_upper_boundary)
            {
                return CrossedBoundary::UPPER;
            }

            sym_diff.clear();

            auto prev_lb = u.getSurfacesIndicesBelowPoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            auto lb = u.getSurfacesIndicesBelowPoint(curve_[index].x(), curve_[index].y(), cross_section_);
            std::set_symmetric_difference(prev_lb.begin(), prev_lb.end(), lb.begin(), lb.end(), std::back_inserter(sym_diff));

            bool at_lower_boundary = !sym_diff.empty() && std::includes(lower_boundary_.begin(), lower_boundary_.end(), sym_diff.begin(), sym_diff.end());

            if (at_lower_boundary)
            {
                return CrossedBoundary::LOWER;
            }

            return CrossedBoundary::INVALID;

        }

        CrossingType getCrossingType( size_t index )
        {
            bool inside_before_cross = liesInsideBoundingBox(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            bool inside_after_cross = liesInsideBoundingBox(curve_[index].x(), curve_[index].y(), cross_section_);

            if ( !inside_before_cross && !inside_after_cross )
            {
                return CrossingType::INVALID;
            }

            if ( !inside_before_cross && inside_after_cross )
            {
                return CrossingType::OUT_IN;
            }

            if ( inside_before_cross && !inside_after_cross )
            {
                return CrossingType::IN_OUT;
            }

            std::vector<size_t> sym_diff = {};

            SUtilitiesWrapper u(modeller_);

            auto prev_ub = u.getSurfacesIndicesAbovePoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            auto ub      = u.getSurfacesIndicesAbovePoint(curve_[index    ].x(), curve_[index    ].y(), cross_section_);
            /* std::set_symmetric_difference(prev_ub.begin(), prev_ub.end(), ub.begin(), ub.end(), std::back_inserter(sym_diff)); */

            bool up_down = std::includes(prev_ub.begin(), prev_ub.end(), ub.begin(), ub.end());
            if ( up_down )
            {
                return CrossingType::UP_DOWN;
            }

            bool down_up = std::includes(ub.begin(), ub.end(), prev_ub.begin(), prev_ub.end());
            if ( down_up )
            {
                return CrossingType::DOWN_UP;
            }

            return CrossingType::INVALID;
        }

        bool getLowerBoundaryForTruncate();

        bool getUpperBoundaryForTruncate();
};


template<typename CurveType>
bool TruncateHelper<CurveType>::getFirstRegionCurveIntersects( SurfacesIndices &lb, SurfacesIndices &ub )
{
    if ( canTruncate() )
    {
		getIntersectedBoundaries(lb, ub);
        return true;
    }

    lower_boundary_ = {};
    upper_boundary_ = {};

    if ( curve_.size() < 2 )
    {
        return false;
    }


    std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
    //bool first_cross_ = false, second_cross_ = false;

    SUtilitiesWrapper u(modeller_);

    lower_boundary_ = u.getSurfacesIndicesBelowPoint(curve_[0].x(), curve_[0].y(), cross_section_);
    upper_boundary_ = u.getSurfacesIndicesAbovePoint(curve_[0].x(), curve_[0].y(), cross_section_);

    for ( size_t i = 1; i < curve_.size(); ++i )
    {
        cur_lbounds = u.getSurfacesIndicesBelowPoint(curve_[i].x(), curve_[i].y(), cross_section_);
        cur_ubounds = u.getSurfacesIndicesAbovePoint(curve_[i].x(), curve_[i].y(), cross_section_);

        if ( (cur_lbounds != lower_boundary_) || (cur_ubounds != upper_boundary_) )
        {
            if ( first_cross_ == false )
            {
                lower_boundary_ = cur_lbounds;
                upper_boundary_ = cur_ubounds;
                first_cross_index_ = i;
                first_cross_ = true;
            }
            else 
            {
                second_cross_index_= i;
                second_cross_ = true;
                break; // for loop
            }
        }
    }

    if ( first_cross_ && second_cross_ )
    {
        bool curve_was_reversed = reverseCurve(curve_);
        if (curve_was_reversed)
        {
            size_t size = curve_.size(), fcross, scross;
            fcross = size - second_cross_index_;
            scross = size - first_cross_index_;

            first_cross_index_ = fcross; 
            second_cross_index_ = scross;
        }

		getIntersectedBoundaries(lb, ub);

        return true;
    }

    return false;
}

template<typename CurveType>
bool TruncateHelper<CurveType>::truncateCurve()
{
    bool success = false;

    success |= getLowerBoundaryForTruncate();
    success |= getUpperBoundaryForTruncate();

    if (!success)
    {
        return false;
    }
    std::cout << "Success!\n";
    // Get upper and lower boundaries
    //
    //std::vector<size_t> lb_intersect, ub_intersect;

    //auto lbound = u.getSurfacesIndicesBelowPoint(in_curve[first].x(), in_curve[first].y(), in_curve_depth);
    //std::set_intersection(intersect_surf.begin(), intersect_surf.end(), lbounds.begin(), lbounds.end(), std::back_inserter(lb_intersect));
    //lbound = lb_intersect;

    //auto ubound = u.getSurfacesIndicesAbovePoint(in_curve[first].x(), in_curve[first].y(), in_curve_depth);
    //std::set_intersection(intersect_surf.begin(), intersect_surf.end(), ubounds.begin(), ubounds.end(), std::back_inserter(ub_intersect));
    //ubound = ub_intersect;

    //auto getUpperBound = [&]() -> std::vector<double> 
    //{

    //};

    // Determine how the curve crosses the region it intersects
    //


    //auto getCrossingType = [&]( size_t index ) -> CrossingType
    //{
    //    auto region = u.getSurfacesIndicesBelowPoint(in_curve[index].x(), in_curve[index].y(), in_curve_depth);

    //    auto prev_lb = u.getSurfacesIndicesBelowPoint(in_curve[index-1].x(), in_curve[index-1].y(), in_curve_depth);
    //    auto lb = u.getSurfacesIndicesBelowPoint(in_curve[index].x(), in_curve[index].y(), in_curve_depth);

    //    /* auto pub = u.getSurfacesIndicesAbovePoint(in_curve[index-1].x(), in_curve[index-1].y(), in_curve_depth); */
    //    /* auto ub = u.getSurfacesIndicesAbovePoint(in_curve[index].x(), in_curve[index].y(), in_curve_depth); */

    //    std::vector<size_t> sym_diff = {};
    //    std::set_symmetric_difference( prev_lb.begin(), prev_lb.end(), lb.begin(), lb.end(), std::back_inserter(sym_diff) );

    //    bool points_upwards = std::includes( lb.begin(), lb.end(), sym_diff.begin(), sym_diff.end() );
    //    bool at_upper_boundary = std::includes( ubound.begin(), ubound.end(), sym_diff.begin(), sym_diff.end() );

    //    if ( points_upwards && at_upper_boundary )
    //    {
    //        return CrossingType::DOWN_UP_UPPER;
    //    }
    //    else if ( points_upwards && !at_upper_boundary )
    //    {
    //        return CrossingType::DOWN_UP_LOWER;
    //    }
    //    else if ( !points_upwards && at_upper_boundary )
    //    {
    //        return CrossingType::UP_DOWN_UPPER;
    //    }
    //    else //if ( !points_upwards && !at_upper_boundary )
    //    {
    //        return CrossingType::UP_DOWN_LOWER;
    //    }
    //};

    // Truncate the curve
    //

    Point2D p, q;

    CrossedBoundary b1 = getCrossedBoundary(first_cross_index_);
    CrossingType t1 = getCrossingType(first_cross_index_);
    q = curve_[first_cross_index_ - 1];
    size_t i = 0;

    switch (b1)
    {
        case CrossedBoundary::UPPER:
            p = upper_curve_.atBegin();
            while ( (p.x() < q.x()) && (i < upper_curve_.size()) )
            {
                truncatePoint<Point2D>(p, q, b1, t1);
                /* p.y() = std::max(p.y(), q.y()); */
                left_part_.add(p);
                i++;
                if (i < upper_curve_.size())
                    p = upper_curve_[i];
            }
            break;

        case CrossedBoundary::LOWER:
            p = lower_curve_.atBegin();
            while ((p.x() < q.x()) && (i < lower_curve_.size()))
            {
                truncatePoint<Point2D>(p, q, b1, t1);
                /* p.y() = std::min(p.y(), q.y()); */
                left_part_.add(p);
                i++;
                if (i < lower_curve_.size())
                    p = lower_curve_[i];
            }
            break;

        case CrossedBoundary::INVALID:
			return false;
            break;
    }

    CrossedBoundary b2 = getCrossedBoundary(second_cross_index_);
    CrossingType t2 = getCrossingType(second_cross_index_);
    q = curve_[second_cross_index_];
    size_t j = 1;

    switch (b2)
    {
        case CrossedBoundary::UPPER:
            p = upper_curve_.atEnd();
            while ((p.x() > q.x()) && (j < upper_curve_.size()))
            {
                truncatePoint<Point2D>(p, q, b2, t2);
                /* p.y() = std::max(p.y(), q.y()); */
                right_part_.add(p);
                j++;
                if (j < upper_curve_.size())
                    p = upper_curve_[ upper_curve_.size() - j];
            }
            break;

        case CrossedBoundary::LOWER:
            p = lower_curve_.atEnd();
            while ((p.x() > q.x()) && (j < lower_curve_.size()))
            {
                truncatePoint<Point2D>(p, q, b2, t2);
                /* p.y() = std::min(p.y(), q.y()); */
                right_part_.add(p);
                j++;
                if (j < lower_curve_.size())
                    p = lower_curve_[lower_curve_.size() - j];
            }
            break;

        case CrossedBoundary::INVALID:
			return false;
            break;
    }

    right_part_.reverse();

    return true;
}

template<typename CurveType>
bool TruncateHelper<CurveType>::getLowerBoundaryForTruncate()
{
    if (!canTruncate())
    {
        return false;
    }

    SurfacesIndices lb;
    std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), lower_boundary_.begin(), lower_boundary_.end(), std::back_inserter(lb));
    auto num_curves = lb.size();

    if (num_curves == 0)
    {
        return false;
    }

    std::vector<double> lb_curve = {}, curve = {};
    bool valid_curve;
    size_t curve_size;
    std::vector<size_t> dummy_elist;

    double displacement = 1.;

    size_t i;
    for (i = 0; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(lb[i], cross_section_, lb_curve, dummy_elist);
        if (valid_curve)
        {
            break;
        }
    }

    for (; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(lb[i], cross_section_, curve, dummy_elist);
        if (valid_curve)
        {
            for (size_t j = 1; j < curve.size(); j += 2)
            {
                if (lb_curve[j] < curve[j])
                {
                    lb_curve[j] = curve[j];
                }
            }
        }
    }

    /* for (size_t j = 1; j < lb_curve.size(); j += 2) */
    /* { */
    /*     lb_curve[j] -= displacement; */
    /* } */

    convertToCurveType(lb_curve, lower_curve_);

    return true;
}

template<typename CurveType>
bool TruncateHelper<CurveType>::getUpperBoundaryForTruncate()
{
    if ( !canTruncate() )
    {
        return false;
    }

    SurfacesIndices ub;
    std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), upper_boundary_.begin(), upper_boundary_.end(), std::back_inserter(ub));
    auto num_curves = ub.size();

    if ( num_curves == 0 )
    {
        return false;
    }

    std::vector<double> ub_curve = {}, curve = {};
    bool valid_curve;
    size_t curve_size;
    std::vector<size_t> dummy_elist;

    double displacement = 1.;

    size_t i;
    for (i = 0; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(ub[i], cross_section_, ub_curve, dummy_elist);
        if (valid_curve)
        {
            break;
        }
    }

    for (; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(ub[i], cross_section_, curve, dummy_elist);
        if (valid_curve)
        {
            for (size_t j = 1; j < curve.size(); j += 2)
            {
                if (ub_curve[j] > curve[j])
                {
                    ub_curve[j] = curve[j];
                }
            }
        }
    }

    /* for (size_t j = 1; j < ub_curve.size(); j += 2) */
    /* { */
    /*     ub_curve[j] += displacement; */
    /* } */

    convertToCurveType(ub_curve, upper_curve_);

    return true;
}

#endif
