/*
 * StratigraphicCurves.hpp
 *
 *  Created on: Sep 14, 2015
 *      Author: felipe
 */

#ifndef _MODEL_STRATIGRAPHICCURVES_HPP_
#define _MODEL_STRATIGRAPHICCURVES_HPP_

#include "Core/Geometry/PolygonalCurve2D.hpp"
#include "StratigraphicSegment.hpp"

namespace RRM
{
	/// @brief As the CGAL::Arrangement_2_with_histoy its keep input curve for
	/// further computation.
	template< class Real >
	class StratigraphicCurves
	{
		public:

			typedef PolygonalCurve2D<Real> 		Curve2D;
			typedef typename Curve2D::Point2D	Point2D;
			typedef	StratigraphicSegment<Real>	Segment;

			StratigraphicCurves ( )
			{
				id_ = -1;
			}

			/// Copy Constructor
			StratigraphicCurves ( const Curve2D& curve )
			{
				id_ = -1;
				this->curve = curve;
			}

			StratigraphicCurves ( std::vector<Point2D> curve)
			{
				id_ = -1;
				this->curve.setCurve(curve);
			}

			virtual ~StratigraphicCurves ( )
			{

			}

		public:
			int id_;
			Curve2D curve;
			std::vector<Segment> segments;

	};
} /* namespace RRM */

#endif /* _MODEL_STRATIGRAPHICCURVES_HPP_ */
