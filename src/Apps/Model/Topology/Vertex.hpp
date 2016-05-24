/*
 * Vertex.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef _TOPOLOGY_VERTEX_HPP_
#define _TOPOLOGY_VERTEX_HPP_

#include <set>

#include "Model/StratigraphicSegment.hpp"

namespace RRM
{

	// todo the Real need to be a geometry traits, with all geometry related
	//	objects, aka, polycurve, point, vector ....
	template < class Real>
	class Vertex
	{
		public:
			typedef Vertex<Real>			Self;
			typedef	StratigraphicSegment<Real>	Segment;
			typedef typename Segment::Curve2D	Curve2D;
			typedef typename Segment::Point2D	Point2D;

			Vertex ( )
			{
				id_ = 0;
			}

			Vertex ( const Self& _vertex)
			{
				*this = _vertex;
			}

			~Vertex ( )
			{

			}

			Self& operator=(const Self& other )
			{
				// Assign to all  member.
				id_ = other.id_;
				location_ = other.location_;

				edges_ = other.edges_;

				return *this;
			}

		public:
			unsigned int id_;

			std::set<unsigned int, std::less<unsigned int >> edges_;

			Point2D location_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_VERTEX_HPP_ */