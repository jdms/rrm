/*
 * Vertex.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef _TOPOLOGY_VERTEX_HPP_
#define _TOPOLOGY_VERTEX_HPP_

#include "Model/StratigraphicSegment.hpp"

namespace RRM
{

	template < class Real>
	class Vertex
	{
		typedef	StratigraphicSegment<Real>	Segment;
		typedef typename Segment::Curve2D	Curve2D;
		typedef typename Segment::Point2D	Point2D;

		public:
			Vertex ( )
			{
				id_ = 0;
			}
			virtual ~Vertex ( )
			{

			}
		public:
			unsigned int id_;

			Point2D location_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_TOPOLOGY_VERTEX_HPP_ */
