/*
 * CrossSectionVertex.hpp
 *
 *  Created on: Sep 24, 2015
 *      Author: felipe
 */

#ifndef _MODEL_CROSSSECTIONVERTEX_HPP_
#define _MODEL_CROSSSECTIONVERTEX_HPP_



#include <utility> // int

/// FIXME Implement Traits to allow customization of the class.
#include <CGAL/HalfedgeDS_vertex_base.h>

#include "Core/Geometry/PolygonalCurve/VectorN.hpp"

namespace RRM
{
	/// @see http://doc.cgal.org/4.7/HalfedgeDS/classCGAL_1_1HalfedgeDS__vertex__base.html
	template < class Refs , class Traits, class Real >
	class CrossSectionVertex: public CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true /// Has reference to an incident half-edge.
	>
	{
		public:
			typedef PointN<Real, 2> Point2D;

			CrossSectionVertex ( )
			{

			}

			CrossSectionVertex ( const Point2D& p )
			{
				point = p;
			}

			~CrossSectionVertex ( )
			{
			}

			Point2D point;
		private:


	};
} /* namespace RRM */

#endif /* _MODEL_CROSSSECTIONVERTEX_HPP_ */
