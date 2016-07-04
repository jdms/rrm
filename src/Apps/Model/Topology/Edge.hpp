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
				this->id_ = 0;
				this->source_id_ = 0;
				this->target_id_ = 0;
				this->is_boundary_ = false;
				this->is_visible_ = true;
				this->is_enable_ = true;

				r =  0.5;
				g = 0.5;
				b = 0.5;
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
				this->is_visible_  = other.is_visible_;
				this->is_enable_   = other.is_enable_;

				this->r  = other.r;
				this->g = other.g;
				this->b = other.b;

				return *this;
			}

			void setColor(Real _r, Real _g, Real _b )
			{
				this->r  = _r;
				this->g = _g;
				this->b =_b;
			}

		public:
			unsigned int id_;
			// Soon, segment will be smart_pointer.
			Segment segment;

			bool is_boundary_;
			bool is_visible_;
			bool is_enable_;

			Real r;
			Real g;
			Real b;

			unsigned int source_id_;
			unsigned int target_id_;

	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_EDGE_HPP_ */
