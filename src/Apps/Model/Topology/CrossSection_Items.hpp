/*
 * CrossSectionItems.hpp
 *
 *  Created on: Sep 24, 2015
 *      Author: felipe
 */

#ifndef _MODEL_CROSSSECTION_ITEMS_HPP_
#define _MODEL_CROSSSECTION_ITEMS_HPP_

#include <CGAL/HalfedgeDS_min_items.h>

#include "CrossSectionEdge.hpp"
#include "CrossSectionFace.hpp"
#include "CrossSectionVertex.hpp"

namespace RRM
{

	template < class Real >
	class CrossSectionItems: public CGAL::HalfedgeDS_min_items
	{
		public:
			template < class Refs , class Traits >
			struct Vertex_wrapper
			{
					typedef RRM::CrossSectionVertex<Refs, Traits, Real> Vertex;
			};

			template < class Refs , class Traits >
			struct Halfedge_wrapper
			{
					typedef RRM::CrossSectionEdge<Refs, Traits, Real> Halfedge;
			};

			template < class Refs , class Traits >
			struct Face_wrapper
			{
					typedef RRM::CrossSectionFace<Refs, Traits> Face;
			};
	};
}
#endif /* _MODEL_CROSSSECTION_ITEMS_HPP_ */
