/*
 * HalfEdge.hpp
 *
 *  Created on: Aug 21, 2015
 *      Author: felipe
 */

#ifndef _CORE_TOPOLOGY_HALFEDGE_HPP_
#define _CORE_TOPOLOGY_HALFEDGE_HPP_


#include <list>

#include <CGAL/HalfedgeDS_halfedge_base.h>

#include "Vertex.hpp"
#include "Face.hpp"

/*!
 * @brief Halfedge of the Arrangements
 * \todo Implement Traits to allow customization of the class.
 */

namespace RRM
{
	namespace Topology
	{
		/// @see http://doc.cgal.org/4.7/HalfedgeDS/classCGAL_1_1HalfedgeDS__halfedge__base.html
		template <class Refs, class Traits>
		class Halfedge : public CGAL::HalfedgeDS_halfedge_base<Refs,
															   CGAL::Tag_true,
															   CGAL::Tag_true,
															   CGAL::Tag_true>
		{
		public:
				Halfedge ( )
				{
					i = 0;
				}
				Halfedge ( int p ) : i ( p )
				{

				}
				~Halfedge ( )
				{

				}

				int i;

		private:


		};

		/// Implementation

	} /* namespace Topology */
} /* namespace RRM */


#endif /* _CORE_TOPOLOGY_HALFEDGE_HPP_ */
