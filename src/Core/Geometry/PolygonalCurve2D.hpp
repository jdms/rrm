/*
 * PolygonalCurve2D.hpp
 *
 *  Created on: Aug 23, 2015
 *      Author: felipe, Emilio Vital Brazil
 */

#ifndef _RRM_POLYGONALCURVE2D_HPP_
#define _RRM_POLYGONALCURVE2D_HPP_

#include <map>
#include <vector>
#include <algorithm>

#include "Core/Base/Constants.hpp"
#include "PolygonalCurve/CurveN.hpp"

namespace RRM
{

	template < class Real >
	class PolygonalCurve2D: public PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> >
	{
			/// TODO Infinity and Zero are for double
		public:
			typedef PolygonalCurve<Real, 2, PointN<Real, 2>, VectorN<Real, 2> > Base;
			typedef PolygonalCurve2D<Real> Self;
			typedef PointN<Real, 2> Point2D;

			PolygonalCurve2D ( ) ;

			PolygonalCurve2D ( const std::vector<Point2D> &points ) : Base()
			{
				this->pPoints = points;

				checkWhetherIsClosed();
			}

			virtual ~PolygonalCurve2D ( );

			Point2D front() const
			{
				return this->pPoints.front();
			}

			Point2D back() const
			{
				return this->pPoints.back();
			}

			Point2D& front()
			{
				return this->pPoints.front();
			}

			Point2D& back()
			{
				return this->pPoints.back();
			}

			void revert ( )
			{
				std::reverse(this->pPoints.begin(),this->pPoints.end());
			}

			bool checkWhetherIsClosed ( );

			bool setCurve( const std::vector<Point2D> &points );

			/// FIXME
		    /// \brief slices curve at vertices.
			///                 0-1 ........... n-1-n
		    ///  minimum range [first=1,last=size()-1]
		    /// Creates 2 or 3 new curves by splitting this curve. If this curve is closed,
		    /// 2 curves (c1 and c2) are returned, and, if not, 3 (c1, c2 and c3).
		    /// The new curves are this curve split at the indices index1 and index2.
		    /// \param index0 index of first split point.
		    /// \param index1 index of second split point.
		    /// \param c1 (output) First segment.
		    /// \param c2 (output) Second segment.
		    /// \param c3 (output) Third segment.
		    bool slices( std::vector<std::size_t>& indices, std::deque<Self>& segments) const
		    {
		    	segments.clear();
		    	/// The end will be the [begin()+jump=indices[last],end()];
		    	segments.resize(indices.size()+1);
		    	/// Testing whether is out of range or not
		        bool is_in_range = (indices.size() < this->size()) && (indices.size() < this->size());
		        if( is_in_range )
		        {
		        	std::size_t jump = 0;

		        	for (std::size_t it = 0; it < indices.size(); ++it)
		        	{
		        		segments[it].setCurve(std::vector<Point2D>( this->pPoints.begin() + jump , this->pPoints.begin()+(indices[it]+1) ));
		        		jump = indices[it];
					}

	        		segments[indices.size()].setCurve(std::vector<Point2D>( this->pPoints.begin() + jump , this->pPoints.end()) );

//		        	for (std::size_t it = 0; it < indices.size() + 1; ++it)
//		        	{
//		        		{
//		        			std::cout << "Point2D first " << segments[it].first() << std::endl;
//		        			std::cout << "Point2D last  " << segments[it].last() << std::endl;
//		        			std::cout << "Point2D " << segments[it].size() << std::endl;
//		        		}
//		        	}
		        }
		        return is_in_range;
		    }


		public:
			/// FIXME Line related functions

			/// \brief Rotate a Point2D p 90 degrees around the origin.
			/// This function returns one Point2D where the coordinates x = - p.y and y = p.x
			Point2D perpendicular ( Point2D p ) const;

			/// \brief Test if a point is left or right to other 2
			///
			/// This function returns a real value, if it is positive means the point q
			/// is in the left side of p1p2, if negative means the it is in right side
			/// and zero means they are 3 colinear points

			Real CCW ( Point2D p1 , Point2D p2 , Point2D q ) const;

			/// \brief project a point on a line described by 2 points
			///
			/// Project q over a line defined by p1p2 and returns the projection qp.
			/// alpha is pq = q + alpha*[perpendicular(p2-p1)/|p2-p1|] that means alpha is the
			/// distance between q and pq beta is pq = p1 + beta(p2-p1), that means if
			/// 0 >= beta <=1 pq is inside the p2p1 segment. If p1 = p2 return p1 and
			/// beta = (q-p1).norm()

			Point2D projectPoint ( Point2D p1 , Point2D p2 , Point2D q , Real& alpha , Real& beta ) const;

			/// \brief Find the intersection of 2 lines defined by 2 points each
			///
			/// Find the intersection point pq and returns alpha and beta,
			/// pq = p1 + alpha*(p2-p1) and pq = q1 + beta*(q2-q1). If the lines are
			/// parallel then pq = (inf,inf), beta = alpha = inf. If the points are
			/// colinear then find alpha such that q1 = p1 + alpha*(p2-p1) and beta
			/// such that p1 = q1 + beta*(q2-q1) and returns the point (inf,inf)

			Point2D intersectionSegments ( Point2D p1 , Point2D p2 , Point2D q1 , Point2D q2 , Real& alpha , Real& beta ) const;

			//// FIXME Curve Related Functions

			bool intersectSegmentPolygonalCurve2D ( Point2D p0 , Point2D p1 , Point2D &intrs ) const;


			/// \brief finds all intersection between two curves.
			///
			/// This function calculates the intersection points between two curves.
			/// For this curve and for the testCurve it store the index \a i of the
			/// curve vertex that the segment intersect the other curve and the
			/// \f$\alpha\f$ value such that \f$\alpha((i+i)-(i))+(i)\f$;
			/// \f$\alpha \in [0,1]\f$.
			///
			/// \param testCurve the curve that will be tested against this curve
			/// \param thisIndex (output) a std::vector, stores the vertex indexes of
			///        this curve, these are the start point of the segment that
			///        intersects the testCurve.
			/// \param thisAlphas (output) a std::vector, stores this alpha values to
			///        recover the intersection point.
			/// \param testIndex (output) a std::vector, stores the vertex indexes of
			///        testCurve, these are the start point of the segment that
			///        intersects this curve.
			/// \param testAlphas (output) a std::vector, stores testCurve alpha values
			///        to recover the intersection point.
			/// \param itersPoints (output) a std::vector, stores the intersection
			///        points.
			/// \return TRUE if at least one intersection is found

			bool intersections (	const PolygonalCurve2D& testCurve ,
						std::vector<std::size_t>& thisIndex ,
						std::vector<Real>& thisAlphas ,
						std::vector<std::size_t>& testIndex ,
						std::vector<Real>& testAlphas ,
						std::vector<Point2D> &itersPoints ) const;

			/// \brief finds all intersection between two curves, and insert then if
			///        nescessary in the espective curves.
			///
			/// This function calculates the intersection points between two curves. In
			/// case that exist intersection point(s) that are not a vertice both curves
			///  are agumented with these points. This function uses the
			/// intersections(...) function
			///
			/// \param testCurve (input/output) the curve that will be tested against
			///        this curve
			/// \param prPoints (output) a std::vector, stores the intersection points.

			bool intersectionPolygonalCurve2D ( PolygonalCurve2D &testCurve , std::vector<Point2D> &prPoints );

	};

	/// Default Constructor;
	template < class Real >
	PolygonalCurve2D<Real>::PolygonalCurve2D ( ) : Base()
	{

	}

	template < class Real >
	PolygonalCurve2D<Real>::~PolygonalCurve2D ( )
	{

	}

	template < class Real >
	bool PolygonalCurve2D<Real>::setCurve ( const std::vector<Point2D> &points )
	{
		this->pPoints.clear();
		this->pPoints = points;
		return checkWhetherIsClosed ( );
	}

	template < class Real >
	bool PolygonalCurve2D<Real>::checkWhetherIsClosed ( )
	{

		/// FIXME - The right way to test the first and last element of a std::vector is following:
		///         vector.front() , vector.back();
		/// @see http://www.cplusplus.com/reference/vector/vector/front/

		if ( (this->pPoints.size ( ) > 1) && ((this->pPoints.front()) == (this->pPoints.back())) )
		{
			this->pIsClosed = true;

			std::cout << "Closed " <<  (this->pPoints.front()).x() << "  " << (this->pPoints.back()).x() << std::endl;

			this->pPoints.pop_back ( );
		}
		else
		{
			this->pIsClosed = false;
		}

		return this->pIsClosed;
	}

	/// XXX 2D Implementation of basic operations on Lines
	template < class Real >
	typename PolygonalCurve2D<Real>::Point2D PolygonalCurve2D<Real>::perpendicular ( Point2D p ) const
	{
		return Point2D ( -p[1] , p[0] );
	}

	/// XXX Follow the LEFT->RIGHT, TOP->DOWN
	template < class Real >
	Real PolygonalCurve2D<Real>::CCW ( Point2D p1 , Point2D p2 , Point2D q ) const
	{
		return ( p2[0] - p1[0] ) * ( q[1] - p1[1] ) - ( q[0] - p1[0] ) * ( p2[1] - p1[1] );
	}

	template < class Real >
	typename PolygonalCurve2D<Real>::Point2D PolygonalCurve2D<Real>::projectPoint ( Point2D p1 , Point2D p2 , Point2D q , Real& alpha , Real& beta ) const
	{
		Point2D p2p1 = p2 - p1;
		Real norm = p2p1.norm ( );
		if ( norm <= Math::Constants::Epsilon )
		{
			alpha = ( p1 - q ).norm ( );
			beta = 0.0;
			return p1;
		}

		Point2D N = perp ( p2p1 ) / norm;
		alpha = ( p1 - q ).dot ( N );

		Point2D qp = q + alpha * N;

		unsigned indexMax = ( std::abs ( p2p1[0] ) < std::abs ( p2p1[1] ) ) ? 1 : 0;

		beta = ( ( qp - p1 )[indexMax] ) / p2p1[indexMax];

		return qp;
	}

	template < class Real >
	typename PolygonalCurve2D<Real>::Point2D PolygonalCurve2D<Real>::intersectionSegments ( Point2D p1 , Point2D p2 , Point2D q1 , Point2D q2 , Real& alpha , Real& beta ) const
	{
		Point2D p2p1 = p2 - p1;
		Point2D q1q2 = q1 - q2;
		Point2D q1p1 = q1 - p1;

		alpha = Math::Constants::Infinty;
		beta = Math::Constants::Infinty;

		Real det = p2p1[0] * q1q2[1] - p2p1[1] * q1q2[0];
		if ( std::abs ( det ) > Math::Constants::Epsilon )
		{
			alpha = ( q1p1[0] * q1q2[1] - q1p1[1] * q1q2[0] ) / det;
			beta = ( q1p1[1] * p2p1[0] - q1p1[0] * p2p1[1] ) / det;

			Point2D pq = p1 + alpha * ( p2 - p1 );

			return pq;
		}

		/// Special cases ///

		if ( p2p1.norm2 ( ) <= Math::Constants::Epsilon && q1q2.norm2 ( ) <= Math::Constants::Epsilon )
		{
			if ( p1 == q1 || p1 == q2 )
			{
				alpha = 0.0, beta = 0.0;
				return p1;
			}
			return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
		}

		if ( p2p1.norm2 ( ) <= Math::Constants::Epsilon )
		{
			if ( p1 == q1 )
			{
				alpha = 0.0, beta = 0.0;
				return p1;
			}
			if ( p1 == q2 )
			{
				alpha = 0.0, beta = 1.0;
				return p1;
			}
			return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
		}

		if ( q1q2.norm2 ( ) <= Math::Constants::Epsilon )
		{
			if ( q1 == p1 )
			{
				alpha = 0.0, beta = 0.0;
				return q1;
			}
			if ( q1 == p2 )
			{
				alpha = 0.0, beta = 1.0;
				return q1;
			}
			return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
		}

		///
		/// if det = 0 test if the points are colinear if yes find alpha such that
		/// q1 = p1 + alpha*(p2-p1) and beta such that p1 = q1 + beta*(q2-q1)
		/// and returns pq = ( inf , inf )
		///
		if ( std::abs ( CCW ( p1 , p2 , q1 ) ) <= Math::Constants::Epsilon )
		{
			unsigned indexMax = ( std::abs ( p2p1[0] ) < std::abs ( p2p1[1] ) ) ? 1 : 0;
			alpha = ( ( q1 - p1 )[indexMax] ) / p2p1[indexMax];

			Point2D q2q1 = q2 - q1;
			indexMax = ( std::abs ( q2q1[0] ) < std::abs ( q2q1[1] ) ) ? 1 : 0;
			beta = ( ( p1 - q1 )[indexMax] ) / q2q1[indexMax];
		}

		return Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );
	}

	/// XXX Intersection Between PolygonalCurve2D
	template < class Real >
	bool PolygonalCurve2D<Real>::intersectSegmentPolygonalCurve2D ( Point2D p0 , Point2D p1 , Point2D &intrs ) const
	{
		intrs = Point2D ( Math::Constants::Infinty , Math::Constants::Infinty );

		for ( std::size_t i = 1; i < this->size ( ); ++i )
		{
			Real uThis, uTest;
			Point2D prPoint;
			prPoint = intersectionSegments ( this->at ( i - 1 ) , this->at ( i ) , p0 , p1 , uThis , uTest );
			if ( uThis >= 0.0 && uThis <= 1.0 && uTest >= 0.0 && uTest <= 1.0 )
			{
				intrs = p0 + uTest * ( p1 - p0 );
				return true;
			}
		}

		return false;
	}

	template < class Real >
	bool PolygonalCurve2D<Real>::intersections (	const PolygonalCurve2D& testCurve ,
							std::vector<std::size_t>& thisIndex ,
							std::vector<Real>& thisAlphas ,
							std::vector<std::size_t>& testIndex ,
							std::vector<Real>& testAlphas ,
							std::vector<Point2D> &itersPoints ) const
	{
		thisIndex.clear ( );
		thisAlphas.clear ( );
		testIndex.clear ( );
		testAlphas.clear ( );
		itersPoints.clear ( );

		PolygonalCurve2D cTestCurve = testCurve;
		if ( cTestCurve.isClosed ( ) )
			cTestCurve.add ( cTestCurve.atBegin ( ) );
		PolygonalCurve2D cThisCurve = ( *this );
		if ( cThisCurve.isClosed ( ) )
			cThisCurve.add ( cThisCurve.atBegin ( ) );

		for ( std::size_t i = 1; i < cThisCurve.size ( ); ++i )
		{
			for ( std::size_t j = 1; j < cTestCurve.size ( ); ++j )
			{
				Real uThis, uTest;
				Point2D prPoint;
				/// FIXME Brute Force Procedure
				prPoint = intersectionSegments ( cThisCurve[i - 1] , cThisCurve[i] , cTestCurve[j - 1] , cTestCurve[j] , uThis , uTest );
				if ( uThis >= 0 && uThis <= 1.0 && uTest >= 0 && uTest <= 1.0 )
				{
					thisIndex.push_back ( i - 1 );
					testIndex.push_back ( j - 1 );
					thisAlphas.push_back ( uThis );
					testAlphas.push_back ( uTest );
					itersPoints.push_back ( prPoint );

					if ( 1.0 - uThis <= Math::Constants::Epsilon )
						++i;
					if ( 1.0 - uTest <= Math::Constants::Epsilon )
						++j;
				}
			}
		}
		return thisIndex.size ( ) > 0;
	}

	template < class Real >
	bool PolygonalCurve2D<Real>::intersectionPolygonalCurve2D ( PolygonalCurve2D &testCurve , std::vector<Point2D> &prPoints )
	{
		prPoints.clear ( );

		std::vector<std::size_t> myIndex;
		std::vector<Real> myAlphas;
		std::vector<std::size_t> theirIndex;
		std::vector<Real> theirAlphas;

		bool intersected = this->intersections ( testCurve , myIndex , myAlphas , theirIndex , theirAlphas , prPoints );
		if ( intersected )
		{
			std::vector<std::size_t> auxTheirIndex, auxMyIndex;
			std::map<std::size_t, Point2D> myMap, theirMap;

			/// The index has the same size. Do it for both curves.
			for ( std::size_t i = 0; i < myIndex.size ( ); ++i )
			{
				std::size_t myI = myIndex[i];
				Real myAlpha = myAlphas[i];

				std::size_t theirI = theirIndex[i];
				Real theirAlpha = theirAlphas[i];

				// avoiding repeated points
				std::size_t nearIndex = ( myAlpha < 0.5 ) ? 0 : 1;
				if ( ( this->at ( myI + nearIndex ) - prPoints[i] ).norm ( ) > Math::Constants::Epsilon )
				{
					auxMyIndex.push_back ( myI );
					myMap[myI] = prPoints[i];
				}
				else
				{
					prPoints[i] = this->at ( myI + nearIndex );
				}

				nearIndex = ( theirAlpha < 0.5 ) ? 0 : 1;
				if ( ( testCurve.at ( theirI + nearIndex ) - prPoints[i] ).norm ( ) > Math::Constants::Epsilon )
				{
					auxTheirIndex.push_back ( theirI );
					theirMap[theirI] = prPoints[i];
				}
				else
				{
					prPoints[i] = testCurve.at ( theirI + nearIndex );
				}
			}

			std::sort ( auxMyIndex.begin ( ) , auxMyIndex.end ( ) );
			for ( std::size_t i = 0; i < auxMyIndex.size ( ); ++i )
			{
				this->insert ( auxMyIndex[i] + i + 1 , myMap[auxMyIndex[i]] );
			}

			std::sort ( auxTheirIndex.begin ( ) , auxTheirIndex.end ( ) );
			for ( std::size_t i = 0; i < auxTheirIndex.size ( ); ++i )
			{
				testCurve.insert ( auxTheirIndex[i] + i + 1 , theirMap[auxTheirIndex[i]] );
			}
		}

		return intersected;
	}

} /* namespace RRM */

#endif /* _RRM_POLYGONALCURVE2D_HPP_ */
