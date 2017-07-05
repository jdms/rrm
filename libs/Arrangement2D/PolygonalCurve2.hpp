/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



/*
 * PolygonalCurve2D.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: Emilio Vital Brazil, Felipe de Carvalho
 */

#ifndef _RRM_POLYGONALCURVE2_HPP_
#define _RRM_POLYGONALCURVE2_HPP_

#include <map>
#include <vector>
#include <algorithm>
#include <tuple>

#include "./src/Core/Base/Constants/constants.hpp"
#include "./src/Core/Geometry/PolygonalCurve/polygonal_curve_2d.hpp"

/*!
 * @brief  Wrapper to class PolygonalCurve, which represents curves built from the concatenation of straight lines.
 */
namespace RRM
{
    class PolygonalCurve2: public Curve2D
    {
            /// \TODO Infinity and Zero are for double
        public:
            typedef PolygonalCurve2 Self;

            PolygonalCurve2 ( ) ;

            PolygonalCurve2 ( const Curve2D& ) ;

            /// \brief Slices curve at vertices: 0,1 ........... n-1,n.
            /// Minimum range [first=1,last=size()-1]
            /// Creates n new curves by splitting this curve.
            /// \param indices of the split points.
            /// \param segments (output) list of curve segments.
            ///
            /// \return bool
            bool slices ( std::vector<std::size_t>& indices , std::deque<Self>& segments ) const;


            /// \brief Return all x_monotone sub curve of the curve
            /// \param _curve. The curve to be tested
            /// \param _x_monotone_subcurves. The list of all x_monotone sub curve.
            ///
            /// \return true if the curve is x_monotone
            ///         false and the list of x_monotone sub curves
            bool is_x_monotonic_curve ( const Curve2D& _curve, std::map<std::size_t,std::vector<std::size_t> >& x_monotone_subcurves ) const;

        public:
            /// FIXME Line related functions

            /// \brief Rotate a Point2D p 90 degrees around the origin.
            /// This function returns one Point2D where the coordinates x = - p.y and y = p.x
            Point2D perpendicular ( Point2D p ) const;

            /// \brief Test if a point is left or right to other 2
            ///
            /// This function returns a real value, if it is positive means the point q
            /// is in the left side of p1p2, if negative means the it is in right side
            /// and zero means they are 3 colinear points

            double CCW ( Point2D p1 , Point2D p2 , Point2D q ) const;

            /// \brief project a point on a line described by 2 points
            ///
            /// Project q over a line defined by p1p2 and returns the projection qp.
            /// alpha is pq = q + alpha*[perpendicular(p2-p1)/|p2-p1|] that means alpha is the
            /// distance between q and pq beta is pq = p1 + beta(p2-p1), that means if
            /// 0 >= beta <=1 pq is inside the p2p1 segment. If p1 = p2 return p1 and
            /// beta = (q-p1).norm()

            Point2D projectPoint ( Point2D p1 , Point2D p2 , Point2D q , double& alpha , double& beta ) const;

            /// \brief Find the intersection of 2 lines defined by 2 points each
            ///
            /// Find the intersection point pq and returns alpha and beta,
            /// pq = p1 + alpha*(p2-p1) and pq = q1 + beta*(q2-q1). If the lines are
            /// parallel then pq = (inf,inf), beta = alpha = inf. If the points are
            /// colinear then find alpha such that q1 = p1 + alpha*(p2-p1) and beta
            /// such that p1 = q1 + beta*(q2-q1) and returns the point (inf,inf)

            Point2D intersectionSegments ( Point2D p1 , Point2D p2 , Point2D q1 , Point2D q2 , double& alpha , double& beta ) const;


            /// \brief finds all intersection between two curves.
            ///
            /// This function calculates the intersection points between two curves.
            /// For this curve and for the testCurve it store the index \a i of the
            /// curve vertex that the segment intersect the other curve and the
            /// \f$\alpha\f$ value such that \f$\alpha((i+i)-(i))+(i)\f$;
            /// \f$\alpha \in [0,1]\f$.
            ///
            /// \param testCurve the curve that will be tested against this curve
            /// \param thisIndex (output) a std::vector, stores the vertex indexes of
            ///        this curve, these are the start point of the segment that
            ///        intersects the testCurve.
            /// \param thisAlphas (output) a std::vector, stores this alpha values to
            ///        recover the intersection point.
            /// \param testIndex (output) a std::vector, stores the vertex indexes of
            ///        testCurve, these are the start point of the segment that
            ///        intersects this curve.
            /// \param testAlphas (output) a std::vector, stores testCurve alpha values
            ///        to recover the intersection point.
            /// \param itersPoints (output) a std::vector, stores the intersection
            ///        points.
            /// \return TRUE if at least one intersection is found

            bool intersections (    const PolygonalCurve2& testCurve ,
                        std::vector<std::size_t>& thisIndex ,
                        std::vector<double>& thisAlphas ,
                        std::vector<std::size_t>& testIndex ,
                        std::vector<double>& testAlphas ,
                        std::vector<Point2D> &itersPoints ) const;


            /// \brief add sequence of intersection point to the curve
            /// Insert point to the curve and return the new indices.
            /// \param _intersection_points, points to be inserted
            ///
            void addPoints (std::vector<std::pair<std::size_t,Point2D> >& _intersection_points );
    };

} /* namespace RRM */

#endif /* _RRM_POLYGONALCURVE2_HPP_ */

