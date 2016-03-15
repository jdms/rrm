/*
 * Vertex.hpp
 *
 *  Created on: Aug 21, 2015
 *      Author: felipe
 */

#ifndef _CORE_TOPOLOGY_VERTEX_HPP_
#define _CORE_TOPOLOGY_VERTEX_HPP_

#include <utility> // int


/*!
 * @brief Vertex of the arrangements
 * \todo Implement Traits to allow customization of the class.
 */


#include <CGAL/HalfedgeDS_vertex_base.h>
#include <list>

#include "Geometry/PolygonalCurve/VectorN.hpp"

namespace RRM
{
	namespace Topology
	{
		// @see http://doc.cgal.org/4.7/HalfedgeDS/classCGAL_1_1HalfedgeDS__vertex__base.html
		template <class Refs, class Traits>
		class Vertex : public CGAL::HalfedgeDS_vertex_base<Refs,
								   CGAL::Tag_true // Has reference to an incident half-edge.
							          >
		{
			public:
				Vertex ( )
				{
					index_ = -1;
				}

				Vertex ( int index )
				{
					index_ = index;
				}

				~Vertex ( )
				{

				}

			private:

				int index_;

		};
	} /* namespace Topology */
} /* namespace RRM */

#endif /* _CORE_TOPOLOGY_VERTEX_HPP_ */
