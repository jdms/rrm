/*
 * Arrangment.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef SRC_APPS_MODEL_CROSSSECTION_HPP_
#define SRC_APPS_MODEL_CROSSSECTION_HPP_

#include <map>
#include <algorithm>

#include "Topology/Vertex.hpp"
#include "Topology/Edge.hpp"
#include "Topology/Face.hpp"


#include "Model/IDManager.hpp"


namespace RRM
{
	template < class _Real>
	class CrossSection
	{
		public:
			/// \todo design a geometry trait for all model classes
			typedef _Real		 		Real;
			typedef CrossSection<Real>		Self;
			typedef	StratigraphicSegment<Real>	Segment;
			typedef typename Segment::Curve2D	Curve2D;
			typedef typename Segment::Point2D	Point2D;

			CrossSection ( )
			{
				id_ = 0;
			}

			CrossSection ( const Self& _cross_section )
			{
				*this = _cross_section;
			}

			~CrossSection ( )
			{

			}

			/// Most important Operation
			Self& operator=(const Self& other )
			{
				// Assign to all  member.
				vertex_index_ = other.vertex_index_;
				edge_index_   = other.edge_index_;
				face_index_   = other.face_index_;

				vertices_ = other.vertices_;
				edges_    = other.edges_;
				faces_    = other.faces_;

				id_ 	  = 	other.id_;

				image_    = other.image_;

				return *this;
			}

			void initialize( Real x , Real y , Real width , Real height )
			{
				clear();
				boundary(x,y,width,height);
			}

			void initialize( )
			{
				clear();
			}

			void clear ( )
			{
				vertex_index_.initialize ( 1000 );
				edge_index_.initialize ( 1000 );
				face_index_.initialize ( 1000 );

				vertices_.clear ( );
				edges_.clear ( );
				faces_.clear ( );

				id_ = 0;
			}

			/// Quad in coordinates  (x , y) (Default OpenGL Coordinate System)
			void boundary ( Real x , Real y , Real width , Real height )
			{
				// (x,height)  (width, height)  (width,y)
				//   v1----------*-----------v2
				Curve2D segment_1;
				segment_1.add(Point2D ( x , height ));
				segment_1.add(Point2D ( width , height ));
				segment_1.add(Point2D ( width , y ));
				// (width,y)  (x,y)  (x,height)
				//   v2----------*-----------v1
				Curve2D segment_2;
				segment_2.add(Point2D ( width , y ));
				segment_2.add(Point2D ( x , y ));
				segment_2.add(Point2D ( x , height ));

				Edge<Real> e1;
				e1.id_ = edge_index_.getID();;
				e1.segment.curve = segment_1;
				e1.is_boundary_ = true;
				/// add to list of edges
				edges_[e1.id_] = e1;

				Edge<Real> e2;
				e2.id_ = edge_index_.getID();
				e2.segment.curve = segment_2;
				e2.is_boundary_ = true;
				/// add to list of edges
				edges_[e2.id_] = e2;

				Vertex<Real> v1;
				v1.id_ = vertex_index_.getID();
				v1.edges_.insert(e1.id_);
				v1.edges_.insert(e2.id_);
				v1.location_ = Point2D(x,height);
				/// add to list of vertices
				vertices_[v1.id_] = v1;

				Vertex<Real> v2;
				v2.id_ = vertex_index_.getID();
				v2.edges_.insert(e1.id_);
				v2.edges_.insert(e2.id_);
				v2.location_ = Point2D ( width , y );
				/// add to list of vertices
				vertices_[v2.id_] = v2;

				e1.source_id_ = v1.id_;
				e1.target_id_ = v2.id_;

				e2.source_id_ = v2.id_;
				e2.target_id_ = v1.id_;

				std::cout << " New Boundary " << std::endl;

				this->log();

			}

			/// Insert the curve without any restriction
			unsigned int insertCurve( const Curve2D& _curve )
			{

//				std::vector<std::size_t> thisIndex;
//				std::vector<std::size_t> testIndex;
//				std::vector<Point2D> prPoints;
//
//				std::vector<std::size_t> intersection_indices;
//				std::vector<std::size_t> intersection_points;
//
//				Curve2D test = _curve;
//
//				for ( auto& edge_iterator: edges_ )
//				{
//					edge_iterator.second.segment.curve.intersectionPolygonalCurve2D(test,thisIndex,testIndex,prPoints );
//					for ( std::size_t it = 0; it < prPoints.size(); it++)
//					{
////						Vertex<Real> v;
////						v.id_ = vertex_index_.getID();
////						v.location_ = prPoints[it];
////						vertices_[v.id_] = v;
//					}
//
//					intersection_indices.insert(intersection_indices.end(),testIndex.begin(),testIndex.end());
//				}
//
//				std::sort(intersection_indices.begin(),intersection_indices.end(),std::less<std::size_t>());
//
//				std::deque<Curve2D> segments;
//				segments.resize(3);
//
//				//test.split(intersection_indices[0],intersection_indices[1],segments[0],segments[1],segments[2]);
//
//				//segments.pop_front();
//				//segments.pop_back();
//
//				std::cout << " size segment " << intersection_indices.size() << std::endl;
//
//				for ( auto& vertex_iterator: intersection_indices)
//				{
//					std::cout << " Vertex " << segments.size() << std::endl;
//					Vertex<Real> v;
//					v.id_ = vertex_index_.getID();
//					v.location_ = test[vertex_iterator];
//					vertices_[v.id_] = v;
//
//				}

//
//				for ( auto& segment_iterator: segments)
//				{
					unsigned int id = edge_index_.getID();

//					std::cout << "---- id ----  " << segment_iterator.size() << std::endl;

					Edge<Real> e;

					e.segment.curve = _curve;

					edges_[id] = e;
//				}

				return 0;
			}


			void log ( ) const
			{
				std::cout << "---- Size ---- " << std::endl;
				std::cout << "- vertices --- " << vertices_.size()  << std::endl;
				std::cout << "- edges ------ " << edges_.size()     << std::endl;
				std::cout << "- faces ------ " << faces_.size()     << std::endl;
				std::cout << "-------------- " << std::endl;
			}

		public:
			std::map<unsigned int , Vertex<Real> > vertices_;
			std::map<unsigned int , Edge<Real> >   edges_;
			std::map<unsigned int , Face<Real> >   faces_;

			unsigned int id_;

			std::vector<unsigned char> image_;

			IDManager vertex_index_;
			IDManager face_index_;
			IDManager edge_index_;


	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_CROSSSECTION_HPP_ */
