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

        TruncateHelper(const CurveTuple &curve_tuple, SModellerWrapper &m, SurfacesIndices &isurf) : in_curve_(std::get<0>(curve_tuple)), modeller_( m )
		{
			curve_ = in_curve_;
			sanitizeCurve(curve_);
			cross_section_ = std::get<1>(curve_tuple);
			intersected_surfaces_ = isurf;
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

        double tol = 1E-1;

        enum class CrossedBoundary { UPPER, LOWER, BOUNDING_BOX, INVALID };

        void sanitizeCurve( CurveType &c )
        {
            if ( c.size() > 1 )
            {
                auto p = c.atBegin();
                auto q = c.atEnd();

                if ( p.x() > q.x() )
                {
                    c.reverse();
                }
            }
        }

        void trimIntersectionList()
        {
            if ( !canTruncate() )
            {
                lower_boundary_.clear();
                upper_boundary_.clear();
            }

            std::vector<size_t> lb_intersect, ub_intersect;
            std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), lower_boundary_.begin(), lower_boundary_.end(), std::back_inserter(lb_intersect));
            std::set_intersection(intersected_surfaces_.begin(), intersected_surfaces_.end(), upper_boundary_.begin(), upper_boundary_.end(), std::back_inserter(ub_intersect));

            lower_boundary_ = lb_intersect;
            upper_boundary_ = ub_intersect;
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
            std::vector<size_t> sym_diff = {};

            SUtilitiesWrapper u(modeller_);

            auto prev_ub = u.getSurfacesIndicesAbovePoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            auto ub		 = u.getSurfacesIndicesAbovePoint(curve_[index    ].x(), curve_[index    ].y(), cross_section_);
            std::set_symmetric_difference(prev_ub.begin(), prev_ub.end(), ub.begin(), ub.end(), std::back_inserter(sym_diff));

            bool at_upper_boundary = std::includes(upper_boundary_.begin(), upper_boundary_.end(), sym_diff.begin(), sym_diff.end());

            if (at_upper_boundary)
            {
                std::cout << "Cross at the upper boundary\n";
                return CrossedBoundary::UPPER;
            }

            sym_diff.clear();

            auto prev_lb = u.getSurfacesIndicesBelowPoint(curve_[index - 1].x(), curve_[index - 1].y(), cross_section_);
            auto lb = u.getSurfacesIndicesBelowPoint(curve_[index].x(), curve_[index].y(), cross_section_);
            std::set_symmetric_difference(prev_lb.begin(), prev_lb.end(), lb.begin(), lb.end(), std::back_inserter(sym_diff));

            bool at_lower_boundary = std::includes(lower_boundary_.begin(), lower_boundary_.end(), sym_diff.begin(), sym_diff.end());

            if (at_lower_boundary)
            {
                std::cout << "Cross at the lower boundary\n";
                return CrossedBoundary::LOWER;
            }

            std::cout << "Error evaluating crossing\n";
            return CrossedBoundary::INVALID;

        }

        bool getLowerBoundaryForTruncate();

        bool getUpperBoundaryForTruncate();

        /* struct { double x, y, z; } origin_, lenght_; */
};


    template<typename CurveType>
bool TruncateHelper<CurveType>::getFirstRegionCurveIntersects( SurfacesIndices &lb, SurfacesIndices &ub )
{
    lower_boundary_ = {};
    upper_boundary_ = {};

    if ( curve_.size() < 2 )
    {
        return false;
    }

    std::vector<size_t> cur_region, region, cur_lbounds, cur_ubounds;
    bool first_cross_ = false, second_cross_ = false;

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

    trimIntersectionList();

    if ( first_cross_ && second_cross_ )
    {
        lb = lower_boundary_;
        ub = upper_boundary_;
        return true;
    }

    return false;
}

    template<typename CurveType>
bool TruncateHelper<CurveType>::truncateCurve()
{
    // Initialization
    //
    //size_t num_curves = intersect_surf.size();
    //auto &in_curve = std::get<0>(curve_tuple);
    //auto &in_curve_depth = std::get<1>(curve_tuple);

    std::cout << "Inside truncateSurface()\n";
    if ( !canTruncate() )
    {
        return false;
    }

    std::cout << "canTruncate() == true\n";
    bool success = false;
    success |= getLowerBoundaryForTruncate();
    std::cout << "Got lower boundary\n";
    success |= getUpperBoundaryForTruncate();
    std::cout << "Got upper boundary\n";

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

    std::cout << "Getting first crossed boundary: ";
    CrossedBoundary t1 = getCrossedBoundary(first_cross_index_);
    q = curve_[first_cross_index_ - 1];
    size_t i = 0;

    switch (t1)
    {
        case CrossedBoundary::UPPER:
            std::cout << " upper\n";
            p = upper_curve_.atBegin();
            while ( (p.x() < q.x()) && (i < upper_curve_.size()) )
            {
                left_part_.add(p);
                i++;
                if (i < upper_curve_.size())
                    p = upper_curve_[i];
            }
            break;

        case CrossedBoundary::LOWER:
            std::cout << " lower\n";
            p = lower_curve_.atBegin();
            while ((p.x() < q.x()) && (i < lower_curve_.size()))
            {
                left_part_.add(p);
                i++;
                if (i < lower_curve_.size())
                    p = lower_curve_[i];
            }
            break;

        case CrossedBoundary::INVALID:
            std::cout << " invalid\n";
            break;
    }

    std::cout << "Getting second crossed boundary: ";
    CrossedBoundary t2 = getCrossedBoundary(second_cross_index_);
    q = curve_[second_cross_index_];
    size_t j = 1;

    switch (t2)
    {
        case CrossedBoundary::UPPER:
            std::cout << " upper\n";
            p = upper_curve_.atEnd();
            while ((p.x() > q.x()) && (j < upper_curve_.size()))
            {
                right_part_.add(p);
                j++;
                if (j < upper_curve_.size())
                    p = upper_curve_[ upper_curve_.size() - j];
            }
            break;

        case CrossedBoundary::LOWER:
            std::cout << " lower\n";
            p = lower_curve_.atEnd();
            while ((p.x() > q.x()) && (j < lower_curve_.size()))
            {
                right_part_.add(p);
                j++;
                if (j < lower_curve_.size())
                    p = lower_curve_[lower_curve_.size() - j];
            }
            break;

        case CrossedBoundary::INVALID:
            std::cout << " invalid\n";
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

    auto num_curves = lower_boundary_.size();

    if (num_curves == 0)
    {
        std::cout << "Lower boundary is empty\n";
        return false;
    }

    std::vector<double> lb_curve = {}, curve = {};
    bool valid_curve;
    size_t curve_size;
    std::vector<size_t> dummy_elist;

    size_t i;
    for (i = 0; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(lower_boundary_[i], cross_section_, lb_curve, dummy_elist);
        if (valid_curve)
        {
            break;
        }
    }

    for (; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(lower_boundary_[i], cross_section_, curve, dummy_elist);
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

    for (size_t j = 1; j < lb_curve.size(); j += 2)
    {
        lb_curve[j] -= 0;
    }

    std::cout << "Converting lower boundary to a curve\n";
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

    auto num_curves = upper_boundary_.size();

    if ( num_curves == 0 )
    {
        std::cout << "Upper boundary is empty\n";
        return false;
    }

    std::vector<double> ub_curve = {}, curve = {};
    bool valid_curve;
    size_t curve_size;
    std::vector<size_t> dummy_elist;

    size_t i;
    for (i = 0; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(upper_boundary_[i], cross_section_, ub_curve, dummy_elist);
        if (valid_curve)
        {
            break;
        }
    }

    for (; i < num_curves; ++i)
    {
        valid_curve = modeller_.getLengthCrossSectionCurve(upper_boundary_[i], cross_section_, curve, dummy_elist);
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

    for (size_t j = 1; j < ub_curve.size(); j += 2)
    {
        ub_curve[j] += 0;
    }
    std::cout << "Converting upper boundary to a curve\n";
    convertToCurveType(ub_curve, upper_curve_);

    return true;
}

#endif
