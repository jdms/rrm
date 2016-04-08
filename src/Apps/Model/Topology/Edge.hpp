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

		public:
			typedef Edge<Real>			Self;
			typedef	StratigraphicSegment<Real>	Segment;

			Edge ( )
			{
				id_ = 0;
			}

			Edge ( const Self& _edge)
			{
				*this = _edge;
			}

			virtual ~Edge ( )
			{

			}

			Self& operator=(const Self& other )
			{
				// Assign to all  member.
				this->id_ = other.id_;

				this->source_ = other.source_;
				this->target_ = other.source_;

				this->segment = other.segment;

				return *this;
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
