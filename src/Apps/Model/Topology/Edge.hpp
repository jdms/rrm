/*
 * Edge.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef _TOPOLOGY_EDGE_HPP_
#define _TOPOLOGY_EDGE_HPP_


#include "Model/StratigraphicSegment.hpp"

namespace RRM
{

	template < class Real>
	class Edge
	{

		typedef	StratigraphicSegment<Real>	Segment;

		public:
			Edge ( )
			{
				id_ = 0;
			}
			virtual ~Edge ( )
			{

			}
		public:
			unsigned int id_;
			// Soon, segment will be smart_pointer.
			Segment segment;

			Vertex<Real> source_;
			Vertex<Real> target_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_EDGE_HPP_ */
