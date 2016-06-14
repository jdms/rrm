/*
 * Arrangment.hpp
 *
 *  Created on: Apr 5, 2016
 *      Author: felipe
 */

#ifndef SRC_APPS_MODEL_CROSSSECTION_HPP_
#define SRC_APPS_MODEL_CROSSSECTION_HPP_

#include <map>
#include <tuple>
#include <algorithm>
#include <list>
#include <iostream>

#include "Topology/Vertex.hpp"
#include "Topology/Edge.hpp"
#include "Topology/Face.hpp"

#include "RegionDetection.hpp"
#include "Model/IDManager.hpp"

#include "Geology/GeologicRules.hpp"


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

			class IntersectionVertex
			{
				public:

					IntersectionVertex( )
					{
						this->input_curve  	= 0;
						this->source_curve 	= 0;
						this->source_curve_id 	= 0;
						this->location_     	= Point2D(0.0,0.0);
						this->is_extreme   	= false;
						this->vertex_id_ 	= 0;
					}

					IntersectionVertex( std::size_t  _input_curve,
							    std::size_t  _source_curve,
							    unsigned int _source_curve_id,
							    Point2D _location,
							    bool _is_extreme,
							    unsigned int _id )
					{
						this->input_curve     = _input_curve;
						this->source_curve    = _source_curve;
						this->source_curve_id = _source_curve_id;
						this->location_        = _location;
						this->is_extreme      = _is_extreme;
						this->vertex_id_      = _id;

					}

					IntersectionVertex ( const IntersectionVertex& other )
					{
						*this = other;
					}

					IntersectionVertex& operator= ( const IntersectionVertex& other )
					{
						this->input_curve     = other.input_curve;
						this->source_curve    = other.source_curve;
						this->source_curve_id = other.source_curve_id;
						this->location_        = other.location_;
						this->is_extreme      = other.is_extreme;

						this->vertex_id_      = other.vertex_id_;

						return *this;
					}
					bool operator< ( const IntersectionVertex& c ) const
					{
						return this->input_curve < c.input_curve;
					}

					bool operator> ( const IntersectionVertex& c ) const
					{
						return this->input_curve > c.input_curve;
					}

					std::size_t  input_curve;
					std::size_t  source_curve;
					unsigned int source_curve_id;
					Point2D location_;
					bool is_extreme;

					// User to create the vertices
					unsigned int vertex_id_;

			};

			CrossSection ( )
			{
				id_ = 0;
				viewPort_.first = Point2D(0,0);
				viewPort_.second = Point2D(700,400);
				current_rule = GeologicRules::Sketch;
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
				curve_index_  = other.curve_index_;

				vertices_ = other.vertices_;
				edges_    = other.edges_;
				faces_    = other.faces_;
				curves_history_ = other.curves_history_;

				id_ 	  = 	other.id_;

				image_    = other.image_;

				viewPort_ = other.viewPort_;

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
				curve_index_.initialize ( 1000 );

				vertices_.clear ( );
				edges_.clear ( );
				faces_.clear ( );
				curves_history_.clear ( );
			}

			void setRule( GeologicRules _new_rule )
			{
				this->current_rule = _new_rule;
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
				segment_1.superSample(3.0);
				// (width,y)  (x,y)  (x,height)
				//   v2----------*-----------v1
				Curve2D segment_2;
				segment_2.add(Point2D ( width , y ));
				segment_2.add(Point2D ( x , y ));
				segment_2.add(Point2D ( x , height ));
				segment_2.superSample(3.0);

				Edge<Real> e1;
				e1.id_ = edge_index_.getID();;
				e1.segment.curve = segment_1;
				e1.is_boundary_ = true;

				Edge<Real> e2;
				e2.id_ = edge_index_.getID();
				e2.segment.curve = segment_2;
				e2.is_boundary_ = true;

				Vertex<Real> v1;
				v1.id_ = vertex_index_.getID();
				v1.edges_.insert(e1.id_);
				v1.edges_.insert(e2.id_);
				v1.location_ = Point2D(x,height);

				Vertex<Real> v2;
				v2.id_ = vertex_index_.getID();
				v2.edges_.insert(e1.id_);
				v2.edges_.insert(e2.id_);
				v2.location_ = Point2D ( width , y );

				e1.source_id_ = v1.id_;
				e1.target_id_ = v2.id_;

				e2.source_id_ = v2.id_;
				e2.target_id_ = v1.id_;

				/// TODO Handle to an element on the Arrangement
				/// add to list of edges
				edges_[e1.id_] = e1;
				/// add to list of edges
				edges_[e2.id_] = e2;
				/// add to list of vertices
				vertices_[v1.id_] = v1;
				/// add to list of vertices
				vertices_[v2.id_] = v2;

				std::cout << " New Boundary " << std::endl;

				viewPort_.first  = Point2D(x,y);
				viewPort_.second = Point2D(width,height);

				this->log();

			}

			/// Insert the curve without any restriction
			unsigned int insertCurve( const Curve2D& _curve )
			{

				// PolygonalCurve2D output
				std::vector<std::size_t> thisIndex;
				std::vector<Real>        thisAlphas;
				std::vector<std::size_t> testIndex;
				std::vector<Real> 	 testAlphas;
				std::vector<Point2D>     intersection_points;

				// Save all intersection vertex to later, sort it increase order.
				std::vector<IntersectionVertex> intersection_vertices;
				// Save all intersection vertex to later, sort it increase order.
				std::vector<std::size_t>  intersected_edges;

				std::vector<std::pair<std::size_t,Point2D> > intersection_pair;

				Curve2D test;

				/// The intersection has a limited precision. Using douglasPeuckerSimplify as 1.0
				/// Increase precision in the upcoming releases.
				if ( _curve.size() < 5 )
				{
					return 0;
				}

				test = _curve;

				//_curve.douglasPeuckerSimplify(test,0.5);
				std::cout << "CURVE SIZE" << test.size() << std::endl;
				test.lineFilter(0.5,4);
				test.meanFilter();
				std::cout << "CURVE SIZE After" << test.size() << std::endl;


				if ( test.back().x() < test.front().x())
				{
					test.reverse();
				}

				// Intersetion over the current CrossSection
				for ( auto& edge_iterator: edges_ )
				{
					// Test input curve against the current Arrangement
					// Find the intersection points

					if ( (!edge_iterator.second.is_visible_) && (!edge_iterator.second.is_enable_) )
					{
						continue;
					}
					if ( edge_iterator.second.segment.curve.intersections ( test , thisIndex , thisAlphas, testIndex ,testAlphas, intersection_points ) )
					{

						if ( thisIndex.size ( ) == 2 )
						{
							for ( std::size_t it = 0; it < testIndex.size(); it++)
							{
								IntersectionVertex vi (testIndex[it],
										      thisIndex[it],
										      edge_iterator.second.id_,
										      intersection_points[it],
										      false,
										      0);

								intersection_vertices.push_back(vi);
							}

							intersected_edges.push_back(edge_iterator.second.id_);

						}
						else if ( thisIndex.size ( ) == 1 )
						{

							for ( std::size_t it = 0; it < testIndex.size(); it++)
							{

								IntersectionVertex vi ( testIndex[it],
										        thisIndex[it],
										        edge_iterator.second.id_,
										        intersection_points[it],
										        false,
										        0);

								intersection_vertices.push_back(vi);

							}

							intersected_edges.push_back(edge_iterator.second.id_);

						}
						else
						{
							// Case not accepted
							std::cout << "Case not accepted" << std::endl;
							return 0;
						}

					}else
					{
						/// no intersection
						continue;
					}

				}

				// Not a valid Curve
				if ( intersection_vertices.size () == 1 )
				{
					std::cout << " Only hanging Points " << thisIndex.size ( ) << std::endl;
					return 0;
				}
				else if ( intersection_vertices.size () >= 2 )
				{

					std::sort(intersection_vertices.begin(),intersection_vertices.end());

					intersection_vertices.front().is_extreme = true;
					intersection_vertices.back().is_extreme  = true;

					for ( auto& vertex_iterator: intersection_vertices)
					{
						Vertex<Real> vertex;
						vertex.id_ = vertex_index_.getID();
						vertex.location_ = vertex_iterator.location_;

						vertex_iterator.vertex_id_ = vertex.id_;

						vertices_[vertex.id_] = vertex;

						intersection_pair.push_back(std::make_pair(vertex_iterator.input_curve,vertex_iterator.location_));
					}

					std::cout << " Test Size " << test.size() << std::endl;

					test.addPoints(intersection_pair);

					std::cout << " after Test Size " << test.size() << std::endl;

					std::deque<Curve2D> segments;

					// Used into the segmentation of the input Curve
					std::vector<std::size_t> indices;
					// Extract pair element  http://stackoverflow.com/a/24642972
					// Could use auto in lambda expression, with c++ > 11
					std::transform(intersection_pair.begin(), intersection_pair.end(), std::back_inserter(indices), [](std::pair<std::size_t,Point2D> p) { return p.first; });
					// Return the segments of the input curve at the intersection point index.
					test.slices(indices,segments);


					// Curve History
						Segment source_sketch;
						source_sketch.curve_index = curve_index_.getID();
						curves_history_[source_sketch.curve_index] = source_sketch;

					std::cout << " Segments Size " << segments.size() << std::endl;
					// There is no source
					unsigned int last_edge;
					Edge<Real> hanging_1;
					hanging_1.id_ = edge_index_.getID();
					hanging_1.segment.curve = segments.front();
					hanging_1.segment.curve.superSample(3.0);
					hanging_1.segment.curve_index = source_sketch.curve_index;
					hanging_1.target_id_ = intersection_vertices.front().vertex_id_;
					hanging_1.is_enable_ = false;
					hanging_1.is_visible_ = false;
					edges_[hanging_1.id_] = hanging_1;
					last_edge = hanging_1.id_;

					for ( std::size_t it = 0; it < intersection_vertices.size()-1; it++)
					{
						Edge<Real> e;
						e.id_ = edge_index_.getID();
						e.segment.curve = segments[it+1];
						e.segment.curve.superSample(3.0);
						e.segment.curve_index = source_sketch.curve_index;
						e.source_id_ = intersection_vertices[it].vertex_id_;
						e.target_id_ = intersection_vertices[it+1].vertex_id_;
						vertices_[intersection_vertices[it].vertex_id_].edges_.insert(e.id_);
						vertices_[intersection_vertices[it].vertex_id_].edges_.insert(last_edge);
						edges_[e.id_] = e;
						last_edge = e.id_;
					}

					// There is no target
					Edge<Real> hanging_2;
					hanging_2.id_ = edge_index_.getID();
					hanging_2.segment.curve = segments.back();
					hanging_2.segment.curve.superSample(3.0);
					hanging_2.segment.curve_index = source_sketch.curve_index;
					hanging_2.source_id_ = intersection_vertices.back().vertex_id_;
					hanging_2.is_enable_ = false;
					hanging_2.is_visible_ = false;
					edges_[hanging_2.id_] = hanging_2;
					vertices_[intersection_vertices.back().vertex_id_].edges_.insert(hanging_2.id_);
					vertices_[intersection_vertices.back().vertex_id_].edges_.insert(last_edge);

					std::vector<IntersectionVertex> edge_vertices;

					/// Updating the CrossSection with new Configuration
					for ( auto edges_id_iterator: intersected_edges  )
					{
						// Take the vertices which cut the source edge.
						for ( auto vertex_iterator: intersection_vertices)
						{
							if ( vertex_iterator.source_curve_id == edges_id_iterator)
							{
								edge_vertices.push_back(vertex_iterator);
							}
						}
						// sort it
						std::sort(edge_vertices.begin(),edge_vertices.end(),
							  [](const IntersectionVertex& left,const IntersectionVertex& right)
							  {
								return left.source_curve < right.source_curve;
							  });

						if ( edge_vertices.size() == 1)
						{

							Curve2D c1;
							Curve2D c2;
								intersection_pair.clear();
								for ( auto& vertex_iterator: edge_vertices)
								{
									intersection_pair.push_back(std::make_pair(vertex_iterator.source_curve,vertex_iterator.location_));
								}

								// Add the Point to the curve and get the new indices.
								edges_[edge_vertices[0].source_curve_id].segment.curve.addPoints(intersection_pair);

								indices.clear();
								// Extract pair element  http://stackoverflow.com/a/24642972
								// Could use auto in lambda expression, with c++ > 11
								std::transform(intersection_pair.begin(), intersection_pair.end(), std::back_inserter(indices), [](std::pair<std::size_t,Point2D> p) { return p.first; });

							edges_[edge_vertices[0].source_curve_id].segment.curve.split ( indices[0], c1 , c2 );
							// before
							// (vs) - e - (vt)
							// C = c1;
							edges_[edge_vertices[0].source_curve_id].segment.curve = c1;
							// after
							// (vs)- e - (v1) - e2 - (vt)

							Edge<Real> e2;
							e2.id_ = edge_index_.getID();
							e2.segment.curve = c2;
							e2.segment.curve.superSample(3.0);
							e2.segment.curve_index = edges_[edge_vertices[0].source_curve_id].segment.curve_index;
							e2.is_boundary_ = edges_[edge_vertices[0].source_curve_id].is_boundary_;

							e2.source_id_ = edge_vertices[0].vertex_id_;
							e2.target_id_ = edges_[edge_vertices[0].source_curve_id].target_id_;

							// Original edge
							edges_[edge_vertices[0].source_curve_id].target_id_ = edge_vertices[0].vertex_id_;

							vertices_[edge_vertices[0].vertex_id_].edges_.insert(edge_vertices[0].source_curve_id);
							vertices_[edge_vertices[0].vertex_id_].edges_.insert(e2.id_);

							// Updating (vt)
							vertices_[e2.target_id_].edges_.erase(edge_vertices[0].source_curve_id);
							vertices_[e2.target_id_].edges_.insert(e2.id_);

							edges_[e2.id_] = e2;

							/// Applying on rule

							if ( (!edge_vertices[0].is_extreme) && (!edges_[e2.id_].is_boundary_) )
							{
//								/// REMOVE BELOW
								if ( (test[edge_vertices[0].input_curve-1].y() > edge_vertices[0].location_.y()) )
								{

									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}

								}else
								{
									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;
									}
								}
								/// REMOVE ABOVE
							}



						}else if ( edge_vertices.size() == 2)
						{
							Curve2D c1;
							Curve2D c2;
							Curve2D c3;

								intersection_pair.clear();
								for ( auto& vertex_iterator: edge_vertices)
								{
									intersection_pair.push_back(std::make_pair(vertex_iterator.source_curve,vertex_iterator.location_));
								}

								edges_[edge_vertices[0].source_curve_id].segment.curve.addPoints(intersection_pair);

								indices.clear();
								// Extract pair element  http://stackoverflow.com/a/24642972
								// Could use auto in lambda expression, with c++ > 11
								std::transform(intersection_pair.begin(), intersection_pair.end(), std::back_inserter(indices), [](std::pair<std::size_t,Point2D> p) { return p.first; });

							edges_[edge_vertices[0].source_curve_id].segment.curve.split ( indices[0],indices[1], c1 , c2 , c3);

							// before
							// (vs) - e - (vt)
							// C = c1;
							edges_[edge_vertices[0].source_curve_id].segment.curve = c1;
							// after
							// (vs)- e - (v1) - e2 - (vt)
							Edge<Real> e2;
							e2.id_ = edge_index_.getID();
							e2.segment.curve = c2;
							e2.segment.curve.superSample(3.0);
							e2.segment.curve_index = edges_[edge_vertices[0].source_curve_id].segment.curve_index;
							e2.is_boundary_ = edges_[edge_vertices[0].source_curve_id].is_boundary_;

							Edge<Real> e3;
							e3.id_ = edge_index_.getID();
							e3.segment.curve = c3;
							e3.segment.curve.superSample(3.0);
							e3.segment.curve_index = edges_[edge_vertices[0].source_curve_id].segment.curve_index;
							e3.is_boundary_ = edges_[edge_vertices[0].source_curve_id].is_boundary_;


							e2.source_id_ = edge_vertices[0].vertex_id_;
							e2.target_id_ = edge_vertices[1].vertex_id_;

							e3.source_id_ = edge_vertices[1].vertex_id_;
							e3.target_id_ = edges_[edge_vertices[0].source_curve_id].target_id_;

							// Original edge
							edges_[edge_vertices[0].source_curve_id].target_id_ = edge_vertices[0].vertex_id_;

							vertices_[edge_vertices[0].vertex_id_].edges_.insert(edge_vertices[0].source_curve_id);
							vertices_[edge_vertices[0].vertex_id_].edges_.insert(e2.id_);

							vertices_[edge_vertices[1].vertex_id_].edges_.insert(e2.id_);
							vertices_[edge_vertices[1].vertex_id_].edges_.insert(e3.id_);

//							// Updating (vt)
							vertices_[e3.target_id_].edges_.erase(edge_vertices[0].source_curve_id);
							vertices_[e3.target_id_].edges_.insert(e3.id_);

							edges_[e2.id_] = e2;
							edges_[e3.id_] = e3;


							if ( ( !edge_vertices[0].is_extreme && !edge_vertices[1].is_extreme ) && ( !edges_[e2.id_].is_boundary_ ) )
							{
//								/// REMOVE BELOW
								if ( ( test[edge_vertices[0].input_curve - 1].y ( ) > edge_vertices[0].location_.y ( ) ) )
								{

									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;

										edges_[e3.id_].is_visible_ = false;
										edges_[e3.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}

								}

//								/// REMOVE Above
								else
								{
									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;

										edges_[e3.id_].is_visible_ = false;
										edges_[e3.id_].is_enable_ = false;
									}
								}

							}else if ( (!edge_vertices[0].is_extreme) && (!edges_[e2.id_].is_boundary_) )
							{
//								/// REMOVE BELOW
								if ( (test[edge_vertices[0].input_curve-1].y() > edge_vertices[0].location_.y()) )
								{

									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}

								}else
								{
									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[edge_vertices[0].source_curve_id].is_enable_ = false;
										edges_[edge_vertices[0].source_curve_id].is_visible_ = false;
									}
								}
								/// REMOVE ABOVE
							}else if ( (!edge_vertices[1].is_extreme) && (!edges_[e2.id_].is_boundary_) )
							{
//								/// REMOVE BELOW
								if ( (test[edge_vertices[1].input_curve-1].y() > edge_vertices[1].location_.y()) )
								{

									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[e3.id_].is_visible_ = false;
										edges_[e3.id_].is_enable_ = false;
									}

								}else
								{
									if ( current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION )
									{
										edges_[e3.id_].is_visible_ = false;
										edges_[e3.id_].is_enable_ = false;
									}
									else if ( current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION )
									{
										edges_[e2.id_].is_visible_ = false;
										edges_[e2.id_].is_enable_ = false;
									}
								}
								/// REMOVE ABOVE
							}


						}else
						{
							std::cout << "Case not accepted" << std::endl;
						}

						for(auto& vertex_iterator: vertices_ )
						{
							vertex_iterator.second.vertices_.clear();
							for ( auto edges_iterator: vertex_iterator.second.edges_ )
							{

								if ( (edges_[edges_iterator].source_id_ != 0) && ((edges_[edges_iterator].source_id_ != vertex_iterator.second.id_)) )
								{
									vertex_iterator.second.vertices_.insert ( edges_[edges_iterator].source_id_ );
								}
								if ( (edges_[edges_iterator].target_id_ != 0 ) && (edges_[edges_iterator].target_id_ != vertex_iterator.second.id_))
								{
									vertex_iterator.second.vertices_.insert ( edges_[edges_iterator].target_id_ );
								}

							}
						}

						edge_vertices.clear();
					}

				}
				else
				{
					return 0;
				}

//				std::cout << " New Configuration " << std::endl;
//				for(auto& vertex_iterator: vertices_ )
//				{
//					// Case not accepted
//					std::cout << "Vertex Valence : " << vertex_iterator.second.edges_.size() << std::endl;
//					for ( auto vit: vertex_iterator.second.vertices_ )
//					{
//						std::cout << "vertex id : " << vertex_iterator.second.id_ <<  " - " << vit << std::endl;
//					}
//					for ( auto edges_iterator: vertex_iterator.second.edges_ )
//					{
//						std::cout << "edges id : " << edges_iterator << " s " << edges_[edges_iterator].source_id_ << " t " << edges_[edges_iterator].target_id_ << std::endl;
//					}
//					std::cout << "---" << std::endl;
//				}
//
//				updateFaces();

				log();
				return 1;
			}

			void updateFaces()
			{

				RegionDetection regions;

				for ( auto vertex_iterator: vertices_)
				{
					int exist = 0;
					for ( auto edge_iterator: vertex_iterator.second.edges_)
					{
						if ( edges_[edge_iterator].is_visible_ &&  edges_[edge_iterator].is_enable_ )
						{
							if ( edges_[edge_iterator].source_id_ == vertex_iterator.second.id_ )
							{
								RegionDetection::Path p(edges_[edge_iterator].target_id_,0);
								for ( auto path : regions.vm_[vertex_iterator.second.id_] )
								{
									if ( path.first == p.first )
									{
										p.second = exist;
										exist++;
										path.second = exist;
									}
								}
								regions.vm_[vertex_iterator.second.id_].push_back(p);
							}else
							{
								RegionDetection::Path p(edges_[edge_iterator].source_id_,0);
								for ( auto path : regions.vm_[vertex_iterator.second.id_] )
								{
									if ( path.first == p.first )
									{
										p.second = exist;
										exist++;
										path.second = exist;
									}
								}
								regions.vm_[vertex_iterator.second.id_].push_back(p);
							}
						}

						//std::cout << "vertex id : " << vertex_iterator.second.id_ <<  " - " << path << std::endl;
					}
				}

				regions.printGraph(regions.vm_);
			}

			void log ( ) const
			{
				std::cout << "---- Size ---- " << std::endl;
				std::cout << "- vertices --- " << vertices_.size()  << std::endl;
				std::cout << "- edges ------ " << edges_.size()     << std::endl;
				std::cout << "- faces ------ " << faces_.size()     << std::endl;
				std::cout << "--viewPort---- " << viewPort_.second.x() <<std::endl;
				std::cout << "-------------- " << std::endl;
			}

		public:
			std::map<unsigned int , Vertex<Real> > vertices_;
			std::map<unsigned int , Edge<Real> >   edges_;
			std::map<unsigned int , Face<Real> >   faces_;

			std::map<unsigned int , Segment >   curves_history_;

			unsigned int id_;

			std::vector<unsigned char> image_;

			IDManager vertex_index_;
			IDManager face_index_;
			IDManager edge_index_;

			IDManager curve_index_;

			std::pair<Point2D,Point2D> viewPort_;

			GeologicRules current_rule;


	};

} /* namespace RRM */

#endif /* SRC_APPS_MODEL_CROSSSECTION_HPP_ */
