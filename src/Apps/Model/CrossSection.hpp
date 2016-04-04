/*
 * GeologicalCrossSection.hpp
 *
 *  Created on: Aug 15, 2015
 *      Author: felipe
 *
 *  TODO GeologicalCrossSection is Temporary Name !
 */

// C++ Libraries
#include <iostream>
#include <string>    // std::to_string
#include <deque>     // std::deque
#include <list>      // std::list
#include <vector>    // std::vector
#include <algorithm> // std::sort
#include <functional>// std::less

#ifndef _CROSSSECTIONMESH_HPP_
#define _CROSSSECTIONMESH_HPP_

// CurveNDimension
#include "Core/Geometry/PolygonalCurve2D.hpp"
// Our Half-Edge items to DCEL
#include "Topology/CrossSection_Items.hpp"
// RRM Decorator
#include "Topology/CrossSection_Decorator.hpp"
// @CGAL Library
#include <CGAL/HalfedgeDS_default.h>
#include <CGAL/HalfedgeDS_decorator.h>
// Edge Iterator @see http://doc.cgal.org/4.7/HalfedgeDS/HalfedgeDS_2hds_prog_edge_iterator_8cpp-example.html
#include <CGAL/N_step_adaptor.h>

// Kernel
#include <CGAL/Simple_cartesian.h>

#include "StratigraphicCurves.hpp"
#include "StratigraphicSegment.hpp"
#include "Geology/GeologicRules.hpp"
#include "IDManager.hpp"

namespace RRM
{
	/// @brief Main class of the SBIM module. It stores the post-processed stratigraphy (represented as curves). It also contains the geological rules, which operate on the stratigraphy.
	/// In out case, Polygonal Curves
	/// This class, intent to be a CGAL::HalfedgeDS_decorator suitable to our application demands.
	/// The particularities include: insert a curve give a geologic rules; keep history of input curves
	/// according of which curve is intersect, by given the geologic attributes, decides which operator perform.
	template < class Real >
	class CrossSection
	{
		public:
			typedef VectorN<Real, 2> Vector2D;
			typedef PointN<Real, 2> Point2D;
			typedef PolygonalCurve2D<Real> Curve2D;

			typedef StratigraphicCurves<Real>  Stratigraphic;
			typedef StratigraphicSegment<Real> Segment;

			typedef CGAL::Simple_cartesian<Real> Kernel;
			typedef typename Kernel::Point_2 CGAL_Point2D;
			typedef CGAL::HalfedgeDS_default<int, CrossSectionItems<Real> > DCEL;
			typedef CGAL::HalfedgeDS_decorator<DCEL> Decorator;
			typedef RRM::CrossSection_Decorator<DCEL> CrossSectionDecorator;
			typedef typename DCEL::Halfedge Halfedge;
			typedef typename DCEL::Halfedge_handle Halfedge_handle;
			typedef typename Halfedge::Base HBase;
			typedef typename DCEL::Halfedge_iterator Halfedge_iterator;
			typedef CGAL::N_step_adaptor<Halfedge_iterator, 2> Segment_iterator;

			typedef typename DCEL::Vertex Vertex;
			typedef typename DCEL::Vertex_handle Vertex_handle;
			typedef typename DCEL::Vertex_iterator Vertex_iterator;
			typedef typename DCEL::Face Face;
			typedef typename DCEL::Face_handle Face_handle;
			typedef typename DCEL::Face_iterator Face_iterator;

			typedef typename std::vector<Segment>::iterator curves_iterator;

			/*!
			 * @brief Struct to store curve segments that can be targeted by a geological rule.
			 *  It stores segments of the input curve, the current geological model, and also decides, based on the geological rules, which curve segment need to be discarded.
			 */
			struct CurveRules
			{
					CurveRules ( )
					{

					}

					CurveRules ( const CurveRules& other )
					{
						s = other.s;
						curve_indices = other.curve_indices;
						segment_indices = other.segment_indices;
						segment_alpha = other.segment_alpha;
						intersection_points = other.intersection_points;
					}

					CurveRules& operator= ( const CurveRules& other )
					{
						/// Assign to all  member.
						s = other.s;
						curve_indices = other.curve_indices;
						segment_indices = other.segment_indices;
						segment_alpha = other.segment_alpha;
						intersection_points = other.intersection_points;

						return *this;
					}

					Segment_iterator s;

					std::vector<std::size_t> curve_indices;

					std::vector<std::size_t> segment_indices;
					std::vector<Real> segment_alpha;
					std::vector<Point2D> intersection_points;
			};

			/*!
			 * @brief  Struct to store intersection points of of input curves. It helps in the curve segmentation by keeping the intersection points sorted out
			 */
			struct CurveSlice
			{
					CurveSlice ( )
					{
						index = -1;
					}

					CurveSlice ( std::size_t i , const Point2D& p )
					{
						index = i;
						point = p;
					}

					CurveSlice ( const CurveSlice& other )
					{
						index = other.index;
						point = other.point;
					}

					CurveSlice& operator= ( const CurveSlice& other )
					{
						// Assign to all  member.
						index = other.index;
						point = other.point;

						return *this;
					}
					bool operator< ( const CurveSlice& c ) const
					{
						return index < c.index;
					}

					bool operator> ( const CurveSlice& c ) const
					{
						return index > c.index;
					}

					std::size_t index;
					Point2D point;
			};

			bool compareByLength ( const CurveSlice &a , const CurveSlice &b )
			{
				return a.index < b.index;
			}

			std::vector<CurveRules> curve_sew;
			std::vector<CurveSlice> curve_slice;

			bool is_ready;

			Curve2D raw_curve_;

			Stratigraphic canonical_boundary;

			DCEL topology_;
			std::vector<Point2D> segment_1;
			std::vector<Point2D> segment_2;

			std::vector<Stratigraphic> curves_history_;

			std::vector<Segment> sketch_curves;

			GeologicRules current_rule;

			CrossSection ( )
			{
				is_ready = false;
				current_rule = Sketch;

			}

			// FIXME Create crossSection with boundary as argument.
			CrossSection ( Real x , Real y , Real width , Real height )
			{
				is_ready = initialization ( x , y , width , height );
				current_rule = Sketch;
			}
			// FIXME Test whether the boundary is valid.
			// For while, testing with the input is a rectangle.
			bool initialization ( Real x , Real y , Real width , Real height )
			{
				// (x,y)   (width,y)    (width,height)
				//   |----------*----------->
			segment_1 =
			{	Point2D ( x , y ), Point2D ( width , y ), Point2D ( width , height )};
			// (x,y)     (height,0)    (width,height)
			//   |----------*----------->
			segment_2 =
			{	Point2D ( x , y ), Point2D ( x , height ), Point2D ( width , height )};

			boundary(segment_1,segment_2);

			//boundary_sketch(segment_1,segment_2);

			return is_ready;
		}

		// How to create a proper ! @see https://www.youtube.com/watch?v=6mdBw3SG5io
		virtual ~CrossSection ( )
		{

		}

		void changeRule( GeologicRules the_rule )
		{
			current_rule = the_rule;
		}

		bool insertCurve ( Curve2D curve )
		{
			Stratigraphic s(curve);

			std::vector<Curve2D> segments;

			return insertSegment( s, segments);
		}

		void clear()
		{
			topology_.clear();

			boundary(segment_1,segment_2);

		}

		protected:

		private:

		// FIXME Test whether is a close curve (2D) or a mainfold surface (3D).
		// @see https://en.wikipedia.org/wiki/Differentiable_manifold

		/// @brief The initial boundary is a Face with 2 halfedge
		/// The end Points are repeated, thus, curve_1.end().point() == curve_2.begin().point()
		/// Make life easy when translate.
		///			            x
		///                    / \
		///                    \ /
		///                     x
		/// Slice the curve in half: segment_1, segment_2
		bool boundary ( std::vector<Point2D> segment_1, std::vector<Point2D> segment_2 )
		{
			if ( true )
			{
				// Valid boundary
				std::cout << "The boundary is closed, and size is :" << canonical_boundary.curve.size() << std::endl;

			}
			else
			{
				/// TODO Message " Cannot add a open boundary "
				std::cerr << " Not a closed geometry" << std::endl;
				return false;
			}

			// Two Curve/Surface defining the boundary of the sketch.
			Segment canonical_boundary_segment_1;
			Segment canonical_boundary_segment_2;

			canonical_boundary_segment_1.curve.setCurve(segment_1);
			canonical_boundary_segment_2.curve.setCurve(segment_2);

			// Fresh DCEL
			topology_.clear();
			//
			// *------
			// |      |
			//  ------*
			// The Most important Class for CGAL. Ensure changes;
			CrossSectionDecorator d(topology_);
			// First segment of the boundary
			Halfedge_handle h1 = d.create_loop();
			// Second segment of the boundary
			Halfedge_handle h2 = d.create_loop();

			h1->face()->name = "outside";
			h1->opposite()->face()->name = "inside";

			h1->left_to_right = 1;
			h1->is_boundary = true;
			h1->opposite()->left_to_right = 0;
			h1->opposite()->is_boundary = true;
			h1->segment.curve.setCurve(segment_1);
			h1->segment.curve.superSample(3.0);
			h1->is_visible = true;

			Curve2D test;

			test.push_back(Point2D(0.0,0.0));
			test.push_back(Point2D(0.0,100.0));

			test.superSample(1.0);

			std::vector<std::size_t> slices_indices =
			{	1,99};
			std::deque<Curve2D> slices_curves;

			test.slices(slices_indices,slices_curves);

			Curve2D c1,c2,c3;

			test.split(10,50,c1,c2,c3);

			std::vector<Curve2D> segs =
			{	c1,c2,c3};

			// Deleting h2 faces, to keep the structure consistent
			d.faces_erase(h2->face());
			d.faces_erase(h2->opposite()->face());
			// Grab h1 face information
			d.set_face(h2,h1->opposite()->face());// inside
			d.set_face(h2->opposite(),h1->face());// outside

			h2->left_to_right = 1;
			h2->is_boundary = true;
			h2->opposite()->left_to_right = 0;
			h2->opposite()->is_boundary = true;
			h2->segment.curve.setCurve(segment_2);
			h2->segment.curve.superSample(3.0);
			h2->is_visible = true;

			// Outside the boundary: face()->name == outside;
			h1->HBase::set_next(h2->opposite());
			h1->HBase::set_prev(h2->opposite());

			// Inside the boundary: face()->name == inside;
			h1->opposite()->HBase::set_next(h2);
			h1->opposite()->HBase::set_prev(h2);

			// Outside the boundary: face()->name == outside;
			h2->HBase::set_next(h1->opposite());
			h2->HBase::set_prev(h1->opposite());

			// Inside the boundary: face()->name == inside;
			h2->opposite()->HBase::set_next(h1);
			h2->opposite()->HBase::set_prev(h1);

			return true;
		}

		// A horizon as a input ( cut the boundary twice )
		// If the input curve have only one intersection point, that means,
		// the input is invalid;

		// FIXME For while, it uses the closest segment point as intersection point.
		// More precise/robust/fast algorithm from the geometry package is needed.
		bool insertSegment( Stratigraphic curve, std::vector<Curve2D>& segments)
		{

			if ( curve.curve.size() < 5 )
			{
				return false;
			}
			/// Assume that the curve do not have self intersection
			if (curve.curve.front().x() > curve.curve.back().x())
			{
				curve.curve.revert();
				std::cout << "revert" << std::endl;
			}

			std::vector<std::size_t> curve_all_indices;
			std::deque<Point2D> curve_all_intersections;
			std::vector<std::size_t> curve_indices;
			std::vector<Real> curve_alpha;

			// Testing new slice
			std::deque<Curve2D> curve_all_segments;

			// VERY IMPORTANT  --- need a decorator to ensure halfedge update
			Decorator d(topology_);
			Segment_iterator s;

			curve_sew.clear();
			curve_slice.clear();

			for ( s = topology_.halfedges_begin(); s != topology_.halfedges_end();s++)
			{

				std::vector<std::size_t> segment_indices;
				std::vector<Real> segment_alpha;
				std::vector<Point2D> intersection_points;

				if (s->left_to_right)
				{

					if ( s->is_visible )
					{
						curve.curve.intersections ( s->segment.curve , curve_indices , curve_alpha , segment_indices , segment_alpha , intersection_points );

						if ( segment_indices.size ( ) > 2 )
						{
							std::cout << "More than 2 Intersections: " << curve_indices.size ( ) << std::endl;
							// unacceptable, just return false
							return false;
						}

						curve_all_indices.insert ( curve_all_indices.end ( ) , curve_indices.begin ( ) , curve_indices.end ( ) );
						curve_all_intersections.insert ( curve_all_intersections.end(),intersection_points.begin(),intersection_points.end());

						for (std::size_t it = 0; it < curve_indices.size(); it++)
						{
							curve_slice.push_back(CurveSlice(curve_indices[it],intersection_points[it]));
						}

						CurveRules c;

						c.s = s;
						c.curve_indices = curve_indices;
						c.segment_indices = segment_indices;
						c.segment_alpha = segment_alpha;
						c.intersection_points = intersection_points;

						curve_sew.push_back ( c );
					}
					// curve_sew.back().s->is_boundary = s->is_boundary;
				}
			}

			// Because the order of half-edge can vary. The segments are Left-Right, thus, the intersection.
			// Indices are also Left-Right.

			std::sort(curve_slice.begin(),curve_slice.end(),std::less<CurveSlice>());

			std::cout << " Curve Slice " << curve_slice.size() << std::endl;

			std::sort(curve_all_indices.begin(),curve_all_indices.end(),std::less<std::size_t>());

			// Just Sketching. nothing to worry about.
			// One surface is generate.
			// One Face have to be split

			if ( (current_rule == GeologicRules::Sketch) && (curve_slice.size ( ) > 2))
			{
				return false;
			}
			else if ( ( (current_rule == GeologicRules::REMOVE_ABOVE_INTERSECTION)   ||
                                    (current_rule == GeologicRules::REMOVE_BELOW_INTERSECTION) ) && (curve_slice.size ( ) <= 2) )
			{
				return false;
			}

			// Just hanging or internal lines.
			// One intersection does not change the geometry. It does not create any new surface.
			if ( curve_all_indices.size() <= 1)
			{
				return false;
			}
			// Here, surfacing is crossing. We need to apply one rule, Above/Below
			// More than two intersections means the surface cross one or more surface.
			// By using the geologic rules, we can choose which surface will be kept.
			else
			{
				curve.curve.slices(curve_all_indices,curve_all_segments);

			    // Assume the most left segment and the most right segment are hanging.
				curve_all_segments.pop_front();
				curve_all_segments.pop_back();

				if ( curve_all_segments.size() == 1 )
				{
					std::cout << "More than 3: " << std::endl;
					// return false;
				}

				for ( std::size_t it = 0; it < curve_slice.size( ) - 1; it++)
				{
					Halfedge_handle h = d.create_loop();
					h->left_to_right = 1;
					h->opposite()->left_to_right = 0;

					curve_all_segments[it].front() = curve_slice[it].point;
					curve_all_segments[it].back() = curve_slice[it+1].point;

					h->segment.curve = curve_all_segments[it];
					h->segment.curve.superSample(3.0);
					h->is_visible = true;
					h->is_boundary = false;
				}
				// DCEL log information
				// std::cout << DCELLog();
			}


			for ( typename std::vector<CurveRules>::iterator it = curve_sew.begin(); it < curve_sew.end(); it++ )
			{
				if ( ( it->segment_indices.size() == 1 ) && ( curve_all_indices.size() > 2 ) )
				{
					Curve2D hanging_left;
					Curve2D hanging_right;

					it->s->segment.curve.split ( it->segment_indices[0] , hanging_left, hanging_right );

					hanging_left.back() = it->intersection_points[0];
					hanging_right.front() = it->intersection_points[0];

					if ( !it->s->is_boundary )
					{
						// Just segment the Surface
						if ( (curve_all_indices.front() == it->curve_indices[0]) || (curve_all_indices.back() == it->curve_indices[0]))
						{

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = true;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = true;

							continue;
						}

						bool rule_ = curve.curve[it->curve_indices[0]].y() > it->s->segment.curve[it->segment_indices[0]].y();

						if ( current_rule == REMOVE_BELOW_INTERSECTION )
						{
							rule_ = !rule_;
						}

						if ( rule_ )
						{
							std::cout << "Remove Above " << it->s->is_boundary << std::endl;

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = false;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = true;
						}
						else
						{
							std::cout << "Remove Bellow " << curve_indices.size ( ) << std::endl;

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = true;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = false;
						}

					}

				}
				else if (it->segment_indices.size() == 2)
				{
					Curve2D hanging_left;
					Curve2D middle;
					Curve2D hanging_right;

					it->s->segment.curve.split ( it->segment_indices[0] , it->segment_indices[1] , hanging_left , middle , hanging_right );

					hanging_left.back() = it->intersection_points[0];
					middle.front() = it->intersection_points[0];
					middle.back() = it->intersection_points[1];
					hanging_right.front() = it->intersection_points[1];

					if ( !it->s->is_boundary )
					{
						// Update the geometry only
						// Otherwise the curve will float ... But Segment the Curve
						if ( (curve_all_indices.front() == it->curve_indices[0]) && (curve_all_indices.back() == it->curve_indices[1]))
						{

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = true;

							Halfedge_handle h1 = d.create_loop ( );
							h1->left_to_right = 1;
							h1->opposite ( )->left_to_right = 0;
							h1->segment.curve = middle;
							h1->segment.curve.superSample ( 3.0 );
							h1->is_visible = true;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = true;

							continue;

						}
						else if ( (curve_all_indices.front() == it->curve_indices[0]) )
						{

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = true;

							Halfedge_handle h1 = d.create_loop ( );
							h1->left_to_right = 1;
							h1->opposite ( )->left_to_right = 0;
							h1->segment.curve = middle;
							h1->segment.curve.superSample ( 3.0 );
							h1->is_visible = false;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = false;
							continue;
						}
						else if ( (curve_all_indices.back() == it->curve_indices[1]))
						{

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = false;

							Halfedge_handle h1 = d.create_loop ( );
							h1->left_to_right = 1;
							h1->opposite ( )->left_to_right = 0;
							h1->segment.curve = middle;
							h1->segment.curve.superSample ( 3.0 );
							h1->is_visible = true;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = true;
							continue;
						}

						// The input Curve Cross ! Apply Remove Above, Remove Below
						bool rule_ = curve.curve[it->curve_indices[0]].y() > it->s->segment.curve[it->segment_indices[0]].y();

						if ( current_rule == REMOVE_ABOVE_INTERSECTION )
						{
							rule_ = !rule_;
						}

						if ( rule_ )
						{
							std::cout << "Remove Bellow " << it->s->is_boundary << std::endl;

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = true;

							Halfedge_handle h1 = d.create_loop ( );
							h1->left_to_right = 1;
							h1->opposite ( )->left_to_right = 0;
							h1->segment.curve = middle;
							h1->segment.curve.superSample ( 3.0 );
							h1->is_visible = false;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = true;
						}
						else
						{
							std::cout << "Remove Bellow " << curve_indices.size ( ) << std::endl;

							it->s->segment.curve.copy ( hanging_left );
							it->s->is_visible = false;

							Halfedge_handle h1 = d.create_loop ( );
							h1->left_to_right = 1;
							h1->opposite ( )->left_to_right = 0;
							h1->segment.curve = middle;
							h1->segment.curve.superSample ( 3.0 );
							h1->is_visible = true;

							Halfedge_handle h2 = d.create_loop ( );
							h2->left_to_right = 1;
							h2->opposite ( )->left_to_right = 0;
							h2->segment.curve = hanging_right;
							h2->segment.curve.superSample ( 3.0 );
							h2->is_visible = false;
						}
					}
				}
				else
				{
					std::cout << "Should never reach this point" << curve_indices.size() << std::endl;
				}
			}
			return false;
		}

		// Test whether the curve make the boundary split;
		bool curve_against_boundary ( Stratigraphic& curve )
		{
			return false;
		}

		bool addSegment()
		{
			return false;
		}

		std::string DCELLog ( ) const
		{
			std::string log("Nothing");

			log += "---------- \n";
			log += "Vertices " + std::to_string(topology_.size_of_vertices()) + "\n";
			log += "Faces    " + std::to_string(topology_.size_of_faces()) + "\n";
			log += "Edges    " + std::to_string(topology_.size_of_halfedges()) + "\n";
			log += "---------- \n";

			return log;
		}

	};
}
/* namespace RRM */

#endif /* _CROSSSECTIONMESH_HPP_ */

