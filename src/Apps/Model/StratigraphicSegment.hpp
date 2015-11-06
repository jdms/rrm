/*
 * Segmenet.hpp
 *
 *  Created on: Sep 21, 2015
 *      Author: felipe
 */

#ifndef _CROSSSECTION_MODEL_SEGMENET_HPP_
#define _CROSSSECTION_MODEL_SEGMENET_HPP_

#include "Core/Geometry/PolygonalCurve2D.hpp"

namespace RRM
{
	/// @brief The element of a halfedge.
	/// FIXME
	/// The point at begin() (source) and the point at end() (target)
	/// have the same geometry of the vertex that they point to.
	/// Keep the curve id for history propose
	template < class Real >
	class StratigraphicSegment
	{
		public:
			typedef StratigraphicSegment<Real>	Self;
			typedef PolygonalCurve2D<Real> 		Curve2D;
			typedef typename Curve2D::Point2D	Point2D;

			StratigraphicSegment ( )
			{
				curve_index = -1;
			}
			// COPY CONSTRUCTOR !!!
			StratigraphicSegment ( const Self& segment )
			{
				this->curve_index = segment.curve_index;
				this->curve 	  = segment.curve;

			}

			Self& operator=(const Self& other )
			{
				/// Assign to all  member.
				this->curve_index = other.curve_index;
				this->curve 	  = other.curve;

				return *this;
			}


			virtual ~StratigraphicSegment ( )
			{

			}
		public:
			/// TODO Index class, like Point Cloud Library
			int curve_index;
			/// FIXME The curve itself
			Curve2D curve;
			/// Boundary edge;

	};

} /* namespace RRM */

#endif /* _CROSSSECTION_MODEL_SEGMENET_HPP_ */
