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
	// @brief Represents the geometry of a geological object. The input sketch is assigned to class
	// As the CGAL::Arrangement_2_with_histoy its keep input curve for
	// further computation.
	template< class Real >
	class StratigraphicCurves
	{
		public:

			typedef PolygonalCurve2D<Real> 		Curve2D;
			typedef typename Curve2D::Point2D	Point2D;
			typedef	StratigraphicSegment<Real>	Segment;

			StratigraphicCurves ( )
			{
				index = 0;
			}

			// Copy Constructor
			StratigraphicCurves ( const Curve2D& curve )
			{
				index = 0;
				this->curve = curve;
				this->segments = segments;
			}

			StratigraphicCurves ( std::vector<Point2D> curve)
			{
				index = 0;
				this->curve.setCurve(curve);
			}

			virtual ~StratigraphicCurves ( )
			{

			}

		public:
			unsigned int index;
			Curve2D curve;
			std::vector<Segment> segments;

	};
} /* namespace RRM */

#endif /* _MODEL_STRATIGRAPHICCURVES_HPP_ */
