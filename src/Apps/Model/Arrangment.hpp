/*
 * Arrangment.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef SRC_APPS_MODEL_ARRANGMENT_HPP_
#define SRC_APPS_MODEL_ARRANGMENT_HPP_

#include <map>

#include "Topology/Vertex.hpp"
#include "Topology/Edge.hpp"
#include "Topology/Face.hpp"


#include "Model/IDManager.hpp"


namespace RRM
{
	template < class Real>
	class Arrangment
	{

		typedef	StratigraphicSegment<Real>	Segment;
		typedef typename Segment::Curve2D	Curve2D;


		public:
			Arrangment ( )
			{

			}


			void initialize( )
			{
				clear();
			}

			unsigned int insertCurve( const Curve2D& _curve )
			{

				unsigned int id = edge_index_.getID();

				std::cout << "---- id ----  " << id << std::endl;

				Edge<Real> e;

				e.segment.curve = _curve;

				edges_[id] = e;

				return id;

			}

			void clear()
			{
				vertex_index_.initialize( 1000 );
				edge_index_.initialize( 1000 );
				face_index_.initialize( 1000 );

				vertices_.clear();
				edges_.clear();
				faces_.clear();
			}

			virtual ~Arrangment ( )
			{

			}

			void log ( )
			{

				std::cout << "---- Size ---- " << std::endl;
				std::cout << "- vertices "     << vertices_.size()  << std::endl;
				std::cout << "- edges "        << edges_.size()     << std::endl;
				std::cout << "- faces "        << faces_.size()     << std::endl;
				std::cout << "----  ---- "     << std::endl;
			}
		public:

			IDManager vertex_index_;
			IDManager face_index_;
			IDManager edge_index_;

			std::map<unsigned int , Vertex<Real> > vertices_;
			std::map<unsigned int , Edge<Real> >   edges_;
			std::map<unsigned int , Face<Real> >   faces_;
	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_ARRANGMENT_HPP_ */
