/*
 * Edge.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef _TOPOLOGY_EDGE_HPP_
#define _TOPOLOGY_EDGE_HPP_


#include "Model/StratigraphicSegment.hpp"

#include "Vertex.hpp"

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
				source_id_ = 0;
				target_id_ = 0;
				is_boundary_ = false;
			}

			Edge ( const Self& _edge)
			{
				*this = _edge;
			}

			~Edge ( )
			{

			}

			Self& operator=(const Self& other )
			{
				// Assign to all  member.
				this->id_ = other.id_;

				this->source_id_ = other.source_id_;
				this->target_id_ = other.target_id_;

				this->segment = other.segment;

				this->is_boundary_ = other.is_boundary_;

				return *this;
			}

		public:
			unsigned int id_;
			// Soon, segment will be smart_pointer.
			Segment segment;

			bool is_boundary_;

			unsigned int source_id_;
			unsigned int target_id_;

	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_EDGE_HPP_ */
