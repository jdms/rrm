/*
 *  DCEL.hpp
 *
 *  Created on: Sep 2, 2015
 *      Author: felipe
 */

#ifndef _CORE_TOPOLOGY_DCEL_HPP_
#define _CORE_TOPOLOGY_DCEL_HPP_

/**!
 * @brief CGAL Arrangements. RRM model of CrossSection
 */

/// Standard Library
#include <iostream>

#include <CGAL/HalfedgeDS_default.h>

#include "DCEL_Items.hpp"

// Where the RRM_Items are defined
// This class will provide an implementation of an arrangement of curves in 2D
// An items type using a half-edge with previous-pointer.

// \TODO Arrangement of Polygonal Curve 2-Dimension with history, specially design to work with Geological Curve.

namespace RRM
{
	namespace Topology
	{
		template<class Real, class Items>
		class DCEL
		{
			public:

				typedef CGAL::HalfedgeDS_default<int,Items> 				HalfedgeDataStructure;

				typedef typename HalfedgeDataStructure::Halfedge_handle 	Halfedge_handle;
				typedef typename HalfedgeDataStructure::Halfedge			Halfedge;
				typedef typename HalfedgeDataStructure::Vertex 				Vertex;
				typedef typename HalfedgeDataStructure::Vertex_handle 		Vertex_handle;
				typedef typename HalfedgeDataStructure::Face 				Face;
				typedef typename HalfedgeDataStructure::Face_handle 		Face_handle;

				DCEL ( )
				{
				}

				virtual ~DCEL ( )
				{
				}

			protected:

			private:
				HalfedgeDataStructure hds;

		};
	} /* namespace Topology */
} /* namespace RRM */

#endif /* _CORE_TOPOLOGY_DCEL_HPP_ */
