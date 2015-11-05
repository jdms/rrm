/*
 * CrossSectionEdge.hpp
 *
 *  Created on: Sep 24, 2015
 *      Author: felipe
 */

#ifndef _MODEL_CROSSSECTIONEDGE_HPP_
#define _MODEL_CROSSSECTIONEDGE_HPP_

#include <CGAL/HalfedgeDS_halfedge_base.h>
#include <CGAL/tags.h>
#include "Model/StratigraphicSegment.hpp"

namespace RRM
{
	/// @see http://doc.cgal.org/4.7/HalfedgeDS/classCGAL_1_1HalfedgeDS__halfedge__base.html
	template < class Refs , class Traits, class Real >
	class CrossSectionEdge: public CGAL::HalfedgeDS_halfedge_base<Refs, CGAL::Tag_true, CGAL::Tag_true, CGAL::Tag_true>
	{
		public:

			typedef CrossSectionEdge<Refs,Traits,Real>  Self;

			typedef	StratigraphicSegment<Real>	Segment;
			typedef CGAL::HalfedgeDS_halfedge_base<Refs, CGAL::Tag_true, CGAL::Tag_true, CGAL::Tag_true> Base;
			typedef typename Base::Halfedge_handle  Halfedge_handle;

			CrossSectionEdge ( ) : Base()
			{
				left_to_right = 1;
				is_boundary = false;
				is_visible  = 0;
			}

			CrossSectionEdge ( const Self& other) : Base()
			{
				segment 	= other.segment;
				left_to_right   = other.left_to_right;
				is_boundary 	= other.is_boundary;
				is_visible 	= other.is_visible;
			}

			Self& operator=(const Self& other )
			{
				/// Assign to all  member.
				segment 	= other.segment;
				left_to_right   = other.left_to_right;
				is_boundary 	= other.is_boundary;
				is_visible 	= other.is_visible;

				return *this;
			}

			virtual ~CrossSectionEdge ( )
			{

			}
			// Only the left_to_right halfedge holds the curve.
			// In this case, left_to_right is assign with 1.
			// v0 (curve.begin())                        v1 (curve.end())
			// *  ----------------------------------------->  *
			// Soon, segment will be smart_pointer.
			Segment segment;
			// With vertex it points v0 (0) or v1(1)
			int left_to_right;

			bool is_boundary;

			bool is_visible;

			// The born of the Arrangement of PolyCurve2D with History
			Halfedge_handle curve_history_;

	};

} /* namespace RRM */

#endif /* _MODEL_CROSSSECTIONEDGE_HPP_ */
