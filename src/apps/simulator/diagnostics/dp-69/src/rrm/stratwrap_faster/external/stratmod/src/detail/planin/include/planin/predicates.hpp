/******************************************************************************/
/*                                                                            */
/* This file is part of the "Planar Interpolation Library" (PlanInLib)        */
/* Copyright (C) 2016, Julio Daniel Machado Silva.                            */
/*                                                                            */
/* PlanInLib is free software; you can redistribute it and/or                 */
/* modify it under the terms of the GNU Lesser General Public                 */
/* License as published by the Free Software Foundation; either               */
/* version 3 of the License, or (at your option) any later version.           */
/*                                                                            */
/* PlanInLib is distributed in the hope that it will be useful,               */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of             */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/* Lesser General Public License for more details.                            */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public           */
/* License along with PlanInLib.  If not, see <http://www.gnu.org/licenses/>, */
/* or write to the Free Software Foundation, Inc., 51 Franklin Street,        */
/* Fifth Floor, Boston, MA  02110-1301  USA.                                  */
/*                                                                            */
/******************************************************************************/

#ifndef PLANIN_ROBUST_PREDICATES_WRAPPER_HPP
#define PLANIN_ROBUST_PREDICATES_WRAPPER_HPP

#include <optional>
#include <utility>


class RobustPredicates {
    public:
        ~RobustPredicates() = default;
        RobustPredicates(RobustPredicates&) = delete;
        RobustPredicates& operator=(RobustPredicates&) = delete;
        RobustPredicates(RobustPredicates&&) = delete;
        RobustPredicates& operator=(RobustPredicates&&) = delete;

        static RobustPredicates& Get();

        // Robust
        /* Return a positive value if the points pa, pb, and pc occur  */
        /* in counterclockwise order; a negative value if they occur   */
        /* in clockwise order; and zero if they are collinear.  The    */
        /* result is also a rough approximation of twice the signed    */
        /* area of the triangle defined by the three points.           */
        template<typename Point2D>
        double orient2d(const Point2D& a, const Point2D& b, const Point2D& c) const;

        // Robust
        /* Return a positive value if the point pd lies inside the     */
        /* circle passing through pa, pb, and pc; a negative value if  */
        /* it lies outside; and zero if the four points are cocircular.*/
        /* The points pa, pb, and pc must be in counterclockwise       */
        /* order, or the sign of the result will be reversed.          */
        template<typename Point2D>
        double incircle(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d) const;

        // Robust
        template<typename Point2D>
        double signed_area(const Point2D& a, const Point2D& b, const Point2D& c) const;

        // Non-robust
        template<typename Point2D>
        double signed_area_fast(const Point2D& a, const Point2D& b, const Point2D& c) const;

        // Robust
        template<typename Point2D, typename Segment = std::pair<Point2D, Point2D>>
        std::optional<Point2D> intersect_segments(const Segment& segment1, const Segment& segment2) const;

        // Robust
        /* Return a positive value if the point pd lies below the      */
        /* plane passing through pa, pb, and pc; "below" is defined so */
        /* that pa, pb, and pc appear in counterclockwise order when   */
        /* viewed from above the plane.  Returns a negative value if   */
        /* pd lies above the plane.  Returns zero if the points are    */
        /* coplanar.  The result is also a rough approximation of six  */
        /* times the signed volume of the tetrahedron defined by the   */
        /* four points.                                                */
        template<typename Point3D>
        double orient3d(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const;

        // Robust
        /* Return a positive value if the point pe lies inside the     */
        /* sphere passing through pa, pb, pc, and pd; a negative value */
        /* if it lies outside; and zero if the five points are         */
        /* cospherical.  The points pa, pb, pc, and pd must be ordered */
        /* so that they have a positive orientation (as defined by     */
        /* orient3d()), or the sign of the result will be reversed.    */
        template<typename Point3D>
        double insphere(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d, const Point3D& e) const;

        // Robust
        template<typename Point3D>
        double signed_volume(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const;

        // Non-robust
        template<typename Point3D>
        double signed_volume_fast(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const;

    private:
        RobustPredicates();

        double call_orient2d(const double* a, const double* b, const double* c) const;
        double call_orient2d_fast(const double* a, const double* b, const double* c) const;
        double call_orient2d_exact(const double* a, const double* b, const double* c) const;
        double call_incircle(const double* a, const double* b, const double* c, const double* d) const;

        double call_orient3d(const double* a, const double* b, const double* c, const double* d) const;
        double call_orient3d_fast(const double* a, const double* b, const double* c, const double* d) const;
        double call_orient3d_exact(const double* a, const double* b, const double* c, const double* d) const;
        double call_insphere(const double* a, const double* b, const double* c, const double* d, const double* e) const;
};

template<typename Point2D>
double RobustPredicates::orient2d(const Point2D& a, const Point2D& b, const Point2D& c) const
{
    return call_orient2d(a.data(), b.data(), c.data());
}

template<typename Point2D>
double RobustPredicates::incircle(const Point2D& a, const Point2D& b, const Point2D& c, const Point2D& d) const
{
    return call_incircle(a.data(), b.data(), c.data(), d.data());
}

template<typename Point2D>
double RobustPredicates::signed_area(const Point2D& a, const Point2D& b, const Point2D& c) const
{
    return call_orient2d_exact(a.data(), b.data(), c.data());
}

template<typename Point2D>
double RobustPredicates::signed_area_fast(const Point2D& a, const Point2D& b, const Point2D& c) const
{
    return call_orient2d_fast(a.data(), b.data(), c.data());
}

template<typename Point3D>
double RobustPredicates::orient3d(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const
{
    return call_orient3d(a.data(), b.data(), c.data(), d.data());
}

template<typename Point3D>
double RobustPredicates::insphere(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d, const Point3D& e) const
{
    return call_insphere(a.data(), b.data(), c.data(), d.data(), e.data());
}

template<typename Point3D>
double RobustPredicates::signed_volume(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const
{
    return call_orient3d_exact(a.data(), b.data(), c.data(), d.data());
}

template<typename Point3D>
double RobustPredicates::signed_volume_fast(const Point3D& a, const Point3D& b, const Point3D& c, const Point3D& d) const
{
    return call_orient3d_fast(a.data(), b.data(), c.data(), d.data());
}

template<typename Point2D, typename Segment>
std::optional<Point2D> RobustPredicates::intersect_segments(const Segment& segment1, const Segment& segment2) const
{
    std::optional<Point2D> q;

    auto& [a, b] = segment1;
    auto& [c, d] = segment2;

    double orient2_cab = orient2d(c, a, b);
    double orient2_dab = orient2d(d, a, b);

    // intersect_general_position = (orient2_cab > 0.) xor (orient2_dab > 0.)
    bool intersect_general_position = ((orient2_cab > 0.) != (orient2_dab > 0.));
    bool intersect_c_lies_in_ab = ((orient2_cab == 0.) && (orient2_dab != 0.));
    bool intersect_d_lies_in_ab = ((orient2_cab != 0.) && (orient2_dab == 0.));

    bool intersect = intersect_general_position || intersect_c_lies_in_ab || intersect_d_lies_in_ab;
    if (intersect)
    {
        double area_cab = signed_area(c, a, b);
        double area_dab = signed_area(d, a, b);
        double s = area_cab / (area_cab - area_dab);

        q = std::make_optional<Point2D>({c[0] + s * (d[0] - c[0]), c[1] + s * (d[1] - c[1])});
    }

    return q;
}

#endif
