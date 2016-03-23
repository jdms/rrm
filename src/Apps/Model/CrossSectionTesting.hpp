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

#ifndef _CROSSSECTIONTESTING_HPP_
#define _CROSSSECTIONTESTING_HPP_

// CurveNDimension
#include "Core/Geometry/PolygonalCurve2D.hpp"
// Our Half-Edge items to DCEL
#include "Topology/CrossSection_Items.hpp"
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

namespace RRM
{
	/// @brief Main class of the SBIM�s module. It stores the post-processed stratigraphy (represented as curves). It also contains the geological rules, which operate on the stratigraphy.
	/// In out case, Polygonal Curves
	/// This class, intent to be a CGAL::HalfedgeDS_decorator suitable to our application demands.
	/// The particularities include: insert a curve give a geologic rules; keep history of input curves
	/// according of which curve is intersect, by given the geologic attributes, decides which operator perform.
	template < class Real >
	class CrossSectionTesting
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

			RRM::GeologicRules current_rule;

			CrossSectionTesting ( )
			{
				is_ready = false;
				current_rule = RRM::GeologicRules::Sketch;
			}

			// FIXME Create crossSection with boundary as argument.
			CrossSectionTesting ( Real x , Real y , Real width , Real height )
			{
				is_ready = initialization ( x , y , width , height );
				current_rule = RRM::GeologicRules::Sketch;
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

			clear();

			boundary(segment_1,segment_2);

			//boundary_sketch(segment_1,segment_2);

			return is_ready;
		}

		// How to create a proper ! @see https://www.youtube.com/watch?v=6mdBw3SG5io
		virtual ~CrossSectionTesting ( )
		{

		}

		void changeRule( GeologicRules the_rule )
		{
			current_rule = the_rule;
		}

		bool insertCurve ( Curve2D curve )
		{
			Stratigraphic s(curve);

			std::cout << " adding curve  " << std::endl;

			std::vector<Curve2D> segments;


			return insertSegment( s, segments);
		}

		// A horizon as a input ( cut the boundary twice )
		// If the input curve have only one intersection point, that means,
		// the input is invalid;

		// FIXME For while, it uses the closest segment point as intersection point.
		// More precise/robust/fast algorithm from the geometry package is needed.
		bool insertSegment( Stratigraphic curve, std::vector<Curve2D>& segments)
		{
			createSegment(curve.curve);

			std::cout << DCELLog() ;

			return true;
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


		void createSegment( Curve2D& _curve )
		{
		        Halfedge_handle h = topology_.edges_push_back( Halfedge(), Halfedge());
			h->left_to_right = 1;
			h->is_boundary = false;
			h->opposite()->left_to_right = 0;
			h->opposite()->is_boundary = false;
			h->segment.curve = _curve;
			h->segment.curve.superSample(3.0);
			h->is_visible = true;

			Vertex_handle v = topology_.vertices_push_back(Vertex());

		}

		void createVertex( Point2D& _point )
		{

		}

		bool boundary ( std::vector<Point2D> segment_1, std::vector<Point2D> segment_2 )
		{

			return true;
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

#endif /* _CROSSSECTIONTESTING_HPP_ */

